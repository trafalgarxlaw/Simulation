/**
 * Testing the `utils` module with CUnit.
 *
 * @author Alexandre Blondin Masse
 */
#include "utils.h"
#include "CUnit/Basic.h"

void test_min() {
    CU_ASSERT_EQUAL(min(2, 5), 2);
    CU_ASSERT_EQUAL(min(3, 8), 3);
    CU_ASSERT_EQUAL(min(4, 4), 4);
}

void test_max() {
    CU_ASSERT_EQUAL(max(2, 5), 5);
    CU_ASSERT_EQUAL(max(3, 8), 8);
    CU_ASSERT_EQUAL(max(4, 4), 4);
}

void test_with_occurrences() {
    CU_ASSERT_EQUAL(num_occurrences("abaab", 'b'), 2);
    CU_ASSERT_EQUAL(num_occurrences("banana", 'a'), 3);
}

void test_without_occurrences() {
    CU_ASSERT_EQUAL(num_occurrences("", 'a'), 0);
    CU_ASSERT_EQUAL(num_occurrences("abaab", 'c'), 0);
}

void test_modulo_positive_dividend() {
    CU_ASSERT_EQUAL(mod( 8, 3), 2);
    CU_ASSERT_EQUAL(mod( 0, 3), 0);
}

void test_modulo_negative_dividend() {
    CU_ASSERT_EQUAL(mod(-2, 3), 1);
}

int main() {
    CU_pSuite pSuite = NULL;
    if (CU_initialize_registry() != CUE_SUCCESS )
        return CU_get_error();

    // Min and max functions
    pSuite = CU_add_suite("Testing min and max functions", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Computing the min of two values",
                    test_min) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Computing the max of two values",
                    test_max) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Number of occurrences
    pSuite = CU_add_suite("Testing the number of occurrences", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "When the character occurs at least once",
                    test_with_occurrences) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "When the character does not occur",
                    test_without_occurrences) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Modulo
    pSuite = CU_add_suite("Testing the positive modulo", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Testing positive dividend",
                    test_modulo_positive_dividend) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Testing negative dividend",
                    test_modulo_negative_dividend) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int num_failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    return num_failures;
}
