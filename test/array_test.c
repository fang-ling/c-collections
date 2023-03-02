//
//  array_test.c
//  c-collections
//
//  Created by Fang Ling on 2023/3/1.
//

#include "array_test.h"

#define var __auto_type

static Int int_cmp(void* lhs, void* rhs) {
    if (*(Int*)lhs > *(Int*)rhs) {
        return 1;
    } else if(*(Int*)lhs < *(Int*)rhs) {
        return -1;
    }
    return 0;
}

static Bool int_equal(void* lhs, void* rhs) {
    if (int_cmp(lhs, rhs) == 0) {
        return true;
    }
    return false;
}

struct Task {
    const Char* name;
    Int priority;
};

static Int task_cmp(void* lhs, void* rhs) {
    return int_cmp(&(((struct Task*)lhs) -> priority),
                   &(((struct Task*)rhs) -> priority));
}

static Bool test_is_empty(void) {
    var array = array_init(sizeof(Int));
    expect_true(array -> is_empty);

    var delta = 19358ll;
    array_append(array, &delta);
    expect_false(array -> is_empty);

    array_remove_lastn(array);
    expect_true(array -> is_empty);

    array_insert(array, 0, &delta);
    expect_false(array -> is_empty);

    array_remove_firstn(array);
    expect_true(array -> is_empty);

    array_deinit(array);

    array = array_init2(sizeof(Int), delta);
    expect_false(array -> is_empty);

    array_deinit(array);

    array = array_init3(sizeof(Int), &delta, delta);
    expect_false(array -> is_empty);

    array_deinit(array);
    return true;
}

static Bool test_count(void) {
    var array = array_init(sizeof(Int));
    var delta = 0ll;
    expect_equal(&delta, &array -> count, int_equal);

    delta = 19358ll;
    array_append(array, &delta);
    delta = 1ll;
    expect_equal(&delta, &array -> count, int_equal);
    array_insert(array, 0, &delta);
    delta = 2ll;
    expect_equal(&delta, &array -> count, int_equal);

    free(array_remove_first(array));
    delta = 1ll;
    expect_equal(&delta, &array -> count, int_equal);

    array_remove_lastn(array);
    delta = 0ll;
    expect_equal(&delta, &array -> count, int_equal);

    array_deinit(array);
    return true;
}

static Bool test_get(void) {
    var array = array_init(sizeof(Int));
    Int input[] = {19358ll, 12333ll, 19348ll, 19306ll, 19306ll};

    for (var i = 0; i < 5; i += 1) {
        array_append(array, &input[i]);
    }
    for (var i = 0; i < 5; i += 1) {
        expect_equal(&input[i], array_get(array, i), int_equal);
    }

    array_deinit(array);
    return true;
}

static Bool test_set(void) {
    var array = array_init2(sizeof(Int), 5);
    Int input[] = {19358ll, 12333ll, 19348ll, 19306ll, 19306ll};

    for (var i = 0; i < 5; i += 1) {
        array_set(array, i, &input[i]);
    }

    for (var i = 0; i < 5; i += 1) {
        expect_equal(&input[i], array_get(array, i), int_equal);
    }

    array_deinit(array);
    return true;
}

static Bool test_first(void) {
    var array = array_init(sizeof(Int));
    expect_null(array_first(array));

    Int input[] = {19358ll, 12333ll, 19348ll, 19306ll, 19306ll};
    for (var i = 0; i < 5; i += 1) {
        array_insert(array, 0, &input[i]);
        expect_equal(&input[i], array_first(array), int_equal);
    }
    for (var i = 5 - 1; i >= 0; i -= 1) { /* The order is reversed */
        expect_equal(&input[i], array_first(array), int_equal);
        free(array_remove_first(array));
    }

    array_deinit(array);
    return true;
}

static Bool test_last(void) {
    var array = array_init(sizeof(Int));
    expect_null(array_last(array));

    Int input[] = {19358ll, 12333ll, 19348ll, 19306ll, 19306ll};
    for (var i = 5 - 1; i >= 0; i -= 1) {
        array_append(array, &input[i]);
        expect_equal(&input[i], array_last(array), int_equal);
    }
    for (var i = 0; i < 5; i += 1) {
        expect_equal(&input[i], array_last(array), int_equal);
        array_remove_lastn(array);
    }

    array_deinit(array);
    return true;
}

static Bool test_append(void) {
    var array = array_init(sizeof(struct Task));

    var delta = 0ll;
    expect_equal(&delta, &array -> count, int_equal);

    struct Task alpha;
    alpha.name = "Alice";
    alpha.priority = 12333;
    array_append(array, &alpha);

    delta = 1ll;
    expect_equal(&delta, &array -> count, int_equal);

    array_deinit(array);
    return true;
}

static Bool test_insert(void) {
    Int buf[] = {19358ll, 19358ll, 19358ll, 100ll, 19358ll, 19358ll, 200ll};
    var delta = 19358ll;
    var array = array_init3(sizeof(Int), &delta, 5);

    delta = 100ll;
    array_insert(array, 3, &delta);
    delta = 200ll;
    array_insert(array, array -> count, &delta);
    expect_equal_elements(array -> data, buf, 7, sizeof(Int), int_equal);

    array_deinit(array);

    array = array_init(sizeof(Int));
    array_insert(array, 0, &delta);
    expect_equal(array_get(array, 0), &delta, int_equal);

    array_deinit(array);

    array = array_init(sizeof(Int));
    for (var i = 0; i < 7; i += 1) {
        array_insert(array, 0, &buf[i]);
    }
    for (int i = 6; i >= 0; i -= 1) {
        expect_equal(&buf[i], array_get(array, 6 - i), int_equal);
    }

    array_deinit(array);
    return true;
}

void array_test(void) {
    run_test("Array", "test_is_empty", test_is_empty);
    run_test("Array", "test_count", test_count);
    run_test("Array", "test_get", test_get);
    run_test("Array", "test_set", test_set);
    run_test("Array", "test_first", test_first);
    run_test("Array", "test_last", test_last);
    run_test("Array", "test_append", test_append);
    run_test("Array", "test_insert", test_insert);
}
