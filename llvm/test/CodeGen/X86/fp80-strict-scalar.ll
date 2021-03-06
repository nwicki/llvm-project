; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-unknown-unknown -O3 -disable-strictnode-mutation | FileCheck %s --check-prefixes=CHECK,X86
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -O3 -disable-strictnode-mutation | FileCheck %s --check-prefixes=CHECK,X64

declare x86_fp80 @llvm.experimental.constrained.fadd.x86_fp80(x86_fp80, x86_fp80, metadata, metadata)
declare x86_fp80 @llvm.experimental.constrained.fsub.x86_fp80(x86_fp80, x86_fp80, metadata, metadata)
declare x86_fp80 @llvm.experimental.constrained.fmul.x86_fp80(x86_fp80, x86_fp80, metadata, metadata)
declare x86_fp80 @llvm.experimental.constrained.fdiv.x86_fp80(x86_fp80, x86_fp80, metadata, metadata)
declare x86_fp80 @llvm.experimental.constrained.fpext.x86_fp80.f32(float, metadata)
declare x86_fp80 @llvm.experimental.constrained.fpext.x86_fp80.f64(double, metadata)
declare x86_fp80 @llvm.experimental.constrained.sqrt.x86_fp80(x86_fp80, metadata, metadata)
declare float @llvm.experimental.constrained.fptrunc.f32.x86_fp80(x86_fp80, metadata, metadata)
declare double @llvm.experimental.constrained.fptrunc.f64.x86_fp80(x86_fp80, metadata, metadata)
declare i1  @llvm.experimental.constrained.fptosi.i1.x86_fp80(x86_fp80, metadata)
declare i8  @llvm.experimental.constrained.fptosi.i8.x86_fp80(x86_fp80, metadata)
declare i16 @llvm.experimental.constrained.fptosi.i16.x86_fp80(x86_fp80, metadata)
declare i32 @llvm.experimental.constrained.fptosi.i32.x86_fp80(x86_fp80, metadata)
declare i64 @llvm.experimental.constrained.fptosi.i64.x86_fp80(x86_fp80, metadata)
declare i1 @llvm.experimental.constrained.fptoui.i1.x86_fp80(x86_fp80, metadata)
declare i8  @llvm.experimental.constrained.fptoui.i8.x86_fp80(x86_fp80, metadata)
declare i16 @llvm.experimental.constrained.fptoui.i16.x86_fp80(x86_fp80, metadata)
declare i32 @llvm.experimental.constrained.fptoui.i32.x86_fp80(x86_fp80, metadata)
declare i64 @llvm.experimental.constrained.fptoui.i64.x86_fp80(x86_fp80, metadata)

