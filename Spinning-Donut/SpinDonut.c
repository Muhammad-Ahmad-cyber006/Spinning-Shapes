#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
int main()
{
    float A=0,B=0;  // rotation angle
    char  pixels[1760];
    float depth[1760];

    printf("\x1b[2J\x1b[?25l"); 
    while(1)
    {
        memset(pixels,' ',1760);
        memset(depth,0,sizeof(depth));

        float sinA=sin(A);
        float cosA=cos(A);
        float sinB=sin(B); 
        float cosB=cos(B);

        float j=0;
        //torus parametric surface 2 angles i,j sweep every point on donut
        while (j<6.28)//j sweeps around the big donut ring
        {
            float sinJ=sin(j), cosJ = cos(j);

            float i = 0;
            while (i < 6.28)//i sweeps around the tube cross-section
            {
                float sinI = sin(i), cosI = cos(i);

                // torus point:h is distance from tube center to donut center
                // +2 =bigger hole in the donut
                float h=cosJ+2;

                // perspective divide
                float invZ =1.0/(sinI*h*sinA+sinJ*cosA+
                                                        5);//distance cam

                float t=sinI*h*cosA-sinJ*sinA;

                // project onto screen
                int sx=(int)(40+30*invZ*(cosI * h * cosB - t * sinB));
                int sy=(int)(12+15*invZ*(cosI * h * sinB + t * cosB));
                int idx=sx+80*sy;

                // lambertian lighting->dot product of normal with light direction
                int N=(int)(8*((sinJ *sinA - sinI * cosJ *cosA) 
                * cosB- sinI * cosJ *sinA- sinJ *cosA- cosI * cosJ*sinB));

                if (sy>0&&sy<22&&sx>0&&sx<80&&invZ>depth[idx])
                {
                    depth[idx]=invZ;
                    pixels[idx]=".,_~:;=!*#$@"[N>0 ?N:0];
                }

                i+=0.02;
            }
            j+=0.07;
        }
        printf("\x1b[H");

        int k=0;
        while (k<1760)
        {
            putchar(k%80 ? pixels[k]:'\n');
            k++;
        }
        fflush(stdout);
        // spin speed
        A += 0.17f;
        B += 0.07f;

        usleep(16000);
    }

    printf("\x1b[?25h");
    return 0;
}