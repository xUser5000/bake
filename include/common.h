#define ASSERT_LIST_EQ(list1, arr) {                        \
    size_t sz = sizeof(arr) / sizeof(arr[0]);               \
    ASSERT_EQ(list_size(list1), sz);                        \
    size_t i;                                               \
    list_itr_t *itr = list_itr_init(list1);                 \
    for (i = 0; i < sz; i++) {                              \
        ASSERT_STREQ((char*) list_itr_next(itr), arr[i]);   \
    }                                                       \
}

#define ASSERT_THROWS(code, exception) {                    \
    int handled = 0;                                        \
    E4C_TRY {                                               \
        code;                                               \
    } E4C_CATCH (exception) {                               \
        handled = 1;                                        \
    }                                                       \
    ASSERT_TRUE(handled);                                   \
}
