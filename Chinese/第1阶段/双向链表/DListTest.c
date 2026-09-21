#include <stdio.h>
#include "DList.h"
#include "DList.c"
/* ==================== 测试辅助 ==================== */

static void section(const char *title){
    printf("\n================ %s ================\n", title);
}

/* 用正向迭代器收集：从 head->next 到 tail */
static size_t collect_fwd(DList *list, int *buf, size_t cap){
    size_t n = 0;
    int v;
    dlist_iter_begin(list);
    while(n < cap && dlist_iter_next(list, &v)) buf[n++] = v;
    return n;
}

/* 用反向迭代器收集：从 tail->prev 到 head */
static size_t collect_bwd(DList *list, int *buf, size_t cap){
    size_t n = 0;
    int v;
    dlist_iter_begin_back(list);
    while(n < cap && dlist_iter_prev(list, &v)) buf[n++] = v;
    return n;
}

static void print_arr(const char *tag, const int *buf, size_t n){
    printf("%s[", tag);
    for(size_t i = 0; i < n; i++){
        printf("%d", buf[i]);
        if(i + 1 < n) printf(", ");
    }
    printf("]\n");
}

static int same_arr(const int *a, const int *b, size_t n){
    for(size_t i = 0; i < n; i++) if(a[i] != b[i]) return 0;
    return 1;
}

/* 校验：正向 == exp_fwd，反向 == exp_fwd 的逆序 */
static void check(DList *list, const int *exp_fwd, size_t n){
    int exp_bwd[128];
    for(size_t i = 0; i < n; i++) exp_bwd[i] = exp_fwd[n - 1 - i];

    int buf[128];
    size_t gf = collect_fwd(list, buf, 128);
    int ok_f = (gf == n) && same_arr(buf, exp_fwd, n);
    if(!ok_f){ printf("  [FAIL] 正向: 期望 "); print_arr("", exp_fwd, n);
               printf("              得到 "); print_arr("", buf, gf); }

    size_t gb = collect_bwd(list, buf, 128);
    int ok_b = (gb == n) && same_arr(buf, exp_bwd, n);
    if(!ok_b){ printf("  [FAIL] 反向: 期望 "); print_arr("", exp_bwd, n);
               printf("              得到 "); print_arr("", buf, gb); }

    if(ok_f && ok_b){ printf("  [PASS] "); print_arr("", exp_fwd, n); }
}

/* ==================== main ==================== */

