#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    //Checking boundry conditions
    if(x<0)x=0;
    if(y<0)y=0;
    if(x>im.w)x=im.w-1;
    if(y>im.h)y=im.h-1;
    // TODO Fill this in
    int pixel_location=x+y*im.w+c*im.w*im.h;

    return im.data[pixel_location];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if(x<0 || y<0 || x>im.w || y>im.h)
    {
        return;
    }else{
    // TODO Fill this in
    int pixel_location=x+y*im.w+c*im.w*im.h;
    float *pixel_value=&im.data[pixel_location];
    *pixel_value=v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    //Using get_pixel and set_pixel
    /*for(int c=0;c<im.c;c++){
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                float v=get_pixel(im,x,y,c);
                set_pixel(copy,x,y,c,v);
            }
        }
    }*/

    //Using memcpy
    memcpy(&copy,&im,sizeof(im));

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    int i=1;
    image gray = make_image(im.w,im.h,i);
    // TODO Fill this in
    //Noob's(Brute Force) Approach
    /*for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
                float v=(0.299*get_pixel(im,x,y,0)+0.587*get_pixel(im,x,y,1)+0.114*get_pixel(im,x,y,2));
                set_pixel(gray,x,y,(i-1),v);
            }
        }*/


        //Creating a pointer to struct(gray image,image)
        image *ptr_gray, *ptr_im;
        ptr_gray=&gray;
        //float *gray_data=ptr_gray->data;
        float (*gray_data_matrix)[im.w][im.h]=ptr_gray->data;
        //Given Image
        ptr_im=&im;
        //float * im_data=ptr_im->data;
        float (*im_data_matrix)[im.w][im.h]=ptr_im->data;

        //Define the data member of gray to be that of im:
        ptr_gray->h=ptr_im->h;
        ptr_gray->w=ptr_im->w;

        float value;
        for(int c=0;c<im.c;c++){
            if(c==0){value=0.299;}
            else if(c==1){value=0.587;}
            else if(c==2){value=0.144;}
            for(int width=0;width<gray.w;width++){
                for(int height=0;height<gray.h;height++){
                        (*gray_data_matrix)[width][height]+=(*(*(*(im_data_matrix+c)+width)+height))*value;
            }
        }





        }

    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in

    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
                float shift=get_pixel(im,x,y,c)+v;
                if(shift>1){
                    shift=1;
                }else if(shift<0){
                    shift=0;
                }
                set_pixel(im,x,y,c,shift);

        }
    }

}

void clamp_image(image im)
{
    // TODO Fill this in
    /*
    for(int c=0;c<im.c;c++){
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            //float pix
        }
    }
    }
    */
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float R,G,B,V,m,C,S,H_dash,H;
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                R=get_pixel(im,x,y,0);
                G=get_pixel(im,x,y,1);
                B=get_pixel(im,x,y,2);
                V=three_way_max(R,G,B);
                m=three_way_min(R,G,B);
                C=V-m;
                if(V==0){
                    S=0;
                }else{
                    S=C/V;
                }
                if(C==0){
                    H=0;
                }else{
                if(V==R){
                    H_dash=(G-B)/C;
                }else if(V==G){
                H_dash=((B-R)/C)+2;
                }else if(V==B){
                H_dash=((R-G)/C)+4;
                }
                }

                if(H_dash<0){
                    H = (H_dash/6)+1;
                }else{
                    H=(H_dash/6);
                }

            set_pixel(im,x,y,0,H);
            set_pixel(im,x,y,1,S);
            set_pixel(im,x,y,2,V);
            }

    }
}
void scale_image(image im,int c,float v){
for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
                float scale=get_pixel(im,x,y,c)*v;
                if(scale>1){
                    scale=1;
                }else if(scale<0){
                    scale=0;
                }
                set_pixel(im,x,y,c,scale);

        }
    }

}
void hsv_to_rgb(image im)
{
    // TODO Fill this in
    //X is the intermediate value for the second largest component of the color
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
             float R1=0,B1=0,G1=0,R=0,G=0,B=0;

            float H=get_pixel(im,x,y,0)*360;
            float S=get_pixel(im,x,y,1);
            float V=get_pixel(im,x,y,2);


                float C=S*V;
                float H_dash=H/60;
                float X=C*(1-abs((fmod(H_dash,2)-1)));

                if(H_dash<=0){
                    R1=0;G1=0;B1=0;
                }
                else if(H_dash>0 && H_dash<=1){
                    R1=C;
                    G1=X;
                    B1=0;
                }

                else if(H_dash>1 && H_dash<=2){
                    R1=X;
                    G1=C;
                    B1=0;
                }


                else if(H_dash>2 && H_dash<=3){
                    R1=0;
                    G1=C;
                    B1=X;
                }

                else if(H_dash>3 && H_dash<=4){
                    R1=0;
                    G1=X;
                    B1=C;
                }

                else if(H_dash>4 && H_dash<=5){
                    R1=X;
                    G1=0;
                    B1=C;
                }
                else if(H_dash>5 && H_dash<=6){
                    R1=C;
                    G1=0;
                    B1=C;
                }

                float m=V-C;
                R=R1+m;
                G=G1+m;
                B=B1+m;


            set_pixel(im,x,y,0,R);
            set_pixel(im,x,y,1,G);
            set_pixel(im,x,y,2,B);

        }
    }
}


