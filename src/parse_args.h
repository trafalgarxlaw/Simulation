/**
 * Provides basic services to process the main arguments. Error codes are also
 * provided in the case of invalid input from the user.
 *
 * @author Alexandre Blondin Masse
 */
#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <stdbool.h>
#include "cellular.h"

#define GOF_TYPE "game-of-life"
#define PANDEMY_TYPE "pandemy"
#define FIRE_TYPE "fire"
#define SUPPORTED_TYPES "\"" GOF_TYPE "\", \"" PANDEMY_TYPE "\" and \""\
    FIRE_TYPE "\""
#define BOUNDARY_PERIODIC "periodic"
#define BOUNDARY_TRUNCATE "truncate"
#define SUPPORTED_BOUNDARIES "\"" BOUNDARY_TRUNCATE "\" and \""\
    BOUNDARY_PERIODIC "\""
#define DEFAULT_TYPE GOF_TYPE
#define DEFAULT_BOUNDARY BOUNDARY_TRUNCATE
#define DEFAULT_CELLS ".X"
#define DEFAULT_DISTRIBUTION "5,1,1,1"
#define NUM_ROWS_DEFAULT 5
#define NUM_COLS_DEFAULT 5
#define NUM_STEPS_DEFAULT 5

#define USAGE "\
Usage: %s [-h|--help] [-r|--num-rows VALUE] [-c|--num-cols VALUE]\n\
    [-n|--num_steps VALUE] [-t|--type STRING] [-a|--allowed-cells STRING]\n\
    [-d|--distribution VALUES] [-i|--interactive] [-s|--stdin]\n\
\n\
Simulates a cellular automaton.\n\
\n\
Optional arguments:\n\
  -h, --help                  Shows this help message and exit\n\
  -r, --num-rows VALUE        The number of rows.\n\
                              The default value is 5.\n\
  -c, --num-cols VALUE        The number of columns.\n\
                              The default value is 5.\n\
  -n, --num-steps VALUE       The number of steps in the simulation.\n\
                              The default value is 5.\n\
  -t, --type STRING           The type of simulation.\n\
                              Currently, there are 3 supported types:\n\
                              \"%s\", \"%s\" and \"%s\".\n\
                              The default type is \"%s\".\n\
  -b, --boundary STRING       The boundary type.\n\
                              Currently, there are 2 supported types:\n\
                              \"%s\" and \"%s\".\n\
                              The default boundary is \"%s\".\n\
  -a, --allowed-cells STRING  The allowed cells, as characters.\n\
                              The number of cells must be consistent\n\
                              with the type of simulation.\n\
  -d, --distribution VALUES   The initial probability distribution,\n\
                              which are comma-separated positive values.\n\
                              E.g: if the allowed cells are \"abc\"\n\
                              and the distribution is \"1,1,2\", then, 'c'\n\
                              will appear twice as more as 'a' and 'b'.\n\
  -i, --interactive           Enables interactive simulation.\n\
  -s, --stdin                 Reads from file the initial state of the automaton.\n\
"

/**
 * The status of the program.
 */
enum Status {
    TP2_OK,                         /**< Everything is alright */
    TP2_WRONG_VALUE,                /**< Wrong value */
    TP2_WRONG_SIMULATION_TYPE,      /**< Wrong type of simulation */
    TP2_WRONG_BOUNDARY,             /**< Wrong boundary */
    TP2_WRONG_DISTRIBUTION,         /**< Wrong probability distribution */
    TP2_ERROR_TOO_MANY_ARGUMENTS,   /**< Too many arguments */
    TP2_INCONSISTENT_ARGS,          /**< Some arguments are inconsistent */
    TP2_BAD_OPTION,                  /**< Bad option */
    TP2_ERROR_STDIN_WITH_ROW_COL     /**< rows and columns cannot be indicated together with stdin */

};

/**
 * The parsed arguments.
 */
struct Arguments {
    unsigned int num_rows;          /**< Number of rows */
    unsigned int num_cols;          /**< Number of columns */
    unsigned int num_steps;         /**< Number of steps in the simulation */
    enum CellularType type;         /**< The type of cellular automaton */
    enum CellularBoundary boundary; /**< The behavior on the boundaries */
    char *allowed_cells;            /**< The allowed cells */
    unsigned int num_cells;         /**< The number of allowed cells */
    unsigned int *distribution;     /**< Probability distribution */
    bool interactive;               /**< Is the simulation interactive? */
    enum Status status;             /**< The status of the parsing */
    bool initialState;              /**< If there is an initial state to read*/
};

/**
 * Prints how to use the program.
 *
 * @param argv  The arguments provided by the user
 */
void print_usage(char **argv);

/**
 * Returns the parsed arguments provided by the user.
 *
 * @param argc  The number of arguments including the program name
 * @param argv  The arguments provided by the user
 * @return      The parsed arguments
 */
struct Arguments *parse_arguments(int argc, char **argv);

/**
 * Prints the parsed arguments to stdout.
 *
 * @param arguments  The arguments to print
 */
void print_arguments(const struct Arguments *arguments);

/**
 * Frees the arguments.
 *
 * @param arguments  The arguments to free
 */
void free_arguments(struct Arguments *arguments);

#endif
