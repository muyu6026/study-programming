#ifndef DLIST_H_
#define DLIST_H_

#include <stddef.h>
#include <stdbool.h>

/* ==================== 数据结构 ==================== */

/* 双向链表节点：数据 + 前驱指针 + 后继指针 */
typedef struct DNode {
    int           value;  /* 该节点存的数据 */
    struct DNode *prev;   /* 指向前一个节点 */
    struct DNode *next;   /* 指向后一个节点 */
} DNode;

/* 双向链表本体（双哨兵设计）：
 *   head    —— 头哨兵（不存数据，永不删），head->next 是第一个真节点
 *   tail    —— 尾哨兵（不存数据，永不删），tail->prev 是最后一个真节点
 *   length  —— 真实元素个数（不含两个哨兵）
 *   current —— 迭代器游标
 *
 * 空表状态：head ⇄ tail（两个哨兵直接相连）
 */
typedef struct DList {
    DNode  *head;     /* 头哨兵，永远非 NULL */
    DNode  *tail;     /* 尾哨兵，永远非 NULL */
    size_t  length;   /* 元素个数 */
    DNode  *current;  /* 当前遍历位置 */
} DList;

/*节点初始化*/
void dnode_init(DNode* node,DNode* next,DNode* prev,int value);

/* ==================== 生命周期 ==================== */

/* 创建一个空双向链表（malloc 结构体 + 两个哨兵）。
 * 返回：成功返回链表指针；内存不足返回 NULL。
 * 调用者最终必须用 dlist_destroy 释放。 */
DList *dlist_create(void);

/* 销毁整个链表：释放所有真节点 + 两个哨兵 + 结构体本身。
 * 参数 list：要销毁的链表；NULL 安全。 */
void dlist_destroy(DList *list);

/* 清空所有真节点，保留哨兵和结构体，可继续使用。
 * 参数 list：要清空的链表；NULL 安全。
 * 清空后长度归 0，头尾哨兵重新相连。 */
void dlist_clear(DList *list);


/* ==================== 插入 ==================== */

/* 头插：在第一个真节点之前插入 value。
 * 参数 list ：目标链表（不能为 NULL）。
 * 参数 value：要存入的值。
 * 返回：0 成功；-1 参数为空；-2 malloc 失败。
 * 复杂度：O(1)。 */
int dlist_push_front(DList *list, int value);

/* 尾插：在最后一个真节点之后追加 value。
 * 参数 list ：目标链表。
 * 参数 value：要存入的值。
 * 返回：0 成功；-1 参数为空；-2 malloc 失败。
 * 复杂度：O(1)（靠 tail 哨兵直接定位尾部）。 */
int dlist_push_back(DList *list, int value);

/* 在下标 index 处插入 value（新节点占据 index 位置，原 index 及之后后移）。
 * 参数 list ：目标链表。
 * 参数 index：插入位置，从 0 开始；index == length 表示尾插。
 * 参数 value：要存入的值。
 * 返回：0 成功；-1 参数为空；-2 index 越界（index > length）；-3 malloc 失败。 */
int dlist_insert(DList *list, size_t index, int value);


/* ==================== 删除 ==================== */

/* 删除头节点（第一个真元素），值通过 *out 带出。
 * 参数 list：目标链表。
 * 参数 out ：接收被删值，可为 NULL。
 * 返回：0 成功；-1 参数为空；-2 链表为空。
 * 复杂度：O(1)。 */
int dlist_pop_front(DList *list, int *out);

/* 删除尾节点（最后一个真元素），值通过 *out 带出。
 * 参数 list：目标链表。
 * 参数 out ：接收被删值，可为 NULL。
 * 返回：0 成功；-1 参数为空；-2 链表为空。
 * 复杂度：O(1)。 */
int dlist_pop_back(DList *list, int *out);

/* 删除下标为 index 的节点，值通过 *out 带出。
 * 参数 list ：目标链表。
 * 参数 index：要删除的下标，从 0 开始。
 * 参数 out  ：接收被删值，可为 NULL。
 * 返回：0 成功；-1 参数为空；-2 index 越界（index >= length）。
 * 提示：实现时可从较近的一端（head 或 tail）出发，平均少走一半。 */
