#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
// /* 链表节点：一个 value + 一个指向下一个节点的指针 */
// typedef struct Node {
//     int          value;   /* 该节点存的数据 */
//     struct Node *Next;    /* 指向下一个节点，尾节点为 NULL */
// } Node;

// /* 单链表本体：
//  *   Head     —— 哨兵头节点（不存数据，永不删除），Head->Next 才是第一个真节点
//  *   Length   —— 真实元素个数（不含哨兵）
//  *   Current  —— 迭代器游标，配合 iter_* 系列函数使用
//  *        Node* Tail;  —— 指向当前链表的尾部节点 
//  */
// typedef struct LinkedList {
//     Node  *Head;          /* 哨兵头，永远非 NULL */
//     size_t Length;        /* 元素个数 */
//     Node  *Current;       /* 当前遍历位置，可被 iter 函数修改 */
//     Node* Tail;           /* 指向当前链表的尾部节点 */
// } LinkedList;

LinkedList* list_create(void){
    //申请新的内存给链表
    LinkedList* Lin = malloc(sizeof(LinkedList));
    if(Lin == NULL ){
        printf("函数:%s,在给链表申请内存时失败",__func__);
        return NULL;
    }

    //申请新的内存给哨兵节点
    Lin->Head = malloc(sizeof(Node));
    if(Lin->Head == NULL) {
        //内存申请失败 销毁链表的内存并返回NULL
        free(Lin);
        printf("函数:%s,在给哨兵节点申请内存时失败",__func__);
        return NULL;
    }

    Lin->Head->value = 0;
    Lin->Head->Next = NULL;

    Lin->Tail = Lin->Head; 
    Lin->Length = 0;
    Lin->Current = Lin->Head;
    printf("函数:%s,创建链表成功，地址为：%p",__func__,Lin);
    return Lin;
}

void list_destroy(LinkedList *list){
    if(list == NULL) return;

    Node* N = list->Head->Next;
    
    while(N != NULL){
        Node* Next = N->Next;
        free(N);
        N = Next;
    }
    free(list->Head);
    free(list);
    return ;
}

void list_clear(LinkedList *list){
    if(list == NULL) return;

    Node* N = list->Head->Next;
    while(N != NULL){
        Node* Next = N->Next;
        free(N);
        N = Next;
    }
    list->Head->Next = NULL;
    list->Tail = list->Head;
    list->Length = 0;
    list->Current = list->Head;

    return;
}

int list_push_front(LinkedList *list, int value){
    if(list == NULL ) return -1;

    Node* New_node = malloc(sizeof(Node));

    if(New_node == NULL) return -2;

    New_node->Next = list->Head->Next;

    New_node->value = value;

    list->Head->Next = New_node;

    if(list->Tail == list->Head){
        list->Tail = New_node;
    }

    list->Length++;

    return 0;
}

int list_push_back(LinkedList *list, int value){
    if(list == NULL) return -1;

    Node* New_Node = malloc(sizeof(Node));

    if(New_Node == NULL) return -2;

    New_Node->Next = NULL;
    New_Node->value = value;

    list->Tail->Next = New_Node;
    list->Tail = New_Node;

    list->Length++;

    return 0;

}

int list_insert(LinkedList *list, size_t index, int value){
    if(list == NULL) return -1;
    if(index > list->Length) return -2; 
 
    if(index == 0) return list_push_front(list,value);
    if(index==list->Length) return list_push_back(list,value);

    Node* New_Node = malloc(sizeof(Node));
    if(New_Node == NULL) return -3;

    Node* p = list->Head;

    for(size_t i = 0 ; i < index;i++){
        p = p->Next;
    }

    New_Node->Next = p->Next;
    New_Node->value = value;

    p->Next = New_Node;
    list->Length++;

    return 0;
}

int list_pop_front(LinkedList *list, int *out){
    if(list == NULL) return -1;
    if(list->Head->Next == NULL) return -2;

    Node* N = list->Head->Next; 

    list->Head->Next = N->Next;

    if(N == list->Tail) list->Tail = list->Head;
    

    if(out != NULL) *out = N->value;

    free(N);

    list->Length--;

    return 0;
}

int list_pop_back(LinkedList *list, int *out){
    if(list == NULL) return -1;
    if(list->Head->Next == NULL) return -2;

    Node* N1 = list->Tail;

    if(out != NULL) *out = list->Tail->value;

    if(list->Head->Next == list->Tail){

        list->Head->Next = NULL;
        list->Tail = list->Head;
    }else{
        Node* N2 = list->Head;
        while(N2->Next != list->Tail){
            N2 = N2->Next;
        }
        N2->Next = NULL;
        list->Tail = N2;
    }
    free(N1);

    list->Length--;

    return 0;
}

