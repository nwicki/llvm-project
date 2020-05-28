// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <sanitizer/dfsan_interface.h>
#include <assert.h>
#include <stdlib.h>
  
dfsan_label m_label;
dfsan_label n_label;

int main(int argc, char **argv)
{
  int m = 10;
  int n = 10;
  m_label = dfsan_create_label("m", 0);
  n_label = dfsan_create_label("n", 0);
  dfsan_set_label(m_label, &m, sizeof(m));
  dfsan_set_label(n_label, &n, sizeof(n));

  // First case: pure dataflow effect
  int sum = 0;
  sum = m*n;
  dfsan_label label = dfsan_get_label(sum);
  assert(dfsan_has_label(label, m_label));
  assert(dfsan_has_label(label, n_label));

  // Second case: control-flow, one loop
  // Tainted by main loop, label m*n
  int sum2_1 = 0;
  for(int i = 0; i < m*n; ++i)
    sum2_1++;
  label = dfsan_get_label(sum2_1);
  assert(dfsan_has_label(label, m_label));
  assert(dfsan_has_label(label, n_label));
  
  // Second case, other verson: control-flow + data-flow
  // Tainted by main loop, label m
  int sum2_2 = 0;
  for(int i = 0; i < m; ++i)
    sum2_2 += n;
  label = dfsan_get_label(sum2_2);
  assert(dfsan_has_label(label, m_label));
  assert(dfsan_has_label(label, n_label));
  
  // Third case: control-flow, two loops
  // Tainted twice, labels m and n
  int sum3 = 0;
  for(int i = 0; i < m; ++i)
    for(int j = 0; j < n; ++j)
      sum3++;
  label = dfsan_get_label(sum3);
  assert(dfsan_has_label(label, m_label));
  assert(dfsan_has_label(label, n_label));

  // Fourth case: control-flow only on inner loop
  // Tainted once, only through a label n of inner loop
  for(int i = 0; i < m; ++i) {
    int sum4 = 0;
    for(int j = 0; j < n; ++j)
      sum4++;
    label = dfsan_get_label(sum4);
  }
  assert(dfsan_has_label(label, n_label));
  // Fifth case: control-flow through store
  // Tainted once, with a combined label m*n
  int* val = (int*) calloc(1,sizeof(int));
  for(int i = 0; i < m; ++i) {
    for(int j = 0; j < n; ++j)
      (*val)++;
  }
  label = dfsan_get_label(*val);
  assert(dfsan_has_label(label, m_label));
  assert(dfsan_has_label(label, n_label));
  free(val);
  return 0;
}
 