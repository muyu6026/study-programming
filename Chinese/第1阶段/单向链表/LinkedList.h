#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool   */

/* ==================== 数据结构 ==================== */

/* 链表节点：一个 value + 一个指向下一个节点的指针 */
typedef struct Node {
    int          value;   /* 该节点存的数据 */
    struct Node *Next;    /* 指向下一个节点，尾节点为 NULL */
} Node;

/* 单链表本体：
 *   Head     —— 哨兵头节点（不存数据，永不删除），Head->Next 才是第一个真节点
 *   Length   —— 真实元素个数（不含哨兵）
 *   Current  —— 迭代器游标，配合 iter_* 系列函数使用
 */
typedef struct LinkedList {
    Node  *Head;          /* 哨兵头，永远非 NULL */
    size_t Length;        /* 元素个数 */
    Node  *Current;       /* 当前遍历位置，可被 iter 函数修改 */
    Node* Tail;           /* 指向当前链表的尾部节点 */
} LinkedList;

/* ==================== 生命周期 ==================== */

/* 创建一个空链表（内部会 malloc 结构体 + 一个哨兵头节点）。
 * 返回：成功返回堆上的链表指针；内存不足返回 NULL。
 * 调用者最终必须用 list_destroy 释放。 */
LinkedList *list_create(void);

/* 销毁整个链表：释放所有真节点 + 哨兵 + 链表结构体本身。
 * 参数 list：要销毁的链表；传 NULL 安全（什么都不做）。
 * 调用后 list 指针失效，不能再使用。 */
void list_destroy(LinkedList *list);

/* 清空所有真节点，但保留链表结构和哨兵，可继续使用。
 * 参数 list：要清空的链表；传 NULL 安全。
 * 清空后 list_size(list) == 0，链表回到刚 create 的状态。 */
void list_clear(LinkedList *list);


/* ==================== 插入 ==================== */

/* 头插：在链表最前面插入 value。
 * 参数 list ：目标链表（不能为 NULL）。
 * 参数 value：要存入的整数值。
 * 返回：0 成功；-1 参数为空；-2 malloc 失败。 */
int list_push_front(LinkedList *list, int value);

/* 尾插：在链表最后面追加 value。
 * 参数 list ：目标链表。
 * 参数 value：要存入的值。
 * 返回：0 成功；-1 参数为空；-2 malloc 失败。
 * 注意：无尾指针时是 O(n)，要遍历到尾。 */
int list_push_back(LinkedList *list, int value);

/* 在指定下标处插入 value（新节点占据 index 位置，原 index 及之后元素后移）。
 * 参数 list ：目标链表。
 * 参数 index：插入位置，从 0 开始；index == Length 等价于尾插。
 * 参数 value：要存入的值。
 * 返回：0 成功；-1 参数为空；-2 index 越界（index > Length）；-3 malloc 失败。 */
int list_insert(LinkedList *list, size_t index, int value);


/* ==================== 删除 ==================== */

/* 删除头节点（第一个真元素），把它的值通过 *out 带出。
 * 参数 list：目标链表。
 * 参数 out ：输出参数，用来接收被删除节点的 value；允许传 NULL（不接收值）。
 * 返回：0 成功；-1 参数为空；-2 链表为空，无可删。 */
int list_pop_front(LinkedList *list, int *out);

/* 删除尾节点（最后一个真元素），值通过 *out 带出。
 * 参数 list：目标链表。
 * 参数 out ：接收被删值，可为 NULL。
 * 返回：0 成功；-1 参数为空；-2 链表为空。 */
int list_pop_back(LinkedList *list, int *out);

/* 删除下标为 index 的节点，值通过 *out 带出。
 * 参数 list ：目标链表。
 * 参数 index：要删除的下标，从 0 开始。
 * 参数 out  ：接收被删值，可为 NULL。
 * 返回：0 成功；-1 参数为空；-2 index 越界（index >= Length）。 */
int list_remove_at(LinkedList *list, size_t index, int *out);

/* 删除第一个值等于 value 的节点（只删一个）。
 * 参数 list ：目标链表。
 * 参数 value：要匹配的值。
 * 返回：0 成功；-1 参数为空；-2 没找到该值。 */
int list_remove_value(LinkedList *list, int value);


/* ==================== 访问 / 查询 ==================== */

/* 读取下标 index 处的值，通过 *out 带出。
 * 参数 list ：目标链表（只读，不被修改）。
 * 参数 index：下标，从 0 开始。
 * 参数 out  ：输出参数，必须非 NULL，用来接收读到的值。
 * 返回：0 成功；-1 参数为空（list 或 out）；-2 index 越界。 */
int list_get(const LinkedList *list, size_t index, int *out);

/* 把下标 index 处的值改成 value。
 * 参数 list ：目标链表。
 * 参数 index：下标，从 0 开始。
 * 参数 value：新的值。
 * 返回：0 成功；-1 参数为空；-2 index 越界。 */
int list_set(LinkedList *list, size_t index, int value);

/* 查找第一个值等于 value 的元素下标。
 * 参数 list ：目标链表（只读）。
 * 参数 value：要查找的值。
 * 返回：找到返回下标（>=0）；没找到返回 -2；list 为 NULL 返回 -1。 */
int list_index_of(const LinkedList *list, int value);

/* 判断链表中是否存在值 value。
 * 参数 list ：目标链表（只读）。
 * 参数 value：要查找的值。
 * 返回：存在 true；不存在或 list 为 NULL 返回 false。 */
bool list_contains(const LinkedList *list, int value);


/* ==================== 状态 ==================== */

/* 返回链表中真实元素的个数（不含哨兵）。
 * 参数 list：目标链表（只读）；NULL 时返回 0。 */
size_t list_size(const LinkedList *list);

/* 判断链表是否为空。
 * 参数 list：目标链表（只读）；NULL 视为空，返回 true。 */
bool list_is_empty(const LinkedList *list);


/* ==================== 遍历 / 高级 ==================== */

/* 从头到尾打印所有元素（调试用）。
 * 参数 list：目标链表（只读）。 */
void list_print(const LinkedList *list);

/* 原地反转整个链表（Head 之后的所有节点逆序）。
 * 参数 list：目标链表。
 * 返回：0 成功；-1 参数为空。
 * 注意：哨兵 Head 位置不变，只反转真节点。 */
int list_reverse(LinkedList *list);

/* 深拷贝一份新链表（新节点、新内存，与原链表完全独立）。
 * 参数 list：被拷贝的源链表（只读）。
 * 返回：成功返回新链表指针（调用者负责 list_destroy）；list 为 NULL 或内存不足返回 NULL。 */
LinkedList *list_clone(const LinkedList *list);


/* ==================== 迭代器（基于 Current 游标） ==================== */

/* 把 Current 重置到第一个真节点（即 Head->Next）。
 * 参数 list：目标链表；NULL 安全。
 * 调用后配合 list_iter_next 依次取值。 */
void list_iter_begin(LinkedList *list);

/* 取当前 Current 节点的值到 *out，然后 Current 前进一格。
 * 参数 list：目标链表。
 * 参数 out ：接收当前值，可为 NULL。
 * 返回：成功读到值返回 true；已到末尾（Current 为 NULL）返回 false。 */
bool list_iter_next(LinkedList *list, int *out);

#endif /* LINKEDLIST_H_ */