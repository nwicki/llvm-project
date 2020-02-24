// RUN: %clang_dfsan %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 0;
	int x = 0;
	int y = 0;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	if(a)
	{
		x = 5;
	}
	else 
	{
		x = 3;
	}
	y = 2;
	dfsan_label x_label = dfsan_get_label(x);
	dfsan_label y_label = dfsan_get_label(y);
	assert(dfsan_has_label(a_label,x_label));
	assert(!dfsan_has_label(a_label,y_label));
	return 0;
}