int dlist_remove_at(DList *list, size_t index, int *out);

/* 删除第一个值等于 value 的节点。
 * 参数 list ：目标链表。
 * 参数 value：要匹配的值。
 * 返回：0 成功；-1 参数为空；-2 没找到。
 * 优势：找到节点即可 O(1) 摘除，不需要另找前驱。 */
int dlist_remove_value(DList *list, int value);


/* ==================== 访问 / 查询 ==================== */

/* 读取下标 index 处的值，通过 *out 带出。
 * 参数 list ：目标链表（只读）。
 * 参数 index：下标，从 0 开始。
 * 参数 out  ：输出参数，必须非 NULL。
 * 返回：0 成功；-1 参数为空；-2 index 越界。 */
int dlist_get(const DList *list, size_t index, int *out);

/* 把下标 index 处的值改为 value。
 * 参数 list ：目标链表。
 * 参数 index：下标，从 0 开始。
 * 参数 value：新值。
 * 返回：0 成功；-1 参数为空；-2 index 越界。 */
int dlist_set(DList *list, size_t index, int value);

/* 查找第一个值等于 value 的元素下标。
 * 参数 list ：目标链表（只读）。
 * 参数 value：要查找的值。
 * 返回：找到返回下标（>=0）；没找到或 list 为 NULL 返回 -1。 */
int dlist_index_of(const DList *list, int value);

/* 判断链表中是否存在 value。
 * 参数 list ：目标链表（只读）。
 * 参数 value：要查找的值。
 * 返回：存在 true；否则 false。 */
bool dlist_contains(const DList *list, int value);


/* ==================== 状态 ==================== */

/* 返回元素个数（不含哨兵）。
 * 参数 list：目标链表（只读）；NULL 返回 0。 */
size_t dlist_size(const DList *list);

/* 判断链表是否为空。
 * 参数 list：目标链表（只读）；NULL 视为空。 */
bool dlist_is_empty(const DList *list);


/* ==================== 遍历 / 高级 ==================== */

/* 从头到尾打印所有元素（正向）。
 * 参数 list：目标链表（只读）。 */
void dlist_print_forward(const DList *list);

/* 从尾到头打印所有元素（反向，双向链表独有）。
 * 参数 list：目标链表（只读）。 */
void dlist_print_backward(const DList *list);

/* 原地反转整个链表。
 * 参数 list：目标链表。
 * 返回：0 成功；-1 参数为空。
 * 注意：两个哨兵位置不动，只反转真节点的连接方向。 */
int dlist_reverse(DList *list);

/* 深拷贝一份新链表（新节点、新内存，独立于原链表）。
 * 参数 list：源链表（只读）。
 * 返回：新链表指针（调用者负责 dlist_destroy）；失败返回 NULL。 */
DList *dlist_clone(const DList *list);


/* ==================== 迭代器（基于 current 游标） ==================== */

/* 把 current 重置到第一个真节点（head->next），准备正向遍历。
 * 参数 list：目标链表；NULL 安全。 */
void dlist_iter_begin(DList *list);

/* 把 current 重置到最后一个真节点（tail->prev），准备反向遍历。
 * 参数 list：目标链表；NULL 安全。
 * 双向链表独有。 */
void dlist_iter_begin_back(DList *list);

/* 取 current 的值到 *out，然后 current 前进一格（向 next 方向）。
 * 参数 list：目标链表。
 * 参数 out ：接收当前值，可为 NULL。
 * 返回：读到返回 true；已到末尾返回 false。 */
bool dlist_iter_next(DList *list, int *out);

/* 取 current 的值到 *out，然后 current 后退一格（向 prev 方向）。
 * 参数 list：目标链表。
 * 参数 out ：接收当前值，可为 NULL。
 * 返回：读到返回 true；已到开头返回 false。
 * 双向链表独有。 */
bool dlist_iter_prev(DList *list, int *out);

#endif /* DLIST_H_ */