/**
 * Implements cellular.h.
 *
 * @author Alexandre Blondin Massé
 */
#include "cellular.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// ------- //
// Private //
// ------- //

#define CELLULAR_PANDEMY_STRING "\
Pandemy-type cellular automaton\n\
  %c -> unoccupied,\n\
  %c -> sick,\n\
  %c -> healthy\n\
"
#define CELLULAR_GAME_OF_LIFE_STRING "\
Game-of-life-type cellular automaton\n\
  %c -> live,\n\
  %c -> dead,\n\
"
#define CELLULAR_FIRE_STRING "\
Fire-type cellular automaton\n\
  %c -> growing,\n\
  %c -> ignitable,\n\
  %c -> burning\n\
  %c -> burnt\n\
"

/**
 * Prints the type of cellular automaton to stdout.
 *
 * @param automaton  The automaton whose type is printed
 */
void Cellular_print_type(const struct CellularAutomaton *automaton) {
    switch (automaton->type) {
        case CELLULAR_PANDEMY:
            printf(CELLULAR_PANDEMY_STRING, automaton->allowed_cells[0],
                   automaton->allowed_cells[1], automaton->allowed_cells[2]);
            break;
        case CELLULAR_GAME_OF_LIFE:
            printf(CELLULAR_GAME_OF_LIFE_STRING, automaton->allowed_cells[0],
                   automaton->allowed_cells[1]);
            break;
        case CELLULAR_FIRE:
            printf(CELLULAR_FIRE_STRING, automaton->allowed_cells[0],
                   automaton->allowed_cells[1], automaton->allowed_cells[2],
                   automaton->allowed_cells[3]);
            break;
    }
}

/**
 * Returns the cell at given row and column.
 *
 * If the row or column number is invalid, a boundary strategy is used. See
 * `enum CellularBoundary` for more details.
 *
 * @param automaton  The automaton
 * @param row        The row number
 * @param col        The column number
 * @return           The cell
 */
char Cellular_safe_get(
    const struct CellularAutomaton *automaton,
    int row,
    int col
) {
    switch (automaton->boundary) {
        case CELLULAR_TRUNCATE:
            if (row < 0 || col < 0 || row >= (int)automaton->num_rows ||
                col >= (int)automaton->num_cols) {
                return automaton->allowed_cells[0];
            } else {
                return automaton->cells[row][col];
            }
        case CELLULAR_WRAP_AROUND:
            return automaton->cells[mod(row, automaton->num_rows)]
                                   [mod(col, automaton->num_cols)];
        default:
            return UNINITIALIZED_CELL;
    }
}

/**
 * Returns the number of neighbors of a cell having given type.
 *
 * @param automaton  The cellular automaton
 * @param row        The row number
 * @param col        The column number
 * @param cell       The type of the cell
 * @param diagonals  If true, include diagonal neighbors
 */
unsigned int Cellular_num_neighbors(
    const struct CellularAutomaton *automaton,
    int row,
    int col,
    char cell,
    bool diagonals
) {
    unsigned int num_cells = 0;
    for (int drow = -1; drow <= 1; ++drow) {
        for (int dcol = -1; dcol <= 1; ++dcol) {
            unsigned int dist = abs(drow) + abs(dcol);
            bool is_neighbor = dist == 1 || (dist == 2 && diagonals);
            if (is_neighbor &&
                Cellular_safe_get(automaton, row + drow, col + dcol) == cell) {
                ++num_cells;
            }
        }
    }
    return num_cells;
}

/**
 * Returns the next cell according to its neighborhood in the pandemy case.
 *
 * The rules are the following:
 *
 * 1. A cell is either unoccupied, healthy or sick.
 * 2. The neighborhood of a cell is of Van Neuman type, i.e., it includes the
 *    diagonals. Therefore, each cell has exactly eight neighbors (except on
 *    the boundary).
 * 3. An unoccupied cell becomes occupied if exactly 3 of its neighbors are
 *    occupied. If the majority of the neighbors are infected, then the cell is
 *    infected, otherwise it is healthy.
 * 4. An occupied cell stays occupied if either 2 or 3 of its neighbors are
 *    occupied. If at least half of its neighbors are infected, then it becomes
 *    infected as well, otherwise, it stays in its current state.
 * 5. If a cell has less than 2 or more than 3 neighbors, then it becomes
 *    unoccupied: It dies from loneliness or from suffocation.
 *
 * @param automaton  The automaton
 * @param row        The row number
 * @param col        The column number
 * @return           The cell obtained by applying the rule
 */
char Cellular_next_cell_pandemy(
    const struct CellularAutomaton *automaton,
    unsigned int row,
    unsigned int col
) {
    char empty = automaton->allowed_cells[0];
    char sick = automaton->allowed_cells[1];
    char healthy = automaton->allowed_cells[2];
    unsigned int num_healthy = Cellular_num_neighbors(
        automaton, row, col, healthy, true);
    unsigned int num_sick = Cellular_num_neighbors(
        automaton, row, col, sick, true);
    unsigned int num_alive = num_healthy + num_sick;
    char current = automaton->cells[row][col];
    if (current == empty && num_alive == 3) {
        return num_sick > num_healthy ? sick : healthy;
    } else if (current != empty && (num_alive == 2 || num_alive == 3)) {
        return num_sick >= num_healthy ? sick : current;
    } else {
        return empty;
    }
}

