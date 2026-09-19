#include <stdio.h>
#include "LinkedList.c"
static void section(const char *title) {
    printf("\n================ %s ================\n", title);
}

int main(void) {
    int v;

    /* ========== 1. create / is_empty / size ========== */
    section("1. create / is_empty / size");
    LinkedList *L = list_create();
    if (L == NULL) { printf("create failed\n"); return 1; }
    printf("is_empty = %d (expect 1)\n", list_is_empty(L));
    printf("size     = %zu (expect 0)\n", list_size(L));

    /* ========== 2. push_front / push_back ========== */
    section("2. push_front / push_back");
    list_push_front(L, 10);   /* [10] */
    list_push_back (L, 20);   /* [10, 20] */
    list_push_back (L, 30);   /* [10, 20, 30] */
    list_push_front(L, 5);    /* [5, 10, 20, 30] */
    list_print(L);
    printf("size = %zu (expect 4)\n", list_size(L));

    /* ========== 3. insert ========== */
    section("3. insert");
    list_insert(L, 0, 1);                    /* [1, 5, 10, 20, 30] */
    list_insert(L, list_size(L), 99);        /* [1, 5, 10, 20, 30, 99] 尾插 */
    list_insert(L, 3, 77);                   /* [1, 5, 10, 77, 20, 30, 99] */
    list_print(L);
    printf("size = %zu (expect 7)\n", list_size(L));

    /* ========== 4. get / set ========== */
    section("4. get / set");
    list_get(L, 0, &v);                     printf("get(0)    = %d (expect 1)\n", v);
    list_get(L, 3, &v);                     printf("get(3)    = %d (expect 77)\n", v);
    list_get(L, list_size(L) - 1, &v);      printf("get(last) = %d (expect 99)\n", v);
    list_set(L, 3, 88);                     /* 77 -> 88 */
    list_get(L, 3, &v);                     printf("after set(3,88): get(3) = %d (expect 88)\n", v);
    list_print(L);

    /* ========== 5. index_of / contains ========== */
    section("5. index_of / contains");
    printf("index_of(88)  = %d (expect 3)\n",  list_index_of(L, 88));
    printf("index_of(999) = %d (expect -2)\n", list_index_of(L, 999));
    printf("contains(88)  = %d (expect 1)\n",  list_contains(L, 88));
    printf("contains(999) = %d (expect 0)\n",  list_contains(L, 999));

    /* ========== 6. 迭代�? ========== */
    section("6. iterator (iter_begin / iter_next)");
    printf("iterate: ");
    int sum = 0;
    list_iter_begin(L);
    while (list_iter_next(L, &v)) {
        printf("%d ", v);
        sum += v;
    }
    printf("\nsum = %d\n", sum);

    /* ========== 7. pop_front / pop_back ========== */
    section("7. pop_front / pop_back");
    list_pop_front(L, &v);  printf("pop_front = %d (expect 1)\n", v);
    list_pop_back (L, &v);  printf("pop_back  = %d (expect 99)\n", v);
    list_print(L);

    /* ========== 8. remove_at ========== */
    section("8. remove_at");
    list_print(L);
    list_remove_at(L, 0, &v);                 printf("remove_at(0)    = %d\n", v);
    list_remove_at(L, list_size(L) - 1, &v);  printf("remove_at(last) = %d\n", v);
    list_print(L);

    /* ========== 9. remove_value ========== */
    section("9. remove_value");
    list_print(L);
    printf("remove_value(88)  = %d (expect 0)\n",  list_remove_value(L, 88));
    printf("remove_value(999) = %d (expect -2)\n", list_remove_value(L, 999));
    list_print(L);

    /* ========== 10. reverse ========== */
    section("10. reverse");
    printf("before reverse: "); list_print(L);
    list_reverse(L);
    printf("after  reverse: "); list_print(L);

    /* ========== 11. clone ========== */
    section("11. clone");
    LinkedList *C = list_clone(L);
    printf("original: "); list_print(L);
    printf("clone   : "); list_print(C);
    list_set(C, 0, -1);       /* 只改 clone */
    printf("--- 修改 clone �? ---\n");
    printf("original: "); list_print(L);   /* 不受影响 */
    printf("clone   : "); list_print(C);
    list_destroy(C);

    /* ========== 12. clear ========== */
    section("12. clear");
    list_clear(L);
    printf("after clear: size = %zu (expect 0), is_empty = %d (expect 1)\n",
           list_size(L), list_is_empty(L));
    /* clear 后仍可继续使�? */
    list_push_back(L, 42);
    list_push_back(L, 43);
    printf("after push 42,43: "); list_print(L);

    /* ========== 13. destroy ========== */
    section("13. destroy");
    list_destroy(L);
    printf("destroyed OK\n");

    /* ========== 14. 边界情况 ========== */
    section("14. 边界情况");
    LinkedList *E = list_create();

    /* 空表操作 */
    printf("empty: pop_front = %d (expect -2)\n", list_pop_front(E, &v));
    printf("empty: pop_back  = %d (expect -2)\n", list_pop_back (E, &v));
    printf("empty: get(0)    = %d (expect -2)\n", list_get(E, 0, &v));
    printf("empty: remove_at(0) = %d (expect -2)\n", list_remove_at(E, 0, &v));
    printf("empty: remove_value(1) = %d (expect -2)\n", list_remove_value(E, 1));
    printf("empty: index_of(1) = %d (expect -2)\n", list_index_of(E, 1));
    printf("empty: contains(1) = %d (expect 0)\n",  list_contains(E, 1));

    /* 单元素：反转 */
    list_push_back(E, 7);
    list_reverse(E);
    printf("single elem after reverse: "); list_print(E);

    /* 参数 NULL 安全�? */
    list_destroy(NULL);
    list_clear(NULL);
    list_print(NULL);
    printf("NULL safe: size=%zu, is_empty=%d\n",
           list_size(NULL), list_is_empty(NULL));
    list_iter_begin(NULL);
    printf("NULL iter_next = %d (expect 0)\n", list_iter_next(NULL, &v));

    list_destroy(E);

    return 0;
}