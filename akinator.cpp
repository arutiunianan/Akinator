#include "akinator.h"

int GetFileSize( FILE* text, int start )
{
    assert( text != NULL );
    fseek( text, 0, SEEK_END );
    int fileSize = ftell( text ) - start;
    fseek( text, 0, start );
    return fileSize;
}

void GetString( Akin* akin, char** str, int* i, int start )
{
    assert( akin );

    if ( start < 0 )
    {
        akin->errors |= POINTER_OF_BUF_IS_NEGATIVE;
        return; 
    }
    int end = start;

    if( akin->buffer[*i] == '<' )
    {
        while( akin->buffer[*i] != '>' )
        {
            end = *i;
            (*i)++;
        
            if( *i >= akin->file_size )
            {
                akin->errors |= INCORRECT_FILE_DATA;
                return; 
            }
        }
        (*i)++;
        *str = ( char* )calloc( end - start + 1, sizeof( char ) );
        strncpy( *str, akin->buffer + start + 1, end - start  );
    }
}

void InviteStrNode( Akin* akin, Node* node, int* i, int* bracket_counter, WhichChild side )
{
    assert( akin );

    if( !node )
    {
        akin->tree->errors |= NODE_PTR_IS_NULL;
        return; 
    }
    int is_root = 0;
    while( akin->buffer[*i] == ' ' ) (*i)++;
    if( akin->buffer[*i] == '(' )
    {
        (*bracket_counter)++;
        (*i)++;
        is_root++;
    }

    while( akin->buffer[*i] == ' ' ) (*i)++;
    char* str = "";
    GetString( akin, &str, i, *i );

    if( strcmp( str, "nil" ) )
        InviteNode( akin->tree, node, side, str );
    if(side == Left && is_root )
      ReadingNodes( akin, node->left, i, bracket_counter );
    else if( side == Right && is_root )
      ReadingNodes( akin, node->right, i, bracket_counter );
}

void ReadingNodes( Akin* akin, Node* node, int* i, int* bracket_counter )
{
    assert( akin );

    if( !node )
    {
        akin->tree->errors |= NODE_PTR_IS_NULL;
        return; 
    }

    InviteStrNode( akin, node, i, bracket_counter, Left );
    InviteStrNode( akin, node, i, bracket_counter, Right );

    while( akin->buffer[*i] == ' ' ) (*i)++;
    if( akin->buffer[*i] == ')' )
        (*bracket_counter)--;
    else
    {
        akin->errors |= INCORRECT_FILE_DATA;
        return;
    }
    (*i)++;
}

void ReadFile( Akin* akin )
{
    assert( akin );

    akin->file_size = GetFileSize( akin->input, SEEK_SET );
    if( akin->file_size < 1 )
    {
        akin->errors |= FILESIZE_IS_ZERO;
        return;
    } 
    akin->buffer = ( char* )calloc( akin->file_size + 1, sizeof( char ) );
    fread( akin->buffer, sizeof( char ), akin->file_size, akin->input );

    int bracket_counter = 0;
    int i = 0;
    while( akin->buffer[i] != '(' )
    {
        if( akin->buffer[i] != ' ' )
        {
            akin->errors |= INCORRECT_FILE_DATA;
            return;
        }  
        i++;
    }
    bracket_counter++;;
    i++;
    while( akin->buffer[i] == ' ' ) i++;

    char* str = "";
    GetString( akin, &str, &i, i );
    InviteNode( akin->tree, akin->tree->root, Right, str );

    ReadingNodes( akin, akin->tree->root, &i, &bracket_counter );
}

void WriteFile( Akin* akin, Node* node, FILE* file )
{
    assert( akin );

    if( !node )
    {
        akin->tree->errors |= NODE_PTR_IS_NULL;
        return;
    } 

    fprintf( file, "( <%s> ", node->value );

    if( node->left )
        WriteFile( akin, node->left, file );
    else
        fprintf( file, "<nil> " );
    if( node->right )
        WriteFile( akin, node->right, file );
    else
        fprintf( file, "<nil> " );

    fprintf( file, ") " );
}

void RewriteTree( Akin* akin, Node* node, char* right_answer, char* question )
{
    assert( akin );

    if( !node ) 
    {
        akin->tree->errors |= NODE_PTR_IS_NULL;
        return;
    }

    InviteNode( akin->tree, node, Left, right_answer );
    InviteNode( akin->tree, node, Right, node->value );
    node->value = question;

    if( !akin->output )
    {
        akin->errors |= CANT_OPEN_FILE;
        return;
    }
    WriteFile( akin, akin->tree->root, akin->output );
}

