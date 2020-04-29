// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int a = 0;
	int b = 0;
	int c = 0;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	dfsan_label b_label = dfsan_create_label("b", 0);
	dfsan_set_label(b_label,&b,sizeof(b));
	dfsan_label c_label = dfsan_create_label("c", 0);
	dfsan_set_label(c_label,&c,sizeof(c));

	dfsan_label ab_label = dfsan_union(a_label,b_label);
	dfsan_label ba_label = dfsan_union(b_label,a_label);
	dfsan_label ac_label = dfsan_union(a_label,c_label);
	dfsan_label ca_label = dfsan_union(c_label,a_label);
	dfsan_label bc_label = dfsan_union(b_label,c_label);
	dfsan_label cb_label = dfsan_union(c_label,b_label);

	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int size = 10;

	dfsan_label ab_labels[size];
	dfsan_label ac_labels[size];
	dfsan_label bc_labels[size];
	dfsan_label abc_labels[size+2];

	ab_labels[i++] = ab_label;
	ab_labels[i++] = ba_label;
	ac_labels[j++] = ac_label;
	ac_labels[j++] = ca_label;
	bc_labels[k++] = bc_label;
	bc_labels[k++] = cb_label;

	// Here, we create all unions of the original labels with unions of
	// 2 other original labels.

	// They are split in 3 sections.

	// This section covers all cases where label a is either first or second argument.
	assert(dfsan_contains(a_label,ab_label) || dfsan_contains(ab_label,a_label));
	ab_labels[i++] = dfsan_union(a_label,ab_label);
	assert(dfsan_contains(a_label,ba_label) || dfsan_contains(ba_label,a_label));
	ab_labels[i++] = dfsan_union(a_label,ba_label);
	assert(dfsan_contains(a_label,ac_label) || dfsan_contains(ac_label,a_label));
	ac_labels[j++] = dfsan_union(a_label,ac_label);
	assert(dfsan_contains(a_label,ca_label) || dfsan_contains(ca_label,a_label));
	ac_labels[j++] = dfsan_union(a_label,ca_label);
	assert(!dfsan_contains(a_label,bc_label) || dfsan_contains(bc_label,a_label));
	abc_labels[l++] = dfsan_union(a_label,bc_label);
	assert(!dfsan_contains(a_label,cb_label) || dfsan_contains(cb_label,a_label));
	abc_labels[l++] = dfsan_union(a_label,cb_label);

	ab_labels[i++] = dfsan_union(ab_label,a_label);
	ab_labels[i++] = dfsan_union(ba_label,a_label);
	ac_labels[j++] = dfsan_union(ac_label,a_label);
	ac_labels[j++] = dfsan_union(ca_label,a_label);
	abc_labels[l++] = dfsan_union(bc_label,a_label);
	abc_labels[l++] = dfsan_union(cb_label,a_label);

	// This section covers all cases where label b is either first or second argument.
	assert(dfsan_contains(ab_label,b_label) || dfsan_contains(b_label,ab_label));
	ab_labels[i++] = dfsan_union(b_label,ab_label);
	assert(dfsan_contains(ba_label,b_label) || dfsan_contains(b_label,ba_label));
	ab_labels[i++] = dfsan_union(b_label,ba_label);
	assert(dfsan_contains(bc_label,b_label) || dfsan_contains(b_label,bc_label));
	bc_labels[k++] = dfsan_union(b_label,bc_label);
	assert(dfsan_contains(cb_label,b_label) || dfsan_contains(b_label,cb_label));
	bc_labels[k++] = dfsan_union(b_label,cb_label);
	assert(!dfsan_contains(ac_label,b_label) || dfsan_contains(b_label,ac_label));
	abc_labels[l++] = dfsan_union(b_label,ac_label);
	assert(!dfsan_contains(ca_label,b_label) || dfsan_contains(b_label,ca_label));
	abc_labels[l++] = dfsan_union(b_label,ca_label);

	ab_labels[i++] = dfsan_union(ab_label,b_label);
	ab_labels[i++] = dfsan_union(ba_label,b_label);
	bc_labels[k++] = dfsan_union(bc_label,b_label);
	bc_labels[k++] = dfsan_union(cb_label,b_label);
	abc_labels[l++] = dfsan_union(ac_label,b_label);
	abc_labels[l++] = dfsan_union(ca_label,b_label);

	// This section covers all cases where label c is either first or second argument.
	assert(dfsan_contains(ac_label,c_label) || dfsan_contains(c_label,ac_label));
	ac_labels[j++] = dfsan_union(c_label,ac_label);
	assert(dfsan_contains(ca_label,c_label) || dfsan_contains(c_label,ca_label));
	ac_labels[j++] = dfsan_union(c_label,ca_label);
	assert(dfsan_contains(bc_label,c_label) || dfsan_contains(c_label,bc_label));
	bc_labels[k++] = dfsan_union(c_label,bc_label);
	assert(dfsan_contains(cb_label,c_label) || dfsan_contains(c_label,cb_label));
	bc_labels[k++] = dfsan_union(c_label,cb_label);
	assert(!dfsan_contains(ab_label,c_label) || dfsan_contains(c_label,ab_label));
	abc_labels[l++] = dfsan_union(c_label,ab_label);
	assert(!dfsan_contains(ba_label,c_label) || dfsan_contains(c_label,ba_label));
	abc_labels[l++] = dfsan_union(c_label,ba_label);

	ac_labels[j++] = dfsan_union(ac_label,c_label);
	ac_labels[j++] = dfsan_union(ca_label,c_label);
	bc_labels[k++] = dfsan_union(bc_label,c_label);
	bc_labels[k++] = dfsan_union(cb_label,c_label);
	abc_labels[l++] = dfsan_union(ab_label,c_label);
	abc_labels[l++] = dfsan_union(ba_label,c_label);
	return 0;
}