#include<graphics.h>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include <cstdlib>
#include <ctime>

using namespace std;


#define black 1
#define white (-1)
#define Valid 2
#define EMPTY 0
#define ESC         27
#define UP_ARROW    72
#define LEFT_ARROW  75
#define DOWN_ARROW  80
#define RIGHT_ARROW 77
#define F1          59
#define F2          60
#define F3          61
#define F4          62
#define F5          63

void drawboard();
void initiate_board();
int inboard_or_not( int i, int j );
int distance( int i1, int j1, int i2, int j2 );
int valid_or_not( int i, int j );
void reset_valid();
void mark_valid_moves();
void reverse_disks(int i, int j);
void take_move( );
void computer_mode_menu();
void menu();
void easy_ai();
void medium_ai();
void hard_ai();
void exit_menu(int x);
int count_flippable_pieces(int i, int j);
void two_player_mode(int initial_or_fromexitmenu);
void one_player_mode(int initial_or_fromexitmenu);
void display_winner();

int hard_initial_value[8][8] = {{99, -8, 8, 6, 6, 8, -8, 99},
                                {-8, -24, -4, -3, -3, -4, -24, -8},
                                {8, -4, 7, 4, 4, 7, -4, 8},
                                {6, -3, 4, 0, 0, 4, -3, 6},
                                {6, -3, 4, 0, 0, 4, -3, 6},
                                {8, -4, 7, 4, 4, 7, -4, 8},
                                {-8, -24, -4, -3, -3, -4, -24, -8},
                                {99, -8, 8, 6, 6, 8, -8, 99}};


struct Board
{
    int up,down,right,left,ul,ur,dl,dr, value;
};

struct Board board[8][8];
struct Board dummy_board[8][8];


int  player;
int white_score =2, black_score = 2;
int endgame, didnt_move;

int has_move;
int ai_mode = 0;
int mobility_playable=0;



void drawboard()
{
    int left, top, right, bottom, x, y;
    char str[2];

    setfillstyle(SOLID_FILL,16); //coloring outside the Board
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

    /*
    for(int i =0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            cout << board[i][j].value << "  ";
        }
        cout << endl;
    }
    */

   // cout << endl<< endl;
    white_score = 0;
    black_score = 0;

    for(int i =0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            if(board[i][j].value==black) black_score++;
            if(board[i][j].value==white)  white_score++;
            if(board[i][j].value==black || board[i][j].value== white || board[i][j].value == Valid)
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
                else if (board[i][j].value==Valid)
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
    setcolor(WHITE);
    outtextxy(475, 140, "SCOREBOARD");
    line(475, 158, 570, 158);
    line(475, 160, 570, 160);

    outtextxy(475, 185, "WHITE: ");
    outtextxy(475, 210, "BLACK: ");
    sprintf(str, "%d" , white_score);
    outtextxy(550, 185 , str );
    sprintf(str, "%d" , black_score);
    outtextxy(550, 210 , str );


    //more than 1 digit er ta thik kora lagbe
    //showing player turn
    outtextxy(475, 275, "TURN : ");
    if(player==black)
    {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, BLACK);
        circle(555, 283 , 17);
        floodfill(555, 283,DARKGRAY);
    }
    else
    {
        setcolor(DARKGRAY);
        setfillstyle(SOLID_FILL, WHITE);
        circle(555, 283 , 17);
        floodfill(555, 283,DARKGRAY);
    }

}

