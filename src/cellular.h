/**
 * Provides types and services to play with cellular automata.
 *
 * The main provided type is `struct CellularAutomaton`, which represents a 2D
 * cellular automaton
 *
 * @author Alexandre Blondin Massé
 */
#ifndef CELLULAR_H
#define CELLULAR_H

#define UNINITIALIZED_CELL '?'

#include <stdbool.h>

// ----- //
// Types //
// ----- //

typedef struct 
{
   unsigned int row,col;
   char elemets[50][50];

}InitialState;

/**
 * The type of cellular automaton.
 */
enum CellularType {
    CELLULAR_PANDEMY,               /**< Virus propagation */
    CELLULAR_GAME_OF_LIFE,          /**< Conway's game-of-life */
    CELLULAR_FIRE                   /**< Fire propagation */
};

/**
 * The type of boundary of the cellular automaton.
 */
enum CellularBoundary {
    CELLULAR_TRUNCATE,              /**< Truncate the neighborhood */
    CELLULAR_WRAP_AROUND            /**< The neighborhood is a torus */
};

/**
 * A cellular automaton.
 */
struct CellularAutomaton {
    unsigned int num_rows;          /**< Its number of rows */
    unsigned int num_cols;          /**< Its number of columns */
    char *allowed_cells;            /**< The allowed cells */
    char **cells;                   /**< Its cells */
    enum CellularType type;         /**< Its type */
    enum CellularBoundary boundary; /**< Its boundary type */
};

// --------- //
// Functions //
// --------- //

/**
 * Creates a cellular automaton.
 *
 * By default, the cells are uninitialized.
 *
 * @param num_rows       Its number of rows
 * @param num_cols       Its number of columns
 * @param type           Its type
 * @param boundary       How to process the boundaries
 * @param allowed_cells  The allowed cells
 * @return               The automaton
 */
struct CellularAutomaton *Cellular_init(
    unsigned int num_rows,
    unsigned int num_cols,
    enum CellularType type,
    enum CellularBoundary boundary,
    const char *allowed_cells
);

/**
 * Creates a cellular automaton.
 *
 * Uses the initial state defined by the user input file.
 *
 * @param num_rows       Its number of rows
 * @param num_cols       Its number of columns
 * @param type           Its type
 * @param boundary       How to process the boundaries
 * @param allowed_cells  The allowed cells
 * @param cellularArray  The 2D array of the initial state to read
 * @return               The automaton
 */
struct CellularAutomaton *Cellular_init_with_state(
    unsigned int num_rows,
    unsigned int num_cols,
    enum CellularType type,
    enum CellularBoundary boundary,
    const char *allowed_cells,
    InitialState cellularArray
);

/**
 * Returns a copy of a cellular automaton.
 *
 * @param automaton  The automaton to copy
 * @return           A copy of the automaton
 */
struct CellularAutomaton *Cellular_duplicate(
    const struct CellularAutomaton *automaton
);

/**
 * Randomly sets the cells with respect to the uniform distribution.
 *
 * The probability distribution is given by positive weights.
 *
 * @param automaton     The automaton to set
 * @param distribution  The probability distribution
 */
void Cellular_set_random(
    struct CellularAutomaton *automaton,
    const unsigned int *distribution
);

/**
 * Frees the given automaton.
 *
 * @param automaton  The automaton to free
 */
void Cellular_free(struct CellularAutomaton *automaton);

/**
 * Prints the given automaton to stdout.
 *
 * @param automaton   The automaton to print
 * @param print_type  If true, also prints the type
 */
void Cellular_print(const struct CellularAutomaton *automaton,
                    bool print_type);

/**
 * Returns an automaton updated according to the rules.
 *
 * @param automaton  The automaton to update
 * @return           The updated automaton
 */
struct CellularAutomaton *Cellular_next(
    const struct CellularAutomaton *automaton
);

/**
 * Returns the number of allowed cells for a given type.
 *
 * @param type  The type of cellular automaton
 * @return      The number of allowed cells for the type
 */
unsigned int Cellular_num_cells(enum CellularType type);

/**
 * Returns true if the type of cellular is consistent with the given allowed
 * cells.
 *
 * @param type           The type of cellular automaton
 * @param allowed_cells  The allowed cells
 * @return               True if the values are consistent
 */
bool Cellular_is_valid(enum CellularType type, const char *allowed_cells);

/**
 * Verify is there is a cell that is not
 * allowed in the given initial state.
 *
 * @param allowed_cells  The allowed cells
 * @param automaton      The automaton to verify.
 * 
 */
void Cellular_Valid_StateCells(const char *allowed_cells,
const struct CellularAutomaton * automaton);

#endif
