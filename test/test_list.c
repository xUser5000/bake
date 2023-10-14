#include "common.h"
#include "utest.h"
#include "list.h"

UTEST_MAIN();

#define CNT 10L

UTEST(List, ListPushBack) {
    list_t *list = list_init();
    ASSERT_EQ(list_size(list), 0);

    for (long i = 0; i < CNT; i++) {
        list_push_back(list, (void*) i);
        ASSERT_EQ(list_size(list), (size_t) i+1);
        ASSERT_EQ(list->tail->val, (void*) i);
    }
    ASSERT_EQ(list->head->val, (void*) 0);
    
    list_itr_t *it = list_itr_init(list);
    for (long i = 0; i < CNT; i++) {
        ASSERT_TRUE(list_itr_has_next(it));
        ASSERT_EQ(list_itr_next(it), (void*) i);
    }
    ASSERT_FALSE(list_itr_has_next(it));

    list_itr_free(it);
    list_free(list, 0);
}

UTEST(List, ListPopBack) {
    list_t *list = list_init();
    for (long i = 0; i < 10; i++)
        list_push_back(list, (void*) i);
    
    for (long i = CNT - 1; i >= 0; i--) {
        ASSERT_EQ(list->tail->val, (void*) i);
        list_pop_back(list);
        ASSERT_EQ(list_size(list), (size_t) i);
    }

    ASSERT_EQ(list->head, NULL);
    ASSERT_EQ(list->tail, NULL);

    ASSERT_THROWS(list_pop_back(list), EmptyListException);
    
    list_free(list, 0);
}

UTEST(List, ListPushFront) {
    list_t *list = list_init();
    ASSERT_EQ(list_size(list), 0);

    for (long i = 0; i < CNT; i++) {
        list_push_front(list, (void*) i);
        ASSERT_EQ(list_size(list), (size_t) i+1);
        ASSERT_EQ(list->head->val, (void*) i);
    }
    ASSERT_EQ(list->tail->val, (void*) 0);
    
    list_itr_t *it = list_itr_init(list);
    for (long i = CNT - 1; i >= 0; i--) {
        ASSERT_TRUE(list_itr_has_next(it));
        ASSERT_EQ(list_itr_next(it), (void*) i);
    }
    ASSERT_FALSE(list_itr_has_next(it));
    
    list_itr_free(it);
    list_free(list, 0);
}

UTEST(List, ListPopFront) {
    list_t *list = list_init();
    for (long i = 0; i < 10; i++)
        list_push_front(list, (void*) i);
    
    for (long i = CNT - 1; i >= 0; i--) {
        ASSERT_EQ(list->head->val, (void*) i);
        list_pop_front(list);
        ASSERT_EQ(list_size(list), (size_t) i);
    }

    ASSERT_EQ(list->head, NULL);
    ASSERT_EQ(list->tail, NULL);

    ASSERT_THROWS(list_pop_front(list), EmptyListException);
    
    list_free(list, 0);
}