void turn_white_plus_into_green()
{
    int x, y;
    for(int i =0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            if( board[i][j].value == Valid)
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

void take_move( )
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
        else if(board[n][m].value==Valid)
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

            if(ch==UP_ARROW)                    //move upward
            {
                if(y != 75 )
                    y-=50;
            }
            else if(ch==LEFT_ARROW)          //move left
            {
                if(x!=75)
                    x-=50;
            }

            else if(ch==RIGHT_ARROW)          //move right
            {

                if(x!=425)
                    x+=50;
            }

            else if(ch==DOWN_ARROW)          //move downward
            {
                if(y!=425)
                    y+=50;
            }
            else if(ch=='f' || ch == 'F' || ch == '\r')
            {

                //cout << endl << " p = " << p << " q =" << q << " m =" << m << " n =" << n << endl;
                if(board[n][m].value ==Valid)
                {
                    break;
                }

            }
            else if( ch == ESC){
                cleardevice();
                exit_menu(ai_mode);
            }
            setfillstyle(SOLID_FILL,GREEN);
            floodfill(p,q,WHITE);
            if((ch==UP_ARROW)|| (ch==DOWN_ARROW)|| (ch==LEFT_ARROW)|| (ch==RIGHT_ARROW))
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
                else if(board[n][m].value==Valid)
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
    //cout << endl << " x = " << x << " y =" << y << " row =" << row << " column =" << column << endl;
    if ( inboard_or_not( row, column ) && board[row][column].value == Valid )
    {
        board[row][column].value = player;
        reverse_disks( row, column );
        player = player*(-1);
    }

    turn_white_plus_into_green();


}


void reverse_disks( int i, int j )
{
    int opponent = player*(-1);
    int temp_row, temp_col;

    //  UL diagonal
    if ( board[i][j].ul == 1 )
    {
        temp_row = i-1, temp_col = j-1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_row -= 1;
            temp_col -= 1;
        }
    }

    //  UP
    if ( board[i][j].up == 1)
    {
        temp_row = i-1, temp_col = j;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_row -= 1;
        }
    }

    //  UR diagonal
    if ( board[i][j].ur == 1 )
    {
        temp_row = i-1, temp_col = j+1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_row -= 1;
            temp_col += 1;
        }
    }

    //  LEFT
    if ( board[i][j].left == 1 )
    {
        temp_row = i, temp_col = j-1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_col -= 1;
        }
    }

    //  RIGHT
    if ( board[i][j].right== 1 )
    {
        temp_row = i, temp_col = j+1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_col += 1;
        }
    }

    //  DL
    if ( board[i][j].dl == 1 )
    {
        temp_row = i+1, temp_col = j-1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_row += 1;
            temp_col -= 1;
        }
    }

    //  DOWN
    if ( board[i][j].down == 1 )
    {
        temp_row = i+1, temp_col = j;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_row += 1;
        }
    }

    //  DR diagonal
    if ( board[i][j].dr == 1 )
    {
        temp_row = i+1, temp_col = j+1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            board[temp_row][temp_col].value = player;
            temp_row += 1;
            temp_col += 1;
        }
    }
}


int hard_board_val(int i, int j){
    int count =   hard_initial_value[i][j];
    return count;
}

void hard_mobility(int i, int j){
    mobility_playable = 0;


    for ( int i=0; i<8; ++i )
    {
        for ( int j=0; j<8; ++j )
        {
             dummy_board[i][j].value = board[i][j].value;
             dummy_board[i][j].up = board[i][j].up;
             dummy_board[i][j].down = board[i][j].down;
             dummy_board[i][j].left = board[i][j].left;
             dummy_board[i][j].right = board[i][j].right;
             dummy_board[i][j].ul = board[i][j].ul;
             dummy_board[i][j].ur = board[i][j].ur;
             dummy_board[i][j].dr = board[i][j].dr;
             dummy_board[i][j].dl = board[i][j].dl;
        }
    }


    board[i][j].value = player;

    reverse_disks( i,  j );
    player = player*(-1);
    mark_valid_moves();
    player = player*(-1);


    for ( int i=0; i<8; ++i )
    {
        for ( int j=0; j<8; ++j )
        {
             board[i][j].value = dummy_board[i][j].value ;
             board[i][j].up= dummy_board[i][j].up ;
             board[i][j].down=dummy_board[i][j].down  ;
             board[i][j].left= dummy_board[i][j].left ;
             board[i][j].right=dummy_board[i][j].right  ;
             board[i][j].ul=dummy_board[i][j].ul  ;
             board[i][j].ur=dummy_board[i][j].ur  ;
             board[i][j].dr=dummy_board[i][j].dr  ;
             board[i][j].dl = dummy_board[i][j].dl ;
        }
    }


}

