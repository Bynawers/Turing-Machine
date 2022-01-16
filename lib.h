typedef struct MachineTuring{
    char* code;
    char currentState;
    char* tape;
    int headPosition;
    char finalState;
    int success;
}MachineTuring;