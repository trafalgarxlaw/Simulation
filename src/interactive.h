/**
 * Provides types and services for handling an interactive simulation of a
 * cellular automaton.
 *
 * @author Alexandre Blondin Massé
 */
#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <ncurses.h>
#include "cellular.h"

// ----- //
// Types //
// ----- //

/**
 * The current state of the application.
 */
enum ApplicationState {
    APPLICATION_QUITTING,                /**< Quitting the application */
    APPLICATION_PAUSED,                  /**< Paused */
    APPLICATION_PLAYING_FORWARD,         /**< Playing forward */
    APPLICATION_PLAYING_BACKWARD         /**< Playing backward */
};

/**
 * An interactive application.
 */
struct InteractiveApplication {
    struct CellularAutomaton **automata; /**< The automaton at each step */
    unsigned int current_frame;          /**< The current step */
    unsigned int num_frames;             /**< The number of steps */
    enum ApplicationState state;         /**< The current state */
    WINDOW *cells_window;                /**< The window of the cells */
    WINDOW *keys_window;                 /**< The window with the keys */
};

// --------- //
// Functions //
// --------- //

/**
 * Initializes an interactive application.
 *
 * @param automaton   The initial automaton (at step 0)
 * @param num_frames  The total number of steps (or frames)
 * @return            The initialized application
 */
struct InteractiveApplication *Interactive_init(
    const struct CellularAutomaton *automaton,
    unsigned int num_frames
);

/**
 * Starts the interactive application.
 *
 * @param application  The application to start
 */
void Interactive_run(struct InteractiveApplication *application);

/**
 * Frees an interactive application
 *
 * @param application  The application to free
 */
void Interactive_free(struct InteractiveApplication *application);

#endif