void hard_c_square_value_update(){

    if(board[0][0].value!=0){

        if(board[0][0].value==white){
           // cout << "yes 1 ";
            if(hard_initial_value[0][1]==-8){
                hard_initial_value[0][1]+=18;
            }
            if(hard_initial_value[1][0]==-8){
                hard_initial_value[1][0]+=18;
            }
        }

        else{
            //check down
            int col = 0;
            int row = 2;
            int white_count = 0;

            for(;row<8;row++){
                if(board[row][col].value==black){
                    if(hard_initial_value[1][0]==-8) hard_initial_value[1][0]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                    white_count++;
                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[1][0]==-8){
                hard_initial_value[1][0]+=18;
            }
            }
            //check right
            col = 2;
            row = 0;
            white_count=0;

            for(;col<8;col++){
                if(board[row][col].value==black){
                    if(hard_initial_value[0][1]==-8) hard_initial_value[0][1]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                        white_count++;

                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[0][1]==-8){
                hard_initial_value[0][1]+=18;
            }
            }

        }
    }

    if(board[7][0].value!=0){
       // cout << "YA 2" << endl;
        if(board[7][0].value==white){
           // cout << "yes 1 ";
            if(hard_initial_value[7][1]==-8){
                hard_initial_value[7][1]+=18;
            }
            if(hard_initial_value[6][0]==-8){
                hard_initial_value[6][0]+=18;
            }
        }

        else{
            //check up
            int col = 0;
            int row = 5;
            int white_count = 0;

            for(;row<8;row--){
                if(board[row][col].value==black){
                    if(hard_initial_value[6][0]==-8) hard_initial_value[6][0]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                    white_count++;
                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[6][0]==-8){
                hard_initial_value[6][0]+=18;
            }
            }
            //check right
            col = 2;
            row = 0;
            white_count=0;

            for(;col<8;col++){
                if(board[row][col].value==black){
                    if(hard_initial_value[7][1]==-8) hard_initial_value[7][1]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                                        white_count++;

                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[7][1]==-8){
                hard_initial_value[7][1]+=18;
            }
            }

        }
    }

    if(board[0][7].value!=0){
        //cout << "YA 3" << endl;
        if(board[0][7].value==white){
           // cout << "yes 1 ";
            if(hard_initial_value[0][6]==-8){
                hard_initial_value[0][6]+=18;
            }
            if(hard_initial_value[1][7]==-8){
                hard_initial_value[1][7]+=18;
            }
        }

        else{
            //check down
            int col = 7;
            int row = 2;
            int white_count = 0;

            for(;row<8;row++){
                if(board[row][col].value==black){
                    if(hard_initial_value[1][7]==-8) hard_initial_value[1][7]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                    white_count++;
                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[1][7]==-8){
                hard_initial_value[1][7]+=18;
            }
            }
            //check left
            col = 5;
            row = 0;
            white_count=0;

            for(;col>=0;col--){
                if(board[row][col].value==black){
                    if(hard_initial_value[0][6]==-8) hard_initial_value[0][6]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                        white_count++;

                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[0][6]==-8){
                hard_initial_value[0][6]+=18;
            }
            }

        }
    }

    if(board[7][7].value!=0){
       // cout << "YA 4" << endl;
        if(board[7][7].value==white){
          //  cout << "yes 1 ";
            if(hard_initial_value[6][7]==-8){
                hard_initial_value[6][7]+=18;
            }
            if(hard_initial_value[7][6]==-8){
                hard_initial_value[7][6]+=18;
            }
        }

        else{
            //check up
            int col = 7;
            int row = 5;
            int white_count = 0;

            for(;row>=0;row--){
                if(board[row][col].value==black){
                    if(hard_initial_value[6][7]==-8) hard_initial_value[6][7]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                    white_count++;
                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[6][7]==-8){
                hard_initial_value[6][7]+=18;
            }
            }
            //check left
            col = 5;
            row = 7;
            white_count=0;

            for(;col>=0;col--){
                if(board[row][col].value==black){
                    if(hard_initial_value[7][6]==-8) hard_initial_value[7][6]+=18;
                    break;
                }
                else if(board[row][col].value==white){
                        white_count++;

                    continue;
                }
                else {
                    break;
                }
            }
            if(white_count==6){
                if(hard_initial_value[7][6]==-8){
                hard_initial_value[7][6]+=18;
            }
            }

        }
    }

   // for ( int i=0; i<8; ++i )
   // {
     //   for ( int j=0; j<8; ++j )
      //  {
           // cout << hard_initial_value[i][j] << " ";
      //  }
        //cout << endl;
   // }
    //cout << endl << endl;
}


