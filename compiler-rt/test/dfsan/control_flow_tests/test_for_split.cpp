// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 5;
	int b = 10;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	dfsan_label b_label = dfsan_create_label("b", 0);
	dfsan_set_label(b_label,&b,sizeof(b));
	int sum_a = 0;
	int sum_b = 0;
	dfsan_label j_label = 0;
	for (int i = 0; i < a; ++i)
	{
		sum_a += 1;
		for (int j = 0; j < b; ++j)
		{
			sum_b += 1;
			j_label = dfsan_get_label(j);
		}
	}
	dfsan_label sum_a_label = dfsan_get_label(sum_a);
	dfsan_label sum_b_label = dfsan_get_label(sum_b);
	assert(!dfsan_has_label(j_label,a_label));
	assert(dfsan_has_label(j_label,b_label));
	assert(dfsan_has_label(sum_a_label,a_label));
	assert(!dfsan_has_label(sum_a_label,b_label));
	assert(dfsan_has_label(sum_b_label,a_label));
	assert(dfsan_has_label(sum_b_label,b_label));
	return 0;
}