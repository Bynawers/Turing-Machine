#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printTuringMachine(MachineTuring mt, char* name){
    printf("\n     \033[0;36m//%s//\033[0m\n", name);
    printf("Code: %s\n", mt.code);
    printf("CurrentState: %c\n", mt.currentState);
    printf("Tape: %s\n", mt.tape);
    printf("HeadPosition: %d\n", mt.headPosition);
}

void printStep(MachineTuring mt, char* name){
    printf("\n     \033[0;36m//%s//\033[0m\n", name);
    printf("Tape: %s\n", mt.tape);
    printf("CurrentState: %c\n", mt.currentState);
    printf("HeadPosition: %d\n", mt.headPosition);
}

MachineTuring fileToChar(char* fileName){

    MachineTuring mt;
    int size_of_file;
    char c[1];
    char* s;

    FILE* file;
    file = fopen(fileName,"r");
    if (file == NULL){ 
        printf("\033[0;31mERROR FILE DOSNT EXIST\033[0m\n"); 
        exit(EXIT_FAILURE);
    }
    fseek(file, 0L, SEEK_END);
    size_of_file = ftell(file);
    rewind(file);

    mt.code = malloc(size_of_file);
    memset(mt.code, '\0', sizeof(char)*size_of_file);
    s = malloc(5*sizeof(char));

    /* LECTURE FICHIER */
    fscanf(file, "%[^=]", s);
    fscanf(file,"%c",c);
    fscanf(file,"%c",c);
    mt.finalState = c[0];
    fscanf(file,"%c",c);

    while(fscanf(file,"%c",c) == 1){

        fscanf(file,"%c",c);
        strcat(mt.code, c);
        fscanf(file,"%c",c);
        fscanf(file,"%c",c);
        strcat(mt.code, c);
        fscanf(file,"%c",c);

        fscanf(file,"%c",c);
        strcat(mt.code, c);
        fscanf(file,"%c",c);
        fscanf(file,"%c",c);
        strcat(mt.code, c);
        fscanf(file,"%c",c);
        fscanf(file,"%c",c);
        strcat(mt.code, c);
        strcat(mt.code, "#");
        fscanf(file,"%c",c);
    }
    free(s);
    fclose(file);
    return mt;
}

MachineTuring fileToCharAlphabet(char* fileName){

    MachineTuring mt;
    int size_of_file;
    char c[1];
    char* s;

    FILE* file;
    file = fopen(fileName,"r");
    if (file == NULL){ 
        printf("\033[0;31mERROR FILE DOSNT EXIST\033[0m\n"); 
        exit(EXIT_FAILURE);
    }
    fseek(file, 0L, SEEK_END);
    size_of_file = ftell(file);
    rewind(file);

    mt.code = malloc(size_of_file);
    memset(mt.code, '\0', sizeof(char)*size_of_file);
    s = malloc(5*sizeof(char));

    /* LECTURE FICHIER */
    fscanf(file, "%[^=]", s);
    fscanf(file,"%c",c);
    fscanf(file,"%c",c);
    mt.finalState = c[0];
    fscanf(file,"%c",c);

    while(fscanf(file,"%c",c) == 1){

        fscanf(file,"%c",c);   //A
        strcat(mt.code, c);
        fscanf(file,"%c",c);   //,
        fscanf(file,"%c",c);   //0
        if (isdigit(c[0])){    //00 ou 10 ou 01 ou 11
            strcat(mt.code, c);
            fscanf(file,"%c",c);
            strcat(mt.code, c);
        }
        else {   // '_'
            strcat(mt.code, c);
        }
        fscanf(file,"%c",c);    //\n

        fscanf(file,"%c",c);   //A
        strcat(mt.code, c);
        fscanf(file,"%c",c);    //,
        fscanf(file,"%c",c);    //0
        if (isdigit(c[0])){    //00 ou 10 ou 01 ou 11
            strcat(mt.code, c);
            fscanf(file,"%c",c);
            strcat(mt.code, c);
        }
        else {    // '_'
            strcat(mt.code, c);
        }
        fscanf(file,"%c",c);    //,
        fscanf(file,"%c",c);    //<
        strcat(mt.code, c);
        strcat(mt.code, "#");
        fscanf(file,"%c",c);
    }
    free(s);
    fclose(file);
    return mt;
}

