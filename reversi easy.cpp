#include<graphics.h>
#include<stdio.h>
#include<iostream>

using namespace std;

#define black 1
#define white (-1)
#define PLAYABLE 2
#define EMPTY 0
#define TRUE 1
#define FALSE 0

struct Board
{
    int up,down,right,left,ul,ur,dl,dr, value;
};
struct Board board[8][8];

int  player;
int white_score =2, black_score = 2;
int game_ended = FALSE;
int skipped_turn = FALSE;
int wrong_move = FALSE;
int has_valid_move = FALSE;

void drawboard()
{
    int left, top, right, bottom, x, y;
    char str[1];

    setfillstyle(SOLID_FILL,RED); //coloring outside the BLUE
    rectangle(50,50,100,50);
    floodfill(25,75,WHITE);

    left = 50;
    top = 50;
    right = 100;
    bottom = 100;
    for(int i=0; i<8; i++)              //drawing the empty board 8*8
    {
        left = 50;
        right = 100;
        for(int j=0; j<8; j++)
        {
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,GREEN);
            rectangle(left,top,right,bottom);
            floodfill((left+25),(top+25),WHITE);
            left += 50;
            right += 50;
        }
        top+=50;
        bottom+=50;
    }

    for(int i =0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            cout << board[i][j].value << "  ";
        }
        cout << endl;
    }
    cout << endl<< endl;
    white_score = 0;
    black_score = 0;
    for(int i =0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            if(board[i][j].value==black) black_score++;
            if(board[i][j].value==white)  white_score++;
            if(board[i][j].value==black || board[i][j].value== white || board[i][j].value == PLAYABLE)
            {
                x=i+1;
                y=j+1;
                x = (x*50) + 25;
                y = (y*50) + 25;
                if(board[i][j].value==black)
                {
                    setcolor(LIGHTGRAY);
                    setfillstyle(SOLID_FILL, BLACK);

                    circle(y, x , 17);
                    floodfill(y,x,LIGHTGRAY);
                }
                else if(board[i][j].value==white)
                {
                    setcolor(LIGHTGRAY);
                    setfillstyle(SOLID_FILL,WHITE);
                    circle(y, x , 17);
                    floodfill(y,x,LIGHTGRAY);
                }
                else if (board[i][j].value==PLAYABLE)
                {
                    setcolor(WHITE);
                    setfillstyle(SOLID_FILL, GREEN);
                    outtextxy(y-5, x-10, "+");
                    floodfill(y-5,x-10,WHITE);
                }
            }

        }
    }

    //showing scores
    outtextxy(475, 175, "WHITE: ");
    outtextxy(475, 200, "BLACK: ");
    sprintf(str, "%d" , white_score);
    outtextxy(550, 175 , str );
    sprintf(str, "%d" , black_score);
    outtextxy(550, 200 , str );

    //showing player turn
    outtextxy(475, 275, "PLAYER : ");
    if(player==black)
    {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, BLACK);
        circle(580, 283 , 17);
        floodfill(580, 283,DARKGRAY);
    }
    else
    {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, WHITE);
        circle(580, 283 , 17);
        floodfill(580, 283,DARKGRAY);
    }

}
void initial_board()
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
            board[i][j].value = 0;
    }
    board[4][3].value =black;
    board[3][4].value = black;
    board[4][4].value = white;
    board[3][3].value = white;
    player = black;
}

int is_valid_position( int i, int j )
{
    if ( i < 0 || i >= 8 || j < 0 || j >= 8 ) return FALSE;
    return TRUE;
}

int distance( int i1, int j1, int i2, int j2 )
{
    int di = abs( i1 - i2 ), dj = abs( j1 - j2 );
    if ( di > 0 ) return di;
    return dj;
}

int is_playable( int i, int j )
{
    board[i][j].up = board[i][j].down = board[i][j].right = board[i][j].left =board[i][j].ul =board[i][j].ur =board[i][j].dl =board[i][j].dr= 0;
    if ( !is_valid_position( i, j ) ) return FALSE;
    if ( board[i][j].value != EMPTY ) return FALSE;
    int playable = FALSE;

    int opposing_player = player*(-1);

    // Test UL diagonal
    int i_it = i-1, j_it = j-1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
    {
        i_it -= 1;
        j_it -= 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].ul = 1;
        playable = TRUE;
    }

    // Test UP path
    i_it = i-1, j_it = j;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
        i_it -= 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].up = 1;
        playable = TRUE;
    }

    // Test UR diagonal
    i_it = i-1, j_it = j+1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
    {
        i_it -= 1;
        j_it += 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].ur = 1;
        playable = TRUE;
    }

    // Test LEFT path
    i_it = i, j_it = j-1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
        j_it -= 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].left = 1;
        playable = TRUE;
    }

    // Test RIGHT path
    i_it = i, j_it = j+1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
        j_it += 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].right = 1;
        playable = TRUE;
    }

    // Test DL diagonal
    i_it = i+1, j_it = j-1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
    {
        i_it += 1;
        j_it -= 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].dl = 1;
        playable = TRUE;
    }

    // Test DOWN path
    i_it = i+1, j_it = j;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
        i_it += 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].down = 1;
        playable = TRUE;
    }

    // Test DR diagonal
    i_it = i+1, j_it = j+1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it].value == opposing_player )
    {
        i_it += 1;
        j_it += 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it].value == player )
    {
        board[i][j].dr = 1;
        playable = TRUE;
    }
    return playable;
}


