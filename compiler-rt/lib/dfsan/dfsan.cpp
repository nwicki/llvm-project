//===-- dfsan.cpp ---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is a part of DataFlowSanitizer.
//
// DataFlowSanitizer runtime.  This file defines the public interface to
// DataFlowSanitizer as well as the definition of certain runtime functions
// called automatically by the compiler (specifically the instrumentation pass
// in llvm/lib/Transforms/Instrumentation/DataFlowSanitizer.cpp).
//
// The public interface is defined in include/sanitizer/dfsan_interface.h whose
// functions are prefixed dfsan_ while the compiler interface functions are
// prefixed __dfsan_.
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include "sanitizer_common/sanitizer_atomic.h"
#include "sanitizer_common/sanitizer_common.h"
#include "sanitizer_common/sanitizer_file.h"
#include "sanitizer_common/sanitizer_flags.h"
#include "sanitizer_common/sanitizer_flag_parser.h"
#include "sanitizer_common/sanitizer_libc.h"

#include "dfsan/dfsan.h"

using namespace __dfsan;

typedef atomic_uint16_t atomic_dfsan_label;
static const dfsan_label kInitializingLabel = -1;

static const uptr kNumLabels = 1 << (sizeof(dfsan_label) * 8);

static atomic_dfsan_label __dfsan_last_label;
static dfsan_label_info __dfsan_label_info[kNumLabels];

Flags __dfsan::flags_data;

SANITIZER_INTERFACE_ATTRIBUTE THREADLOCAL dfsan_label __dfsan_retval_tls;
SANITIZER_INTERFACE_ATTRIBUTE THREADLOCAL dfsan_label __dfsan_arg_tls[64];

SANITIZER_INTERFACE_ATTRIBUTE uptr __dfsan_shadow_ptr_mask;

// On Linux/x86_64, memory is laid out as follows:
//
// +--------------------+ 0x800000000000 (top of memory)
// | application memory |
// +--------------------+ 0x700000008000 (kAppAddr)
// |                    |
// |       unused       |
// |                    |
// +--------------------+ 0x200200000000 (kUnusedAddr)
// |    union table     |
// +--------------------+ 0x200000000000 (kUnionTableAddr)
// |   shadow memory    |
// +--------------------+ 0x000000010000 (kShadowAddr)
// | reserved by kernel |
// +--------------------+ 0x000000000000
//
// To derive a shadow memory address from an application memory address,
// bits 44-46 are cleared to bring the address into the range
// [0x000000008000,0x100000000000).  Then the address is shifted left by 1 to
// account for the double byte representation of shadow labels and move the
// address into the shadow memory range.  See the function shadow_for below.

// On Linux/MIPS64, memory is laid out as follows:
//
// +--------------------+ 0x10000000000 (top of memory)
// | application memory |
// +--------------------+ 0xF000008000 (kAppAddr)
// |                    |
// |       unused       |
// |                    |
// +--------------------+ 0x2200000000 (kUnusedAddr)
// |    union table     |
// +--------------------+ 0x2000000000 (kUnionTableAddr)
// |   shadow memory    |
// +--------------------+ 0x0000010000 (kShadowAddr)
// | reserved by kernel |
// +--------------------+ 0x0000000000

// On Linux/AArch64 (39-bit VMA), memory is laid out as follow:
//
// +--------------------+ 0x8000000000 (top of memory)
// | application memory |
// +--------------------+ 0x7000008000 (kAppAddr)
// |                    |
// |       unused       |
// |                    |
// +--------------------+ 0x1200000000 (kUnusedAddr)
// |    union table     |
// +--------------------+ 0x1000000000 (kUnionTableAddr)
// |   shadow memory    |
// +--------------------+ 0x0000010000 (kShadowAddr)
// | reserved by kernel |
// +--------------------+ 0x0000000000

