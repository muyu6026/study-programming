#ifndef ARRAY_H_
#define ARRAY_H_
#include <stddef.h>    // size_t
typedef struct Array{
    int* array;
    size_t length;
    size_t max;
}Array;
Array array_create(int init_size);
void array_free(Array *arr);
size_t array_size(const Array *a);
int array_add(Array* a,int index);
int array_inflate(Array* a);

#endif