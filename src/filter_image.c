#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853
//Normalization:
/*
Image normalization is a typical process in image processing that changes the range of pixel
intensity values. Its normal purpose is to convert an input image into a range of pixel values that
are more familiar or normal to the senses, hence the term normalization.
*/
void l1_normalize(image im)
{
    // Normalize an image to sum to 1;
    float sum=0;
    for(int c=0;c<im.c;c++){
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                int pix_loc=x+y*im.w+im.w*im.h*c;
                sum+=im.data[pix_loc];
            }
        }
    }

    for(int c=0;c<im.c;c++){
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                int pix_loc=x+y*im.w+im.w*im.h*c;
                im.data[pix_loc]=im.data[pix_loc]/sum;
            }
        }
    }

}

image make_box_filter(int w)
{
    // Box filter is an square image with one channel with uniform entries that sum to 1.
    image box_filter=make_image(w,w,1);
    for(int c=0;c<box_filter.c;c++){
        for(int y=0;y<box_filter.h;y++){
            for(int x=0;x<box_filter.w;x++){
                set_pixel(box_filter,x,y,c,1.0);
            }
        }
    }
    l1_normalize(box_filter);
    return box_filter;
}
float box_filter_result(image im,image filter,int imx,int imy,int c,int preserve){
    float q=0;
    int f_c;

    if(filter.c==1)f_c=0;
    else f_c=c;

    for(int y=0;y<filter.h;y++){
        for(int x=0;x<filter.w;x++){
            float fil_pix=get_pixel(filter,x,y,f_c);
            int fx = imx+x-filter.w/2;
            int fy = imy+y-filter.h/2;
            float im_pix=get_pixel(im,fx,fy,c);
            q+=im_pix*fil_pix;
        }
    }
    return q;

}
image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c==im.c || filter.c==1);
    image f;
    if(preserve){
        f=make_image(im.w,im.h,im.c);
        for(int c=0;c<im.c;c++){
            for(int y=0;y<im.h;y++){
                for(int x=0;x<im.w;x++){
                    set_pixel(f,x,y,c,box_filter_result(im,filter,x,y,c,preserve));
                }
            }
        }
    }else{
    f=make_image(im.w,im.h,1);
    for(int y=0;y<im.h;y++){
                for(int x=0;x<im.w;x++){
                    set_pixel(f,x,y,0,box_filter_result(im,filter,x,y,0,preserve));
                }
            }
    }
    return f;
}

image make_highpass_filter()
{
    /*A high-pass filter can be used to make an image appear sharper. These filters emphasize fine details in the image */
    image high_pass=make_image(3,3,1);

    //Fill the zeroes
    set_pixel(high_pass,0,0,0,0);
    set_pixel(high_pass,0,2,0,0);
    set_pixel(high_pass,2,0,0,0);
    set_pixel(high_pass,2,2,0,0);

    //Fill the -1
    set_pixel(high_pass,0,1,0,-1);
    set_pixel(high_pass,1,0,0,-1);
    set_pixel(high_pass,1,2,0,-1);
    set_pixel(high_pass,2,1,0,-1);

    //Fill the 4
    set_pixel(high_pass,1,1,0,4);
    return high_pass;
}

image make_sharpen_filter()
{
    // TODO
    image sharpen=make_image(3,3,1);

    //Fill the zeroes
    set_pixel(sharpen,0,0,0,0);
    set_pixel(sharpen,0,2,0,0);
    set_pixel(sharpen,2,0,0,0);
    set_pixel(sharpen,2,2,0,0);

    //Fill the -1
    set_pixel(sharpen,0,1,0,-1);
    set_pixel(sharpen,1,0,0,-1);
    set_pixel(sharpen,1,2,0,-1);
    set_pixel(sharpen,2,1,0,-1);

    //Fill the 5
    set_pixel(sharpen,1,1,0,5);
    return sharpen;
}

image make_emboss_filter()
{
    // TODO
    image emboss=make_image(3,3,1);

    //Fill the zeroes
    set_pixel(emboss,0,0,0,-2);
    set_pixel(emboss,0,2,0,0);
    set_pixel(emboss,2,0,0,0);
    set_pixel(emboss,2,2,0,2);

    //Fill the -1
    set_pixel(emboss,0,1,0,-1);
    set_pixel(emboss,1,0,0,-1);
    set_pixel(emboss,1,2,0,1);
    set_pixel(emboss,2,1,0,1);

    //Fill the 4
    set_pixel(emboss,1,1,0,1);
    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // Gaussian filter
    int size=6*sigma;
    size=(size%2==0) ? size+1:size;

    image gaussian = make_image(size,size,1);

    for(int row=0;row<gaussian.h;row++){
        for(int col=0;col<gaussian.w;col++){
            int x=col-gaussian.w/2;
            int y=row-gaussian.h/2;
            float pixel=((1/(TWOPI*sigma*sigma))*exp(-(pow(x,2)+pow(y,2))/(2*pow(sigma,2))));
            set_pixel(gaussian,col,row,0,pixel);
        }
    }
    l1_normalize(gaussian);
    return gaussian;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w==b.w && a.h==b.h && a.c==b.c);
    image result=make_image(a.w,a.h,a.c);
    for(int channel=0;channel<a.c;channel++){
        for(int row=0;row<a.h;row++){
            for(int col=0;col<a.w;col++){
                float pixel=get_pixel(a,col,row,channel)+get_pixel(b,col,row,channel);
                set_pixel(result,col,row,channel,pixel);
            }
        }
    }

    return result;
}

