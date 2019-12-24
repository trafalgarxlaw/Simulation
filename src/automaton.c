/**
 * Main entry point of the program.
 *
 * Basically, it retrieves the arguments provided by the user and launch the
 * simulation, either by printing it to stdout as a simple liste of states, or
 * by using the interactive mode.
 *
 * @author Alexandre Blondin Massé
 */
#include <stdio.h>
#include "parse_args.h"
#include "cellular.h"
#include "interactive.h"
#include <string.h>

/**
 * Returns if all the lenghts of the rows and columns of the 
 * Initial state in the txt file is the same.
 * 
 * @param initialState  the String representing the initial state
 * @return isvalid      if the lenghts are correct.
 */
bool lenghtState_is_valid(char *initialState){

    //initialisation
    bool isvalid=true;
    int col=0;
    int Tablenght[10000];
    int TabCursor=0;
    
    //Here, we are reading a string. The initial state with '\n' newlines
    //to differentiate the rows.
    for (int i = 0; i < strlen(initialState); i++)
    {
        //if we encounter a newline, we stop counting and
        //the lenght of the row is stored in an array.
        if(initialState[i]=='\n'){
                Tablenght[TabCursor]=col;
                TabCursor++;
                col=0;
            }else
            {
              col++;
            }
        
    }
    //veryfing if all the lenghts in the array are the same
    for (int i = 1; i < TabCursor; i++)
    {
        if(Tablenght[i]!=Tablenght[i-1]){isvalid=false;}
    }
    
    return isvalid;

}

/**
 * Reads in stdin the initial state of the simulation
 * from the redirected input file (.txt)
 *
 * @param arguments       The arguments given by the user
 * @return cellularArray  The structure representing the initial state in a 2D Array
 */
InitialState ReadStdin(struct Arguments *arguments ){
        
        //initialisation
        int row=0,col=0;
        char initialState[10000]="";
        char initialState_With_Newline[10000]="";
        char ReadState[10000]="";
        
        //Reading the initial state from stdin until no input left.
        while (fgets(ReadState, 100 , stdin) != NULL)
        {
        char substring[10000]="";
        char substring2[10000]="";

        // this function keeps the newline in the string 
        // to be able to verify if all row & col length are correct.

        strcat(initialState_With_Newline,strncpy(substring2, ReadState, strlen(ReadState)));

        //This function is used to remove newlines in the initial state

        strcat(initialState,strncpy(substring, ReadState, strlen(ReadState)-1));

       // the lenght of the row gives us the number of columns
       // we can compare this value to the lenght of other rows to see if evrything is correct.

        col = strlen(substring); 

        row++;
        }

        // Verifying if All rows and columns have the same length

        if (!lenghtState_is_valid(initialState_With_Newline))
        {
            // if so there is an Error
            fprintf(stderr,"Error: All rows and columns should be of the same length\n");
            exit(10);
        }

        //If evrything is fine, we store the initial state in a structure.
        int cursor=0;
        InitialState cellularArray;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cellularArray.elemets[i][j]=initialState[cursor];
                cursor++;
            }

        }
        //Updating arguments rows and cols in the arguments.
        arguments->num_cols=col;
        arguments->num_rows=row;


        
 return cellularArray;
}

int main(int argc, char **argv) {
    struct Arguments *arguments = parse_arguments(argc, argv); //takes the arguments in the structure
    if (arguments->status != TP2_OK) {  //if it fails
        return arguments->status;
    } else if (arguments->initialState) // if there is an initial state
    {
        InitialState cellularArray=ReadStdin(arguments);
        
        struct CellularAutomaton *automaton, *next; //next here means the next automaton step
        automaton = Cellular_init_with_state(arguments->num_rows, //initialisation of automaton, takes all the arguments
                                  arguments->num_cols, //num_rows and  num_cols are not set if --stdin is set
                                  arguments->type,
                                  arguments->boundary,
                                  arguments->allowed_cells,
                                  cellularArray);

        Cellular_Valid_StateCells(arguments->allowed_cells,automaton);
        
        if (arguments->interactive) { //if the interactive mod is choosen
        freopen("/dev/tty", "rw", stdin);
                    struct InteractiveApplication *application =
                Interactive_init(automaton, arguments->num_steps);
            Interactive_run(application);
            Interactive_free(application);
     
        } else { //if not
            for (unsigned int step = 0; step < arguments->num_steps; ++step) {
                printf("Step %d\n", step);
                Cellular_print(automaton, false);// prints the State initialisation of automaton
                next = Cellular_next(automaton);
                Cellular_free(automaton);
                automaton = next;
            }
        }
        Cellular_free(automaton);   


    
    } else {//if it pass
        struct CellularAutomaton *automaton, *next; 
        automaton = Cellular_init(arguments->num_rows, 
                                  arguments->num_cols, 
                                  arguments->type,
                                  arguments->boundary,
                                  arguments->allowed_cells);
        Cellular_set_random(automaton, arguments->distribution); //creates a random initial state
        if (arguments->interactive) { //if the interactive mod is choosen
            struct InteractiveApplication *application =
                Interactive_init(automaton, arguments->num_steps);
            Interactive_run(application);
            Interactive_free(application);
        } else { //if not
            for (unsigned int step = 0; step < arguments->num_steps; ++step) {
                printf("Step %d\n", step);
                Cellular_print(automaton, false);// prints the random initialisation of automaton
                next = Cellular_next(automaton);
                Cellular_free(automaton);
                automaton = next;
            }
        }
        Cellular_free(automaton);
    }
    free_arguments(arguments);
    return TP2_OK;
}