int how_many_corners(){
    if(board[0][0].value == black || board[0][0].value == white)
        return 1;
    else if(board[0][7].value == black|| board[0][7].value == white)
        return 1;
    else if(board[7][0].value == black|| board[7][0].value == white)
        return 1;
    else if(board[7][7].value == black|| board[7][7].value == white)
        return 1;
    else return 0;

}


void hard_x_square_value_update(){
    if(board[0][0].value==white){
        if(board[0][1].value==white && board[1][0].value==white){
            hard_initial_value[1][1]+=30;
        }
    }
    if(board[7][0].value==white){
        if(board[7][1].value==white && board[6][0].value==white){
            hard_initial_value[6][1]+=30;
        }
    }
    if(board[0][7].value==white){
        if(board[0][6].value==white && board[1][7].value==white){
            hard_initial_value[1][6]+=30;
        }
    }
    if(board[7][7].value==white){
        if(board[6][7].value==white && board[7][6].value==white){
            hard_initial_value[6][6]+=30;
        }
    }
}


int hard_minimum_or_maximum(int i, int j){
    if(black_score+white_score >44 || how_many_corners()<1){
         return (-1)*count_flippable_pieces(i,j);
    }
    else return 0;
}


int count_flippable_pieces(int i, int j){
    int opponent = player*(-1);
    int temp_row, temp_col , total_flippable_piece=0;

    if ( board[i][j].ul )
    {
        temp_row = i-1, temp_col = j-1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_row -= 1;
            temp_col -= 1;
        }
    }
    if ( board[i][j].up )
    {
        temp_row = i-1, temp_col = j;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_row -= 1;
        }
    }

    if ( board[i][j].ur )
    {
        temp_row = i-1, temp_col = j+1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_row -= 1;
            temp_col += 1;
        }
    }

    if ( board[i][j].left )
    {
        temp_row = i, temp_col = j-1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_col -= 1;
        }
    }

    if ( board[i][j].right )
    {
        temp_row = i, temp_col = j+1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_col += 1;
        }
    }

    if ( board[i][j].dl )
    {
        temp_row = i+1, temp_col = j-1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_row += 1;
            temp_col -= 1;
        }
    }

    if ( board[i][j].down )
    {
        temp_row = i+1, temp_col = j;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_row += 1;
        }
    }

    if ( board[i][j].dr )
    {
        temp_row = i+1, temp_col = j+1;
        while ( board[temp_row][temp_col].value == opponent )
        {
            total_flippable_piece++;
            temp_row += 1;
            temp_col += 1;
        }
    }

    return total_flippable_piece;
}


int randomfunc(int count){

        int random;

      // initialize random
        srand ( time(NULL) );

      // generate secret number
        random = rand() % count  + 1;

     return random;

}


int valid_or_not( int i, int j )
{

    board[i][j].up = board[i][j].down = board[i][j].right = board[i][j].left =board[i][j].ul =board[i][j].ur =board[i][j].dl =board[i][j].dr= 0;

    int playable_or_not = 0, opponent = player*(-1);

    // Test UL diagonal
    int temp_row = i-1, temp_col = j-1;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
    {
        temp_row -= 1;
        temp_col -= 1;
    }
    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].ul = 1;
        playable_or_not = 1;
    }

    // Test UP path
    temp_row = i-1, temp_col = j;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
        temp_row -= 1;

    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].up = 1;
        playable_or_not = 1;
    }

    // Test UR diagonal
    temp_row = i-1, temp_col = j+1;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
    {
        temp_row -= 1;
        temp_col += 1;
    }
    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].ur = 1;
        playable_or_not = 1;
    }

    // Test LEFT path
    temp_row = i, temp_col = j-1;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
        temp_col -= 1;

    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].left = 1;
        playable_or_not = 1;
    }

    // Test RIGHT path
    temp_row = i, temp_col = j+1;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
        temp_col += 1;

    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].right = 1;
        playable_or_not = 1;
    }

    // Test DL diagonal
    temp_row = i+1, temp_col = j-1;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
    {
        temp_row += 1;
        temp_col -= 1;
    }
    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].dl = 1;
        playable_or_not = 1;
    }

    // Test DOWN path
    temp_row = i+1, temp_col = j;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
        temp_row += 1;

    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].down = 1;
        playable_or_not = 1;
    }

    // Test DR diagonal
    temp_row = i+1, temp_col = j+1;
    while (board[temp_row][temp_col].value == opponent && inboard_or_not( temp_row, temp_col ))
    {
        temp_row += 1;
        temp_col += 1;
    }
    if (  board[temp_row][temp_col].value == player && inboard_or_not( temp_row, temp_col ) && distance( i, j, temp_row, temp_col ) > 1  )
    {
        board[i][j].dr = 1;
        playable_or_not = 1;
    }
    return playable_or_not;
}