// On Linux/AArch64 (42-bit VMA), memory is laid out as follow:
//
// +--------------------+ 0x40000000000 (top of memory)
// | application memory |
// +--------------------+ 0x3ff00008000 (kAppAddr)
// |                    |
// |       unused       |
// |                    |
// +--------------------+ 0x1200000000 (kUnusedAddr)
// |    union table     |
// +--------------------+ 0x8000000000 (kUnionTableAddr)
// |   shadow memory    |
// +--------------------+ 0x0000010000 (kShadowAddr)
// | reserved by kernel |
// +--------------------+ 0x0000000000

// On Linux/AArch64 (48-bit VMA), memory is laid out as follow:
//
// +--------------------+ 0x1000000000000 (top of memory)
// | application memory |
// +--------------------+ 0xffff00008000 (kAppAddr)
// |       unused       |
// +--------------------+ 0xaaaab0000000 (top of PIE address)
// | application PIE    |
// +--------------------+ 0xaaaaa0000000 (top of PIE address)
// |                    |
// |       unused       |
// |                    |
// +--------------------+ 0x1200000000 (kUnusedAddr)
// |    union table     |
// +--------------------+ 0x8000000000 (kUnionTableAddr)
// |   shadow memory    |
// +--------------------+ 0x0000010000 (kShadowAddr)
// | reserved by kernel |
// +--------------------+ 0x0000000000

typedef atomic_dfsan_label dfsan_union_table_t[kNumLabels][kNumLabels];

#ifdef DFSAN_RUNTIME_VMA
// Runtime detected VMA size.
int __dfsan::vmaSize;
#endif

static uptr UnusedAddr() {
  return MappingArchImpl<MAPPING_UNION_TABLE_ADDR>()
         + sizeof(dfsan_union_table_t);
}

static atomic_dfsan_label *union_table(dfsan_label l1, dfsan_label l2) {
  return &(*(dfsan_union_table_t *) UnionTableAddr())[l1][l2];
}

// Checks we do not run out of labels.
static void dfsan_check_label(dfsan_label label) {
  if (label == kInitializingLabel) {
    Report("FATAL: DataFlowSanitizer: out of labels\n");
    Die();
  }
}

