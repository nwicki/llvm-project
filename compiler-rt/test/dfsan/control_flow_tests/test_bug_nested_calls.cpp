// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <stdio.h>
#include <assert.h>
#include <sanitizer/dfsan_interface.h>

int x1 = 2;
int x2 = 3;
dfsan_label f11_label = 0;
dfsan_label f12_label = 0;
dfsan_label f13_label = 0;
dfsan_label f21_label = 0;
dfsan_label f22_label = 0;
dfsan_label f23_label = 0;
dfsan_label f31_label = 0;
dfsan_label f32_label = 0;
dfsan_label f33_label = 0;

void f1(int x1, int x2)
{
  int tmp = 0;
  for(int i = 0; i < 100; i += 1) {
    tmp += i*1.1;
  }
  if(x2 == 3) {
    f11_label = dfsan_get_label(tmp);
  }
  else if(x2 == 2) {
    f12_label = dfsan_get_label(tmp);
  }
  else {
    f13_label = dfsan_get_label(tmp);
  }
}
void f2(int x1, int x2)
{
  int tmp = 0;
  for(int i = x1; i < x2; i += 1) {
    tmp += i*1.1;
  }
  if(x2 == 3) {
    f21_label = dfsan_get_label(tmp);
  }
  else if(x2 == 2) {
    f22_label = dfsan_get_label(tmp);
  }
  else {
    f23_label = dfsan_get_label(tmp);
  }
}
void f3(int x1, int x2)
{
  int tmp = 0;
  for(int i = x1; i < 20; i += 1) 
    tmp += i*1.1;
  if(x2 == 3) {
    f31_label = dfsan_get_label(tmp);
  }
  else if(x2 == 2) {
    f32_label = dfsan_get_label(tmp);
  }
  else {
    f33_label = dfsan_get_label(tmp);
  }
}

void g(int x1, int x2)
{
  f1(x1, x2);
  f2(x1, x2);
  f3(x1, x2);
}
void g2(int x1, int x2)
{
  f1(x1, x2);
  f2(x1, x2);
  f3(x1, x2);
}

int f(int x1, int x2)
{
  int tmp = 0;
  for(int i = 0; i < x2; i += 1) {
    tmp += i*1.1;
    for(int j = 0; j < x1; j += 1) {
      g(x1, x2);
    }
    g2(x2, x1);
  }
  g(x1, 100);
  return 100 * x1 * x2 + 2;
}

int main(int argc, char ** argv)
{
  dfsan_label x1_label = dfsan_create_label("x1", 0);
  dfsan_set_label(x1_label,&x1,sizeof(x1));
  dfsan_label x2_label = dfsan_create_label("x2", 0);
  dfsan_set_label(x2_label,&x2,sizeof(x2));
  int result = f(x1, x2);
  assert(dfsan_has_label(dfsan_get_label(result),x1_label));
  assert(dfsan_has_label(dfsan_get_label(result),x2_label));
  assert(!dfsan_has_label(f11_label,x1_label));
  assert(!dfsan_has_label(f11_label,x2_label));
  assert(!dfsan_has_label(f12_label,x1_label));
  assert(!dfsan_has_label(f12_label,x2_label));
  assert(!dfsan_has_label(f13_label,x1_label));
  assert(!dfsan_has_label(f13_label,x2_label));
  assert(dfsan_has_label(f21_label,x1_label));
  assert(dfsan_has_label(f21_label,x2_label));
  assert(!dfsan_has_label(f22_label,x1_label));
  assert(!dfsan_has_label(f22_label,x2_label));
  assert(dfsan_has_label(f23_label,x1_label));
  assert(!dfsan_has_label(f23_label,x2_label));
  assert(dfsan_has_label(f31_label,x1_label));
  assert(!dfsan_has_label(f31_label,x2_label));
  assert(!dfsan_has_label(f32_label,x1_label));
  assert(dfsan_has_label(f32_label,x2_label));
  assert(dfsan_has_label(f33_label,x1_label));
  assert(!dfsan_has_label(f33_label,x2_label));
  return 0;
}
