#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "tree.h"
#include "stack/stack.h"
const char nil[3] = { 'n', 'i', 'l' };

typedef enum AkinErrors
{
    GAME_IS_OK = 0,
    AKIN_IS_NULL = 1 << 0,
    INCORRECT_INPUT_ERROR = 1 << 1,
    INCORRECT_FILE_DATA = 1 << 2,
    FILESIZE_IS_ZERO = 1 << 3,
    CANT_OPEN_FILE = 1 << 4,
    POINTER_OF_BUF_IS_NEGATIVE = 1 << 5,
    STACK_PTR_IS_NULL = 1 << 6,
}AkinErrors;

typedef struct Akin
{
    Tree* tree;
    FILE* log;

    FILE* input;
    int file_size;
    char* buffer;

    FILE* output;
    int errors;
}Akin;

void ReadingNodes( Akin* akin, Node* node, int* i, int* bracket_counter );
void ReadFile( Akin* akin );
void WriteFile( Akin* akin, Node* node, FILE* file );
void GuessMode( Akin* akin );
void CompareMode( Akin* akin );
void InfoMode( Akin* akin );
void AkiCtor( Akin* akin, const char* input, const char* output );
void AkiDtor( Akin* akin );
void Game( const char* input, const char* output );
int AkinVerify( Akin* akin );
int AkinatorDump( Akin* akin, FILE* logger, char* mode );

#endif // #define AKINATOR_H_