void playable_options()
{
    has_valid_move = FALSE;
    for ( int i=0; i<8; ++i )
    {
        for ( int j=0; j<8; ++j )
        {
            if ( board[i][j].value == PLAYABLE )
                board[i][j].value = EMPTY;
            if ( is_playable( i, j ) )
            {
                board[i][j].value = PLAYABLE;
                has_valid_move = TRUE;
            }
        }
    }
}

void capture_pieces( int i, int j )
{
    int opposing_player = player*(-1);
    int i_it, j_it;

    // Capture UL diagonal
    if ( board[i][j].ul )
    {
        i_it = i-1, j_it = j-1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            i_it -= 1;
            j_it -= 1;
        }
    }

    // Capture UP path
    if ( board[i][j].up )
    {
        i_it = i-1, j_it = j;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            i_it -= 1;
        }
    }

    // Capture UR diagonal
    if ( board[i][j].ur )
    {
        i_it = i-1, j_it = j+1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            i_it -= 1;
            j_it += 1;
        }
    }

    // Capture LEFT path
    if ( board[i][j].left )
    {
        i_it = i, j_it = j-1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            j_it -= 1;
        }
    }

    // Capture RIGHT path
    if ( board[i][j].right )
    {
        i_it = i, j_it = j+1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            j_it += 1;
        }
    }

    // Capture DL diagonal
    if ( board[i][j].dl )
    {
        i_it = i+1, j_it = j-1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            i_it += 1;
            j_it -= 1;
        }
    }

    // Capture DOWN path
    if ( board[i][j].down )
    {
        i_it = i+1, j_it = j;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            i_it += 1;
        }
    }

    // Capture DR diagonal
    if ( board[i][j].dr )
    {
        i_it = i+1, j_it = j+1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            board[i_it][j_it].value = player;
            i_it += 1;
            j_it += 1;
        }
    }
}