MachineTuring turingMachine(char* word, char* fileName){

    MachineTuring mt = fileToChar(fileName);
    int i;

    mt.currentState = 'A';
    mt.tape = malloc(strlen(word) + 1);
    mt.headPosition = 0;
    mt.tape[0] = '#';
    i = 1;

    while (i < strlen(word)+1){
        mt.tape[i] = word[i-1];
        i++;
    }
    mt.tape[i+1] = '\0';

    return mt;
}

MachineTuring nextStep(MachineTuring mt){

    char word = mt.tape[mt.headPosition + 1];
    char step[3];
    int codeFind = 0;
    int i = 0;

    while (i < strlen(mt.code)){
        
        if (mt.currentState == mt.code[i]){

            if (mt.code[i+1] == word){
                step[0] = mt.code[i+2];
                step[1] = mt.code[i+3];
                step[2] = mt.code[i+4];
                codeFind = 1;
                break;
            }
            else if (word == NULL && mt.code[i+1] == '_'){
                step[0] = mt.code[i+2];
                step[1] = mt.code[i+3];
                step[2] = mt.code[i+4];
                codeFind = 1;
                break;
            }
            else {
                codeFind = 0;
            }
        }
        i = i + 6; 
    }

    if (codeFind) {
        mt.currentState = step[0];

        if (mt.headPosition < -1){
            printf("\033[0;31mERROR: only one-way infinit tape accepted\033[0m\n");
            exit(EXIT_FAILURE);
        }
        mt.tape[mt.headPosition + 1] = step[1];
        switch (step[2])
        {
        case '>':
            mt.headPosition++;
            break;
        case '<':
            mt.headPosition--;
            break;
        case '-':
            break;
        } 
    }
    printStep(mt, "Transition");
    return mt;
}

MachineTuring runTuringMachine(MachineTuring mt, char* word){

    int infinitLoopValue[3] = {-10, -10, -10};
    int i = 0;
    printTuringMachine(mt, "MACHINE TURING");

    while (mt.finalState != mt.currentState){
        
        mt = nextStep(mt);
        infinitLoopValue[i % 3] = mt.headPosition;

        if (infinitLoopValue[0] == infinitLoopValue[1] && infinitLoopValue[1] == infinitLoopValue[2]){
            mt.success = 0;
            return mt;
        }
        i++;
    }
    mt.success = 1;
    return mt;
}

char* alphabetToBinary(char c){

    char* binary = malloc(sizeof(char) * 2);

    switch (c){
        case 'a':
            binary[0] = '0';
            binary[1] = '0';
            break;
        case 'b':
            binary[0] = '0';
            binary[1] = '1';
            break;
        case 'c':
            binary[0] = '1';
            binary[1] = '0';
            break;
        case 'd':
            binary[0] = '1';
            binary[1] = '1';
            break;
        case '_':
            binary[0] = '_';
            binary[1] = NULL;
            break;
        default:
            printf("\033[0;31mERROR : only {a,b,c,d} language accepted in function alphabetToBinary\033[0m\n");
            exit(EXIT_FAILURE);
    }
    binary[2] = '\0';
    return binary;
}

MachineTuring mtAlphabetToBinary(MachineTuring mt, char* fileName){

    char* binary = malloc(sizeof(char) * 2);
    int i = 0;

    FILE* file;
    file = fopen("file/alphabetToBinary.txt","w");

    if (file == NULL){ 
        printf("\033[0;31mERROR FILE DOSNT EXIST\033[0m\n"); 
    }
    
    fprintf(file, "Final=%c\n\n",mt.finalState);

    while (i < strlen(mt.code)){
        
        if (mt.code[i] == NULL){
            break;
        }
        
        fprintf(file, "%c,",mt.code[i]);
        
        binary = alphabetToBinary(mt.code[i+1]);

        if (binary[1] == NULL){
            fprintf(file, "%c\n",binary[0]);
        }
        else{
            fprintf(file, "%c%c\n",binary[0],binary[1]);
        }

        binary = alphabetToBinary(mt.code[i+3]);

        if (binary[1] == NULL){
            fprintf(file, "%c,%c,%c",mt.code[i + 2],binary[0], mt.code[i + 4]);
        }
        else{
            fprintf(file, "%c,%c%c,%c",mt.code[i + 2],binary[0],binary[1], mt.code[i + 4]);
        }
        if (i+6 < strlen(mt.code)){
            fprintf(file, "\n\n");
        }
        i = i + 6;
    }
    fclose(file);

    MachineTuring mt_new;
    char concat[2];
    mt_new = fileToCharAlphabet("file/alphabetToBinary.txt");

    i = 1;
    while (i < strlen(mt.tape)){
        binary = alphabetToBinary(mt.tape[i]);
        strcat(mt_new.tape, binary);
        i++;
    }
    return mt_new;
}

