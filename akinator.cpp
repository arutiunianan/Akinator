#include "akinator.h"

int GetFileSize(FILE* text, int start)
{
    assert( text != NULL );
    fseek( text, 0, SEEK_END );
    int fileSize = ftell( text ) - start;
    fseek( text, 0, start );
    return fileSize;
}

void GetString( char* buffer, char** str, int fileSize, int* i, int start)
{
    if ( start < 0 )
        return; //ошибка
    int end = start;

    if( buffer[*i] == '<' )
    {
        while( buffer[*i] != '>' )
        {
            end = *i;
            (*i)++;
        
            if( *i >= fileSize )
                return; //ошибка
        }
        (*i)++;
        *str = ( char* )calloc( end - start + 1, sizeof( char ) );
        strncpy( *str, buffer + start + 1, end - start  );
    }
    
}

void InviteStrNode( Tree* tree, Node* node, int fileSize, char* buffer, int* i, int* bracket_counter, WhichChild side )
{
    int is_root = 0;
    while( buffer[*i] == ' ' ) (*i)++;
    if( buffer[*i] == '(' )
    {
        (*bracket_counter)++;
        (*i)++;
        is_root++;
    }

    while( buffer[*i] == ' ' ) (*i)++;
    char* str = "";
    GetString( buffer, &str, fileSize, i, *i );
    //printf("%s %d %d\n",str,*i,side);

    if( strcmp( str, "nil" ) )
        InviteNode( tree, node, side, str );
    if(side == Left && is_root )
      ReadingNodes( tree, node->left, fileSize, buffer, i, bracket_counter );
    else if(side == Right && is_root)
      ReadingNodes( tree, node->right, fileSize, buffer, i, bracket_counter );

}

void ReadingNodes( Tree* tree, Node* node, int fileSize, char* buffer, int* i, int* bracket_counter )
{

    InviteStrNode( tree, node, fileSize, buffer,i, bracket_counter, Left );
        
    InviteStrNode( tree, node, fileSize, buffer,i, bracket_counter, Right );

    while( buffer[*i] == ' ' ) (*i)++;
    if( buffer[*i] == ')' )
        (*bracket_counter)--;
    else 
        return; //ошибка
    (*i)++;
}

void ReadFile( Tree* tree , const char* prefile)
{
    FILE* file = fopen( prefile, "r" );
    int fileSize = GetFileSize( file, SEEK_SET );
    char* buffer = ( char* )calloc( fileSize + 1, sizeof( char ) );
    fread( buffer, sizeof( char ), fileSize, file );

    int bracket_counter = 0;
    int i = 0;
    while( buffer[i] != '(' )
    {
        if( buffer[i] != ' ' )
            return; //ошибка типо
        i++;
    }
    bracket_counter++;;
    i++;
    while( buffer[i] == ' ' ) i++;

    char* str = "";
    GetString( buffer, &str, fileSize, &i, i );
    InviteNode( tree, tree->root, Right, str );

    ReadingNodes( tree, tree->root ,fileSize, buffer, &i, &bracket_counter );
    
}

void WriteFile( Node* node, FILE* file )
{
    
    fprintf( file, "( <%s> ", node->value );

    if( node->left )
        WriteFile( node->left, file );
    else
        fprintf( file, "<nil> " );
    if( node->right )
        WriteFile( node->right, file );
    else
        fprintf( file, "<nil> " );

    fprintf( file, ") " );

}

void RewriteTree( Tree* tree, Node* node, char* right_answer, char* question )
{
    InviteNode(tree,node,Left,right_answer);
    InviteNode(tree,node,Right,node->value);
    node->value = question;
    FILE* file = fopen( "test1.txt", "w" );
    WriteFile( tree->root, file );
}

void GuessMode( Tree* tree )
{
    Node* node = tree->root;
    char* answers = ( char* )calloc( 5, sizeof( char ) );
    while( node->left && node->right )
    {
        printf("%s (yes/no/exit)\n",node->value);
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
        printf("Это:  %s ?  (yes/no/exit) \n",node->value);
        scanf( "%s", answer );
        if( !strcmp( answer, "yes" ) )
            printf("Юхуууу я угадала!!\n");
        else if( !strcmp( answer, "no" ) )
        {
            printf("Жалко. А что было загадано?\n");
            char* right_answer = ( char* )calloc( 100, sizeof( char ) );
            scanf( "%s", right_answer );

            printf("Чем «%s» отличается от «%s» ?\n",right_answer,node->value);
            char* question = ( char* )calloc( 500, sizeof( char ) );
            char* part_of_question = ( char* )calloc( 50, sizeof( char ) );
            scanf("%s%[^\n]",question, part_of_question);
            strcat(question, part_of_question);
            question = ( char* )realloc( question, strlen( question ) );
            free( part_of_question );

            answer = ( char* )calloc( 4, sizeof( char ) );
            while( strcmp( answer, "yes" ) && strcmp( answer, "no" ) )
            {
                printf("Супер, я запомнила. Хочешь записать новые данные?  (yes/no)\n");
                scanf( "%s", answer );
                if( !strcmp( answer, "yes" ) )
                    RewriteTree( tree, node, right_answer, question );
                else if( !strcmp( answer, "no" ) )
                    printf("Хорошо. Надеюсь тебе понравилась игра<3\n");
                else
                    printf("Я тебя не понимаю. Попробуй снова:(\n");
            }

        }
        else if( !strcmp( answer, "exit" ) )
            return;
        else
            printf("Я тебя не понимаю. Попробуй снова:(\n");
    }

}