void GuessMode( Akin* akin )
{
    assert( akin );

    AkinatorDump( akin, akin->log, "guess" );
    Node* node = akin->tree->root;
    if( !node ) 
    {
        akin->tree->errors |= TREE_ROOT_IS_NULL;
        return;
    }
    char* answers = ( char* )calloc( 5, sizeof( char ) );
    while( node->left && node->right )
    {
        printf( "%s (yes/no/exit)\n", node->value );
        scanf( "%s", answers );
        if( !strcmp( answers, "yes" ) )
            node = node->left;
        else if( !strcmp( answers, "no" ) )
            node = node->right;
        else if( !strcmp( answers, "exit" ) )
            return;
        else
            printf("Я тебя не понимаю. Попробуй снова:(\n");
    }

    char* answer = ( char* )calloc( 5, sizeof( char ) );
    while( strcmp( answer, "yes" ) && strcmp( answer, "no" ) && strcmp( answer, "exit" ) )
    {
        printf( "Это:  %s ?  (yes/no/exit) \n",node->value );
        scanf( "%s", answer );
        if( !strcmp( answer, "yes" ) )
            printf( "Юхуууу я угадала!!\n" );
        else if( !strcmp( answer, "no" ) )
        {
            printf( "Жалко. А что было загадано?\n" );
            char* right_answer = ( char* )calloc( 100, sizeof( char ) );
            scanf( "%s", right_answer );

            printf( "Чем «%s» отличается от «%s» ?\n", right_answer,node->value );
            char* question = ( char* )calloc( 500, sizeof( char ) );
            char* part_of_question = ( char* )calloc( 50, sizeof( char ) );
            scanf( "%s%[^\n]", question, part_of_question );
            strcat( question, part_of_question );
            question = ( char* )realloc( question, strlen( question ) );
            free( part_of_question );

            answer = ( char* )calloc( 4, sizeof( char ) );
            while( strcmp( answer, "yes" ) && strcmp( answer, "no" ) )
            {
                printf( "Супер, я запомнила. Хочешь записать новые данные?  (yes/no)\n" );
                scanf( "%s", answer );
                if( !strcmp( answer, "yes" ) )
                    RewriteTree( akin, node, right_answer, question );
                else if( !strcmp( answer, "no" ) )
                    printf( "Хорошо. Надеюсь тебе понравилась игра<3\n" );
                else
                    printf( "Я тебя не понимаю. Попробуй снова:(\n" );
            }
        }
        else if( !strcmp( answer, "exit" ) )
            return;
        else
            printf( "Я тебя не понимаю. Попробуй снова:(\n" );
    }
    AkinatorDump( akin, akin->log, "guess" );
}

void GetWayToNode( Akin* akin, Node* node, Stack* stack, Stack* stack_of_answers, char* value, int* is_find )
{
    assert( akin );

    if( !stack ) 
    {
        akin->errors |= STACK_PTR_IS_NULL;
        return;
    }
    if( !node ) 
    {
        akin->tree->errors |= NODE_PTR_IS_NULL;
        return;
    }

    if( !strcmp( node->value, value ) )
        *is_find = 1;
    else
    {
        StackPush( stack, node->value );
        if( node->left  )
        {
            if( stack_of_answers ) StackPush( stack_of_answers, "yes" );
            
            GetWayToNode( akin, node->left, stack, stack_of_answers, value, is_find );
            if( !(*is_find) )
            {
                StackPop( stack );
                if( stack_of_answers ) StackPop( stack_of_answers );
            }
        }
        if( node->right && !(*is_find) )
        {
            if( stack_of_answers ) StackPush( stack_of_answers, "no" );
            
            GetWayToNode( akin, node->right, stack, stack_of_answers, value, is_find );
            if( !(*is_find) )
            {
                StackPop( stack );
                if( stack_of_answers ) StackPop( stack_of_answers );
            }
        }
    }
}

void BalanceStack( Akin* akin, Stack* bigger_stack, Stack* smaller_stack, Stack* stack_of_answers )
{
    assert( akin );

    if( !bigger_stack ) 
    {
        akin->errors |= STACK_PTR_IS_NULL;
        return;
    }
    if( !smaller_stack ) 
    {
        akin->errors |= STACK_PTR_IS_NULL;
        return;
    }
    do
    {
        StackPop( bigger_stack );
        if( stack_of_answers ) StackPop( stack_of_answers );
    } while ( bigger_stack->size > smaller_stack->size );
}

