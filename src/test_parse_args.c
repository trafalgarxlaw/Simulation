/**
 * Testing the `parse_args` module with CUnit.
 *
 * @author Alexandre Blondin Masse
 */
#include "parse_args.h"
#include "CUnit/Basic.h"

void test_default() {
    char *argv[] = {"bin/automaton", NULL};
    int argc = 1;
    struct Arguments *arguments = parse_arguments(argc, argv);
    CU_ASSERT_EQUAL(arguments->num_rows,  5);
    CU_ASSERT_EQUAL(arguments->num_cols,  5);
    CU_ASSERT_EQUAL(arguments->num_steps, 5);
    CU_ASSERT_EQUAL(arguments->type,      CELLULAR_GAME_OF_LIFE);
    CU_ASSERT_EQUAL(arguments->boundary,  CELLULAR_TRUNCATE);
    free_arguments(arguments);
}

void test_size_short_options() {
    char *argv[] = {"bin/automaton", "-r", "10", "-c", "20", "-n", "30",
                    "-t", "pandemy", "-i", "-a", ".XH", NULL};
    int argc = 12;
    struct Arguments *arguments = parse_arguments(argc, argv);
    CU_ASSERT_EQUAL(arguments->num_rows,    10);
    CU_ASSERT_EQUAL(arguments->num_cols,    20);
    CU_ASSERT_EQUAL(arguments->num_steps,   30);
    CU_ASSERT_EQUAL(arguments->type,        CELLULAR_PANDEMY);
    CU_ASSERT_EQUAL(arguments->boundary,    CELLULAR_TRUNCATE);
    CU_ASSERT_EQUAL(arguments->interactive, true);
    CU_ASSERT_STRING_EQUAL(arguments->allowed_cells,
                           ".XH")
    free_arguments(arguments);
}

void test_size_long_options() {
    char *argv[] = {"bin/automaton", "--num-rows", "10", "--num-cols", "20",
                    "--num-steps", "30", "--type", "pandemy",
                    "--interactive", "--allowed-cells", ".XH", NULL};
    int argc = 12;
    struct Arguments *arguments = parse_arguments(argc, argv);
    CU_ASSERT_EQUAL(arguments->num_rows,    10);
    CU_ASSERT_EQUAL(arguments->num_cols,    20);
    CU_ASSERT_EQUAL(arguments->num_steps,   30);
    CU_ASSERT_EQUAL(arguments->type,        CELLULAR_PANDEMY);
    CU_ASSERT_EQUAL(arguments->boundary,    CELLULAR_TRUNCATE);
    CU_ASSERT_EQUAL(arguments->interactive, true);
    CU_ASSERT_STRING_EQUAL(arguments->allowed_cells,
                           ".XH")
    free_arguments(arguments);
}

int main() {
    CU_pSuite pSuite = NULL;
    if (CU_initialize_registry() != CUE_SUCCESS )
        return CU_get_error();

    // Testing arguments parsing
    pSuite = CU_add_suite("Testing arguments parsing", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Checking default arguments",
                    test_default) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Checking basic short options",
                    test_size_short_options) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Checking basic long options",
                    test_size_long_options) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int num_failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    return num_failures;
}