void GetWayToNode( Node* node, Stack* stack, Stack* stack_of_answers, char* value, int* is_find )
{
    assert( stack );

    if( !strcmp( node->value, value ) )
        *is_find = 1;
    else
    {
        StackPush( stack, node->value );
        if( node->left  )
        {
            if( stack_of_answers ) StackPush( stack_of_answers, "yes" );
            
            GetWayToNode( node->left, stack, stack_of_answers, value, is_find );
            if( !(*is_find) )
            {
                StackPop( stack );
                if( stack_of_answers ) StackPop( stack_of_answers );
            }
        }
        if( node->right && !(*is_find) )
        {
            if( stack_of_answers ) StackPush( stack_of_answers, "no" );
            
            GetWayToNode( node->right, stack, stack_of_answers, value, is_find );
            if( !(*is_find) )
            {
                StackPop( stack );
                if( stack_of_answers ) StackPop( stack_of_answers );
            }
        }
    }
}

void BalanceStack( Stack* bigger_stack, Stack* smaller_stack, Stack* stack_of_answers )
{
    do
    {
        StackPop( bigger_stack );
        if( stack_of_answers ) StackPop( stack_of_answers );
    } while ( bigger_stack->size > smaller_stack->size );
    
}

char* StrWithOutQuestionMarkAndFace( char* str )
{
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
    difference = StrWithOutQuestionMarkAndFace( difference );
    printf( "\n%s не %s, а %s %s\n", value1, difference, value2, difference);
}

void CompareMode( Tree* tree )
{
    
    Stack stack1 = {};
    Stack stack2 = {};
    Stack stack_of_answers = {};
    int is_find = 0;
    StackCtor( &stack1 );
    StackCtor( &stack2 );
    StackCtor( &stack_of_answers );

    char* value1 = ( char* )calloc( 500, sizeof( char ) );
    char* value2 = ( char* )calloc( 500, sizeof( char ) );

    printf("Введи первого человека:\n");
    scanf( "%s", value1 );
    printf("Введи второго человека:\n");
    scanf( "%s", value2 );

    if( !strcmp( value1,value2 ) )
        return;//все с кайфом
    
    GetWayToNode( tree->root, &stack1, &stack_of_answers, value1, &is_find );
    if( !is_find ) 
        return;//ошибка

    is_find = 0;

    GetWayToNode( tree->root, &stack2, NULL, value2, &is_find );
    if( !is_find ) 
        return;//ошибка

    
    if( stack1.size > stack2.size )
        BalanceStack( &stack1, &stack2, &stack_of_answers );
    else if( stack1.size < stack2.size )
        BalanceStack( &stack2, &stack1, NULL );
    
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
        return;//ошибка
    //StackDtor();
    
}

void InfoMode( Tree* tree )
{
    Stack stack = {};
    Stack stack_of_answers = {};
    int is_find = 0;
    StackCtor( &stack );
    StackCtor( &stack_of_answers );

    char* value = ( char* )calloc( 500, sizeof( char ) );
    printf("Введи, чью информацию хочешь узнать:\n");
    scanf( "%s", value );

    GetWayToNode( tree->root, &stack, &stack_of_answers, value, &is_find );
    if( !is_find ) 
        return;//ошибка

    char* question = ( char* )calloc( 500, sizeof( char ) );
    char* answer = ( char* )calloc( 500, sizeof( char ) );

    while( stack.size )
    {
        printf("%s ",value);
        StackPop(&stack,&question);
        StackPop(&stack_of_answers,&answer);
        if(!strcmp( answer, "no" ))
            printf("не ");
        else if( strcmp( answer, "yes" ) )
            return;//ошибка
        printf("%s\n",StrWithOutQuestionMarkAndFace(question));
    }

}

void Game()
{
    Tree tree = {};
    TreeCtor( &tree );

    ReadFile( &tree, "test.txt");
    
    printf("Привееет, добро пожаловать в мою игру акинатор<3\n");
    
    char* mode = ( char* )calloc( 9, sizeof( char ) );
    char* play_again = ( char* )calloc( 5, sizeof( char ) );
    do
    {
        printf("Выбери режим, в который ты хочешь поиграть:   (Guess/Compare/Info)\n");
        scanf( "%s", mode );
        if( !strcmp( mode, "Guess" ) )
            GuessMode( &tree );
        else if( !strcmp( mode, "Compare" ) )
            CompareMode( &tree );
        else if( !strcmp( mode, "Info" ) )
            InfoMode( &tree );
        else
            return;//ошибка
        printf("\nХочешь поиграть еще?  (yes/no)\n");
        scanf( "%s", play_again );
        if( !strcmp( play_again, "no" ) )
            printf("Хорошо, спасибо, что поиграл/а. Люблю целую:)\n");
        else if( strcmp( play_again, "yes" ) )
            return;//ошибка
    } while ( !strcmp( play_again, "yes" ) );
    
    TreeDump( &tree, tree.log );

    //FILE* file = fopen( "test1.txt", "w" );
    //WriteFile( tree.root, file );

}