char* StrWithOutQuestionMarkAndFace( char* str )
{
    assert( str );
    for( int i = 0; i < strlen( str ); i++ )
        if ( str[i] == ' ' ) 
        {
            str = &str[i] + 1;
            break;
        }
    char* newstr = ( char* )calloc( 500, sizeof( char ) );
    for( int i = 0; i < strlen( str ); i++ )
        if( str[i] == '?' )
        {
            strncpy( newstr, str, i );
        }
    return newstr;
}

void WriteDifferences( char* value1, char* value2, char* difference )
{
    assert( value1 && value2 && difference );
    difference = StrWithOutQuestionMarkAndFace( difference );
    printf( "\n%s не %s, а %s %s\n", value1, difference, value2, difference);
}

void CompareMode( Akin* akin )
{
    assert( akin );

    AkinatorDump( akin, akin->log, "compare" );
    Stack stack1 = {};
    Stack stack2 = {};
    Stack stack_of_answers = {};
    int is_find = 0;
    StackCtor( &stack1 );
    StackCtor( &stack2 );
    StackCtor( &stack_of_answers );

    char* value1 = ( char* )calloc( 500, sizeof( char ) );
    char* value2 = ( char* )calloc( 500, sizeof( char ) );

    printf( "Введи первого человека:\n" );
    scanf( "%s", value1 );
    printf( "Введи второго человека:\n" );
    scanf( "%s", value2 );

    if( !strcmp( value1,value2 ) )
        return;//все с кайфом
    
    GetWayToNode( akin, akin->tree->root, &stack1, &stack_of_answers, value1, &is_find );
    if( !is_find ) 
    {
        akin->errors |= INCORRECT_INPUT_ERROR;
        return;
    }

    is_find = 0;

    GetWayToNode( akin, akin->tree->root, &stack2, NULL, value2, &is_find );
    if( !is_find ) 
    {
        akin->errors |= INCORRECT_INPUT_ERROR;
        return;
    }
    
    if( stack1.size > stack2.size )
        BalanceStack( akin, &stack1, &stack2, &stack_of_answers );
    else if( stack1.size < stack2.size )
        BalanceStack( akin, &stack2, &stack1, NULL );
    
    char* pop1 = ( char* )calloc( 500, sizeof( char ) );
    char* pop2 = ( char* )calloc( 500, sizeof( char ) );
    char* answer = ( char* )calloc( 500, sizeof( char ) );
    do
    {
        StackPop( &stack1, &pop1 );
        StackPop( &stack2, &pop2 );
        StackPop( &stack_of_answers, &answer );
    }while( strcmp( pop1, pop2 ) );

    if( !strcmp( answer, "yes" ) )
        WriteDifferences( value2, value1, pop1 );
    else if( !strcmp( answer, "no" ) )
        WriteDifferences( value1, value2, pop1 );
    else
    {
        akin->errors |= INCORRECT_INPUT_ERROR;
        return;
    }
    StackDtor( &stack1 );
    StackDtor( &stack2 );
    StackDtor( &stack_of_answers );
    AkinatorDump( akin, akin->log, "compare" );
}

void InfoMode( Akin* akin )
{
    assert( akin );

    AkinatorDump( akin, akin->log, "info" );
    Stack stack = {};
    Stack stack_of_answers = {};
    int is_find = 0;
    StackCtor( &stack );
    StackCtor( &stack_of_answers );

    char* value = ( char* )calloc( 500, sizeof( char ) );
    printf("Введи, чью информацию хочешь узнать:\n");
    scanf( "%s", value );

    GetWayToNode( akin, akin->tree->root, &stack, &stack_of_answers, value, &is_find );
    if( !is_find ) 
    {
        akin->errors |= INCORRECT_INPUT_ERROR;
        return;
    }

    char* question = ( char* )calloc( 500, sizeof( char ) );
    char* answer = ( char* )calloc( 500, sizeof( char ) );

    while( stack.size )
    {
        printf( "%s ",value );
        StackPop( &stack,&question );
        StackPop( &stack_of_answers, &answer );
        if(!strcmp( answer, "no" ))
            printf("не ");
        else if( strcmp( answer, "yes" ) )
        {
            akin->errors |= INCORRECT_INPUT_ERROR;
            return;
        }
        printf( "%s\n",StrWithOutQuestionMarkAndFace( question ) );
    }
    StackDtor( &stack );
    StackDtor( &stack_of_answers );
    AkinatorDump( akin, akin->log, "info" );
}

