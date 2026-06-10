#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

 float A,B,C;
 float cubeWidth=10;
 int width=67;
 int height=55;
 float zBuffer[67*55];
 char buffer[67*150];
 char screen[(67*150)+55+1];
 int backgroundACSIICODE=' ';
 float horizontalOffset;

 float incermentSpeed=0.7; 
 int idx;
 int distanceFromCam =60;
 float K1=28; 
 float x,y,z;
 float ooz;
 int xp,yp;
 
 float calculateX(float i, float j, float k)
 {
     return j*sin(A)*sin(B)*cos(C)
     - k*cos(A)*sin(B)*cos(C)
     + j * cos(A)*sin(C) + k * sin(A)*sin(C)
     + i *cos(B) *cos(C);
     
 }
    
float calculateY(float i,float j ,float k)
{
    return j*cos(A)*cos(C) + k *sin(A) * cos(C) - j *sin(A)*sin(B)*sin(C)
          +k*cos(A)*sin(B)*sin(C) - i*cos(B)*sin(C);
}

float calculateZ(float i, float j ,float k)
{
    return k*cos(A)*cos(B) - j*sin(A)*cos(B)
        +i *sin(B);
}

 void calculateForSurface(float cubeX,float cubeY,float cubeZ, int ch)
 {
    x= calculateX(cubeX,cubeY,cubeZ);
    y= calculateY(cubeX,cubeY,cubeZ);
    z= calculateZ(cubeX,cubeY,cubeZ)+ distanceFromCam;

    ooz=1.0f/z;

    xp=(int)(width/2+horizontalOffset+K1*ooz*x*2);
    yp=(int)(height/2+K1*ooz*y);

    if(xp>=0&&xp<width&&yp>=0&&yp<height)
    {
        idx=xp+yp*width;

        if(ooz>zBuffer[idx])
        {
            zBuffer[idx]=ooz;
            buffer[idx]=ch;
        }

    }

}

int main()
{
    printf("\x1b[2J");
    printf("\x1b[?25l");

    while(1)
    {
        memset(buffer,backgroundACSIICODE,sizeof(buffer));
        memset(zBuffer,0,sizeof(zBuffer));

        horizontalOffset=18;

        for(float cubeX=-cubeWidth;cubeX<cubeWidth;cubeX+=incermentSpeed)
        {
            for(float cubeY=-cubeWidth;cubeY<cubeWidth;cubeY+=incermentSpeed)
            {
                // Front face
                calculateForSurface(cubeX,cubeY,-cubeWidth,'.');

                // Right face
                calculateForSurface(cubeWidth,cubeY,cubeX,'$');

                // Left face
                calculateForSurface(-cubeWidth,cubeY,-cubeX,'#');

                // Back face
                calculateForSurface(-cubeX,cubeY,cubeWidth,'@');

                // Bottom face
                calculateForSurface(cubeX,-cubeWidth,-cubeY,'+');

                // Top face
                calculateForSurface(cubeX,cubeWidth,cubeY,'&');
            }
        }

        int p=0;
        for(int row=0;row<height;row++)
        {
            for(int col=0;col<width;col++)
            {
                screen[p++]=buffer[col+row*width];
            }

            screen[p++]='\n';
        }

        screen[p]='\0';
   
        printf("\x1b[H%s",screen);
        fflush(stdout);

        A+=0.03;
        B+=0.03;
        C+=0.01;

        usleep(16000);
    }

    printf("\x1b[?25h");

    return 0;
}