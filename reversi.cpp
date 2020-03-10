#include<graphics.h>
#include<stdio.h>
#include<iostream>

#define black 1
#define red (-1)
#define PLAYABLE 2
#define EMPTY 0
#define TRUE 1
#define FALSE 0

using namespace std;
int board[8][8]= {0}, player;
int red_score =2, black_score = 2;
int playable_direction[8][8][8];
int game_ended = FALSE;
int skipped_turn = FALSE;
int wrong_move = FALSE;
int has_valid_move = FALSE;


void initial_board()
{
    board[3][3] = red;
    board[4][4] = red;
    board[3][4] = black;
    board[4][3] = black;
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
    //memset( playable_direction[i][j], 0, 8 );
    for(int x=0; x<8; x++){
        playable_direction[i][j][x] = 0;
    }
    if ( !is_valid_position( i, j ) ) return FALSE;
    if ( board[i][j] != EMPTY ) return FALSE;
    int playable = FALSE;

    int opposing_player = player*(-1);

    // Test UL diagonal
    int i_it = i-1, j_it = j-1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
    {
        i_it -= 1;
        j_it -= 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][0] = 1;
        playable = TRUE;
    }

    // Test UP path
    i_it = i-1, j_it = j;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
        i_it -= 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][1] = 1;
        playable = TRUE;
    }

    // Test UR diagonal
    i_it = i-1, j_it = j+1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
    {
        i_it -= 1;
        j_it += 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][2] = 1;
        playable = TRUE;
    }

    // Test LEFT path
    i_it = i, j_it = j-1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
        j_it -= 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][3] = 1;
        playable = TRUE;
    }

    // Test RIGHT path
    i_it = i, j_it = j+1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
        j_it += 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][4] = 1;
        playable = TRUE;
    }

    // Test DL diagonal
    i_it = i+1, j_it = j-1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
    {
        i_it += 1;
        j_it -= 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][5] = 1;
        playable = TRUE;
    }

    // Test DOWN path
    i_it = i+1, j_it = j;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
        i_it += 1;

    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][6] = 1;
        playable = TRUE;
    }

    // Test DR diagonal
    i_it = i+1, j_it = j+1;
    while ( is_valid_position( i_it, j_it ) && board[i_it][j_it] == opposing_player )
    {
        i_it += 1;
        j_it += 1;
    }
    if ( is_valid_position( i_it, j_it ) && distance( i, j, i_it, j_it ) > 1 && board[i_it][j_it] == player )
    {
        playable_direction[i][j][7] = 1;
        playable = TRUE;
    }
    return playable;
}


void mark_playable_positions()
{
    has_valid_move = FALSE;
    for ( int i=0; i<8; ++i )
    {
        for ( int j=0; j<8; ++j )
        {
            if ( board[i][j] == PLAYABLE )
                board[i][j] = EMPTY;
            if ( is_playable( i, j ) )
            {
                board[i][j] = PLAYABLE;
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
    if ( playable_direction[i][j][0] )
    {
        i_it = i-1, j_it = j-1;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            i_it -= 1;
            j_it -= 1;
        }
    }

    // Capture UP path
    if ( playable_direction[i][j][1] )
    {
        i_it = i-1, j_it = j;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            i_it -= 1;
        }
    }

    // Capture UR diagonal
    if ( playable_direction[i][j][2] )
    {
        i_it = i-1, j_it = j+1;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            i_it -= 1;
            j_it += 1;
        }
    }

    // Capture LEFT path
    if ( playable_direction[i][j][3] )
    {
        i_it = i, j_it = j-1;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            j_it -= 1;
        }
    }

    // Capture RIGHT path
    if ( playable_direction[i][j][4] )
    {
        i_it = i, j_it = j+1;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            j_it += 1;
        }
    }

    // Capture DL diagonal
    if ( playable_direction[i][j][5] )
    {
        i_it = i+1, j_it = j-1;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            i_it += 1;
            j_it -= 1;
        }
    }

    // Capture DOWN path
    if ( playable_direction[i][j][6] )
    {
        i_it = i+1, j_it = j;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            i_it += 1;
        }
    }

    // Capture DR diagonal
    if ( playable_direction[i][j][7] )
    {
        i_it = i+1, j_it = j+1;
        while ( board[i_it][j_it] == opposing_player )
        {
            board[i_it][j_it] = player;
            i_it += 1;
            j_it += 1;
        }
    }
}
void make_move( )
{
    int row, column;
    cout << "Enter row (1-8) and column (1-8) separated by a single space (eg.: 2 4).\n";
    cin >> row >> column;
    row--;
    column--;
    if ( is_valid_position( row, column ) && board[row][column] == PLAYABLE )
    {
        board[row][column] = player;
        capture_pieces( row, column );
        player = player*(-1);
    }
    else wrong_move = TRUE;
}


