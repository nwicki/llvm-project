// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

dfsan_label a_label;
dfsan_label b_label;
dfsan_label c_label;

void has_ab(dfsan_label label) {
	assert(dfsan_has_label(label,a_label));
	assert(dfsan_has_label(label,b_label));
}

void has_ac(dfsan_label label) {
	assert(dfsan_has_label(label,a_label));
	assert(dfsan_has_label(label,c_label));
}

void has_bc(dfsan_label label) {
	assert(dfsan_has_label(label,b_label));
	assert(dfsan_has_label(label,c_label));
}

void has_abc(dfsan_label label) {
	assert(dfsan_has_label(label,a_label));
	assert(dfsan_has_label(label,b_label));
	assert(dfsan_has_label(label,c_label));
}

int main(int argc, char const *argv[]) {
	int a = 0;
	int b = 0;
	int c = 0;
	a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	b_label = dfsan_create_label("b", 0);
	dfsan_set_label(b_label,&b,sizeof(b));
	c_label = dfsan_create_label("c", 0);
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
	ab_labels[i++] = dfsan_union(a_label,ab_label);
	ab_labels[i++] = dfsan_union(a_label,ba_label);
	ac_labels[j++] = dfsan_union(a_label,ac_label);
	ac_labels[j++] = dfsan_union(a_label,ca_label);
	abc_labels[l++] = dfsan_union(a_label,bc_label);
	abc_labels[l++] = dfsan_union(a_label,cb_label);

	ab_labels[i++] = dfsan_union(ab_label,a_label);
	ab_labels[i++] = dfsan_union(ba_label,a_label);
	ac_labels[j++] = dfsan_union(ac_label,a_label);
	ac_labels[j++] = dfsan_union(ca_label,a_label);
	abc_labels[l++] = dfsan_union(bc_label,a_label);
	abc_labels[l++] = dfsan_union(cb_label,a_label);

	// This section covers all cases where label b is either first or second argument.
	ab_labels[i++] = dfsan_union(b_label,ab_label);
	ab_labels[i++] = dfsan_union(b_label,ba_label);
	bc_labels[k++] = dfsan_union(b_label,bc_label);
	bc_labels[k++] = dfsan_union(b_label,cb_label);
	abc_labels[l++] = dfsan_union(b_label,ac_label);
	abc_labels[l++] = dfsan_union(b_label,ca_label);

	ab_labels[i++] = dfsan_union(ab_label,b_label);
	ab_labels[i++] = dfsan_union(ba_label,b_label);
	bc_labels[k++] = dfsan_union(bc_label,b_label);
	bc_labels[k++] = dfsan_union(cb_label,b_label);
	abc_labels[l++] = dfsan_union(ac_label,b_label);
	abc_labels[l++] = dfsan_union(ca_label,b_label);

	// This section covers all cases where label c is either first or second argument.
	ac_labels[j++] = dfsan_union(c_label,ac_label);
	ac_labels[j++] = dfsan_union(c_label,ca_label);
	bc_labels[k++] = dfsan_union(c_label,bc_label);
	bc_labels[k++] = dfsan_union(c_label,cb_label);
	abc_labels[l++] = dfsan_union(c_label,ab_label);
	abc_labels[l++] = dfsan_union(c_label,ba_label);

	ac_labels[j++] = dfsan_union(ac_label,c_label);
	ac_labels[j++] = dfsan_union(ca_label,c_label);
	bc_labels[k++] = dfsan_union(bc_label,c_label);
	bc_labels[k++] = dfsan_union(cb_label,c_label);
	abc_labels[l++] = dfsan_union(ab_label,c_label);
	abc_labels[l++] = dfsan_union(ba_label,c_label);


	// This function ensures that all labels of a union of 2 original labels
	// are not modified if one or the other component is again added.
	// The check of unions with all 3 labels should also work once dfsan_contains
	// is properly implemented. 
	// We also check that every union has the labels it was created from.
	for(int m = 0; m < size; m++) {
		assert(ab_labels[0] == ab_labels[m]);
		assert(ac_labels[0] == ac_labels[m]);
		assert(bc_labels[0] == bc_labels[m]);
		assert(abc_labels[0] == abc_labels[m]);
		has_ab(ab_labels[m]);
		has_ac(ac_labels[m]);
		has_bc(bc_labels[m]);
		has_abc(abc_labels[m]);
	}
	for(int m = size; m < l; m++) {
		assert(abc_labels[0] == abc_labels[m]);
		has_abc(abc_labels[m]);
	}
	
	assert(abc_labels[0] == abc_labels[5]);

	// Here, we check that no union of two original labels equals a union of
	// two/three other original labels.
	assert(ab_labels[0] != ac_labels[0]);
	assert(ab_labels[0] != bc_labels[0]);
	assert(ab_labels[0] != abc_labels[0]);
	assert(ac_labels[0] != bc_labels[0]);
	assert(ac_labels[0] != abc_labels[0]);
	assert(bc_labels[0] != abc_labels[0]);

	// Then we check that if we take the union of unions with 2 labels not existing of
	// the same labels does not equal one or the other union since we are adding another
	// label to the union.
	assert(dfsan_union(ab_labels[0],ac_labels[0]) != ac_labels[0]);
	assert(dfsan_union(ac_labels[0],ab_labels[0]) != ac_labels[0]);
	assert(dfsan_union(ab_labels[0],ac_labels[0]) != ab_labels[0]);
	assert(dfsan_union(ac_labels[0],ab_labels[0]) != ab_labels[0]);
	assert(dfsan_union(ab_labels[0],bc_labels[0]) != bc_labels[0]);
	assert(dfsan_union(bc_labels[0],ab_labels[0]) != bc_labels[0]);
	assert(dfsan_union(ab_labels[0],bc_labels[0]) != ab_labels[0]);
	assert(dfsan_union(bc_labels[0],ab_labels[0]) != ab_labels[0]);
	assert(dfsan_union(ab_labels[0],abc_labels[0]) != ab_labels[0]);
	assert(dfsan_union(abc_labels[0],ab_labels[0]) != ab_labels[0]);

	assert(dfsan_union(ac_labels[0],bc_labels[0]) != bc_labels[0]);
	assert(dfsan_union(bc_labels[0],ac_labels[0]) != bc_labels[0]);
	assert(dfsan_union(ac_labels[0],bc_labels[0]) != ac_labels[0]);
	assert(dfsan_union(bc_labels[0],ac_labels[0]) != ac_labels[0]);
	assert(dfsan_union(ac_labels[0],abc_labels[0]) != ac_labels[0]);
	assert(dfsan_union(abc_labels[0],ac_labels[0]) != ac_labels[0]);

	assert(dfsan_union(bc_labels[0],abc_labels[0]) != bc_labels[0]);
	assert(dfsan_union(abc_labels[0],bc_labels[0]) != bc_labels[0]);

	return 0;
}