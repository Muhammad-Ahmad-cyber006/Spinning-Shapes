#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int screenW=67;// number of columns
int screenH=55;// number of rows
float rotX=0;// rotation around X axis
float rotY=0;// rotation around Y axis
float rotZ=0;// rotation around Z axis

float tipHeight=10;   
float baseHalf=10;   
float depth[67*55];
char  pixels[67*55];
char  output[67*55+55+1]; // final string sent to terminal


float camDist=60;//how far the pyramid is from the camera
float zoom=28;//controls how zoomed in the projection looks
float step=0.04;// barycentric step size

void drawPoint(float px,float py,float pz,char ch)
{

    // rotX spins around X, rotY around Y, rotZ around Z.
    float rx =px * cos(rotY) * cos(rotZ)+ py * 
                (sin(rotX) * sin(rotY) * cos(rotZ) - cos(rotX) * sin(rotZ))
               +pz * (cos(rotX) * sin(rotY) * cos(rotZ) + sin(rotX) * sin(rotZ));

    float ry = px * cos(rotY) * sin(rotZ)+ py* 
                (sin(rotX) * sin(rotY) * sin(rotZ) + cos(rotX) * cos(rotZ))
               + pz * (cos(rotX) * sin(rotY) * sin(rotZ) - sin(rotX) * cos(rotZ));

    float rz = -px * sin(rotY)+ py * sin(rotX) * cos(rotY)
               +pz * cos(rotX) * cos(rotY);

    float z=rz+camDist;

    //Perspective divide
    float inv_z=1.0f/z;

    int sx=(int)(screenW/2+ zoom*inv_z*rx*2);
    int sy=(int)(screenH/2+zoom *inv_z*ry);

    //Skip if the pixel is outside the screen
    if (sx<0||sx>=screenW||sy<0||sy>=screenH)
    {
        return;
    }

    //Find the flat array index for this (col,row) position
    int i=sx+sy*screenW;

    if (inv_z>depth[i])
    {
        depth[i]=inv_z;
        pixels[i]=ch;
    }
}

void drawTriangle(float ax, float ay, float az,float bx, float by, float bz,float cx, float cy, float cz,char ch)
{
    float t=0;
    while (t<=1.0f)
    {
        float s=0;
        while (s<=1.0f-t)
        {
            // u is the remaining weight so all three sum to 1
            float u=1.0f-t-s; // orginal equation is u + t + s = 1 //This sweeps every point inside the triangle

            //barycentric coordinates
            float px=u*ax+t*bx+s*cx;
            float py=u*ay+t*by+s*cy;
            float pz=u*az+t*bz+s*cz;

            drawPoint(px,py,pz,ch);

            s=s+step;
        }
        t+=step;
    }
}

int main()
{
    printf("\x1b[?25l");
    printf("\x1b[2J");

    float t_x= 0,        
    t_y =-tipHeight,  
    t_z=0; // tip 
    float fr_x=baseHalf,  fr_y=baseHalf,  fr_z=-baseHalf; // front-right
    float fl_x=-baseHalf, fl_y=baseHalf,  fl_z=-baseHalf; // front-left
    float br_x=baseHalf,  br_y=baseHalf,  br_z=baseHalf; // back-right
    float bl_x=-baseHalf, bl_y=baseHalf,  bl_z=baseHalf; // back-left

    while (1)
    {
        memset(pixels,' ',screenW*screenH); // filling screen with spaces
        memset(depth,0, sizeof(depth));

        // Front face=tip  front-right  front-left
        drawTriangle(t_x,  t_y,  t_z,fr_x, fr_y, fr_z,fl_x, fl_y, fl_z,'#');

        // Right face=tip  back-right  front-right
        drawTriangle(t_x,  t_y,  t_z,br_x, br_y, br_z,fr_x, fr_y, fr_z,'@');

        // Back face=tip  back-left  back-right
        drawTriangle(t_x,  t_y,  t_z,bl_x, bl_y, bl_z,br_x, br_y, br_z,'$');

        // Left face=tip  front-left  back-left
        drawTriangle(t_x,  t_y,  t_z,fl_x, fl_y, fl_z,bl_x, bl_y, bl_z,'.');

        // A square cannot be drawn as one triangle so we split it diagonally
        // Base triangle 1 = front-right  back-right  back-left
        drawTriangle(fr_x, fr_y, fr_z,br_x, br_y, br_z,bl_x, bl_y, bl_z,'-');

        // Base triangle 2 = front-right  back-left  front-left
        drawTriangle(fr_x, fr_y, fr_z,bl_x, bl_y, bl_z,fl_x, fl_y, fl_z,'-');



        int p=0;
        int row=0;
        while (row<screenH)
        {
            int col=0;
            while (col<screenW)
            {
                output[p]=pixels[col+row*screenW];
                p++;
                col++;
            }
            output[p]='\n';
            p++;
            row++;
        }
        output[p]='\0';

        // here "\x1b[H" moves the cursor back to the top-left corner before printing the frame
        printf("\x1b[H%s", output);
        fflush(stdout);// forcing the terminal to display it in instant

        // rotation angles for next frame
        rotX+=0.03f;
        rotY+=0.02f;
        rotZ+=0.01f;


        usleep(15000);
    }

    //Restoring the cursor
    printf("\x1b[?25h");

    return 0;
}