define x86_fp80 @fadd_fp80(x86_fp80 %a, x86_fp80 %b) nounwind strictfp {
; X86-LABEL: fadd_fp80:
; X86:       # %bb.0:
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    faddp %st, %st(1)
; X86-NEXT:    retl
;
; X64-LABEL: fadd_fp80:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    faddp %st, %st(1)
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.fadd.x86_fp80(x86_fp80 %a, x86_fp80 %b,
                                                                    metadata !"round.dynamic",
                                                                    metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define x86_fp80 @fsub_fp80(x86_fp80 %a, x86_fp80 %b) nounwind strictfp {
; X86-LABEL: fsub_fp80:
; X86:       # %bb.0:
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fsubp %st, %st(1)
; X86-NEXT:    retl
;
; X64-LABEL: fsub_fp80:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fsubp %st, %st(1)
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.fsub.x86_fp80(x86_fp80 %a, x86_fp80 %b,
                                                                    metadata !"round.dynamic",
                                                                    metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define x86_fp80 @fmul_fp80(x86_fp80 %a, x86_fp80 %b) nounwind strictfp {
; X86-LABEL: fmul_fp80:
; X86:       # %bb.0:
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fmulp %st, %st(1)
; X86-NEXT:    retl
;
; X64-LABEL: fmul_fp80:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fmulp %st, %st(1)
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.fmul.x86_fp80(x86_fp80 %a, x86_fp80 %b,
                                                                    metadata !"round.dynamic",
                                                                    metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define x86_fp80 @fdiv_fp80(x86_fp80 %a, x86_fp80 %b) nounwind strictfp {
; X86-LABEL: fdiv_fp80:
; X86:       # %bb.0:
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fdivp %st, %st(1)
; X86-NEXT:    retl
;
; X64-LABEL: fdiv_fp80:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fdivp %st, %st(1)
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.fdiv.x86_fp80(x86_fp80 %a, x86_fp80 %b,
                                                                    metadata !"round.dynamic",
                                                                    metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define x86_fp80 @fpext_f32_to_fp80(float %a) nounwind strictfp {
; X86-LABEL: fpext_f32_to_fp80:
; X86:       # %bb.0:
; X86-NEXT:    flds {{[0-9]+}}(%esp)
; X86-NEXT:    retl
;
; X64-LABEL: fpext_f32_to_fp80:
; X64:       # %bb.0:
; X64-NEXT:    movss %xmm0, -{{[0-9]+}}(%rsp)
; X64-NEXT:    flds -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.fpext.x86_fp80.f32(float %a,
                                                                         metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define x86_fp80 @fpext_f64_to_fp80(double %a) nounwind strictfp {
; X86-LABEL: fpext_f64_to_fp80:
; X86:       # %bb.0:
; X86-NEXT:    fldl {{[0-9]+}}(%esp)
; X86-NEXT:    retl
;
; X64-LABEL: fpext_f64_to_fp80:
; X64:       # %bb.0:
; X64-NEXT:    movsd %xmm0, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldl -{{[0-9]+}}(%rsp)
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.fpext.x86_fp80.f64(double %a,
                                                                         metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define float @fptrunc_fp80_to_f32(x86_fp80 %a) nounwind strictfp {
; X86-LABEL: fptrunc_fp80_to_f32:
; X86:       # %bb.0:
; X86-NEXT:    pushl %eax
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fstps (%esp)
; X86-NEXT:    flds (%esp)
; X86-NEXT:    popl %eax
; X86-NEXT:    retl
;
; X64-LABEL: fptrunc_fp80_to_f32:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fstps -{{[0-9]+}}(%rsp)
; X64-NEXT:    movss {{.*#+}} xmm0 = mem[0],zero,zero,zero
; X64-NEXT:    retq
  %ret = call float @llvm.experimental.constrained.fptrunc.f32.x86_fp80(x86_fp80 %a,
                                                                        metadata !"round.dynamic",
                                                                        metadata !"fpexcept.strict") #0
  ret float %ret
}

define double @fptrunc_fp80_to_f64(x86_fp80 %a) nounwind strictfp {
; X86-LABEL: fptrunc_fp80_to_f64:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    andl $-8, %esp
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    fldt 8(%ebp)
; X86-NEXT:    fstpl (%esp)
; X86-NEXT:    fldl (%esp)
; X86-NEXT:    movl %ebp, %esp
; X86-NEXT:    popl %ebp
; X86-NEXT:    retl
;
; X64-LABEL: fptrunc_fp80_to_f64:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fstpl -{{[0-9]+}}(%rsp)
; X64-NEXT:    movsd {{.*#+}} xmm0 = mem[0],zero
; X64-NEXT:    retq
  %ret = call double @llvm.experimental.constrained.fptrunc.f64.x86_fp80(x86_fp80 %a,
                                                                         metadata !"round.dynamic",
                                                                         metadata !"fpexcept.strict") #0
  ret double %ret
}

define x86_fp80 @fsqrt_fp80(x86_fp80 %a) nounwind strictfp {
; X86-LABEL: fsqrt_fp80:
; X86:       # %bb.0:
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fsqrt
; X86-NEXT:    retl
;
; X64-LABEL: fsqrt_fp80:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fsqrt
; X64-NEXT:    retq
  %ret = call x86_fp80 @llvm.experimental.constrained.sqrt.x86_fp80(x86_fp80 %a,
                                                                    metadata !"round.dynamic",
                                                                    metadata !"fpexcept.strict") #0
  ret x86_fp80 %ret
}

define i1 @fp80_to_sint1(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_sint1:
; X86:       # %bb.0:
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistps {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movb {{[0-9]+}}(%esp), %al
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_sint1:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistps -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movb -{{[0-9]+}}(%rsp), %al
; X64-NEXT:    retq
  %result = call i1 @llvm.experimental.constrained.fptosi.i1.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i1 %result
}

define i8 @fp80_to_sint8(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_sint8:
; X86:       # %bb.0:
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistps {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movb {{[0-9]+}}(%esp), %al
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_sint8:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistps -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movb -{{[0-9]+}}(%rsp), %al
; X64-NEXT:    retq
  %result = call i8 @llvm.experimental.constrained.fptosi.i8.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i8 %result
}

define i16 @fp80_to_sint16(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_sint16:
; X86:       # %bb.0:
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistps {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_sint16:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistps -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    retq
  %result = call i16 @llvm.experimental.constrained.fptosi.i16.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i16 %result
}

define i32 @fp80_to_sint32(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_sint32:
; X86:       # %bb.0: # %entry
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw (%esp)
; X86-NEXT:    movzwl (%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistpl {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw (%esp)
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_sint32:
; X64:       # %bb.0: # %entry
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistpl -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    retq
entry:
  %result = call i32 @llvm.experimental.constrained.fptosi.i32.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i32 %result
}

define i64 @fp80_to_sint64(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_sint64:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    .cfi_def_cfa_offset 8
; X86-NEXT:    .cfi_offset %ebp, -8
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    .cfi_def_cfa_register %ebp
; X86-NEXT:    andl $-8, %esp
; X86-NEXT:    subl $16, %esp
; X86-NEXT:    fldt 8(%ebp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistpll {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movl %ebp, %esp
; X86-NEXT:    popl %ebp
; X86-NEXT:    .cfi_def_cfa %esp, 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_sint64:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistpll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movq -{{[0-9]+}}(%rsp), %rax
; X64-NEXT:    retq
  %result = call i64 @llvm.experimental.constrained.fptosi.i64.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i64 %result
}

define i1 @fp80_to_uint1(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_uint1:
; X86:       # %bb.0:
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistps {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movb {{[0-9]+}}(%esp), %al
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_uint1:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistps -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movb -{{[0-9]+}}(%rsp), %al
; X64-NEXT:    retq
  %result = call i1 @llvm.experimental.constrained.fptoui.i1.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i1 %result
}

define i8 @fp80_to_uint8(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_uint8:
; X86:       # %bb.0:
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistps {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movb {{[0-9]+}}(%esp), %al
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_uint8:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistps -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movb -{{[0-9]+}}(%rsp), %al
; X64-NEXT:    retq
  %result = call i8 @llvm.experimental.constrained.fptoui.i8.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i8 %result
}

define i16 @fp80_to_uint16(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_uint16:
; X86:       # %bb.0:
; X86-NEXT:    subl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 12
; X86-NEXT:    fldt {{[0-9]+}}(%esp)
; X86-NEXT:    fnstcw (%esp)
; X86-NEXT:    movzwl (%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistpl {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw (%esp)
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    # kill: def $ax killed $ax killed $eax
; X86-NEXT:    addl $8, %esp
; X86-NEXT:    .cfi_def_cfa_offset 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_uint16:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistpl -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    # kill: def $ax killed $ax killed $eax
; X64-NEXT:    retq
  %result = call i16 @llvm.experimental.constrained.fptoui.i16.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i16 %result
}

define i32 @fp80_to_uint32(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_uint32:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    .cfi_def_cfa_offset 8
; X86-NEXT:    .cfi_offset %ebp, -8
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    .cfi_def_cfa_register %ebp
; X86-NEXT:    andl $-8, %esp
; X86-NEXT:    subl $16, %esp
; X86-NEXT:    fldt 8(%ebp)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    orl $3072, %eax # imm = 0xC00
; X86-NEXT:    movw %ax, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistpll {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl %ebp, %esp
; X86-NEXT:    popl %ebp
; X86-NEXT:    .cfi_def_cfa %esp, 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_uint32:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    orl $3072, %eax # imm = 0xC00
; X64-NEXT:    movw %ax, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistpll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movl -{{[0-9]+}}(%rsp), %eax
; X64-NEXT:    retq
  %result = call i32 @llvm.experimental.constrained.fptoui.i32.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i32 %result
}

define i64 @fp80_to_uint64(x86_fp80 %x) #0 {
; X86-LABEL: fp80_to_uint64:
; X86:       # %bb.0:
; X86-NEXT:    pushl %ebp
; X86-NEXT:    .cfi_def_cfa_offset 8
; X86-NEXT:    .cfi_offset %ebp, -8
; X86-NEXT:    movl %esp, %ebp
; X86-NEXT:    .cfi_def_cfa_register %ebp
; X86-NEXT:    andl $-8, %esp
; X86-NEXT:    subl $16, %esp
; X86-NEXT:    fldt 8(%ebp)
; X86-NEXT:    flds {{\.LCPI.*}}
; X86-NEXT:    fucom %st(1)
; X86-NEXT:    fnstsw %ax
; X86-NEXT:    xorl %edx, %edx
; X86-NEXT:    # kill: def $ah killed $ah killed $ax
; X86-NEXT:    sahf
; X86-NEXT:    setbe %al
; X86-NEXT:    fldz
; X86-NEXT:    ja .LBB18_2
; X86-NEXT:  # %bb.1:
; X86-NEXT:    fstp %st(0)
; X86-NEXT:    fldz
; X86-NEXT:    fxch %st(1)
; X86-NEXT:  .LBB18_2:
; X86-NEXT:    fstp %st(1)
; X86-NEXT:    fsubrp %st, %st(1)
; X86-NEXT:    fnstcw {{[0-9]+}}(%esp)
; X86-NEXT:    movzwl {{[0-9]+}}(%esp), %ecx
; X86-NEXT:    orl $3072, %ecx # imm = 0xC00
; X86-NEXT:    movw %cx, {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    fistpll {{[0-9]+}}(%esp)
; X86-NEXT:    fldcw {{[0-9]+}}(%esp)
; X86-NEXT:    movb %al, %dl
; X86-NEXT:    shll $31, %edx
; X86-NEXT:    xorl {{[0-9]+}}(%esp), %edx
; X86-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X86-NEXT:    movl %ebp, %esp
; X86-NEXT:    popl %ebp
; X86-NEXT:    .cfi_def_cfa %esp, 4
; X86-NEXT:    retl
;
; X64-LABEL: fp80_to_uint64:
; X64:       # %bb.0:
; X64-NEXT:    fldt {{[0-9]+}}(%rsp)
; X64-NEXT:    flds {{.*}}(%rip)
; X64-NEXT:    xorl %eax, %eax
; X64-NEXT:    fucomi %st(1), %st
; X64-NEXT:    setbe %al
; X64-NEXT:    fldz
; X64-NEXT:    fxch %st(1)
; X64-NEXT:    fcmovnbe %st(1), %st
; X64-NEXT:    fstp %st(1)
; X64-NEXT:    fsubrp %st, %st(1)
; X64-NEXT:    fnstcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    movzwl -{{[0-9]+}}(%rsp), %ecx
; X64-NEXT:    orl $3072, %ecx # imm = 0xC00
; X64-NEXT:    movw %cx, -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    fistpll -{{[0-9]+}}(%rsp)
; X64-NEXT:    fldcw -{{[0-9]+}}(%rsp)
; X64-NEXT:    shlq $63, %rax
; X64-NEXT:    xorq -{{[0-9]+}}(%rsp), %rax
; X64-NEXT:    retq
  %result = call i64 @llvm.experimental.constrained.fptoui.i64.x86_fp80(x86_fp80 %x,
                                               metadata !"fpexcept.strict") #0
  ret i64 %result
}

attributes #0 = { strictfp }
