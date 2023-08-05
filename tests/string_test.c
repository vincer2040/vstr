#include "../src/vstring.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_vstring_works) {
    vstring* vstr = vstring_new();
    vstring_push_char(&vstr, 'v');
    vstring_push_char(&vstr, 'i');
    vstring_push_char(&vstr, 'n');
    vstring_push_char(&vstr, 'c');
    vstring_push_char(&vstr, 'e');
    ck_assert_str_eq(vstring_get(vstr), "vince");
    vstring_free(vstr);
}
END_TEST

START_TEST(test_vstring_push_char) {
    vstring* vstr = vstring_from("vince");
    ck_assert_str_eq(vstr->data, "vince");
    vstring_push_char(&vstr, 'n');
    vstring_push_char(&vstr, 't');
    ck_assert_str_eq(vstring_get(vstr), "vincent");
    ck_assert_uint_eq(vstring_len(vstr), 7);
    vstring_free(vstr);
}
END_TEST

START_TEST(test_vstring_push_str) {
    vstring* vstr = vstring_from("vince");
    ck_assert_str_eq(vstr->data, "vince");
    vstring_push_string(&vstr, "nt ");
    vstring_push_string(&vstr, "is cool");
    ck_assert_str_eq(vstring_get(vstr), "vincent is cool");
    ck_assert_uint_eq(vstring_len(vstr), 15);
    vstring_free(vstr);
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("lexer_test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_vstring_works);
    tcase_add_test(tc_core, test_vstring_push_char);
    tcase_add_test(tc_core, test_vstring_push_str);
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
