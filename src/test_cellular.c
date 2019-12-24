/**
 * Testing the `cellular` module with CUnit.
 *
 * @author Alexandre Blondin Masse
 */
#include "cellular.h"
#include "CUnit/Basic.h"

void test_random_game_of_life() {
    unsigned int num_rows = 20, num_cols = 30;
    struct CellularAutomaton *automaton =
        Cellular_init(num_rows, num_cols,
                      CELLULAR_GAME_OF_LIFE, CELLULAR_TRUNCATE, ".X");
    unsigned int distribution[] = {1, 1};
    Cellular_set_random(automaton, distribution);
    for (unsigned int i = 0; i < num_rows; ++i) {
        for (unsigned int j = 0; j < num_cols; ++j) {
            CU_ASSERT(automaton->cells[i][j] == '.' ||
                      automaton->cells[i][j] == 'X');
        }
    }
    Cellular_free(automaton);
}

void test_random_pandemy() {
    unsigned int num_rows = 20, num_cols = 30;
    struct CellularAutomaton *automaton =
        Cellular_init(num_rows, num_cols,
                      CELLULAR_PANDEMY, CELLULAR_WRAP_AROUND, ".XH");
    unsigned int distribution[] = {1, 1, 1};
    Cellular_set_random(automaton, distribution);
    for (unsigned int i = 0; i < num_rows; ++i) {
        for (unsigned int j = 0; j < num_cols; ++j) {
            CU_ASSERT(automaton->cells[i][j] == '.' ||
                      automaton->cells[i][j] == 'X' ||
                      automaton->cells[i][j] == 'H');
        }
    }
    Cellular_free(automaton);
}

void test_random_fire() {
    unsigned int num_rows = 20, num_cols = 30;
    struct CellularAutomaton *automaton =
        Cellular_init(num_rows, num_cols,
                      CELLULAR_FIRE, CELLULAR_TRUNCATE, "._Bb");
    unsigned int distribution[] = {1, 1, 1, 1};
    Cellular_set_random(automaton, distribution);
    for (unsigned int i = 0; i < num_rows; ++i) {
        for (unsigned int j = 0; j < num_cols; ++j) {
            CU_ASSERT(automaton->cells[i][j] == '.' ||
                      automaton->cells[i][j] == '_' ||
                      automaton->cells[i][j] == 'B' ||
                      automaton->cells[i][j] == 'b');
        }
    }
    Cellular_free(automaton);
}

void test_num_cells() {
    CU_ASSERT_EQUAL(Cellular_num_cells(CELLULAR_GAME_OF_LIFE), 2);
    CU_ASSERT_EQUAL(Cellular_num_cells(CELLULAR_PANDEMY), 3);
    CU_ASSERT_EQUAL(Cellular_num_cells(CELLULAR_FIRE), 4);
}

void test_validity() {
    CU_ASSERT(Cellular_is_valid(CELLULAR_GAME_OF_LIFE, "ab"));
    CU_ASSERT(Cellular_is_valid(CELLULAR_PANDEMY, "abc"));
    CU_ASSERT(Cellular_is_valid(CELLULAR_FIRE, "abcd"));
}

void test_duplication() {
    unsigned int num_rows = 20, num_cols = 30;
    struct CellularAutomaton *automaton =
        Cellular_init(num_rows, num_cols,
                      CELLULAR_FIRE, CELLULAR_TRUNCATE, "._Bb");
    unsigned int distribution[] = {1, 1, 1, 1};
    Cellular_set_random(automaton, distribution);
    struct CellularAutomaton *copy = Cellular_duplicate(automaton);
    for (unsigned int i = 0; i < num_rows; ++i) {
        for (unsigned int j = 0; j < num_cols; ++j) {
            CU_ASSERT_EQUAL(automaton->cells[i][j],
                            copy->cells[i][j]);
        }
    }
    Cellular_free(automaton);
    Cellular_free(copy);
}

void test_simulation() {
    struct CellularAutomaton *automaton =
        Cellular_init(3, 3, CELLULAR_PANDEMY, CELLULAR_TRUNCATE, ".XH");
    char **a = automaton->cells;
    a[0][0] = '.'; a[0][1] = 'X'; a[0][2] = '.';
    a[1][0] = '.'; a[1][1] = 'X'; a[1][2] = '.';
    a[2][0] = '.'; a[2][1] = 'X'; a[2][2] = '.';
    struct CellularAutomaton *next = Cellular_next(automaton);
    CU_ASSERT_EQUAL(next->cells[0][0], '.');
    CU_ASSERT_EQUAL(next->cells[0][1], '.');
    CU_ASSERT_EQUAL(next->cells[0][2], '.');
    CU_ASSERT_EQUAL(next->cells[1][0], 'X');
    CU_ASSERT_EQUAL(next->cells[1][1], 'X');
    CU_ASSERT_EQUAL(next->cells[1][2], 'X');
    CU_ASSERT_EQUAL(next->cells[2][0], '.');
    CU_ASSERT_EQUAL(next->cells[2][1], '.');
    CU_ASSERT_EQUAL(next->cells[2][2], '.');
    Cellular_free(automaton);
    Cellular_free(next);
}

int main() {
    CU_pSuite pSuite = NULL;
    if (CU_initialize_registry() != CUE_SUCCESS )
        return CU_get_error();

    // Random initialization
    pSuite = CU_add_suite("Testing random initialization", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Random game-of-life",
                    test_random_game_of_life) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Random pandemy",
                    test_random_pandemy) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Random fire",
                    test_random_fire) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Consistent cellular automata
    pSuite = CU_add_suite("Consistent size and cells", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Consistent number of cells and type",
                    test_num_cells) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Testing validity",
                    test_validity) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Automaton from another
    pSuite = CU_add_suite("Producing a new automaton", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Testing simple duplication",
                    test_duplication) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(pSuite, "Testing one-step simulation",
                    test_simulation) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int num_failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    return num_failures;
}