// Resolves the union of two unequal labels.  Nonequality is a precondition for
// this function (the instrumentation pass inlines the equality test).
extern "C" SANITIZER_INTERFACE_ATTRIBUTE
dfsan_label __dfsan_union(dfsan_label l1, dfsan_label l2) {
  //printf("START __dfsan_union\n");
  //printf("l1 is %d, l2 is %d\n", l1, l2);
  if (flags().fast16labels)
    return l1 | l2;
  DCHECK_NE(l1, l2);

  if (l1 == 0) {
    //printf("END __dfsan_union\n\n");
    return l2;
  }
  if (l2 == 0) {
    //printf("END __dfsan_union\n\n");
    return l1;
  }

  if (l1 > l2)
    Swap(l1, l2);

  atomic_dfsan_label *table_ent = union_table(l1, l2);
  // We need to deal with the case where two threads concurrently request
  // a union of the same pair of labels.  If the table entry is uninitialized,
  // (i.e. 0) use a compare-exchange to set the entry to kInitializingLabel
  // (i.e. -1) to mark that we are initializing it.
  dfsan_label label = 0;
  if (atomic_compare_exchange_strong(table_ent, &label, kInitializingLabel,
                                     memory_order_acquire)) {
    // Check whether l2 subsumes l1.  We don't need to check whether l1
    // subsumes l2 because we are guaranteed here that l1 < l2, and (at least
    // in the cases we are interested in) a label may only subsume labels
    // created earlier (i.e. with a lower numerical value).
    if (__dfsan_label_info[l2].l1 == l1 ||
        __dfsan_label_info[l2].l2 == l1) {
      label = l2;
    } else {
      label =
        atomic_fetch_add(&__dfsan_last_label, 1, memory_order_relaxed) + 1;
      dfsan_check_label(label);
      __dfsan_label_info[label].l1 = l1;
      __dfsan_label_info[label].l2 = l2;
    }
    atomic_store(table_ent, label, memory_order_release);
  } else if (label == kInitializingLabel) {
    // Another thread is initializing the entry.  Wait until it is finished.
    do {
      internal_sched_yield();
      label = atomic_load(table_ent, memory_order_acquire);
    } while (label == kInitializingLabel);
  }
  //printf("unified is %d\n", label);
  //printf("END __dfsan_union\n\n");
  return label;
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE
dfsan_label __dfsan_union_load(const dfsan_label *ls, uptr n) {
  dfsan_label label = ls[0];
  for (uptr i = 1; i != n; ++i) {
    dfsan_label next_label = ls[i];
    if (label != next_label)
      label = __dfsan_union(label, next_label);
  }
  return label;
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE
void __dfsan_unimplemented(char *fname) {
  if (flags().warn_unimplemented)
    Report("WARNING: DataFlowSanitizer: call to uninstrumented function %s\n",
           fname);
}

// Use '-mllvm -dfsan-debug-nonzero-labels' and break on this function
// to try to figure out where labels are being introduced in a nominally
// label-free program.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void __dfsan_nonzero_label() {
  if (flags().warn_nonzero_labels)
    Report("WARNING: DataFlowSanitizer: saw nonzero label\n");
}

// Indirect call to an uninstrumented vararg function. We don't have a way of
// handling these at the moment.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
__dfsan_vararg_wrapper(const char *fname) {
  Report("FATAL: DataFlowSanitizer: unsupported indirect call to vararg "
         "function %s\n", fname);
  Die();
}

// Like __dfsan_union, but for use from the client or custom functions.  Hence
// the equality comparison is done here before calling __dfsan_union.
SANITIZER_INTERFACE_ATTRIBUTE dfsan_label
dfsan_union(dfsan_label l1, dfsan_label l2) {
  if (l1 == l2)
    return l1;
  return __dfsan_union(l1, l2);
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE
dfsan_label dfsan_create_label(const char *desc, void *userdata) {
  dfsan_label label =
    atomic_fetch_add(&__dfsan_last_label, 1, memory_order_relaxed) + 1;
  dfsan_check_label(label);
  __dfsan_label_info[label].l1 = __dfsan_label_info[label].l2 = 0;
  __dfsan_label_info[label].desc = desc;
  __dfsan_label_info[label].userdata = userdata;
  return label;
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE
void __dfsan_set_label(dfsan_label label, void *addr, uptr size) {
  for (dfsan_label *labelp = shadow_for(addr); size != 0; --size, ++labelp) {
    // Don't write the label if it is already the value we need it to be.
    // In a program where most addresses are not labeled, it is common that
    // a page of shadow memory is entirely zeroed.  The Linux copy-on-write
    // implementation will share all of the zeroed pages, making a copy of a
    // page when any value is written.  The un-sharing will happen even if
    // the value written does not change the value in memory.  Avoiding the
    // write when both |label| and |*labelp| are zero dramatically reduces
    // the amount of real memory used by large programs.
    if (label == *labelp)
      continue;

    *labelp = label;
  }
}

SANITIZER_INTERFACE_ATTRIBUTE
void dfsan_set_label(dfsan_label label, void *addr, uptr size) {
  __dfsan_set_label(label, addr, size);
}

SANITIZER_INTERFACE_ATTRIBUTE
void dfsan_add_label(dfsan_label label, void *addr, uptr size) {
  for (dfsan_label *labelp = shadow_for(addr); size != 0; --size, ++labelp)
    if (*labelp != label)
      *labelp = __dfsan_union(*labelp, label);
}

// Unlike the other dfsan interface functions the behavior of this function
// depends on the label of one of its arguments.  Hence it is implemented as a
// custom function.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE dfsan_label
__dfsw_dfsan_get_label(long data, dfsan_label data_label,
                       dfsan_label *ret_label) {
  *ret_label = 0;
  return data_label;
}

SANITIZER_INTERFACE_ATTRIBUTE dfsan_label
dfsan_read_label(const void *addr, uptr size) {
  if (size == 0)
    return 0;
  return __dfsan_union_load(shadow_for(addr), size);
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE
const struct dfsan_label_info *dfsan_get_label_info(dfsan_label label) {
  return &__dfsan_label_info[label];
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE int
dfsan_has_label(dfsan_label label, dfsan_label elem) {
  if (label == elem)
    return true;
  const dfsan_label_info *info = dfsan_get_label_info(label);
  if (info->l1 != 0) {
    return dfsan_has_label(info->l1, elem) || dfsan_has_label(info->l2, elem);
  } else {
    return false;
  }
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE dfsan_label
dfsan_has_label_with_desc(dfsan_label label, const char *desc) {
  const dfsan_label_info *info = dfsan_get_label_info(label);
  if (info->l1 != 0) {
    return dfsan_has_label_with_desc(info->l1, desc) ||
           dfsan_has_label_with_desc(info->l2, desc);
  } else {
    return internal_strcmp(desc, info->desc) == 0;
  }
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE uptr
dfsan_get_label_count(void) {
  dfsan_label max_label_allocated =
      atomic_load(&__dfsan_last_label, memory_order_relaxed);

  return static_cast<uptr>(max_label_allocated);
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
dfsan_dump_labels(int fd) {
  dfsan_label last_label =
      atomic_load(&__dfsan_last_label, memory_order_relaxed);

  for (uptr l = 1; l <= last_label; ++l) {
    char buf[64];
    internal_snprintf(buf, sizeof(buf), "%u %u %u ", l,
                      __dfsan_label_info[l].l1, __dfsan_label_info[l].l2);
    WriteToFile(fd, buf, internal_strlen(buf));
    if (__dfsan_label_info[l].l1 == 0 && __dfsan_label_info[l].desc) {
      WriteToFile(fd, __dfsan_label_info[l].desc,
                  internal_strlen(__dfsan_label_info[l].desc));
    }
    WriteToFile(fd, "\n", 1);
  }
}

void Flags::SetDefaults() {
#define DFSAN_FLAG(Type, Name, DefaultValue, Description) Name = DefaultValue;
#include "dfsan_flags.inc"
#undef DFSAN_FLAG
}

static void RegisterDfsanFlags(FlagParser *parser, Flags *f) {
#define DFSAN_FLAG(Type, Name, DefaultValue, Description) \
  RegisterFlag(parser, #Name, Description, &f->Name);
#include "dfsan_flags.inc"
#undef DFSAN_FLAG
}

static void InitializeFlags() {
  SetCommonFlagsDefaults();
  flags().SetDefaults();

  FlagParser parser;
  RegisterCommonFlags(&parser);
  RegisterDfsanFlags(&parser, &flags());
  parser.ParseStringFromEnv("DFSAN_OPTIONS");
  InitializeCommonFlags();
  if (Verbosity()) ReportUnrecognizedFlags();
  if (common_flags()->help) parser.PrintFlagDescriptions();
}

static void InitializePlatformEarly() {
  AvoidCVE_2016_2143();
#ifdef DFSAN_RUNTIME_VMA
  __dfsan::vmaSize =
    (MostSignificantSetBitIndex(GET_CURRENT_FRAME()) + 1);
  if (__dfsan::vmaSize == 39 || __dfsan::vmaSize == 42 ||
      __dfsan::vmaSize == 48) {
    __dfsan_shadow_ptr_mask = ShadowMask();
  } else {
    Printf("FATAL: DataFlowSanitizer: unsupported VMA range\n");
    Printf("FATAL: Found %d - Supported 39, 42, and 48\n", __dfsan::vmaSize);
    Die();
  }
#endif
}

static void dfsan_fini() {
  if (internal_strcmp(flags().dump_labels_at_exit, "") != 0) {
    fd_t fd = OpenFile(flags().dump_labels_at_exit, WrOnly);
    if (fd == kInvalidFd) {
      Report("WARNING: DataFlowSanitizer: unable to open output file %s\n",
             flags().dump_labels_at_exit);
      return;
    }

    Report("INFO: DataFlowSanitizer: dumping labels to %s\n",
           flags().dump_labels_at_exit);
    dfsan_dump_labels(fd);
    CloseFile(fd);
  }
}

extern "C" void dfsan_flush() {
  UnmapOrDie((void*)ShadowAddr(), UnusedAddr() - ShadowAddr());
  if (!MmapFixedNoReserve(ShadowAddr(), UnusedAddr() - ShadowAddr()))
    Die();
}

static void dfsan_init(int argc, char **argv, char **envp) {
  InitializeFlags();

  ::InitializePlatformEarly();

  if (!MmapFixedNoReserve(ShadowAddr(), UnusedAddr() - ShadowAddr()))
    Die();

  // Protect the region of memory we don't use, to preserve the one-to-one
  // mapping from application to shadow memory. But if ASLR is disabled, Linux
  // will load our executable in the middle of our unused region. This mostly
  // works so long as the program doesn't use too much memory. We support this
  // case by disabling memory protection when ASLR is disabled.
  uptr init_addr = (uptr)&dfsan_init;
  if (!(init_addr >= UnusedAddr() && init_addr < AppAddr()))
    MmapFixedNoAccess(UnusedAddr(), AppAddr() - UnusedAddr());

  InitializeInterceptors();

  // Register the fini callback to run when the program terminates successfully
  // or it is killed by the runtime.
  Atexit(dfsan_fini);
  AddDieCallback(dfsan_fini);

  __dfsan_label_info[kInitializingLabel].desc = "<init label>";
}

#if SANITIZER_CAN_USE_PREINIT_ARRAY
__attribute__((section(".preinit_array"), used))
static void (*dfsan_init_ptr)(int, char **, char **) = dfsan_init;
#endif


// Start Region: Implementation Control-flow Analysis

// Explicit control flow taint analysis is implemented here.
// We taint any operand inside the scope of the control structure with
// the taint incorporated by the condition to enter the control structure.
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
// Size of the data structure for control labels.
static thread_local int __dfsan_control_array_size = 16;
// Data Structure to save the current status of the control structure label.
static thread_local dfsan_label *__dfsan_control_labels = NULL;
// Data Structure to save the current status of the control structure label.
static thread_local int *__dfsan_control_switches = NULL;
// Starting depth for control structures.
static thread_local int __dfsan_control_depth = 0;

// Called when we need the label of the current control structure.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE dfsan_label
__dfsan_control_scope_label (void) {
  if(__dfsan_control_depth < 1){
    return 0;
  }
  return __dfsan_control_labels[__dfsan_control_depth-1];
}
extern "C" SANITIZER_INTERFACE_ATTRIBUTE dfsan_label
dfsan_control_scope_label (void) {
  return __dfsan_control_scope_label();
}

// Called when entering a control structure such as for, if, while, etc.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
__dfsan_control_enter (dfsan_label label, int bi_id) {
  if(!__dfsan_control_labels) {
    __dfsan_control_labels = (dfsan_label *) malloc(__dfsan_control_array_size*sizeof(dfsan_label));
  }
  if(!__dfsan_control_switches) {
    __dfsan_control_switches = (int *) calloc(__dfsan_control_array_size,sizeof(int));
  }
  if(__dfsan_control_depth < 1){
    __dfsan_control_labels[__dfsan_control_depth] = label;
  }
  else {
    __dfsan_control_labels[__dfsan_control_depth] = dfsan_union(__dfsan_control_labels[__dfsan_control_depth-1],label);
  }
  __dfsan_control_switches[__dfsan_control_depth] = bi_id;
  __dfsan_control_depth++;
  if(__dfsan_control_depth >= __dfsan_control_array_size) {
    dfsan_label *new_array_labels = (dfsan_label *) malloc(__dfsan_control_array_size*2*sizeof(dfsan_label));
    int *new_array_switches = (int *) calloc(__dfsan_control_array_size*2,sizeof(int));
    for(int i = 0; i < __dfsan_control_array_size; i++) {
      new_array_labels[i] = __dfsan_control_labels[i];
      new_array_switches[i] = __dfsan_control_switches[i];
    }
    //memcpy(new_array, __dfsan_control_labels, __dfsan_control_array_size*sizeof(dfsan_label));
    free(__dfsan_control_labels);
    free(__dfsan_control_switches);
    __dfsan_control_labels = new_array_labels;
    __dfsan_control_switches = new_array_switches;
    __dfsan_control_array_size *= 2;
  }
  return;
}
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
dfsan_control_enter (dfsan_label label) {
  return __dfsan_control_enter(label, 1);
}

// Called after computing the condition for a loop structure to replace the current taint label.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
__dfsan_control_replace (dfsan_label label) {
  if(__dfsan_control_depth > __dfsan_control_array_size) {
    printf("Depth greater than array.\n");
    return;
  }
  if(__dfsan_control_depth < 0) {
    printf("Depth negative.\n");
    return;
  }
  if(__dfsan_control_depth == 1){
    __dfsan_control_labels[__dfsan_control_depth-1] = label;
  }
  else if(__dfsan_control_depth > 1) {
    __dfsan_control_labels[__dfsan_control_depth-1] = dfsan_union(__dfsan_control_labels[__dfsan_control_depth-2],label);
  }
  return;
}
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
dfsan_control_replace (dfsan_label label) {
  return __dfsan_control_replace(label);
}

// Called when we leave a control structure.
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
__dfsan_control_leave (int bi_id) {
  if(__dfsan_control_depth < 1) {
    return;
  }
  if(__dfsan_control_switches[__dfsan_control_depth-1] != bi_id) {
    return;
  }
  __dfsan_control_depth--;
  __dfsan_control_switches[__dfsan_control_depth] = 0;
  return;
}
extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
dfsan_control_leave (void) {
  return __dfsan_control_leave(1);
}

extern "C" SANITIZER_INTERFACE_ATTRIBUTE void
__dfsan_control_print (dfsan_label label) {
  return ;
}

// export PATH=/home/negolas/Documents/hs19/bachelor_thesis/project_code/llvm-project/build/bin/:$PATH
// clang++ -mllvm -disable-llvm-optzns test.cpp -S -emit-llvm
// opt -dfsan -dfsan-cfsan-enable -dfsan-abilist=/home/negolas/Documents/hs19/bachelor_thesis/project_code/llvm-project/dfsan_abilist.txt test.ll -o test_opt.ll
// llc -relocation-model=pic -filetype=obj test_opt.ll
// clang++ -fsanitize=dataflow test_opt.o
// cmake -DLLVM_ENABLE_PROJECTS="clang;compiler-rt;libcxx;libcxxabi" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(pwd)/../install ../llvm
// LULESH
// make
// llvm-link *.bc -o lulesh_complete.bc
// opt -dfsan -dfsan-cfsan-enable -dfsan-abilist=/home/negolas/Documents/hs19/bachelor_thesis/project_code/llvm-project/dfsan_abilist.txt lulesh_complete.bc -o lulesh_complete_opt.bc
// llc -relocation-model=pic -filetype=obj lulesh_complete_opt.bc
// clang++ lulesh_complete_opt.o -stdlib=libc++ -fsanitize=dataflow -fsanitize-blacklist=/home/negolas/Documents/hs19/bachelor_thesis/project_code/llvm-project/dfsan_abilist.txt -L/home/negolas/Documents/hs19/bachelor_thesis/project_code/llvm-project/build_libcxx/lib -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi/opal/mca/event/libevent2022/libevent -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi/opal/mca/event/libevent2022/libevent/include -I/usr/lib/x86_64-linux-gnu/openmpi/include -pthread -L/usr//lib -L/usr/lib/x86_64-linux-gnu/openmpi/lib -lmpi_cxx -lmpi -Wl,--start-group,-lc++abi
// DFSAN_OPTIONS=warn_unimplemented=0 mpirun -n 1 ./a.out -s 2 > OUT 2>&1
// End Region: Implementation Control-flow Analysis