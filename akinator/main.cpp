#include "akinator.h"

int main(int argc, const char* argv[])
{
    if(argc == 1)
    {
        Game("akinator/test_input.txt", "akinator/test_output.txt");
    }
    else if(argc == 3)
    {
        Game(argv[1], argv[2]);
    }
    else
    {
        printf("Invalid number of args to program");
    }
}