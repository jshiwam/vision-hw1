#include <math.h>
#include "image.h"

float absolute(float x){
    if(x>0)
        return x;
    else
        return -x;

}

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    if(x<0)x=0;
    if(y<0)y=0;
    if(x>im.w)x=im.w-1;
    if(y>im.h)y=im.h-1;

    int pixel_location=round(x)+im.w*round(y)+im.w*im.h*c;
    return im.data[pixel_location];
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    //Create New Image
    image new_im=make_image(w,h,im.c);
    //Match up Coordinates
    //Eqn aX+b=Y finding a,b
    float a=(float)im.w/w;
    float b=-0.5+a*0.5;
    for(int c=0;c<im.c;c++){
        for(int y=0;y<h;y++){
                float new_y=a*y-b;
            for(int x=0;x<w;x++){
                float new_x=a*x-b;
                float nn_value=nn_interpolate(im,new_x,new_y,c);
                set_pixel(new_im,x,y,c,nn_value);
            }
        }
    }
    return new_im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{

    int x1=floor(x);
    int y1=floor(y);
    int x2=ceil(x);
    int y2=ceil(y);

    if(x1<0)x1=0;
    if(y1<0)y1=0;
    if(x2>im.w)x2=im.w-1;
    if(y2>im.h)y2=im.h-1;


    float v1=get_pixel(im,x1,y1,c);
    float v2=get_pixel(im,x2,y1,c);
    float v3=get_pixel(im,x1,y2,c);
    float v4=get_pixel(im,x2,y2,c);

    float d1=absolute((x-x1));
    float d2=absolute((x-x2));
    float d3=absolute((y-y1));
    float d4=absolute((y-y2));

    float q1=v1*d2+v2*d1;
    float q2=v3*d2+v4*d1;

    float q= q1*d4+q2*d3;

    return q;



}



image bilinear_resize(image im, int w, int h)
{
    // TODO
    //Create New Image
   image new_im=make_image(w,h,im.c);
    //Match up Coordinates
    //Eqn aX+b=Y finding a,b
    float a=(float)im.w/w;
    float b=-0.5+a*0.5;
    for(int c=0;c<im.c;c++){
        for(int y=0;y<h;y++){
                float new_y=a*y-b;
            for(int x=0;x<w;x++){
                float new_x=a*x-b;
                float bl_value=bilinear_interpolate(im,new_x,new_y,c);
                set_pixel(new_im,x,y,c,bl_value);
            }
        }
    }
    return new_im;

}

