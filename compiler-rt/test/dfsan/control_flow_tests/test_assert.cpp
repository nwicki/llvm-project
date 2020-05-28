// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <assert.h>
#include <sanitizer/dfsan_interface.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int a = 5;
	dfsan_label label_a = dfsan_create_label("a", 0);
	dfsan_set_label(label_a,&a,sizeof(a));
	int b;
	if(a) {
		b = 42;
		assert(dfsan_get_label(b) != label_a);
	}
	else {
		b = 21;
		assert(dfsan_get_label(b) != label_a);
	}
	assert(dfsan_get_label(b) != label_a);
	return 0;
}