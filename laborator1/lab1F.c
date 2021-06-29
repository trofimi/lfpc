#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SNL 10

typedef struct transition {
    char c;
    char *newState;
    struct transition *nextPtr;
} Transition;

typedef struct state {
    char *sName;
    Transition *tList;
} State;

int getNStates() { // gets the number of states in the automaton
    int n;
    
    if(scanf("%d", &n) != 1 || n <= 0) {
        exit(1);
    }
    
    return n;
}

State *createNewState(char stateName[]) { // creates a new state with the specified name and returns it
    State *newState = malloc(sizeof(State));
    if(newState == NULL) exit(1);
    
    newState->sName = malloc(sizeof(char) * SNL);
    if(newState->sName == NULL) exit(1);
    
    strcpy(newState->sName, stateName);
    newState->tList = NULL;
    
    return newState;
}

void appendTransition(Transition **tList, char *newS, char ch) { // appends a new transition to the existing list, recursively
    if(*tList == NULL) {
        *tList = malloc(sizeof(Transition));
        if(tList == NULL) exit(1);
        
        (*tList)->newState = malloc(sizeof(char) * strlen(newS)); // allocate memory for new state's name
        if((*tList)->newState == NULL) exit(1);
        
        strcpy((*tList)->newState, newS); // initialize new transition
        (*tList)->c = ch;
        (*tList)->nextPtr = NULL;
    } else {
        appendTransition(&((*tList)->nextPtr), newS, ch);
    }
}

State *move(State **automaton, int nStates, State *currState, char ch) { // returns a state if transition is found, NULL is no transition is found
    Transition *tList = currState->tList;
    if(tList == NULL) return NULL;
    
    while(tList != NULL) {
        if(tList->c == ch) { // found correct transition
            for(size_t i = 0; i < nStates; i++) {
                if(!strcmp(automaton[i]->sName, tList->newState)) { // find the correct state in the table
                    return automaton[i];
                }
            }
        }
        tList = tList->nextPtr;
    }
    
    return NULL;
}

int evaluate(State **automaton, int nStates, char *line) {
    if(*automaton == NULL) {
        return 1;
    }
    
    State *st = automaton[0]; // initial state is the first state
    
    for(size_t i = 0; i < strlen(line); i++) {
        st = move(automaton, nStates, st, line[i]); // transition to the next state
        if(st == NULL) {
            return 0;
        }
    }
    
    if(st->sName[0] == 'F') { // if state name begins with 'F', it's a terminal state
        return 1;
    }
    
    return 0;
}

int main() {
    int n = getNStates();
    
    char stateName[SNL]; // used to store current state's name
    char newState[SNL]; // used to store newState's name for every transition
    char c; // used to store character that leads to newState for every transition
    char line[100]; // used to store current line of text
    
    State **automaton = malloc(n*sizeof(State)); // create the automaton table
    if(automaton == NULL) exit(1);
    
    while(getchar() != '\n'); // clear buffer
    
    for(size_t i = 0; i < n; i++) { // get as many lines of text as there are states
        scanf("%[^\n]", line);
        
        automaton[i] = malloc(sizeof(State)); // allocate memory for new state
        if(automaton[i] == NULL) exit(1);
        
        char *token = strtok(line, ";"); // first token is the state name
        automaton[i]->sName = malloc(sizeof(char) * (strlen(token) + 1)); // allocate memory for state name
        if(automaton[i]->sName == NULL) exit(1);

        strcpy(automaton[i]->sName, token); // copy state name into the table
        automaton[i]->tList = NULL; // initialize current state transition table

        while(token = strtok(NULL, ";")) { // keep reading as long as there are transitions
            sscanf(token, "(%[^,],%c)", newState, &c);
            appendTransition(&(automaton[i]->tList), newState, c); // append new transition to state's list
        }
        while(getchar() != '\n'); // clear buffer
    }
    
    printf("enter 'exit' to end\n");
    scanf("%100s", line);
    while(strcmp(line,"exit")) {
        if(evaluate(automaton, n, line)) {
            printf("String accepted\n");
        }
        else printf("String not accepted\n");
        printf("Enter another string\n");
        scanf("%100s", line);
    }
    return 0;
//input    
//4
//S;(B,a);(B,b)
//B;(D,b);(B,c);(S,a)
//D;(F1,b);(D,a)
//F1
//
}