int main(void){
    int v;

    /* ---------- 1. create / is_empty / size ---------- */
    section("1. create / is_empty / size");
    DList *L = dlist_create();
    if(!L){ printf("create failed\n"); return 1; }
    printf("is_empty = %d (expect 1)\n", dlist_is_empty(L));
    printf("size     = %zu (expect 0)\n", dlist_size(L));

    /* ---------- 2. push_front / push_back ---------- */
    section("2. push_front / push_back");
    dlist_push_back (L, 10);
    dlist_push_back (L, 20);
    dlist_push_back (L, 30);
    dlist_push_front(L, 5);
    { int e[] = {5, 10, 20, 30}; check(L, e, 4); }
    printf("size = %zu (expect 4)\n", dlist_size(L));

    /* ---------- 3. insert ---------- */
    section("3. insert");
    dlist_insert(L, 0, 1);                     /* 头插 */
    dlist_insert(L, dlist_size(L), 99);        /* 尾插 */
    dlist_insert(L, 3, 77);                    /* 中间插 */
    { int e[] = {1, 5, 10, 77, 20, 30, 99}; check(L, e, 7); }
    printf("size = %zu (expect 7)\n", dlist_size(L));

    /* ---------- 4. get / set ---------- */
    section("4. get / set");
    dlist_get(L, 0, &v);                   printf("get(0)    = %d (expect 1)\n",  v);
    dlist_get(L, 3, &v);                   printf("get(3)    = %d (expect 77)\n", v);
    dlist_get(L, dlist_size(L)-1, &v);     printf("get(last) = %d (expect 99)\n", v);
    dlist_set(L, 3, 88);
    dlist_get(L, 3, &v);                   printf("after set(3,88): get(3) = %d (expect 88)\n", v);
    { int e[] = {1, 5, 10, 88, 20, 30, 99}; check(L, e, 7); }

    /* ---------- 5. index_of / contains ---------- */
    section("5. index_of / contains");
    printf("index_of(88)  = %d (expect 3)\n",  dlist_index_of(L, 88));
    printf("index_of(999) = %d (expect -1)\n", dlist_index_of(L, 999));
    printf("contains(88)  = %d (expect 1)\n",  dlist_contains(L, 88));
    printf("contains(999) = %d (expect 0)\n",  dlist_contains(L, 999));

    /* ---------- 6. 迭代器：正反双向 ---------- */
    section("6. iterator (fwd & bwd)");
    {
        int sum = 0;
        printf("fwd: ");
        dlist_iter_begin(L);
        while(dlist_iter_next(L, &v)){ printf("%d ", v); sum += v; }
        printf("\nsum = %d (expect 253)\n", sum);

        printf("bwd: ");
        dlist_iter_begin_back(L);
        while(dlist_iter_prev(L, &v)) printf("%d ", v);
        printf("\n");
    }

    /* ---------- 7. pop_front / pop_back ---------- */
    section("7. pop_front / pop_back");
    dlist_pop_front(L, &v); printf("pop_front = %d (expect 1)\n", v);
    dlist_pop_back (L, &v); printf("pop_back  = %d (expect 99)\n", v);
    { int e[] = {5, 10, 88, 20, 30}; check(L, e, 5); }

    /* ---------- 8. remove_at ---------- */
    section("8. remove_at");
    dlist_remove_at(L, 0, &v);                     printf("remove_at(0)    = %d (expect 5)\n", v);
    dlist_remove_at(L, dlist_size(L)-1, &v);       printf("remove_at(last) = %d (expect 30)\n", v);
    { int e[] = {10, 88, 20}; check(L, e, 3); }
    dlist_remove_at(L, 1, &v);                     printf("remove_at(1)    = %d (expect 88)\n", v);
    { int e[] = {10, 20}; check(L, e, 2); }

    /* ---------- 9. remove_value ---------- */
    section("9. remove_value");
    dlist_insert(L, 1, 88);                        /* 重建 [10, 88, 20] */
    { int e[] = {10, 88, 20}; check(L, e, 3); }
    printf("remove_value(88)  = %d (expect 0)\n",  dlist_remove_value(L, 88));
    printf("remove_value(999) = %d (expect -2)\n", dlist_remove_value(L, 999));
    { int e[] = {10, 20}; check(L, e, 2); }
    dlist_remove_value(L, 10);                     /* 删头 */
    { int e[] = {20}; check(L, e, 1); }
    dlist_remove_value(L, 20);                     /* 删唯一 */
    printf("after remove all: is_empty=%d (expect 1), size=%zu (expect 0)\n",
           dlist_is_empty(L), dlist_size(L));

    /* ---------- 10. reverse ---------- */
    section("10. reverse");
    dlist_push_back(L, 10);
    dlist_push_back(L, 20);
    dlist_push_back(L, 30);
    dlist_push_back(L, 40);
    dlist_push_back(L, 50);
    { int e[] = {10, 20, 30, 40, 50}; check(L, e, 5); }
    dlist_reverse(L);
    { int e[] = {50, 40, 30, 20, 10}; check(L, e, 5); }
    /* 单元素 reverse */
    dlist_clear(L);
    dlist_push_back(L, 7);
    dlist_reverse(L);
    { int e[] = {7}; check(L, e, 1); }

    /* ---------- 11. clone（验证深拷贝） ---------- */
    section("11. clone");
    dlist_clear(L);
    dlist_push_back(L, 1);
    dlist_push_back(L, 2);
    dlist_push_back(L, 3);
    DList *C = dlist_clone(L);
    { int e[] = {1, 2, 3};
      printf("original: "); check(L, e, 3);
      printf("clone   : "); check(C, e, 3); }

    dlist_set(C, 0, -1);                           /* 只改 clone */
    { int eo[] = {1, 2, 3};
      int ec[] = {-1, 2, 3};
      printf("--- 修改 clone 后 ---\n");
      printf("original: "); check(L, eo, 3);
      printf("clone   : "); check(C, ec, 3); }
    dlist_destroy(C);

    /* ---------- 12. clear（清空后仍可用） ---------- */
    section("12. clear");
    dlist_clear(L);
    printf("after clear: size=%zu (expect 0), is_empty=%d (expect 1)\n",
           dlist_size(L), dlist_is_empty(L));
    dlist_push_back(L, 42);
    dlist_push_back(L, 43);
    { int e[] = {42, 43}; check(L, e, 2); }

    /* ---------- 13. print_forward / print_backward ---------- */
    section("13. print_forward / print_backward");
    printf("forward : "); dlist_print_forward(L);
    printf("backward: "); dlist_print_backward(L);

    /* ---------- 14. destroy ---------- */
    section("14. destroy");
    dlist_destroy(L);
    printf("destroyed OK\n");

    /* ---------- 15. 边界 ---------- */
    section("15. 边界情况");
    DList *E = dlist_create();
    printf("empty: pop_front      = %d (expect -2)\n", dlist_pop_front(E, &v));
    printf("empty: pop_back       = %d (expect -2)\n", dlist_pop_back (E, &v));
    printf("empty: get(0)         = %d (expect -2)\n", dlist_get(E, 0, &v));
    printf("empty: remove_at(0)   = %d (expect -2)\n", dlist_remove_at(E, 0, &v));
    printf("empty: remove_value(1)= %d (expect -2)\n", dlist_remove_value(E, 1));
    printf("empty: index_of(1)    = %d (expect -1)\n", dlist_index_of(E, 1));
    printf("empty: contains(1)    = %d (expect 0)\n",  dlist_contains(E, 1));

    /* 空表正反迭代 */
    dlist_iter_begin(E);
    printf("empty: iter_next = %d (expect 0)\n", dlist_iter_next(E, &v));
    dlist_iter_begin_back(E);
    printf("empty: iter_prev = %d (expect 0)\n", dlist_iter_prev(E, &v));

    /* NULL 安全 */
    dlist_destroy(NULL);
    dlist_clear(NULL);
    printf("NULL: size=%zu, is_empty=%d (expect 0, 1)\n",
           dlist_size(NULL), dlist_is_empty(NULL));
    printf("NULL: iter_next = %d (expect 0)\n", dlist_iter_next(NULL, &v));

    /* dnode_init 显式调用（公开接口） */
    DNode a, b;
    dnode_init(&a, &b, NULL, 100);
    dnode_init(&b, NULL, &a, 200);
    printf("dnode_init: a.value=%d a.next=%d a.prev=%s, b.value=%d\n",
           a.value, a.next->value, a.prev == NULL ? "NULL" : "非空", b.value);

    dlist_destroy(E);

    printf("\n=== ALL TESTS DONE ===\n");
    return 0;
}