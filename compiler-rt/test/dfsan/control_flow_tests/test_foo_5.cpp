// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int foo(int a)
{
	int x = 0;
	if(a)
	{
		x = 5;
	}
	else
	{
		x = 3;
	}
	int sum = 0;
	for (int i = 0; i < x; ++i)
	{
		sum += i;
	}
	assert(dfsan_has_label(dfsan_get_label(x),dfsan_get_label(a)));
	assert(dfsan_has_label(dfsan_get_label(sum),dfsan_get_label(x)));
	return sum;
}

int main(int argc, char** argv)
{
	int a = 1;
	int b = 2;
	int c = 3;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	dfsan_label b_label = dfsan_create_label("b", 0);
	dfsan_set_label(b_label,&b,sizeof(b));
	dfsan_label c_label = dfsan_create_label("c", 0);
	dfsan_set_label(c_label,&c,sizeof(c));
	int sum_a = 0;
	int sum_b = 0;
	int sum_c = 0;
	for (int i = 0; i < a; ++i)
	{
		sum_a += 1;
		for (int j = 0; j < b; ++j)
		{
			sum_b += foo(c);
		}
		for (int k = 0; k < c; ++k)
		{
			sum_c += foo(b);
		}
	}
	assert(dfsan_has_label(dfsan_get_label(sum_a),a_label));
	assert(dfsan_has_label(dfsan_get_label(sum_b),a_label));
	assert(dfsan_has_label(dfsan_get_label(sum_b),b_label));
	assert(dfsan_has_label(dfsan_get_label(sum_b),c_label));
	assert(dfsan_has_label(dfsan_get_label(sum_c),a_label));
	assert(dfsan_has_label(dfsan_get_label(sum_c),b_label));
	assert(dfsan_has_label(dfsan_get_label(sum_c),c_label));
	return 0;
}