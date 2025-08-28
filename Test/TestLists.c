#include "TestLists.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <SEA/Allocator.h>
#include <SEA/ListChunked.h>
#include <SEA/ListDyn.h>
#include <SEA/ListSegmented.h>
#include <SEA/Compat/StringCompat.h>

// Test data structure
typedef struct {
    int id;
    char name[16];
    double value;
} TestItem;

static TestItem create_test_item(const int id, const char* name, const double value) {
    TestItem item = {0};
    item.id = id;
    SEA_strncpy_s(item.name, 16, name, sizeof(item.name) - 1);
    item.value = value;
    return item;
}

static int compare_test_items(const TestItem* a, const TestItem* b) {
    return (a->id == b->id &&
            strcmp(a->name, b->name) == 0 &&
            a->value == b->value) ? 1 : 0;
}

// Generic test functions that work with all list types
#define DEFINE_LIST_TESTS(ListType, listname) \
static void test_##listname##_basic_operations(void) { \
    printf("Testing %s basic operations...\n", #ListType); \
    \
    struct ListType list = ListType(TestItem, NULL); \
    \
    /* Test initial state */ \
    assert(listname.count(&list) == 0); \
    \
    /* Test adding elements */ \
    TestItem item1 = create_test_item(1, "First", 1.5); \
    TestItem item2 = create_test_item(2, "Second", 2.5); \
    TestItem item3 = create_test_item(3, "Third", 3.5); \
    \
    size_t count1 = listname.add(&list, &item1); \
    assert(count1 == 1); \
    assert(listname.count(&list) == 1); \
    \
    size_t count2 = listname.add(&list, &item2); \
    assert(count2 == 2); \
    assert(listname.count(&list) == 2); \
    \
    size_t count3 = listname.add(&list, &item3); \
    assert(count3 == 3); \
    assert(listname.count(&list) == 3); \
    \
    /* Test getting elements */ \
    TestItem* retrieved1 = (TestItem*)listname.get(&list, 0); \
    TestItem* retrieved2 = (TestItem*)listname.get(&list, 1); \
    TestItem* retrieved3 = (TestItem*)listname.get(&list, 2); \
    \
    assert(retrieved1 != NULL); \
    assert(retrieved2 != NULL); \
    assert(retrieved3 != NULL); \
    \
    assert(compare_test_items(retrieved1, &item1)); \
    assert(compare_test_items(retrieved2, &item2)); \
    assert(compare_test_items(retrieved3, &item3)); \
    \
    /* Test out of bounds access */ \
    TestItem* out_of_bounds = (TestItem*)listname.get(&list, 10); \
    assert(out_of_bounds == NULL); \
    \
    listname.free(&list); \
    printf("%s basic operations passed!\n", #ListType); \
} \
\
static void test_##listname##_alloc_function(void) { \
    printf("Testing %s alloc function...\n", #ListType); \
    \
    struct ListType list = ListType(TestItem, NULL); \
    \
    /* Test allocating slots */ \
    TestItem* slot1 = (TestItem*)listname.alloc(&list); \
    assert(slot1 != NULL); \
    assert(listname.count(&list) == 1); \
    \
    /* Fill the allocated slot */ \
    *slot1 = create_test_item(100, "Allocated", 10.5); \
    \
    TestItem* slot2 = (TestItem*)listname.alloc(&list); \
    assert(slot2 != NULL); \
    assert(listname.count(&list) == 2); \
    \
    *slot2 = create_test_item(200, "Second", 20.5); \
    \
    /* Verify the data is correct */ \
    TestItem* retrieved1 = (TestItem*)listname.get(&list, 0); \
    TestItem* retrieved2 = (TestItem*)listname.get(&list, 1); \
    \
    assert(retrieved1->id == 100); \
    assert(strcmp(retrieved1->name, "Allocated") == 0); \
    assert(retrieved1->value == 10.5); \
    \
    assert(retrieved2->id == 200); \
    assert(strcmp(retrieved2->name, "Second") == 0); \
    assert(retrieved2->value == 20.5); \
    \
    listname.free(&list); \
    printf("%s alloc function passed!\n", #ListType); \
} \
\
static void test_##listname##_large_dataset(void) { \
    printf("Testing %s with large dataset...\n", #ListType); \
    \
    struct ListType list = ListType(int, NULL); \
    const int TEST_SIZE = 10000; \
    \
    /* Add many elements */ \
    for (int i = 0; i < TEST_SIZE; i++) { \
        listname.add(&list, &i); \
    } \
    \
    assert(listname.count(&list) == TEST_SIZE); \
    \
    /* Verify all elements */ \
    for (int i = 0; i < TEST_SIZE; i++) { \
        int* value = (int*)listname.get(&list, i); \
        assert(value != NULL); \
        assert(*value == i); \
    } \
    \
    listname.free(&list); \
    printf("%s large dataset passed!\n", #ListType); \
} \
\
static void test_##listname##_foreach_macro(void) { \
    printf("Testing %s foreach macro...\n", #ListType); \
    \
    struct ListType list = ListType(int, NULL); \
    \
    /* Add test data */ \
    for (int i = 0; i < 10; i++) { \
        listname.add(&list, &i); \
    } \
    \
    /* Test foreach macro */ \
    int expected = 0; \
    ListType##_foreach(int, &list) { \
        assert(it.value != NULL); \
        assert(*it.value == expected); \
        assert(it.index == expected); \
        expected++; \
    } \
    \
    assert(expected == 10); \
    \
    listname.free(&list); \
    printf("%s foreach macro passed!\n", #ListType); \
}

// Generate tests for all three list types
DEFINE_LIST_TESTS(SEA_ListSegmented, SEA_ListSegmented)
DEFINE_LIST_TESTS(SEA_ListChunked, SEA_ListChunked)
//DEFINE_LIST_TESTS(SEA_ListDyn, SEA_ListDyn)

// Additional tests specific to ListDyn (which has more functionality)
static void test_list_dyn_insert_remove(void) {
    printf("Testing SEA_ListDyn insert and remove operations...\n");

    SEA_ListDyn list = SEA_ListDyn(int, NULL);

    // Add initial elements
    for (int i = 0; i < 5; i++) {
        SEA_ListDyn_add(&list, &i);
    }
    // List now contains: [0, 1, 2, 3, 4]

    // Test insert at beginning
    const int insert_val = 99;
    SEA_ListDyn_insert(&list, 0, &insert_val);
    assert(SEA_ListDyn_count(&list) == 6);
    assert(*(int*)SEA_ListDyn_get(&list, 0) == 99);
    assert(*(int*)SEA_ListDyn_get(&list, 1) == 0);
    // List now contains: [99, 0, 1, 2, 3, 4]

    // Test insert in middle
    const int insert_val2 = 88;
    SEA_ListDyn_insert(&list, 3, &insert_val2);
    assert(SEA_ListDyn_count(&list) == 7);
    assert(*(int*)SEA_ListDyn_get(&list, 3) == 88);
    assert(*(int*)SEA_ListDyn_get(&list, 4) == 2);
    // List now contains: [99, 0, 1, 88, 2, 3, 4]

    // Test insert at end
    const int insert_val3 = 77;
    SEA_ListDyn_insert(&list, SEA_ListDyn_count(&list), &insert_val3);
    assert(SEA_ListDyn_count(&list) == 8);
    assert(*(int*)SEA_ListDyn_get(&list, 7) == 77);
    // List now contains: [99, 0, 1, 88, 2, 3, 4, 77]

    // Test remove from beginning
    SEA_ListDyn_remove(&list, 0);
    assert(SEA_ListDyn_count(&list) == 7);
    assert(*(int*)SEA_ListDyn_get(&list, 0) == 0);
    // List now contains: [0, 1, 88, 2, 3, 4, 77]

    // Test remove from middle
    SEA_ListDyn_remove(&list, 2);
    assert(SEA_ListDyn_count(&list) == 6);
    assert(*(int*)SEA_ListDyn_get(&list, 2) == 2);
    // List now contains: [0, 1, 2, 3, 4, 77]

    // Test remove from end
    SEA_ListDyn_remove(&list, SEA_ListDyn_count(&list) - 1);
    assert(SEA_ListDyn_count(&list) == 5);
    // List now contains: [0, 1, 2, 3, 4]

    // Verify final state
    for (int i = 0; i < 5; i++) {
        assert(*(int*)SEA_ListDyn_get(&list, i) == i);
    }

    SEA_ListDyn_free(&list);
    printf("SEA_ListDyn insert and remove operations passed!\n");
}

static void test_list_dyn_capacity_management(void) {
    printf("Testing SEA_ListDyn capacity management...\n");

    SEA_ListDyn list = SEA_ListDyn(int, NULL);

    // Test initial capacity
    assert(SEA_ListDyn_capacity(&list) == 0);

    // Test reserve
    SEA_ListDyn_reserve(&list, 100);
    assert(SEA_ListDyn_capacity(&list) >= 100);
    assert(SEA_ListDyn_count(&list) == 0);

    // Add some elements
    for (int i = 0; i < 50; i++) {
        SEA_ListDyn_add(&list, &i);
    }
    assert(SEA_ListDyn_count(&list) == 50);
    const size_t capacity_after_adds = SEA_ListDyn_capacity(&list);
    assert(capacity_after_adds >= 100);

    // Test shrink
    SEA_ListDyn_shrink(&list);
    assert(SEA_ListDyn_capacity(&list) == 50);
    assert(SEA_ListDyn_count(&list) == 50);

    // Verify data is still intact
    for (int i = 0; i < 50; i++) {
        assert(*(int*)SEA_ListDyn_get(&list, i) == i);
    }

    // Test clear
    SEA_ListDyn_clear(&list);
    assert(SEA_ListDyn_count(&list) == 0);
    assert(SEA_ListDyn_capacity(&list) == 50); // Capacity should remain

    SEA_ListDyn_free(&list);
    printf("SEA_ListDyn capacity management passed!\n");
}

// Additional tests for ListChunked (which has clear functionality)
static void test_list_chunked_clear(void) {
    printf("Testing SEA_ListChunked clear operation...\n");

    struct SEA_ListChunked list = SEA_ListChunked(int, NULL);

    // Add elements
    for (int i = 0; i < 100; i++) {
        SEA_ListChunked.add(&list, &i);
    }
    assert(SEA_ListChunked.count(&list) == 100);

    // Test clear
    SEA_ListChunked.clear(&list);
    assert(SEA_ListChunked.count(&list) == 0);

    // Add elements again to verify list still works
    for (int i = 0; i < 10; i++) {
        SEA_ListChunked.add(&list, &i);
    }
    assert(SEA_ListChunked.count(&list) == 10);

    // Verify data
    for (int i = 0; i < 10; i++) {
        assert(*(int*)SEA_ListChunked.get(&list, i) == i);
    }

    SEA_ListChunked.free(&list);
    printf("SEA_ListChunked clear operation passed!\n");
}

static void test_list_dyn_clear(void) {
    printf("Testing SEA_ListDyn clear operation...\n");

    SEA_ListDyn list = SEA_ListDyn(int, NULL);

    // Reserve capacity and add elements
    SEA_ListDyn_reserve(&list, 100);
    for (int i = 0; i < 50; i++) {
        SEA_ListDyn_add(&list, &i);
    }
    assert(SEA_ListDyn_count(&list) == 50);
    const size_t capacity_before = SEA_ListDyn_capacity(&list);

    // Test clear
    SEA_ListDyn_clear(&list);
    assert(SEA_ListDyn_count(&list) == 0);
    assert(SEA_ListDyn_capacity(&list) == capacity_before); // Capacity should remain

    // Add elements again
    for (int i = 0; i < 10; i++) {
        SEA_ListDyn_add(&list, &i);
    }
    assert(SEA_ListDyn_count(&list) == 10);

    // Verify data
    for (int i = 0; i < 10; i++) {
        assert(*(int*)SEA_ListDyn_get(&list, i) == i);
    }

    SEA_ListDyn_free(&list);
    printf("SEA_ListDyn clear operation passed!\n");
}

// Performance comparison test
static void test_lists_performance_comparison(void) {
    printf("Testing performance comparison between list implementations...\n");

    const int TEST_SIZE = 50000;

    // Test ListSegmented
    printf("  Testing ListSegmented performance...\n");
    struct SEA_ListSegmented seg_list = SEA_ListSegmented(int, NULL);
    for (int i = 0; i < TEST_SIZE; i++) {
        SEA_ListSegmented.add(&seg_list, &i);
    }
    assert(SEA_ListSegmented.count(&seg_list) == TEST_SIZE);
    SEA_ListSegmented.free(&seg_list);

    // Test ListChunked
    printf("  Testing ListChunked performance...\n");
    struct SEA_ListChunked chunk_list = SEA_ListChunked(int, NULL);
    for (int i = 0; i < TEST_SIZE; i++) {
        SEA_ListChunked.add(&chunk_list, &i);
    }
    assert(SEA_ListChunked.count(&chunk_list) == TEST_SIZE);
    SEA_ListChunked.free(&chunk_list);

    // Test ListDyn
    printf("  Testing ListDyn performance...\n");
    SEA_ListDyn dyn_list = SEA_ListDyn(int, NULL);
    for (int i = 0; i < TEST_SIZE; i++) {
        SEA_ListDyn_add(&dyn_list, &i);
    }
    assert(SEA_ListDyn_count(&dyn_list) == TEST_SIZE);
    SEA_ListDyn_free(&dyn_list);

    printf("Performance comparison completed!\n");
}

// Main test functions
void test_list_segmented(void) {
    printf("=== Testing SEA_ListSegmented ===\n");
    test_SEA_ListSegmented_basic_operations();
    test_SEA_ListSegmented_alloc_function();
    test_SEA_ListSegmented_large_dataset();
    test_SEA_ListSegmented_foreach_macro();
    printf("All SEA_ListSegmented tests passed!\n\n");
}

void test_list_chunked(void) {
    printf("=== Testing SEA_ListChunked ===\n");
    test_SEA_ListChunked_basic_operations();
    test_SEA_ListChunked_alloc_function();
    test_SEA_ListChunked_large_dataset();
    test_SEA_ListChunked_foreach_macro();
    test_list_chunked_clear();
    printf("All SEA_ListChunked tests passed!\n\n");
}

void test_list_dyn(void) {
    printf("=== Testing SEA_ListDyn ===\n");
//    test_SEA_ListDyn_basic_operations();
//    test_SEA_ListDyn_alloc_function();
//    test_SEA_ListDyn_large_dataset();
//    test_SEA_ListDyn_foreach_macro();
    test_list_dyn_insert_remove();
    test_list_dyn_capacity_management();
    test_list_dyn_clear();
    printf("All SEA_ListDyn tests passed!\n\n");
}

void test_lists_all(void) {
    printf("========================================\n");
    printf("         SEA List Tests\n");
    printf("========================================\n\n");

    test_list_segmented();
    test_list_chunked();
    test_list_dyn();

    test_lists_performance_comparison();

    printf("========================================\n");
    printf("      All List Tests Passed!\n");
    printf("========================================\n");
}