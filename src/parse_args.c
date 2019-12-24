/**
 * Implements parse_args.h, using the getopt library to simplify the
 * processing.
 *
 * @author Alexandre Blondin Masse
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "parse_args.h"
#include "utils.h"

#define DELIM ','
#define DELIMS ","

// ------- //
// Private //
// ------- //

/**
 * Casts a string to an unsigned value.
 *
 * The returned value indicates if everything went well.
 *
 * @param s      The string to cast
 * @param value  The resulting value
 * @return       The status of the cast
 */
enum Status cast_unsigned_integer(const char *s,
                                  unsigned int *value) {
    char *p;
    int signed_value;
    signed_value = strtol(s, &p, 10);
    if (p != NULL && *p != '\0') {
        return TP2_WRONG_VALUE;
    } else if (signed_value < 0) {
        return TP2_WRONG_VALUE;
    } else {
        *value = (unsigned int)signed_value;
        return TP2_OK;
    }
}

/**
 * Retrives the type of simulation from a string.
 *
 * @param s          The string from which the type is retrieved
 * @param arguments  The parsed arguments
 * @return           The status of the extraction
 */
enum Status get_type(const char *s,
                     struct Arguments *arguments) {
    if (strcmp(s, GOF_TYPE) == 0) {
        arguments->type = CELLULAR_GAME_OF_LIFE;
    } else if (strcmp(s, PANDEMY_TYPE) == 0) {
        arguments->type = CELLULAR_PANDEMY;
    } else if (strcmp(s, FIRE_TYPE) == 0) {
        arguments->type = CELLULAR_FIRE;
    } else {
        return TP2_WRONG_SIMULATION_TYPE;
    }
    return TP2_OK;
}

/**
 * Retrives the boundary type from a string.
 *
 * @param s          The string from which the boundary type is retrieved
 * @param arguments  The parsed arguments
 * @return           The status of the extraction
 */
enum Status get_boundary(const char *s,
                         struct Arguments *arguments) {
    if (strcmp(s, BOUNDARY_TRUNCATE) == 0) {
        arguments->boundary = CELLULAR_TRUNCATE;
    } else if (strcmp(s, BOUNDARY_PERIODIC) == 0) {
        arguments->boundary = CELLULAR_WRAP_AROUND;
    } else {
        return TP2_WRONG_BOUNDARY;
    }
    return TP2_OK;
}

/**
 * Retrives the allowed cells from a string.
 *
 * @param s          The string from which the allowed cells are retrieved
 * @param arguments  The parsed arguments
 * @return           The status of the extraction
 */
enum Status get_allowed_cells(const char* s,
                              struct Arguments *arguments) {
    arguments->allowed_cells = strdupli(s);
    return TP2_OK;
}

/**
 * Retrives a distribution probability from a string.
 *
 * @param s          The string from which the distribution is retrieved
 * @param arguments  The parsed arguments
 * @return           The status of the extraction
 */
enum Status get_distribution(const char* s,
                             struct Arguments *arguments) {
    enum Status status = TP2_OK;
    arguments->num_cells = num_occurrences(s, DELIM) + 1;
    arguments->distribution = calloc(arguments->num_cells, sizeof(unsigned int));
    char *t = strdupli(s);
    char *pc = strtok(t, DELIMS);
    unsigned int d = 0;
    while (status == TP2_OK && d < arguments->num_cells && pc != NULL) {
        status = cast_unsigned_integer(pc, arguments->distribution + d);
        status = status == TP2_OK ? TP2_OK : TP2_WRONG_DISTRIBUTION;
        pc = strtok(NULL, DELIMS);
        ++d;
    }
    free(t);
    return status;
}

/**
 * Sets the default arguments.
 *
 * @param arguments  The parsed arguments
 */
void set_default(struct Arguments *arguments) {
    get_type(DEFAULT_TYPE, arguments);
    get_allowed_cells(DEFAULT_CELLS, arguments);
    arguments->num_cells = Cellular_num_cells(arguments->type);
    get_distribution(DEFAULT_DISTRIBUTION, arguments);
}


// ------ //
// Public //
// ------ //

void print_usage(char **argv) {
    printf(USAGE, argv[0], GOF_TYPE, PANDEMY_TYPE, FIRE_TYPE, DEFAULT_TYPE,
           BOUNDARY_TRUNCATE, BOUNDARY_PERIODIC, DEFAULT_BOUNDARY);
}

struct Arguments *parse_arguments(int argc, char *argv[]) {
    struct Arguments *arguments = malloc(sizeof(struct Arguments));

    // Temporary variables
    bool show_help = false;
    bool use_default = true;
    bool type_set = false;
    bool allowed_cells_set = false;
    bool row_or_column_set=false; //Verify if there is any row/col argument

    // Default argument
    arguments->interactive = false;
    arguments->status = TP2_OK;
    arguments->num_rows = NUM_ROWS_DEFAULT;
    arguments->num_cols = NUM_COLS_DEFAULT;
    arguments->num_steps = NUM_STEPS_DEFAULT;
    arguments->type = -1;
    get_boundary(DEFAULT_BOUNDARY, arguments);
    arguments->allowed_cells = NULL;
    arguments->distribution = NULL;
    arguments->initialState=false; // by default, there is no initial state to read

