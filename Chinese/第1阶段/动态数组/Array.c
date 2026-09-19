#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Array.h"
// typedef struct Array{
//     int* array;
//     size_t length;
//     size_t max;
// }Array;
Array array_create(int init_size){
    Array A;
    if(init_size > 0){
        
    A.length = 0;
    A.max = init_size;
    A.array = (int*)malloc(A.max * sizeof(int));
    if (A.array == NULL) {
        A.length = 0;
        A.max    = 0;
    }
    memset(A.array, 0 , A.max * sizeof(int));
}
    return A;
}
void array_free(Array *arr){
    if(arr == NULL) return;
    free(arr->array);
    arr->array = NULL;
    arr->length = 0;
    arr->max = 0;
}
size_t array_size(const Array *a){
    if(a == NULL) return 0;
    return a->length;
}
int array_add(Array* a,int value){
    if(a == NULL) return -1;
    if(array_size(a) + 1 >= a->max){
        int code = array_inflate(a);
        if(code != 0){return code;}
    }
    a->array[a->length] = value;
    a->length++;
    return 0;
}
int array_inflate(Array* a){
    if(a == NULL) return -1;

    const size_t elem = sizeof(int);

    //检测 增加的个数 与 int的字节数相乘会不会溢出；
    if(a->max > SIZE_MAX / elem) return -2;
    size_t old_size = array_size(a);

    //检测数组的长度 与 int的字节数相乘会不会溢出；
    if(old_size > SIZE_MAX / elem)return -3;

    //相乘安全可做计算
    size_t old_byte = a->max * elem;
    size_t add_byte = a->max * elem;

    //检测相加是否溢出
    if(add_byte > SIZE_MAX - old_byte) return -4;
    
    size_t new_max = a->max * 2;
    size_t new_byte = new_max * elem;

    int* A = realloc(a->array,new_byte);

    if(A == NULL )return -5;

    memset(A + old_size , 0 , add_byte);

    a->array = A;

    a->max = new_max;

    return 0;
}
int main(){
    Array A = array_create(1);
    int i ;
    for(i=0;i<100;i++){
        int code = array_add(&A,i);
        if(code != 0){
            printf("code = %d\n",code);
            break;
        }
    }
    printf("array length : %zu\n",array_size(&A));
    i=0;
    for(i=0;i<100;i++){
        printf("in %d is %d\n",i,A.array[i]);
    }
    array_free(&A);

    printf("fred array length : %zu\n",array_size(&A));
    //于2026/9/17完成
    return 0;
}