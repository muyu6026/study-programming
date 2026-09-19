#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "DList.h"

// /* 双向链表节点：数据 + 前驱指针 + 后继指针 */
// typedef struct DNode {
//     int           value;  /* 该节点存的数据 */
//     struct DNode *prev;   /* 指向前一个节点 */
//     struct DNode *next;   /* 指向后一个节点 */
// } DNode;

// /* 双向链表本体（双哨兵设计）：
//  *   head    —— 头哨兵（不存数据，永不删），head->next 是第一个真节点
//  *   tail    —— 尾哨兵（不存数据，永不删），tail->prev 是最后一个真节点
//  *   length  —— 真实元素个数（不含两个哨兵）
//  *   current —— 迭代器游标
//  *
//  * 空表状态：head ⇄ tail（两个哨兵直接相连）
//  */
// typedef struct DList {
//     DNode  *head;     /* 头哨兵，永远非 NULL */
//     DNode  *tail;     /* 尾哨兵，永远非 NULL */
//     size_t  length;   /* 元素个数 */
//     DNode  *current;  /* 当前遍历位置 */
// } DList;


/* ==================== 节点初始化 ==================== */

void dnode_init(DNode* node,DNode* next,DNode* prev,int value){
    node->next = next;
    node->prev = prev;
    node->value = value;
}
/* ==================== 生命周期 ==================== */

DList *dlist_create(void){
    //创建双向链表
    DList* NewDList = malloc(sizeof(DList));

    if(NewDList == NULL) return NULL;

    NewDList->head = malloc(sizeof(DNode));
    if(NewDList->head == NULL){
        free(NewDList);
        return NULL;
    }

    NewDList->tail = malloc(sizeof(DNode));
    if(NewDList->tail == NULL){
        free(NewDList->head);
        free(NewDList);
        return NULL;
    }

    // head和tail相互连接
    dnode_init(NewDList->head,NewDList->tail,NULL,0);
    dnode_init(NewDList->tail,NULL,NewDList->head,0);

    //游标初始化为指向head
    NewDList->current = NewDList->head;

    NewDList->length = 0;

    return NewDList;
}

void dlist_destroy(DList *list){
    if(list == NULL) return;

    DNode* N = list->head->next;
    while(N != list->tail){
        DNode* next = N->next; //提前存好下一个节点
        free(N);
        N = next;
    }

    free(list->head);
    free(list->tail);
    free(list);
}

void dlist_clear(DList *list){
    if(list == NULL) return;

    DNode* N = list->head->next;
    while(N != list->tail){
        DNode* Next = N->next;
        free(N);
        N = Next;
    }

    dnode_init(list->head,list->tail,NULL,0);
    dnode_init(list->tail,NULL,list->head,0);

    list->length = 0;
    list->current = list->head;

}

/* ==================== 插入 ==================== */

int dlist_push_front(DList *list, int value){
    if(list == NULL) return -1;

    DNode* new_node = malloc(sizeof(DNode));
    if(new_node == NULL) return -2;

    DNode* old_last = list->head->next;

    new_node -> next = old_last;
    new_node -> prev = list->head;
    new_node ->value = value;

    old_last -> prev = new_node;

    list->head->next = new_node;
    list->length++;

    return 0;
}

int dlist_push_back(DList *list, int value){
    if(list == NULL) return -1;

    DNode* new_node = malloc(sizeof(DNode));
    if(new_node == NULL) return -2;

    DNode* old_last = list->tail->prev;

    new_node->next = list->tail;
    new_node->prev = old_last;
    new_node->value = value;

    old_last->next = new_node;

    list->tail->prev = new_node;
    list->length++;

    return 0;

}

int dlist_insert(DList *list, size_t index, int value){
    if(list == NULL) return -1;
    if(index > list->length) return -2;

    if(index == list->length) return dlist_push_back(list,value);

    DNode* new_node = malloc(sizeof(DNode));
    if(new_node == NULL) return -3;

    DNode* N = list->head->next;
    for(size_t i = 0; i < index; i++){
        N = N->next;
    }
    DNode* old_last = N->prev;

    new_node->value = value;
    new_node->next = N;
    new_node->prev = old_last;

    old_last->next = new_node;
    N->prev = new_node;

    list->length++;

    return 0;
}

/* ==================== 删除 ==================== */

int dlist_pop_front(DList *list, int *out);

int dlist_pop_back(DList *list, int *out);

int dlist_remove_at(DList *list, size_t index, int *out);

int dlist_remove_value(DList *list, int value);