    // Resets index
    optind = 0;
    struct option long_opts[] = {
        // Set flag
        {"help",            no_argument,       0, 'h'},
        {"interactive",     no_argument,       0, 'i'},
        {"stdin",           no_argument,       0, 's'},//new long option
        // Don't set flag
        {"num-rows",        required_argument, 0, 'r'},
        {"num-cols",        required_argument, 0, 'c'},
        {"num-steps",       required_argument, 0, 'n'},
        {"type",            required_argument, 0, 't'},
        {"boundary",        required_argument, 0, 'b'},
        {"allowed-cells",   required_argument, 0, 'a'},
        {"distribution",    required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    // Parse options
    while (true) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "hir:c:n:t:b:a:d:s:",
                            long_opts, &option_index);
        if (c == -1) break;
        switch (c) {
            case 's': arguments->initialState=true; //Gustum initialState selected
                      break;
            case 'h': show_help = true;
                      break;
            case 'i': arguments->interactive = true;
                      break;
            case 'r': if (arguments->status == TP2_OK) {
                          arguments->status =
                              cast_unsigned_integer(optarg,
                                                    &arguments->num_rows);
                        row_or_column_set=true;
                      }
                      break;
            case 'c': if (arguments->status == TP2_OK) {
                          arguments->status =
                              cast_unsigned_integer(optarg,
                                                    &arguments->num_cols);
                        row_or_column_set=true;                            
                      }
                      break;
            case 'n': if (arguments->status == TP2_OK) {
                          arguments->status =
                              cast_unsigned_integer(optarg,
                                                    &arguments->num_steps);
                      }
                      break;
            case 't': if (arguments->status == TP2_OK) {
                          use_default = false;
                          type_set = true;
                          arguments->status =
                              get_type(optarg, arguments);
                      }
                      break;
            case 'b': if (arguments->status == TP2_OK) {
                          arguments->status =
                              get_boundary(optarg, arguments);
                      }
                      break;
            case 'a': if (arguments->status == TP2_OK) {
                          use_default = false;
                          allowed_cells_set = true;
                          arguments->status =
                              get_allowed_cells(optarg, arguments);
                      }
                      break;
            case 'd': if (arguments->status == TP2_OK) {
                          arguments->status =
                              get_distribution(optarg, arguments);
                      }
                      break;
            case '?': if (arguments->status == TP2_OK) {
                          arguments->status = TP2_BAD_OPTION;
                      }
                      break;
        }
    }

    if (use_default) {
        set_default(arguments);
    }
    if (arguments->distribution == NULL) {
        get_distribution(DEFAULT_DISTRIBUTION, arguments);
    }
    if (optind < argc) {
        printf("Error: too many arguments\n");
        print_usage(argv);
        arguments->status = TP2_ERROR_TOO_MANY_ARGUMENTS;
    } else if (show_help) {
        print_usage(argv);
        exit(TP2_OK);
    } else if (arguments->status == TP2_WRONG_SIMULATION_TYPE) {
        printf("Error: unrecognized simulation type.\n");
        printf("The supported simulation types are %s\n", SUPPORTED_TYPES);
        print_usage(argv);
    } else if (arguments->status == TP2_WRONG_BOUNDARY) {
        printf("Error: unrecognized boundary.\n");
        printf("The supported boundaries are %s\n", SUPPORTED_BOUNDARIES);
        print_usage(argv);
    } else if (arguments->status == TP2_WRONG_VALUE) {
        printf("Error: the number of rows, columns and steps must be "\
               "positive integers.\n");
        print_usage(argv);
    } else if (arguments->status == TP2_WRONG_DISTRIBUTION) {
        printf("Error: the distribution must be a list of comma-separated "\
               "positive integers.\n");
        print_usage(argv);
    } else if (type_set != allowed_cells_set) {
        printf("Error: The type and the allowed cells must be set "\
               "at the same time.\n");
        arguments->status = TP2_INCONSISTENT_ARGS;
        print_usage(argv);
    } else if (!Cellular_is_valid(arguments->type, arguments->allowed_cells)) {
        printf("Error: The simulation and the allowed cells are inconsistent.\n");
        arguments->status = TP2_INCONSISTENT_ARGS;
        print_usage(argv);
    }
    // if a gutstum initial state and num_row/col is selected, then there is an error.
    if (arguments->initialState && row_or_column_set ) {
        fprintf(stderr,"Error: The number of rows and columns cannot be indicated together with the option --stdin\n");
        arguments->status = TP2_ERROR_STDIN_WITH_ROW_COL;
    } 
    
    return arguments;
}

void print_arguments(const struct Arguments *arguments) {
    printf("struct Arguments {\n");
    printf("  num_rows     = %d\n", arguments->num_rows);
    printf("  num_cols     = %d\n", arguments->num_cols);
    printf("  num_steps    = %d\n", arguments->num_steps);
    printf("  type         = %d\n", arguments->type);
    printf("  boundary     = %d\n", arguments->boundary);
    printf("  cells        = %s\n", arguments->allowed_cells);
    printf("  num_cells    = %d\n", arguments->num_cells);
    printf("  distribution =");
    for (unsigned int i = 0; i < arguments->num_cells; ++i)
        printf(" %d", arguments->distribution[i]);
    printf("\n");
    printf("  interactive  ? %s\n", arguments->interactive ? "yes" : "no");
    printf("  status       = %d\n", arguments->status);
    printf("}\n");
}

void free_arguments(struct Arguments *arguments) {
    free(arguments->allowed_cells);
    free(arguments->distribution);
    free(arguments);
}
