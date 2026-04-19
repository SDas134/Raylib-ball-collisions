#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 900
#define WIDTH 600
#define g 100
#define RADIUS HEIGHT/30
#define e 0.75 //coefficient of restitution
#define n 10  //number of balls
typedef struct
{
    double x,y,vx,vy,m;
    Color color;
} Ball;
void drawball(Ball* b,double dt)
{
    if(b->x<0)
    {
        b->x=0;
        b->vx=-e*(b->vy);
    }
    if(b->x+2*(double)RADIUS>WIDTH)
    {
        b->x=(double)WIDTH-2*(double)RADIUS;
        b->vx=-e*(b->vy);
    }
    if(b->y<0)
    {
        b->y=0;
        b->vy=-e*(b->vy);
    }
    if(b->y+2*(double)RADIUS>HEIGHT)
    {
        b->y=(double)HEIGHT-2*(double)RADIUS;
        b->vy=-e*(b->vy);
    }
    b->y+=b->vy*dt;
    b->x+=b->vx*dt;
    b->vy+=g*dt;
    char str[20];
    sprintf(str, "m=%.3f\n", b->m);
    DrawCircle(b->x+(double)RADIUS,b->y+(double)RADIUS,(float)RADIUS, b->color);
    DrawText(str, b->x+5, b->y+(double)RADIUS, 5, BLACK);
}
void ballcollision(Ball* b1,Ball* b2)
{
    if(b1==b2) return;
    double M,MVx,MVy,dux,duy;
    M=b1->m+b2->m;
    MVx=b1->m*b1->vx + b2->m*b2->vx;
    MVy=b1->m*b1->vy + b2->m*b2->vy;
    dux=b1->vx - b2->vx;
    duy=b1->vy - b2->vy;
    double dx=b1->x - b2->x;
    double dy=b1->y - b2->y;
    double distance=sqrt(dx*dx +dy*dy);
    double overlay = 2*(double)RADIUS - distance;
    if( overlay >0 && distance>0)
    {
        double nx=dx/distance;
        double ny=dy/distance;
        b1->x+=nx*(overlay/2);
        b2->x-=nx*(overlay/2);
        b1->y+=ny*(overlay/2);
        b2->y-=ny*(overlay/2);
        b1->vx = (MVx-e*b2->m*dux)/M;
        b2->vx = (MVx+e*b1->m*dux)/M;
        b1->vy = (MVy-e*b2->m*duy)/M;
        b2->vy = (MVy+e*b1->m*duy)/M;
    }
    
    if(CheckCollisionCircles((Vector2){b1->x+(double)RADIUS,b1->y+(double)RADIUS}, (double)RADIUS,(Vector2){b2->x+(double)RADIUS,b2->y+(double)RADIUS},(double)RADIUS)) 
    {
        b1->vx = (MVx-e*b2->m*dux)/M;
        b2->vx = (MVx+e*b1->m*dux)/M;
        b1->vy = (MVy-e*b2->m*duy)/M;
        b2->vy = (MVy+e*b1->m*duy)/M;
    }
}
Color color[10]={MAGENTA,DARKBLUE,YELLOW,PURPLE,RED,LIME,GREEN,SKYBLUE,ORANGE,MAROON};

int main(int argc,char **argv)
{
    srand(time(0));
    Ball* b=(Ball*)malloc(n*sizeof(Ball));
    for(int i=0;i<n;i++)
    {
        b[i]=(Ball){rand()%(WIDTH-2*(int)RADIUS),rand()%(HEIGHT-2*(int)RADIUS),(double)(rand()%1000),(double)(rand()%1000),(double)(rand()%10000),color[rand()%10]};
    }

    InitWindow(WIDTH,HEIGHT,"Balls colliding");
    SetTargetFPS(60);
    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        //Physics
        double dt=0;
        dt=(double)GetFrameTime();
        for(int i=0;i<n;i++)
        {
            drawball(&b[i], dt);
        }
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                ballcollision(&b[i], &b[j]);
            }
        }
        //total time
        double t=GetTime();
        char str[10];
        sprintf(str, "%lf", t);
        DrawText(str, 10, 40, 20, YELLOW);
        DrawFPS(10, 10);
        
        EndDrawing();
    }
    free(b);
    
}
