// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 1;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	int sum = 0;
	int i = 0;
	while (i < a)
	{
		sum += i;
		i++;
	}
	dfsan_label sum_label = dfsan_get_label(sum);
	assert(dfsan_has_label(a_label,sum_label));
	return 0;
}