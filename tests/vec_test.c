#include "../src/vec.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_vec_works) {
    int test_ints[] = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t i, test_len;
    vec* vec;
    test_len = sizeof(test_ints) / sizeof(test_ints[0]);
    vec = vec_new(sizeof(int));

    for (i = 0; i < test_len; ++i) {
        int t_at = test_ints[i];
        vec_push(&vec, &t_at);
    }
    while (i--) {
        int t_at = test_ints[i];
        int popped = 0;
        vec_pop(vec, &popped);
        ck_assert_int_eq(popped, t_at);
    }
    vec_free(vec, NULL);
}
END_TEST

START_TEST(test_vec_strings) {
    char* test_chars[] = {"vince", "is", "cool"};
    size_t i, test_len;
    vec* vec;
    test_len = sizeof(test_chars) / sizeof(test_chars[0]);
    vec = vec_new(sizeof(char*));

    for (i = 0; i < test_len; ++i) {
        char* t_at = test_chars[i];
        vec_push(&vec, &t_at);
    }
    while (i--) {
        char* t_at = test_chars[i];
        char* popped = 0;
        vec_pop(vec, &popped);
        ck_assert_str_eq(popped, t_at);
    }
    vec_free(vec, NULL);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("lexer_test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_vec_works);
    tcase_add_test(tc_core, test_vec_strings);
    suite_add_tcase(s, tc_core);
    return s;
}

int main() {
    int number_failed;
    Suite* s;
    SRunner* sr;
    s = ht_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