void turn_white_plus_into_green()
{
    int x, y;
    for(int i =0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            if( board[i][j].value == PLAYABLE)
            {
                x=i+1;
                y=j+1;
                x = (x*50) + 25;
                y = (y*50) + 25;
                setcolor(GREEN);
                setfillstyle(SOLID_FILL, GREEN);
                outtextxy(y-5, x-10, "+");
                floodfill(y-5,x-10,GREEN);
            }

        }
    }
}
void make_move( )
{
    int x = 275, y = 275, p, q, m, n;
    int row, column;

    char ch;
    while(1)
    {
        p = x;
        q = y;
        m = (p-25)/50;
        n = (q-25)/50;
        m--;
        n--;
        //cout << m << n;

        if(board[n][m].value==black)
        {
            setcolor(LIGHTGRAY);
            setfillstyle(SOLID_FILL, BLACK);
            circle(p, q , 17);
            floodfill(p,q,LIGHTGRAY);
            setfillstyle(SOLID_FILL,YELLOW);
            floodfill(x-20 ,y-20,WHITE);

        }
        else if(board[n][m].value==white)
        {
            setcolor(BLACK);
            setfillstyle(SOLID_FILL,WHITE);
            circle(p, q, 17);
            floodfill(p,q,BLACK);

            setfillstyle(SOLID_FILL,YELLOW);
            floodfill(x-20 ,y-20,WHITE);
        }
        else if(board[n][m].value==PLAYABLE)
        {
            setfillstyle(SOLID_FILL,YELLOW);
            floodfill(x-20 ,y-20,WHITE);
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, GREEN);
            outtextxy(p-5, q-10, "+");
            floodfill(p-5,q-10,WHITE);
        }
        else
        {
            setfillstyle(SOLID_FILL,YELLOW);
            floodfill(x,y,WHITE);
        }

        if(kbhit())                                            //check if a key is pressed
        {

            ch=getch();

            if(ch==72)                    //move upward
            {
                if(y != 75 )
                    y-=50;
            }
            else if(ch==75)          //move left
            {
                if(x!=75)
                    x-=50;
            }

            else if(ch==77)          //move right
            {

                if(x!=425)
                    x+=50;
            }

            else if(ch==80)          //move downward
            {
                if(y!=425)
                    y+=50;
            }
            else if(ch=='f')
            {

                cout << endl << " p = " << p << " q =" << q << " m =" << m << " n =" << n << endl;
                if(board[n][m].value ==PLAYABLE)
                {
                    turn_white_plus_into_green();
                    break;
                }

            }
            setfillstyle(SOLID_FILL,GREEN);
            floodfill(p,q,WHITE);
            if((ch==80)|| (ch==77)|| (ch==72)|| (ch==75))
            {
                if(board[n][m].value==black)
                {
                    setcolor(LIGHTGRAY);
                    setfillstyle(SOLID_FILL, BLACK);

                    circle(p, q , 17);
                    floodfill(p,q,LIGHTGRAY);
                }
                else if(board[n][m].value==white)
                {
                    /* setcolor(LIGHTGRAY);
                     setfillstyle(SOLID_FILL, RED);
                     circle(p, q , 17);
                     floodfill(p,q,LIGHTGRAY);
                     */
                    setfillstyle(SOLID_FILL,GREEN);
                    floodfill(p-20 ,q-20,WHITE);
                }
                else if(board[n][m].value==PLAYABLE)
                {
                    setcolor(WHITE);
                    setfillstyle(SOLID_FILL, GREEN);
                    outtextxy(p-5, q-10, "+");
                    floodfill(p-5,q-10,WHITE);
                }
            }
        }
    }
    row = (y-25)/50;
    column = (x-25)/50;
    row--;
    column--;
    cout << endl << " x = " << x << " y =" << y << " row =" << row << " column =" << column << endl;
    if ( is_valid_position( row, column ) && board[row][column].value == PLAYABLE )
    {
        board[row][column].value = player;
        capture_pieces( row, column );
        player = player*(-1);
    }
    else wrong_move = TRUE;


}
void display_winner( )
{
    printf( "Final score:\n%s: %d %s: %d\n", "white", white_score, "black", black_score );
    if ( white_score > black_score )
        printf( "%s wins.\n", "white" );
    else if ( white_score < black_score )
        printf( "%s wins.\n",  "black" );
    else
        printf( "Draw game.\n" );
}
int count_flippable_pieces(int i, int j){
    int opposing_player = player*(-1);
    int i_it, j_it , total_flippable_piece=0;

    if ( board[i][j].ul )
    {
        i_it = i-1, j_it = j-1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            i_it -= 1;
            j_it -= 1;
        }
    }
    if ( board[i][j].up )
    {
        i_it = i-1, j_it = j;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            i_it -= 1;
        }
    }

    if ( board[i][j].ur )
    {
        i_it = i-1, j_it = j+1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            i_it -= 1;
            j_it += 1;
        }
    }

    if ( board[i][j].left )
    {
        i_it = i, j_it = j-1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            j_it -= 1;
        }
    }

    if ( board[i][j].right )
    {
        i_it = i, j_it = j+1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            j_it += 1;
        }
    }

    if ( board[i][j].dl )
    {
        i_it = i+1, j_it = j-1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            i_it += 1;
            j_it -= 1;
        }
    }

    if ( board[i][j].down )
    {
        i_it = i+1, j_it = j;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            i_it += 1;
        }
    }

    if ( board[i][j].dr )
    {
        i_it = i+1, j_it = j+1;
        while ( board[i_it][j_it].value == opposing_player )
        {
            total_flippable_piece++;
            i_it += 1;
            j_it += 1;
        }
    }

    return total_flippable_piece;
}


void easy_ai(){

    int min = 10000, row = -1, col = -1, x = 10000;

    for(int i=0;i<8; i++){
        for(int j=0;j<8;j++){
            if(board[i][j].value==PLAYABLE){
                cout << " HEY ";
                x = count_flippable_pieces(i,j);

                cout << "row: " << i << "  column: " << j << " x = " << x << endl;

                if(x<min){
                    min = x;
                    row = i;
                    col = j;
                }
            }
        }
    }
    cout << "AI turn: " << endl << "row: " << row << "  column: " << col << endl;

  /*  turn_white_plus_into_green();
    board[row][col].value = player;
    capture_pieces( row,  column );
    player = player*(-1); */


}
int main()
{
    initial_board();

    int d,m;

    d=DETECT;
    initgraph(&d,&m,"c:\\tc\\bgi");
    //initwindow(getmaxwidth()/2, getmaxheight()/1.5, "Full screen window - press a key to close");

    while ( !game_ended )
    {
        playable_options();
        if ( !has_valid_move )
        {
            if ( skipped_turn )
            {
                game_ended = 1;
                drawboard( );
                continue;
            }
            skipped_turn = 1;
            player=player*-1;
        }
        else
        {
            skipped_turn = 0;
            drawboard( );
            if(player==black) make_move();
            else easy_ai();
        }

    }
    display_winner( );

    getch();
}

