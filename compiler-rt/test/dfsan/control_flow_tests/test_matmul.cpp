// RUN: %clang_dfsan -mllvm -dfsan-cfsan-enable %s -o %t && %run %t
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sanitizer/dfsan_interface.h>
#include <assert.h>

dfsan_label label_scalar = 0;

typedef struct _matrix
{
    size_t rows;
    size_t cols;
    double * data;
} matrix;

matrix create_matrix(int rows, int cols, bool init)
{
    matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (double*) calloc(rows * cols, sizeof(double));
    if(init) {
        size_t size = rows * cols;
        for(size_t i = 0; i < size; ++i) {
            m.data[i] = rand() % 100;
        }
    }
    return m;
}

matrix mat_mul(matrix * left, matrix * right)
{
    size_t a = left->rows, b = left->cols, c = right->cols;
    matrix res = create_matrix(a, c, false);
    for(size_t i = 0; i < a; ++i) {
        for(size_t j = 0; j < c; ++j) {
            double scalar_product = 0.0;
            for(size_t cc = 0; cc < b; ++cc) {
                scalar_product += left->data[i*b + cc] * right->data[cc*c + j];
                label_scalar = dfsan_get_label(scalar_product);
            }
            res.data[i*c + j] = scalar_product;
        }
    }

    return res;
}

int main(int argc, char ** argv)
{
    int a = 2;
    int b = 2;
    int c = 2;
    dfsan_label a_label = dfsan_create_label("a", 0);
    dfsan_set_label(a_label,&a,sizeof(a));
    dfsan_label b_label = dfsan_create_label("b", 0);
    dfsan_set_label(b_label,&b,sizeof(b));
    dfsan_label c_label = dfsan_create_label("c", 0);
    dfsan_set_label(c_label,&c,sizeof(c));

    matrix left = create_matrix(a, b, true);
    matrix right = create_matrix(b, c, true);
    matrix res = mat_mul(&left, &right);
    assert(dfsan_has_label(label_scalar,a_label));
    assert(dfsan_has_label(label_scalar,b_label));
    assert(dfsan_has_label(label_scalar,c_label));

    free(left.data);
    free(right.data);
    free(res.data);

    return 0;
}