void reset_valid(){

    for ( int i=0; i<8; ++i )
    {
        for ( int j=0; j<8; ++j )
        {
            if ( board[i][j].value == Valid )
                 board[i][j].value = EMPTY;
        }
    }

}


void mark_valid_moves()
{
    has_move = 0;
    reset_valid();

    for ( int i=0; i<8; ++i )
    {
        for ( int j=0; j<8; ++j )
        {
            if ( board[i][j].value == EMPTY ){
                if ( valid_or_not( i, j ) )
                {
                    board[i][j].value = Valid;
                    has_move = 1;
                    mobility_playable++;
                }
            }

        }
    }
}


int inboard_or_not( int i, int j )
{
    if ( i < 0 || i >= 8 || j < 0 || j >= 8 ) return 0;
    return 1;
}


int distance( int i1, int j1, int i2, int j2 )
{
    int distance1 = abs( i1 - i2 ), distance2 = abs( j1 - j2 );
    if ( distance1 > 0 ) return distance1;
    else return distance2;
}



void initiate_board()
{
    white_score = 2;
    black_score = 2;
    endgame = 0;
    didnt_move = 0;
    has_move = 0;


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


void one_player_mode(int initial_or_fromexitmenu){
    if(!initial_or_fromexitmenu)initiate_board();


    while ( !endgame )
    {
        mark_valid_moves();

        if(has_move){
            didnt_move = 0;
            drawboard( );
            if(player==black) take_move();
            else {
                Sleep(1000);
                if(ai_mode == 1)
                    easy_ai();
                if(ai_mode == 2)
                    medium_ai();
                if( ai_mode == 3)
                    hard_ai();
            }
        }
        else{

            if ( didnt_move )
            {
                endgame = 1;
                drawboard();
                continue;
            }
            //cout << endl << "skikpper " << endl;
            didnt_move = 1;
            player=player*-1;

        }

        cleardevice();

    }
    cleardevice();
    display_winner();
}

void two_player_mode(int initial_or_fromexitmenu){


    if(!initial_or_fromexitmenu)initiate_board();

    while (!endgame)
    {
        mark_valid_moves();
        if ( has_move )
        {
            didnt_move = 0;
            drawboard( );
            take_move( );

        }
        else
        {
            if ( didnt_move )
            {
                endgame = 1;
                drawboard( );
                continue;
            }
            didnt_move = 1;
            player=player*-1;
        }


        cleardevice();
    }

    cleardevice();
    display_winner();

}


void easy_ai(){

    int min = 10000, row = -1, col = -1, x = 10000 , count = 0 ;

    for(int i=0;i<8; i++){

        for(int j=0;j<8;j++){

            if(board[i][j].value==Valid){
                count++;

            }
        }
    }

    int random = randomfunc(count);
    count = 0;

    //cout << random << endl;

    for(int i=0;i<8; i++){

        for(int j=0;j<8;j++){

            if(board[i][j].value==Valid){
                count++;
                if(count == random){
                    row = i;
                    col = j;
                    break;
                }
            }
        }
    }

    //cout << "AI turn: " << endl << "row: " << row << "  column: " << col << endl;

    board[row][col].value = player;
    reverse_disks( row,  col );
    player = player*(-1);
    turn_white_plus_into_green();


}


void medium_ai(){

    int max = -1, row = -1, col = -1, x = 10000;

    for(int i=0;i<8; i++){
        for(int j=0;j<8;j++){
            if(board[i][j].value==Valid){
                // << " HEY ";
                x = count_flippable_pieces(i,j);

               // cout << "row: " << i << "  column: " << j << " x = " << x << endl;

                if(x>max){
                    max = x;
                    row = i;
                    col = j;
                }
            }
        }
    }
    //cout << "AI turn: " << endl << "row: " << row << "  column: " << col << endl;

    board[row][col].value = player;
    reverse_disks( row,  col );
    player = player*(-1);
   //turn_white_plus_into_green();


}

void hard_ai(){

    int max = -10000, row = -1, col = -1, x;

    hard_x_square_value_update();

    hard_c_square_value_update();

    for(int i=0;i<8; i++){
        for(int j=0;j<8;j++){

            x = 0;
            if(board[i][j].value==Valid){


                x += hard_board_val(i,j);
                hard_mobility(i,j);
                x -= mobility_playable;
                x += hard_minimum_or_maximum(i,j);

                //cout << "row: " << i << "  column: " << j << " x = " << x << endl;

                if(x>max){

                    max = x;
                    row = i;
                    col = j;
                }
                else if(x == max ){
                    if(hard_board_val(i,j)>hard_board_val(row,col)){
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }
   // cout << "AI turn: " << endl << "row: " << row << "  column: " << col << endl;

    board[row][col].value = player;
    reverse_disks( row,  col );
    player = player*(-1);

}

void exit_menu_input(int menu_or_game){
    char ch;

    while(1)
    {


        if(kbhit())                                            //check if a key is pressed
        {

            ch=getch();

            if(ch=='y' || ch == 'Y')
            {

                cleardevice();
                if(menu_or_game == -1)
                    exit(1);
                else{
                    menu();
                }

            }
            else if(ch=='n' || ch == 'N')  // 1 player menu
            {
                cleardevice();
                if(menu_or_game==-1)
                    menu();
                else if(menu_or_game==0){
                    drawboard();
                    take_move();
                    two_player_mode(1);
                }
                else if(menu_or_game>0){
                    drawboard();
                    take_move();
                    one_player_mode(1);
                }

            }

        }
    }
}


void exit_menu(int x){
    cleardevice();

    setcolor(WHITE);

    for(int i=0;i<7;i++)
	rectangle(110-i,120-i,450+i,350+i);

    outtextxy(180,180,"DO  YOU  WANT  TO  QUIT  ?");

    for(int i=0;i<2;i++)
        rectangle(215-i,215-i,253+i,240+i);

	outtextxy(220,220,"YES");

	for(int i=0;i<2;i++)
        rectangle(275-i,215-i,303+i,240+i);

	outtextxy(280,220,"NO");

	outtextxy(180,280,"press Y or N to continue . . . .");
	exit_menu_input(x);

}


void computer_mode_menu_input(){
    char ch;

    while(1)
    {


        if(kbhit())                                            //check if a key is pressed
        {

            ch=getch();

            if(ch==F1 || ch == '1' || ch =='E' || ch == 'e')    // 1 player
            {
                cleardevice();
                ai_mode = 1;
                one_player_mode(0);
            }
            else if(ch==F2 || ch == '2' || ch =='M' || ch == 'm')   // 2 player menu
            {
                cleardevice();
                ai_mode = 2;
                one_player_mode(0);

            }
            else if(ch==F3 || ch == '3' || ch =='H' || ch == 'h')   // 2 player menu
            {
                cleardevice();
                ai_mode = 3;
                one_player_mode(0);
                //computer_mode_menu();

            }

            else if(ch==ESC  || ch == '4' || ch =='B' || ch == 'b')
            {

               cleardevice();
               menu();

            }

        }
    }

}


void computer_mode_menu()
{


    setfillstyle(SOLID_FILL,16); //coloring outside the Board
    outtextxy(270, 50, "REVERSI");
    for(int i=0;i<2;i++)
    rectangle(265-i,45-i,335-i,70-i);

    floodfill(25,75,WHITE);

    for(int i=0;i<7;i++)
	rectangle(110-i,90-i,500+i,370+i);


	//settextstyle(0,HORIZ_DIR,1);
	outtextxy(200,120,"1.");
	outtextxy(240,120,"EASY");
	line(240, 140, 250, 140);

    outtextxy(200,170,"2.");
	outtextxy(240,170,"MEDIUM");
	line(240, 190, 250, 190);

    outtextxy(200,220,"3.");
	outtextxy(240,220,"HARD");
	line(240, 240, 250, 240);

	outtextxy(200,270,"4.");
	outtextxy(240,270,"BACK TO MAIN MENU");
	line(240, 290, 250, 290);

	outtextxy(120,340,"press  1,2,3,4  /  E,C,H,B  /  F1,F2,F3,ESC  to continue . . . .");


	computer_mode_menu_input();


}


void menuinput(){
    char ch;

    while(1)
    {


        if(kbhit())                                            //check if a key is pressed
        {

            ch=getch();

            if(ch==F1 || ch == '1' || ch =='H' || ch == 'h')   // 2 player
            {
                cleardevice();
                two_player_mode(0);
            }
            else if(ch==F2 || ch == '2' || ch =='C' || ch == 'c')   // 1 player menu
            {
                cleardevice();

                computer_mode_menu();

            }

            else if(ch==ESC || ch == '3' || ch == 'E' || ch =='e')
            {
                exit_menu(-1);
               //exit(1);

            }

        }
    }

}


void display_winner()
{
    char str[2];



    setfillstyle(SOLID_FILL,16); //coloring outside the Board
    floodfill(25,75,WHITE);
    setcolor(WHITE);
    setbkcolor(16);
    for(int i=0;i<7;i++)
	rectangle(110-i,90-i,500+i,370+i);



    if ( white_score > black_score )
        {
            outtextxy(260, 135, "WHITE WINS !");
            line(260, 157, 350, 157);
            line(260, 160, 350, 160);
            outtextxy(200, 185 , "score ->" );
            sprintf(str, "%d" , white_score);
            outtextxy(270, 185 , str );
            outtextxy(300, 185 , "vs" );
            sprintf(str, "%d" , black_score);
            outtextxy(340, 185 , str );
        }
    else if ( white_score < black_score )
        {

            outtextxy(260, 135, "BLACK WINS !");
            line(260, 157, 350, 157);
            line(260, 160, 350, 160);
            outtextxy(200, 185 , "score ->" );
            sprintf(str, "%d" , black_score);
            outtextxy(270, 185 , str );
            outtextxy(300, 185 , "vs" );
            sprintf(str, "%d" , white_score);
            outtextxy(340, 185 , str );
        }
    else
        {
            //settextstyle(1,0,15);
            outtextxy(280, 135, "DRAW");
            line(280, 157, 320, 157);
            line(280, 160, 320, 160);
            outtextxy(200, 185 , "score ->" );
            sprintf(str, "%d" , white_score);
            outtextxy(270, 185 , str );
            outtextxy(300, 185 , "vs" );
            sprintf(str, "%d" , black_score);
            outtextxy(340, 185 , str );
        }

    //showing scores

        outtextxy(160,340,"         press  any key  to continue . . . .");

        while(1)
    {


        if(kbhit())                                            //check if a key is pressed
        {
            cleardevice();
            menu();
        }
    }

}

void menu()
{

    ai_mode = 0;
    setfillstyle(SOLID_FILL,16); //coloring outside the Board

    floodfill(25,75,WHITE);

    setcolor(WHITE);

    setbkcolor(16);

    outtextxy(270, 50, "REVERSI");
    for(int i=0;i<2;i++)
    rectangle(265-i,45-i,335-i,70-i);


    for(int i=0;i<7;i++)
	rectangle(110-i,90-i,500+i,370+i);


	//settextstyle(0,HORIZ_DIR,1);
	outtextxy(200,140,"1.");
	outtextxy(240,140,"HUMAN  vs  HUMAN");
	line(240, 160, 250, 160);

    outtextxy(200,200,"2.");
	outtextxy(240,200,"COMPUTER  vs  HUMAN");
	line(240, 220, 250, 220);

    outtextxy(200,260,"3.");
	outtextxy(240,260,"EXIT");
	line(240, 280, 250, 280);

	outtextxy(160,340,"press  1,2,3  /  H,C,E  /  F1,F2,ESC  to continue . . . .");

	menuinput();


}


int main()
{
    int d,m;

    d=DETECT;

    initgraph(&d,&m,"Reversi");


    menu();

    getch();
}

