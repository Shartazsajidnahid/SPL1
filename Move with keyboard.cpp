
HEY QWHKHEJWKK

#include<graphics.h>
#include<conio.h>
#include<iostream>
using namespace std;


main()
{
    int d,m;
    int left, top, right, bottom;

    d=DETECT;
    initgraph(&d,&m,"c:\\tc\\bgi");
    while(1){
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

    int x, y, p, q, flag = 0;

    x = 75;
    y = 75;




    char  ch;

    while(1)
    {

        setfillstyle(SOLID_FILL,YELLOW);
        floodfill(x,y,WHITE);
        p = x;
        q = y;
        if(kbhit())  {                                          //check if a key is pressed

            ch=getch();

            if(ch==72)                           //move upward
            {
                if(y != 75 )
                    y-=50;
                setfillstyle(SOLID_FILL,GREEN);
                floodfill(p,q,WHITE);
            }


            else if(ch==75)          //move left
            {
                if(x!=75)
                    x-=50;
                setfillstyle(SOLID_FILL,GREEN);
                floodfill(p,q,WHITE);
            }

            else if(ch==77)          //move right
            {

                if(x!=425)
                    x+=50;
                setfillstyle(SOLID_FILL,GREEN);
                floodfill(p,q,WHITE);
            }

            else if(ch==80)          //move downward
            {
                if(y!=425)
                    y+=50;
                setfillstyle(SOLID_FILL,GREEN);
                floodfill(p,q,WHITE);
            }
            else if(ch=='f'){

                setfillstyle(SOLID_FILL,GREEN);
                floodfill(p,q,WHITE);

                setfillstyle(SOLID_FILL, RED);
                circle(x, y , 20);
                floodfill(x, y ,WHITE);
                flag = 0;
                break;
            }
        }




    }
        while(1)
    {

        cin >> x >> y;
        if(x==0) break;

        x = (x*50) + 25;
        y = (y*50) + 25;

        setfillstyle(SOLID_FILL, BLACK);
        circle(y, x , 20);
        floodfill(y,x,WHITE);

        cin >> x >> y;

        x = (x*50) + 25;
        y = (y*50) + 25;


        setfillstyle(SOLID_FILL, RED);
        circle(y, x , 20);
        floodfill(y,x,WHITE);
    }




    getch();
    cleardevice();
}
}
