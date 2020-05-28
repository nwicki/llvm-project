// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 1;
	int x = 0;
	int y = 0;
	dfsan_label a_label = dfsan_create_label("a", 0);
	dfsan_set_label(a_label,&a,sizeof(a));
	switch(a)
	{
		case 1:
			x = 5;
		case 2:
			x = 5;
		case 3:
			x = 5;
		case 4:
			x = 5;
		case 5:
			x = 5;
		break;
		default:
			x = 3;
	}
	y = 2;
	dfsan_label x_label = dfsan_get_label(x);
	dfsan_label y_label = dfsan_get_label(y);
	assert(dfsan_has_label(a_label,x_label));
	assert(!dfsan_has_label(a_label,y_label));
	return 0;
}