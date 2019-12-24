/**
 * Implements interactive.h based on the library Ncurses.
 *
 * See http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/ for more details about
 * Ncurses.
 *
 * @author Alexandre Blondin Massé
 */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "interactive.h"
#include "utils.h"

// --------- //
// Constants //
// --------- //

/**
 * Keys text.
 *
 * In other words, it explains to the user how to use the interactive
 * simulator.
 *
 * Note that the `*` characters are used to identify underlined text.
 * Therefore, all strings have the same length when the `*` are removed.
 */
#define KEYS_HEIGHT 4
#define KEYS_LINE  "                                            "
#define KEYS_LINE1 "*F*: Play forward     *B*: Play backward    "
#define KEYS_LINE2 "*s*: Go to start      *e*: Go to end        "
#define KEYS_LINE3 "*f*: One step forward *b*: One step backward"
#define KEYS_LINE4 "*p*: Pause            *q*: Quit             "

#define QUIT_KEY 'q'
#define FORWARD_KEY 'f'
#define BACKWARD_KEY 'b'
#define START_KEY 's'
#define END_KEY 'e'
#define PLAY_FORWARD_KEY 'F'
#define PLAY_BACKWARD_KEY 'B'
#define PAUSE_KEY 'p'

// ------- //
// Private //
// ------- //

/**
 * Sets the current frame of an application.
 *
 * Note: if the given frame is not valid, nothing happens.
 *
 * @param application  The application
 * @param current      The desired frame number
 */
void Interactive_set_current_frame(
    struct InteractiveApplication *application,
    unsigned int current
) {
    if (current < application->num_frames) {
        application->current_frame = current;
    }
}

/**
 * Increments the current frame of an application.
 *
 * Note: if the current frame number is already at the end, then nothing
 * happens.
 *
 * @param application The application
 */
void Interactive_set_to_next_frame(
    struct InteractiveApplication *application
) {
    Interactive_set_current_frame(application, application->current_frame + 1);
}

/**
 * Decrements the current frame of an application.
 *
 * Note: if the current frame number is already at the beginning, then nothing
 * happens.
 *
 * @param application The application
 */
void Interactive_set_to_previous_frame(
    struct InteractiveApplication *application
) {
    Interactive_set_current_frame(application, application->current_frame - 1);
}

/**
 * Returns the automaton at the given frame.
 *
 * Note: The data structure is lazy. This means that if the automaton is
 * already known, then it is simply returned. Otherwise, the automaton is
 * computed (recursively) from the previous one.
 *
 * @param application  The application
 * @param i            The frame number
 * @return             The automaton at the given frame number
 */
const struct CellularAutomaton *Interactive_get_automaton(
    struct InteractiveApplication *application,
    unsigned int i
) {
    if (application->automata[i] == NULL) {
        application->automata[i] = Cellular_next(
            Interactive_get_automaton(application, i - 1)
        );
    }
    return application->automata[i];
}

/**
 * Displays on the screen the cells of the current frame.
 *
 * @param application  The application
 */
void Interactive_display_cells(struct InteractiveApplication *application) {
    const struct CellularAutomaton *automaton =
        Interactive_get_automaton(application, application->current_frame);
    for (unsigned int i = 0; i < automaton->num_rows; ++i) {
        for (unsigned int j = 0; j < automaton->num_cols; ++j) {
            mvwaddch(application->cells_window, i + 1, j + 1,
                     automaton->cells[i][j]);
        }
    }
}

/**
 * Displays on the screen the current application
 *
 * More precisely, it displays both the current cells and the keys explaining
 * how to control the simulation.
 *
 * @param application  The application
 */
void Interactive_display(struct InteractiveApplication *application) {
    refresh();
    box(application->cells_window, 0, 0);
    Interactive_display_cells(application);
    wrefresh(application->cells_window);
    box(application->keys_window, 0, 0);
    mvwprintw(application->keys_window, 0, 2, " Keys ");
    mvwprintw(application->keys_window, 0, 30, " Step: %d ",
              application->current_frame + 1);
    mvwprintw(application->keys_window, 1, 1, KEYS_LINE1);
    mvwprintw(application->keys_window, 2, 1, KEYS_LINE2);
    mvwprintw(application->keys_window, 3, 1, KEYS_LINE3);
    mvwprintw(application->keys_window, 4, 1, KEYS_LINE4);
    wrefresh(application->keys_window);
}

// ------ //
// Public //
// ------ //

struct InteractiveApplication *Interactive_init(
    const struct CellularAutomaton *automaton,
    unsigned int num_frames
) {
    struct InteractiveApplication *application;
    application = malloc(sizeof(struct InteractiveApplication));
    application->automata = calloc(num_frames, sizeof(struct CellularAutomaton*));
    application->automata[0] = Cellular_duplicate(automaton);
    application->current_frame = 0;
    application->num_frames = num_frames;
    application->state = APPLICATION_PAUSED;
    initscr();
    application->cells_window = newwin(automaton->num_rows + 2,
                                       automaton->num_cols + 2,
                                       0, 0);
    application->keys_window = newwin(KEYS_HEIGHT + 2, strlen(KEYS_LINE) + 2,
                                      automaton->num_rows + 2, 0);
    cbreak();
    noecho();
    curs_set(0);
    timeout(300);
    Interactive_display(application);
    return application;
}

void Interactive_run(struct InteractiveApplication *application) {
    while (application->state != APPLICATION_QUITTING) {
        char c = getch();
        if (c == QUIT_KEY) {
            application->state = APPLICATION_QUITTING;
        } else if (c == PAUSE_KEY) {
            application->state = APPLICATION_PAUSED;
        } else if (application->state == APPLICATION_PLAYING_FORWARD) {
            if (application->current_frame == application->num_frames - 1) {
                application->state = APPLICATION_PAUSED;
            } else {
                Interactive_set_to_next_frame(application);
            }
        } else if (application->state == APPLICATION_PLAYING_BACKWARD) {
            if (application->current_frame == 0) {
                application->state = APPLICATION_PAUSED;
            } else {
                Interactive_set_to_previous_frame(application);
            }
        } else if (application->state == APPLICATION_PAUSED) {
            if (c == FORWARD_KEY) {
                Interactive_set_to_next_frame(application);
            } else if (c == BACKWARD_KEY) {
                Interactive_set_to_previous_frame(application);
            } else if (c == START_KEY) {
                Interactive_set_current_frame(application,
                                              0);
            } else if (c == END_KEY) {
                Interactive_set_current_frame(application,
                                              application->num_frames - 1);
            } else if (c == PLAY_FORWARD_KEY) {
                application->state = APPLICATION_PLAYING_FORWARD;
            } else if (c == PLAY_BACKWARD_KEY) {
                application->state = APPLICATION_PLAYING_BACKWARD;
            }
        }
        Interactive_display(application);
    }
}

void Interactive_free(struct InteractiveApplication *application) {
    delwin(application->cells_window);
    delwin(application->keys_window);
    endwin();
    for (unsigned int i = 0; i < application->num_frames; ++i) {
        if (application->automata[i] != NULL) {
            Cellular_free(application->automata[i]);
        }
    }
    free(application->automata);
    free(application);
}
