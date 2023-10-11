#include "utest.h"
#include "map.h"

UTEST_MAIN();

UTEST(Map, MapSet) {
    map_t *map = map_init();
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    map_set(map, "key1", &value1);
    map_set(map, "key2", &value2);
    map_set(map, "key3", &value3);
    ASSERT_EQ(*(int*)map_get(map, "key1"), 10);
    ASSERT_EQ(*(int*)map_get(map, "key2"), 20);
    ASSERT_EQ(*(int*)map_get(map, "key3"), 30);
    map_set(map, "key1", &value3);
    ASSERT_EQ(*(int*)map_get(map, "key1"), 30);
    map_set(map, "key4", &value1);
    ASSERT_EQ(*(int*)map_get(map, "key4"), 10);
}

UTEST(Map, MapGet) {
    map_t *map = map_init();
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    map_set(map, "key1", &value1);
    map_set(map, "key2", &value2);
    map_set(map, "key3", &value3);
    ASSERT_EQ(*(int*)map_get(map, "key1"), 10);
    ASSERT_EQ(*(int*)map_get(map, "key2"), 20);
    ASSERT_EQ(*(int*)map_get(map, "key3"), 30);
    ASSERT_EQ(map_get(map, "key4"), NULL);
}

UTEST(Map, MapSetNull) {
    map_t *map = map_init();
    map_set(map, "key1", NULL);
    ASSERT_EQ(map_get(map, "key1"), NULL);
}

UTEST(Map, MapGetNull) {
    map_t *map = map_init();
    ASSERT_EQ(map_get(map, "key1"), NULL);
}

UTEST(Map, MapSetEmptyKey) {
    map_t *map = map_init();
    int value1 = 10;
    map_set(map, "", &value1);
    ASSERT_EQ(map_get(map, ""), &value1);
}

UTEST(Map, MapGetEmptyKey) {
    map_t *map = map_init();
    ASSERT_EQ(map_get(map, ""), NULL);
}

UTEST(Map, MapSetDuplicateKey) {
    map_t *map = map_init();
    int value1 = 10;
    int value2 = 20;
    map_set(map, "key1", &value1);
    map_set(map, "key1", &value2);
    ASSERT_EQ(*(int*)map_get(map, "key1"), 20);
}

UTEST(Map, MapSetMultipleValues) {
    map_t *map = map_init();
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    map_set(map, "key1", &value1);
    map_set(map, "key2", &value2);
    map_set(map, "key3", &value3);
    ASSERT_EQ(*(int*)map_get(map, "key1"), 10);
    ASSERT_EQ(*(int*)map_get(map, "key2"), 20);
    ASSERT_EQ(*(int*)map_get(map, "key3"), 30);
}

UTEST(Map, MapSetAndGetStrings) {
    map_t *map = map_init();
    char *value1 = "value1";
    char *value2 = "value2";
    char *value3 = "value3";
    map_set(map, "key1", value1);
    map_set(map, "key2", value2);
    map_set(map, "key3", value3);
    ASSERT_EQ(strcmp((char*)map_get(map, "key1"), "value1"), 0);
    ASSERT_EQ(strcmp((char*)map_get(map, "key2"), "value2"), 0);
    ASSERT_EQ(strcmp((char*)map_get(map, "key3"), "value3"), 0);
}

UTEST(Map, MapSetAndGetStructs) {
    map_t *map = map_init();
    struct my_struct {
        int a;
        int b;
    };
    struct my_struct value1 = {1, 2};
    struct my_struct value2 = {3, 4};
    struct my_struct value3 = {5, 6};
    map_set(map, "key1", &value1);
    map_set(map, "key2", &value2);
    map_set(map, "key3", &value3);
    ASSERT_EQ(((struct my_struct*)map_get(map, "key1"))->a, 1);
    ASSERT_EQ(((struct my_struct*)map_get(map, "key1"))->b, 2);
    ASSERT_EQ(((struct my_struct*)map_get(map, "key2"))->a, 3);
    ASSERT_EQ(((struct my_struct*)map_get(map, "key2"))->b, 4);
    ASSERT_EQ(((struct my_struct*)map_get(map, "key3"))->a, 5);
    ASSERT_EQ(((struct my_struct*)map_get(map, "key3"))->b, 6);
}