int list_remove_at(LinkedList *list, size_t index, int *out){
    if(list == NULL) return -1;
    if(index >= list->Length ) return -2; //索引越界

    if(index == 0 ) return list_pop_front(list,out) ; //删除头元素(第一个真值)
    if(index == list->Length-1) return list_pop_back(list,out);//删除尾元素(这是最后一个真值)

    Node* N1 = list->Head;
    Node* N2;
    for(size_t i=0;i < index;i++){
        N1 = N1->Next;
    }
    N2 = N1->Next;
    N1->Next = N2->Next;

    if(N2 == list->Tail) list->Tail = N1;
    if(out != NULL) *out = N2->value;

    free(N2);

    list->Length--;

    return 0;
}

int list_remove_value(LinkedList *list, int value){
    if(list == NULL) return -1;

    if(list->Head->Next == NULL ) return -2; //空表

    Node* N1 = list->Head;
    Node* N2;
    while(N1->Next != NULL){
        if(N1->Next->value == value){

            N2 = N1->Next;

            N1->Next = N2->Next;

            if(N2 == list->Tail) list->Tail = N1;

            free(N2);

            list->Length--;

            return 0;
        }
        N1 = N1->Next;
    }
    return -2;
}

int list_get(const LinkedList *list, size_t index, int *out){
    if(list == NULL || out == NULL) return -1;
    if(index >= list->Length) return -2;

    Node* N1 = list->Head->Next;

    for(size_t i = 0;i < index ; i++){
       N1 = N1->Next;
    }
    *out = N1->value;
    return 0;
}

int list_set(LinkedList *list, size_t index, int value){
    if(list == NULL) return -1;
    if(index >= list->Length) return -2;

    Node* N1 = list->Head->Next;
    for(size_t i =0 ; i < index ; i++){
        N1 = N1->Next;
    }
    N1->value = value;
    return 0;
}

int list_index_of(const LinkedList *list, int value){
    if(list == NULL) return -1;
    
    int i = 0;
    for(Node* N = list->Head->Next; N != NULL ; N = N->Next){
        if(N->value == value){
            return i;
        }
        i++;
    }
    return -2; //找不到
}
bool list_contains(const LinkedList *list, int value){
    //写法1： return list_index_of(list,value) >= 0; (太简单，我先重写一遍)
    //写法2：
    if(list == NULL )return false;
    
    for(Node *P = list->Head->Next ; P->Next !=NULL;P = P->Next){
        if(P->value == value) return true;
    }
    return false; //没找到
}

size_t list_size(const LinkedList *list){
    if(list == NULL) return 0;
    return list->Length;
}

bool list_is_empty(const LinkedList *list){
    if(list == NULL) return true;
    return list->Length == 0;
}

void list_print(const LinkedList *list){
    if(list == NULL ) return;
    int i = 0;
    for(Node*p = list->Head->Next ; p != NULL ; p = p->Next){
        printf("LinkedList all value in %d is %d \n",i,p->value);
        i++;
    }
}

int list_reverse(LinkedList *list){
    if(list == NULL) return -1;
    Node* prev = NULL;  //已反转段的头（curr->Next 要指向它）
    Node* curr = list->Head->Next; //正在处理的节点
    Node* old_first = curr;// 原来的第一个节点，反转后会变成尾

    while(curr != NULL){
        Node* next = curr->Next; //先保存的下一个（防丢）
        curr->Next = prev;
        prev = curr;
        curr = next;
    }

    list->Head->Next = prev;
    list->Tail = (old_first != NULL)?old_first:list->Head;

    return 0;
}

LinkedList* list_clone(const LinkedList *list){
    if(list == NULL) return NULL;

    LinkedList* New_List = list_create();
    if(New_List == NULL) return NULL;

    Node* N = list->Head->Next;

    while(N != NULL){
        //方法1：list_push_back(New_List,N->value);
        //方法2：
        Node* New_N = malloc(sizeof(Node));

        if(New_N == NULL) {
            list_destroy(New_List);
            return NULL;
        } 

        New_N->Next = NULL;

        New_N->value = N->value;
        
        New_List->Tail->Next = New_N;//维护 Tail = Head 同时也更新尾部下一个节点的指向(Tail->Next = New_N)
        New_List->Tail = New_N;

        New_List->Length++;

        N = N->Next;
    }

    return New_List;
}

void list_iter_begin(LinkedList *list){
    if(list == NULL) return;
    list->Current = list->Head->Next;
    return;
}

bool list_iter_next(LinkedList *list, int *out){
    if(list == NULL) return false;

    if(list->Current == NULL) return false;

    if(out != NULL) *out = list->Current->value;

    list->Current = list->Current->Next;

    return true;
    
}

