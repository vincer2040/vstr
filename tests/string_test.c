#include "../src/vstring.h"
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


START_TEST(test_vstr_works) {
    vstr str = vstr_new();
    str = vstr_push_char(str, 'v');
    str = vstr_push_char(str, 'i');
    str = vstr_push_char(str, 'n');
    str = vstr_push_char(str, 'c');
    str = vstr_push_char(str, 'e');
    ck_assert_str_eq(str, "vince");
    vstr_delete(str);
}
END_TEST

START_TEST(test_vstr_push_str) {
    vstr str = vstr_from("vince");
    str = vstr_push_string(str, " is cool");
    ck_assert_str_eq(str, "vince is cool");
    vstr_delete(str);
}
END_TEST

START_TEST(test_vstr_set) {
    vstr str = vstr_from("vince");
    str = vstr_set(str, "vince is cool");
    ck_assert_str_eq(str, "vince is cool");
    vstr_delete(str);
}
END_TEST

START_TEST(test_vstr_dup) {
    vstr str1 = vstr_from("vince");
    vstr str2 = vstr_dup(str1);
    ck_assert_str_eq(str1, str2);
    vstr_delete(str1);
    vstr_delete(str2);
}

START_TEST(test_vstr_format) {
    vstr s = vstr_format("%s %c %d %u", "vince", 'v', 420, 69);
    ck_assert_str_eq(s, "vince v 420 69");
    vstr_delete(s);
}
END_TEST

START_TEST(test_cap_overflow_len) {
    vstr s = vstr_new_len(18446744073709551613UL);
    ck_assert_ptr_null(s);
}
END_TEST

START_TEST(test_vstr_push_str_len) {
    vstr s = vstr_new();

    s = vstr_push_string_len(s, "vincent", 5);
    ck_assert_str_eq(s, "vince");
}
END_TEST

Suite* ht_suite() {
    Suite* s;
    TCase* tc_core;
    s = suite_create("string_test");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_vstr_works);
    tcase_add_test(tc_core, test_vstr_push_str);
    tcase_add_test(tc_core, test_vstr_set);
    tcase_add_test(tc_core, test_vstr_format);
    tcase_add_test(tc_core, test_cap_overflow_len);
    tcase_add_test(tc_core, test_vstr_push_str_len);
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