/**
 * Returns the next cell according to its neighborhood in the game of life
 * case.
 *
 * The rules are the following (from Wikipedia):
 *
 * 1. A cell is either live or dead.
 * 2. The neighbordhood of a cell includes diagonals.
 * 3. Any live cell with fewer than two live neighbors dies, as if by under
 *    population.
 * 4. Any live cell with two or three live neighbors lives on to the next
 *    generation.
 * 5. Any live cell with more than three live neighbors dies, as if by
 *    overpopulation.
 * 6. Any dead cell with exactly three live neighbors becomes a live cell, as
 *    if by reproduction.
 *
 * @param automaton  The automaton
 * @param row        The row number
 * @param col        The column number
 * @return           The cell obtained by applying the rule
 */
char Cellular_next_cell_game_of_life(
    const struct CellularAutomaton *automaton,
    unsigned int row,
    unsigned int col
) {
    char dead = automaton->allowed_cells[0];
    char live = automaton->allowed_cells[1];
    char current = automaton->cells[row][col];
    unsigned int num_lives =
        Cellular_num_neighbors(automaton, row, col, live, true);
    if (current == live) {
        return num_lives == 2 || num_lives == 3 ? live : dead;
    } else {
        return num_lives == 3 ? live : dead;
    }
}

/**
 * Returns the next cell according to its neighborhood in the fire case.
 *
 * The rules are the following:
 *
 * 1. A cell is either growing, ignitable, burning or burnt;
 * 2. The neighborhood of a cell is of Van Neuman type, i.e., it includes the
 *    diagonals. Therefore, each cell has exactly eight neighbors (except on
 *    the boundary).
 * 3. A growing cell becomes ignitable in the next step.
 * 4. An ignitable cell remains ignitable, unless at least one neighbor is
 *    burning. In that case, the cell becomes burning in the next step.
 * 5. A burning cell becomes a burnt cell in the next step.
 * 6. A burnt cell becomes a growing cell in the next step.
 *
 * @param automaton  The automaton
 * @param row        The row number
 * @param col        The column number
 * @return           The cell obtained by applying the rule
 */
char Cellular_next_cell_fire(
    const struct CellularAutomaton *automaton,
    unsigned int row,
    unsigned int col
) {
    char growing = automaton->allowed_cells[0];
    char ignitable = automaton->allowed_cells[1];
    char burning = automaton->allowed_cells[2];
    char burnt = automaton->allowed_cells[3];
    char current = automaton->cells[row][col];
    if (current == ignitable) {
        if (Cellular_num_neighbors(automaton, row, col, burning, true) >= 1) {
            return burning;
        } else {
            return ignitable;
        }
    } else if (current == burning) {
        return burnt;
    } else if (current == burnt) {
        return growing;
    } else {
        return ignitable;
    }
}

/**
 * Returns the next cell according to its neighborhood.
 *
 * The rule for updating the cell depends on the automaton type. See `enum
 * CellularType` for more details.
 *
 * @param automaton  The automaton
 * @param row        The row number
 * @param col        The column number
 * @return           The cell obtained by applying some rule
 */
char Cellular_next_cell(
    const struct CellularAutomaton *automaton,
    unsigned int row,
    unsigned int col
) {
    switch (automaton->type) {
        case CELLULAR_PANDEMY:
            return Cellular_next_cell_pandemy(automaton, row, col);
        case CELLULAR_GAME_OF_LIFE:
            return Cellular_next_cell_game_of_life(automaton, row, col);
        case CELLULAR_FIRE:
            return Cellular_next_cell_fire(automaton, row, col);
        default:
            return UNINITIALIZED_CELL;
    }
}

/**
 * Returns a random cell according to a probability distribution.
 *
 * @param automaton     The automaton
 * @param distribution  The distribution
 * @return              A random cell
 */
char Cellular_get_random_cell(
    const struct CellularAutomaton *automaton,
    const unsigned int *distribution
) {
    unsigned int num_cells = strlen(automaton->allowed_cells);
    unsigned int sum = 0;
    for (unsigned int k = 0; k < num_cells; ++k) {
        sum += distribution[k];
    }
    unsigned int p = rand() % sum;
    unsigned int k = 0;
    unsigned int q = distribution[0];
    while (q <= p) {
        ++k;
        q += distribution[k];
    }
    return automaton->allowed_cells[k];
}

// ------ //
// Public //
// ------ //

