#include<graphics.h>
#include<conio.h>
#include<iostream>

#define black 1
#define red -1

using namespace std;
int board[8][8]= {0}, player;

void initial_board()
{
    board[3][3] = black;
    board[4][4] = black;
    board[3][4] = red;
    board[4][3] = red;
    player = black;
}




int main()
{

    initial_board();


    int d,m;
    int left, top, right, bottom, x, y;

    d=DETECT;
    initgraph(&d,&m,"c:\\tc\\bgi");

    left = 50;
    top = 50;
    right = 100;
    bottom = 100;

    for(int i=0; i<8; i++)
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

    while(1)
    {

        for(int i =0; i<8; i++)
        {
            for(int j = 0; j<8; j++)
            {
                if(board[i][j]==black || board[i][j]== red )
                {
                    x=i+1;
                    y=j+1;
                    x = (x*50) + 25;
                    y = (y*50) + 25;
                    if(board[i][j]==black)
                    {
                        setfillstyle(SOLID_FILL, BLACK);
                        circle(y, x , 20);
                        floodfill(y,x,WHITE);
                    }
                    else if(board[i][j]==red)
                    {
                        setfillstyle(SOLID_FILL, RED);
                        circle(y, x , 20);
                        floodfill(y,x,WHITE);
                    }
                }
            }
        }
        player = player*(-1);
        cin >> x >> y;
        board[x-1][y-1] = player;

    }
    getch();


}