int main()
{

    initial_board();

    int d,m;
    int left, top, right, bottom, x, y;

    d=DETECT;
    initgraph(&d,&m,"c:\\tc\\bgi");

    char str[1];
    for(int i = 0; i<8; i++)            //printing column numbers
    {
        x = i+1;
        sprintf(str,"%d",x);
        outtextxy((x*50)+25,25 , str );
    }
    for(int i = 0; i<8; i++)            //printing row numbers
    {
        x = i+1;
        sprintf(str,"%d",x);
        outtextxy(25, (x*50)+20 , str );
    }

            setfillstyle(SOLID_FILL,RED); //coloring outside the box
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
            setfillstyle(SOLID_FILL,GREEN);
            rectangle(left,top,right,bottom);
            floodfill((left+25),(top+25),WHITE);
            left += 50;
            right += 50;
        }

        top+=50;
        bottom+=50;
    }
    outtextxy(500, 175, "RED    : ");
    outtextxy(500, 200, "BlACK: ");


    while(1)
    {
        mark_playable_positions();
        for(int i =0; i<8; i++)
        {
            for(int j = 0; j<8; j++)
            {
                cout << board[i][j] << "  ";
            }
        cout << endl;
        }
    //showing scores
        sprintf(str, "%d" , red_score);
        outtextxy(575, 175 , str );
        sprintf(str, "%d" , black_score);
        outtextxy(575, 200 , str );

        red_score = 0;
        black_score = 0;
        for(int i =0; i<8; i++)
        {
            for(int j = 0; j<8; j++)
            {
                if(board[i][j]==black) black_score++;
                if(board[i][j]==red)  red_score++;
                if(board[i][j]==black || board[i][j]== red || board[i][j] == PLAYABLE)
                {
                    x=i+1;
                    y=j+1;
                    x = (x*50) + 25;
                    y = (y*50) + 25;
                    if(board[i][j]==black)
                    {
                        setfillstyle(SOLID_FILL, BLACK);
                        circle(y, x , 17);
                        floodfill(y,x,WHITE);
                    }
                    else if(board[i][j]==red)
                    {
                        setfillstyle(SOLID_FILL, RED);
                        circle(y, x , 17);
                        floodfill(y,x,WHITE);
                    }
                    else if (board[i][j]==PLAYABLE){
                        x = i+1;
                        y = j+1;
                        x = (x*50)+25;
                        y = (y*50)+25;
                        setfillstyle(SOLID_FILL,YELLOW);
                        floodfill(y,x,WHITE);
                    }
                }

            }
        }
        cout << endl << red_score;
        make_move();
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                x = i+1;
                y = j+1;
                x = (x*50)+25;
                y = (y*50)+25;
                setfillstyle(SOLID_FILL,GREEN);
                floodfill(y,x,WHITE);
            }
        }

       // player = player*(-1);
       // cin >> x >> y;
        //board[x-1][y-1] = player;

    }

    getch();


}

