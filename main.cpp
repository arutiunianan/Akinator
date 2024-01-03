#include "akinator.h"

int main( int argc, const char* argv[] )
{
    if ( argc == 1 )
        Game( "test_input.txt", "test_output.txt" );
    else if ( argc == 3 )
        Game( argv[1], argv[2] );
    else
        printf( "Invalid number of args to program" );
}