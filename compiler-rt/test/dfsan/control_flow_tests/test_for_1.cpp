// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 1;
	int b = 2;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	dfsan_label b_label = dfsan_create_label("b", 0);
	dfsan_set_label(b_label,&b,sizeof(b));
	int sum = 0;
	for (int i = 0; i < a; ++i)
	{
		for (int j = 0; j < b; ++j)
		{
			sum += i*j;
		}
	}
	dfsan_label sum_label = dfsan_get_label(sum);
	assert(dfsan_has_label(dfsan_union(a_label,b_label),sum_label));
	return 0;
}