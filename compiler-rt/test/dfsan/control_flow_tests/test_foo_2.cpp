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
	return x;
}

int main(int argc, char** argv)
{
	int a = 1;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	int x = foo(a);
	dfsan_label x_label = dfsan_get_label(x);
	assert(dfsan_has_label(a_label,x_label));
	return 0;
}