void AkiCtor( Akin* akin, const char* input, const char* output )
{
    assert( akin );

    akin->tree = ( Tree* )calloc( 1, sizeof( Tree ) );;
    TreeCtor( akin->tree );

    akin->log =  fopen( "akinlog.txt", "wb" );
    akin->output = fopen( output, "wb" );
    akin->input = fopen( input, "r" );
    akin->file_size = 0;
    akin->buffer = "";

    if( !akin->input && !akin->output )
        akin->errors = CANT_OPEN_FILE;
    else 
        akin->errors = GAME_IS_OK;
}

void AkiDtor( Akin* akin )
{
    assert( akin );

    TreeDtor( akin->tree );
    fclose( akin->log );
    fclose( akin->input );
    akin->file_size = -1;
    free( akin->buffer );
    akin->buffer = NULL;
    fclose( akin->output );
    akin->errors = -1;
}

void Game( const char* input, const char* output )
{

    Akin akin ={};
    AkiCtor( &akin, input, output );

    ReadFile( &akin );
    
    printf("Привееет, добро пожаловать в мою игру акинатор<3\n");
    
    char* mode = ( char* )calloc( 9, sizeof( char ) );
    char* play_again = ( char* )calloc( 5, sizeof( char ) );
    do
    {
        AkinatorDump( &akin, akin.log, "choose" );
        printf("Выбери режим, в который ты хочешь поиграть:   (Guess/Compare/Info)\n");
        scanf( "%s", mode );
        if( !strcmp( mode, "Guess" ) )
            GuessMode( &akin );
        else if( !strcmp( mode, "Compare" ) )
            CompareMode( &akin );
        else if( !strcmp( mode, "Info" ) )
            InfoMode( &akin );
        else
        {
            akin.errors |= INCORRECT_INPUT_ERROR;
            break;
        }
        printf( "\nХочешь поиграть еще?  (yes/no)\n" );
        scanf( "%s", play_again );
        AkinatorDump( &akin, akin.log, "choose" );
        if( !strcmp( play_again, "no" ) )
            printf( "Хорошо, спасибо, что поиграл/а. Люблю целую:)\n" );
        else if( strcmp( play_again, "yes" ) )
        {
            akin.errors |= INCORRECT_INPUT_ERROR;
            break;
        }
    } while ( !strcmp( play_again, "yes" ) );

    AkiDtor( &akin );
}

int AkinVerify( Akin* akin )
{
    if( akin == NULL )
        return AKIN_IS_NULL;
    if( !akin->input && !akin->output )
        return CANT_OPEN_FILE;
    return GAME_IS_OK;
}

int AkinatorDump( Akin* akin, FILE* logger, char* mode )
{
    assert( akin );

    int status = AkinVerify( akin );

    static size_t number_of_call = 1;

	fprintf( logger, "=======================================\nAKINATOR DUMP CALL #%zu\nMode: %s\n", number_of_call, mode );
    if ( status || akin->errors )
	{
		fprintf( logger, "-------------ERRORS------------\n" );
		if ( status & AKIN_IS_NULL )
		{
			fprintf( logger, "GAME POINTER IS NULL\n" );
			return 0;
		}
        TreeDump( akin->tree, akin->tree->log );
        if ( akin->errors & INCORRECT_INPUT_ERROR ) fprintf( logger, "USER INPUT INCORRECT DATA\n" );
		if ( akin->errors & INCORRECT_FILE_DATA ) fprintf( logger, "DATA IN FILE IS INCORRECT\n" );
		if ( akin->errors & FILESIZE_IS_ZERO ) fprintf( logger, "SIZE OF FILE IS ZERO OR NEGATIVE\n" );
	    if ( akin->errors & CANT_OPEN_FILE ) fprintf( logger, "FILE CANT BE OPENED\n");
	    if ( akin->errors & POINTER_OF_BUF_IS_NEGATIVE ) fprintf( logger, "POINTER OF BUF IS NEGATIVE \n" );
        if ( akin->errors & STACK_PTR_IS_NULL ) fprintf( logger, "STACK IS NULL \n" );

		fprintf( logger, "----------END_OF_ERRORS--------\n" );
	}
	else
    {
		fprintf( logger, "------------NO_ERRORS----------\n" );
        fprintf( logger, "Current Pre-tree\n" );

        WriteFile( akin, akin->tree->root, logger );
        fprintf( logger, "\n" );

        TreeDump( akin->tree, akin->tree->log );
    }
	fprintf( logger, "=======================================\n\n" );
	number_of_call++;

    return status;
}
