// RUN: %clang_dfsan %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 0;
	int b = 0;
	int c = 0;
	int x = 0;
	int y = 0;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	dfsan_label b_label = dfsan_create_label("b", 0);
	dfsan_set_label(b_label,&b,sizeof(b));
	dfsan_label c_label = dfsan_create_label("c", 0);
	dfsan_set_label(c_label,&c,sizeof(c));
	if(a)
	{
		x = 1;
	}
	else if(b)
	{
		x = 2;
	}
	else if(c)
	{
		x = 3;
	}
	else 
	{
		x = 4;
	}
	y = 2;
	dfsan_label x_label = dfsan_get_label(x);
	dfsan_label y_label = dfsan_get_label(y);
	assert(dfsan_has_label(x_label,dfsan_union(c_label,dfsan_union(a_label,b_label))));
	assert(!dfsan_has_label(y_label,a_label));
	assert(!dfsan_has_label(y_label,b_label));
	assert(!dfsan_has_label(y_label,c_label));
	return 0;
}