image sub_image(image a, image b)
{
    // TODO


    assert(a.w==b.w && a.h==b.h && a.c==b.c);
    image result=make_image(a.w,a.h,a.c);
    for(int channel=0;channel<a.c;channel++){
        for(int row=0;row<a.h;row++){
            for(int col=0;col<a.w;col++){
                float pixel=get_pixel(a,col,row,channel)-get_pixel(b,col,row,channel);
                set_pixel(result,col,row,channel,pixel);
            }
        }
    }

    return result;

}

image make_gx_filter()
{
    // TODO
        /*A high-pass filter can be used to make an image appear sharper. These filters emphasize fine details in the image */
    image gx_filter=make_image(3,3,1);


    set_pixel(gx_filter,0,0,0,-1);
    set_pixel(gx_filter,0,2,0,1);
    set_pixel(gx_filter,2,0,0,-1);
    set_pixel(gx_filter,2,2,0,1);


    set_pixel(gx_filter,0,1,0,0);
    set_pixel(gx_filter,1,0,0,-2);
    set_pixel(gx_filter,1,2,0,2);
    set_pixel(gx_filter,2,1,0,0);


    set_pixel(gx_filter,1,1,0,0);
    return gx_filter;
}

image make_gy_filter()
{
    // TODO
    image gy_filter=make_image(3,3,1);


    set_pixel(gy_filter,0,0,0,-1);
    set_pixel(gy_filter,0,1,0,-2);
    set_pixel(gy_filter,0,2,0,-1);

    set_pixel(gy_filter,2,0,0,1);
    set_pixel(gy_filter,2,1,0,2);
    set_pixel(gy_filter,2,2,0,1);

    set_pixel(gy_filter,1,0,0,0);
    set_pixel(gy_filter,1,1,0,0);
    set_pixel(gy_filter,1,2,0,0);




    return gy_filter;
}
//Feature scaling is a method used to normalize the range of independent variables or features of data.
void feature_normalize(image im)
{
    // TODO
    float pix;
    float range,min=im.data[0],max=im.data[0];
    for(int c=0;c<im.c;c++){
        for(int row=0;row<im.h;row++){
            for(int col=0;col<im.w;col++){
                int pix_loc=col+row*im.w+c*im.w*im.h;
                float pix_val=im.data[pix_loc];
                if(pix_val<min){
                    min=pix_val;
                }
                if(pix_val>max){
                    max=pix_val;
                }
            }
        }
    }
    range=max-min;
    for(int c=0;c<im.c;c++){
        for(int row=0;row<im.h;row++){
            for(int col=0;col<im.w;col++){
                if(range>0){
                pix=(get_pixel(im,col,row,c)-min)/range;}
                else{
                pix=0;
                }
                set_pixel(im,col,row,c,pix);
            }
        }
    }
}
// return type function name()
// int a;
//it creates an image emphasising edges.
image *sobel_image(image im)
{
    // TODO
    image *return_pointer = calloc(2,sizeof(image));// creates two objects of the size image

    return_pointer[0] = make_image(im.w,im.h,1);
    return_pointer[1] = make_image(im.w,im.h,1);

    image gx_filter=make_gx_filter();
    image gy_filter=make_gy_filter();

    image gx=convolve_image(im,gx_filter,0);
    image gy=convolve_image(im,gy_filter,0);


        for(int row=0;row<im.h;row++){
            for(int col=0;col<im.w;col++){
                    //\mathbf {G} ={\sqrt {{\mathbf {G} _{x}}^{2}+{\mathbf {G} _{y}}^{2}}}
                float gx_sq=pow(get_pixel(gx,col,row,0),2);
                float gy_sq=pow(get_pixel(gy,col,row,0),2);
                float magval=pow((gx_sq+gy_sq),0.5);
                set_pixel(return_pointer[0],col,row,0,magval);

                float dirval=atan(get_pixel(gy,col,row,0)/get_pixel(gx,col,row,0));
                set_pixel(return_pointer[1],col,row,0,dirval);
            }
        }


    return return_pointer;
}

image colorize_sobel(image im)
{
    // TODO
    image color_img = make_image(im.w,im.h,im.c);

    image gx_filter=make_gx_filter();
    image gy_filter=make_gy_filter();

    image gx=convolve_image(im,gx_filter,1);
    image gy=convolve_image(im,gy_filter,1);

        for(int c=0;c<im.c;c++){
        for(int row=0;row<im.h;row++){
            for(int col=0;col<im.w;col++){
                    //\mathbf {G} ={\sqrt {{\mathbf {G} _{x}}^{2}+{\mathbf {G} _{y}}^{2}}}
                float gx_sq=pow(get_pixel(gx,col,row,c),2);
                float gy_sq=pow(get_pixel(gy,col,row,c),2);
                float magval=pow((gx_sq+gy_sq),0.5);

                float dirval=atan(get_pixel(gy,col,row,c)/get_pixel(gx,col,row,c));
                set_pixel(color_img,col,row,c,magval);
            }
        }
        }
    return color_img;
}