MachineTuring biTapeToRightTape(MachineTuring mt, char* fileName){

    char* duplicate = malloc(sizeof(char));
    int isExist = 0;
    int indexDuplicate = 0;
    int i = 0;
    int j = 0;

    FILE* file;
    file = fopen("file/biTapeToRightTape.txt","w");

    if (file == NULL){ 
        printf("\033[0;31mERROR FILE DOSNT EXIST\033[0m\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "FINAL=%c\n\n", mt.finalState);
    fprintf(file, "X,0\nZ,_,>\n\nX,1\nY,_,>\n\nY,1\nY,1,>\n\nY,0\nZ,1,>\n\nZ,0\nZ,0,>\n\nZ,1\nY,0,>\n\n");
    fprintf(file, "Y,_\nW,1,<\n\nZ,_\nW,0,<\n\nW,1\nW,1,<\n\nW,0\nW,0,<\n\nW,#\nA,#,>\n\n");

    while (i < strlen(mt.code)){

        if (mt.code[i] == NULL){
            break;
        }
        fprintf(file, "%c,%c\n",mt.code[i], mt.code[i + 1]);
        fprintf(file, "%c,%c,%c",mt.code[i + 2], mt.code[i + 3], mt.code[i + 4]);
        if (i+6 < strlen(mt.code)){
            fprintf(file, "\n\n");
        }

        isExist = 0;
        j = 0;
        while (j < strlen(duplicate)){
            if (duplicate[j] == mt.code[i]){
                isExist = 1;
                break;
            }
            isExist = 0;
            j++;
        }

        if (!isExist){
            indexDuplicate++;
            duplicate = malloc(indexDuplicate * sizeof(char));
            duplicate[indexDuplicate - 1] = mt.code[i];
            fprintf(file, "%c,#\nX,#,>\n\n", duplicate[indexDuplicate - 1]);
        }
        i = i + 6;
    }
    
    fclose(file);
    free(duplicate);

    MachineTuring mt_new;
    mt_new = fileToChar("file/biTapeToRightTape.txt");
    return mt_new;
}

MachineTuring turingMachineBiInfini(char* word, char* fileName){

    MachineTuring mt = fileToChar(fileName);
    MachineTuring new_mt;
    int i;

    mt.currentState = 'A';
    mt.tape = malloc(strlen(word) + 1);
    mt.headPosition = 0;
    mt.tape[0] = '#';
    i = 1;

    while (i < strlen(word)+1){
        mt.tape[i] = word[i-1];
        i++;
    }
    mt.tape[i+1] = '\0';
    printTuringMachine(mt, "Two-way infinite");
    new_mt = biTapeToRightTape(mt, fileName);
    mt.code = new_mt.code;
    return mt;
}

MachineTuring turingMachineAlphabet(char* word, char* fileName){

    MachineTuring mt = fileToChar(fileName);
    MachineTuring new_mt;
    char* binary = malloc(sizeof(char) * 2);
    int i;

    mt.currentState = 'A';
    mt.tape = malloc(strlen(word) * 2);
    mt.headPosition = 0;
    mt.tape[0] = '#';
    i = 1;

    while (i < strlen(word)+1){
        mt.tape[i] = word[i-1];
        i++;
    }
    mt.tape[i+1] = '\0';

    printTuringMachine(mt, "Alphabet {a,b,c,d}");
    new_mt = mtAlphabetToBinary(mt, fileName);
    mt.code = new_mt.code;
    mt.tape = new_mt.tape;
    return mt;
}

MachineTuring optimisation(MachineTuring mt){

    int i = 0;
    int j = 0;

    while (i < strlen(mt.code)){
        
        j = 0;
        while (j < strlen(mt.code)){

            if (i == j){
                j = j + 6;
                continue;
            }
            
            if (mt.code[i+2] == mt.code[j] && mt.code[i+3] == mt.code[j+1] && mt.code[i+4] == '-'){
                mt.code[i+2] = mt.code[j+2];
                mt.code[i+3] = mt.code[j+3];
                mt.code[i+4] = mt.code[j+4];
            }
            j = j + 6;
        }
    i = i + 6;
    }
    
    return mt;
}

MachineTuring codeMort(MachineTuring mt){

    char* new_code = malloc(sizeof(mt.code));
    int reliable = 0;
    int i = 0;
    int j = 0;

    while (i < strlen(mt.code)){
        
        while (j < strlen(mt.code)){

            if (i == j){
                j = j + 6;
                continue;
            }

            if (mt.code[i] == mt.code[j] && mt.code[i+1] == mt.code[j+1]){
                mt.code[j] = '?';
            }   
            j = j + 6;
        }
        i = i + 6;
    }

    i = 0;
    j = 0;
    
    while (i < strlen(mt.code)){

        reliable = 0;
        j = 0;
        while (j < strlen(mt.code)){

            if (i == j){
                j = j + 6;
                continue;
            }

            if (mt.code[i] == 'A' || mt.code[i] == mt.code[j+2]){
                reliable = 1;
                break;
            }
            j = j + 6;
        }
        if (!reliable){
            mt.code[i] = '?';
        }
        i = i + 6;
    }

    i = 0;
    j = 0;

    while (i < strlen(mt.code)){

        if (mt.code[i] == '?'){
            i = i + 6;
            continue;
        }
        new_code[j] = mt.code[i];
        new_code[j+1] = mt.code[i+1];
        new_code[j+2] = mt.code[i+2];
        new_code[j+3] = mt.code[i+3];
        new_code[j+4] = mt.code[i+4];
        new_code[j+5] = '#';
        i = i + 6;
        j = j + 6;
    }
    new_code[j] = '\0';
    mt.code = new_code;

    return mt;
}

int main(int argc, char* argv[]) {

    if (argc == 2){

        printf("\n\033[0;34m        Turing Machine (Question 1)\033[0m\n");
        MachineTuring mt = turingMachine("1101011", "file/binaryPalindrome.txt");
        mt = runTuringMachine(mt, "1101011");

        if (mt.success){
            printf("\n\033[0;32m------------------Word Recognized------------------\033[0m\n\n");
        }
        else {
            printf("\n\033[0;31m----------------Word Not recognized----------------\033[0m\n\n");
        }

        printf("\n\033[0;34m        Turing Machine (Question 1)\033[0m\n");
        MachineTuring mt2 = turingMachine("00001111", "file/binaryAdd1.txt");
        mt2 = runTuringMachine(mt2, "00001111");

        if (mt2.success){
            printf("\n\033[0;32m------------------Word Recognized------------------\033[0m\n\n");
        }
        else {
            printf("\n\033[0;31m----------------Word Not recognized----------------\033[0m\n\n");
        }

        printf("\n\033[0;34m        Turing Machine (Question 1)\033[0m\n");
        MachineTuring mt3 = turingMachine("01000010", "file/wwR.txt");
        mt3 = runTuringMachine(mt3, "00001111");

        if (mt3.success){
            printf("\n\033[0;32m------------------Word Recognized------------------\033[0m\n\n");
        }
        else {
            printf("\n\033[0;31m----------------Word Not recognized----------------\033[0m\n\n");
        }

        printf("\n\033[0;34m Two-way infinite to One-way infinite (Question 6)\033[0m\n");
        MachineTuring mt4 = turingMachineBiInfini("100101", "file/exampleExo6.txt");
        printTuringMachine(mt4, "One-way infinite");
        runTuringMachine(mt4, "100101");

        printf("\n\033[0;34m {a,b,c,d} Alphabet to Binary (Question 7)\033[0m\n");
        MachineTuring mt5 = turingMachineAlphabet("abcdab", "file/exampleExo7.txt");
        printTuringMachine(mt5, "Alphabet Binary");

        printf("\n\033[0;34m          Optimisation (Question 9)\033[0m\n");
        MachineTuring mt7 = turingMachine("01010", "file/optimisation.txt");
        printTuringMachine(mt7, "Before Optimisation");
        optimisation(mt7);
        printTuringMachine(mt7, "After Optimisation");

        printf("\n\033[0;34m          Code Mort (Question 10)\033[0m\n");
        MachineTuring mt8 = turingMachine("01010", "file/optimisation.txt");
        printTuringMachine(mt8, "Before Code Mort");
        mt8 = codeMort(mt8);
        printTuringMachine(mt8, "After Code Mort");
        
    }
    else {
        printf("\n\033[0;34m        Turing Machine (Question 1)\033[0m\n");
        MachineTuring mt6 = turingMachine("1101011", "file/binaryPalindrome.txt");
        mt6 = runTuringMachine(mt6, "1101011");

        if (mt6.success){
            printf("\n\033[0;32m------------------Word Recognized------------------\033[0m\n\n");
        }
        else {
            printf("\n\033[0;31m----------------Word Not recognized----------------\033[0m\n\n");
        }
    }
    
}