struct CellularAutomaton *Cellular_init(
    unsigned int num_rows,
    unsigned int num_cols,
    enum CellularType type,
    enum CellularBoundary boundary,
    const char *allowed_cells
) {
    if (Cellular_is_valid(type, allowed_cells)) {
        struct CellularAutomaton *automaton
            = malloc(sizeof(struct CellularAutomaton));
        automaton->num_rows = num_rows;
        automaton->num_cols = num_cols;
        automaton->type = type;
        automaton->boundary = boundary;
        automaton->allowed_cells = strdupli(allowed_cells);
        automaton->cells = calloc(num_rows, sizeof(char*));
        for (unsigned int i = 0; i < automaton->num_rows; ++i) {
            automaton->cells[i] = calloc(num_cols, sizeof(char));
            for (unsigned int j = 0; j < automaton->num_cols; ++j) {
                automaton->cells[i][j] = UNINITIALIZED_CELL;
            }
        }
        return automaton;
    } else {
        return NULL;
    }
}

struct CellularAutomaton *Cellular_init_with_state(
    unsigned int num_rows,
    unsigned int num_cols,
    enum CellularType type,
    enum CellularBoundary boundary,
    const char *allowed_cells,
    InitialState cellularArray
) {
    if (Cellular_is_valid(type, allowed_cells)) {
        struct CellularAutomaton *automaton
            = malloc(sizeof(struct CellularAutomaton));
        automaton->num_rows = num_rows;
        automaton->num_cols = num_cols;
        automaton->type = type;
        automaton->boundary = boundary;
        automaton->allowed_cells = strdupli(allowed_cells);
        automaton->cells = calloc(num_rows, sizeof(char*));
        for (unsigned int i = 0; i < automaton->num_rows; ++i) {
            automaton->cells[i] = calloc(num_cols, sizeof(char));
            for (unsigned int j = 0; j < automaton->num_cols; ++j) {
                automaton->cells[i][j] = cellularArray.elemets[i][j];
            }
        }
        return automaton;
    } else {
        return NULL;
    }
}

struct CellularAutomaton *Cellular_duplicate(
    const struct CellularAutomaton *automaton
) {
    struct CellularAutomaton *copy = Cellular_init(
        automaton->num_rows, automaton->num_cols, automaton->type,
        automaton->boundary, automaton->allowed_cells
    );
    for (unsigned int i = 0; i < automaton->num_rows; ++i) {
        for (unsigned int j = 0; j < automaton->num_cols; ++j) {
            copy->cells[i][j] = automaton->cells[i][j];
        }
    }
    return copy;
}

void Cellular_set_random(
    struct CellularAutomaton *automaton,
    const unsigned int *distribution
) {
	time_t t;
	srand((unsigned) time(&t));
    for (unsigned int i = 0; i < automaton->num_rows; ++i) {
        for (unsigned int j = 0; j < automaton->num_cols; ++j) {
	        automaton->cells[i][j] =
                Cellular_get_random_cell(automaton, distribution);
        }
    }
}

void Cellular_free(struct CellularAutomaton *automaton) {
    for (unsigned int i = 0; i < automaton->num_rows; ++i) {
        free(automaton->cells[i]);
    }
    free(automaton->cells);
    free(automaton->allowed_cells);
    free(automaton);
}

void Cellular_print(const struct CellularAutomaton *automaton,
                    bool print_type) {
    if (print_type) Cellular_print_type(automaton);
    for (unsigned int i = 0; i < automaton->num_rows; ++i) {
        for (unsigned int j = 0; j < automaton->num_cols; ++j) {
            printf("%c", automaton->cells[i][j]);
        }
        printf("\n");
    }
}


struct CellularAutomaton *Cellular_next(
    const struct CellularAutomaton *automaton
) {
    struct CellularAutomaton *next = Cellular_duplicate(automaton);
    for (unsigned int i = 0; i < automaton->num_rows; ++i) {
        for (unsigned int j = 0; j < automaton->num_cols; ++j) {
            next->cells[i][j] = Cellular_next_cell(automaton, i, j);
        }
    }
    return next;
}

unsigned int Cellular_num_cells(enum CellularType type) {
    switch (type) {
        case CELLULAR_GAME_OF_LIFE:
            return 2;
        case CELLULAR_PANDEMY:
            return 3;
        case CELLULAR_FIRE:
            return 4;
        default:
            return 0;
    }
}

bool Cellular_is_valid(enum CellularType type, const char *allowed_cells) {
    return strlen(allowed_cells) == Cellular_num_cells(type);
}

void Cellular_Valid_StateCells(const char *allowed_cells,
const struct CellularAutomaton * automaton){

    //for all cells in automaton
    for (unsigned int i = 0; i < automaton->num_rows; i++)
    {
        for (unsigned int j = 0; j < automaton->num_cols; j++)
        {
            //if there is a cell that is not allowed
            if(strchr(allowed_cells, automaton->cells[i][j])==NULL){
                fprintf(stderr,"Error: The cell state '%c' is not allowed\n",automaton->cells[i][j]);
                exit(9);

            }
        }
        
    }
   
}