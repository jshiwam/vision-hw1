from uwimg import *

#Enlarging image using nearest neighbour interpolation
im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-nn")

#Enlarging image using bilinear interpolation
a = bilinear_resize(im,im.w*4,im.h*4)
save_image(a,"dog4x-bl")

#Reducing image using bilinear interpolation
a = bilinear_resize(im,im.w//7,im.h//7)
save_image(a,"dog7th-bl")

#box_filter
f=make_box_filter(7)
blur=convolve_image(im,f,1)
save_image(blur,"dog-box7")

#high pass filter
f=make_highpass_filter()
blur=convolve_image(im,f,1)
save_image(blur,"dog-highpass")

#gaussian filter
im=load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss2")

#low freq,high freq and its reconstruct
im=load_image("data/dog.jpg")
f = make_gaussian_filter(2)
lfreq = convolve_image(im, f, 1)
hfreq = sub_image(im,lfreq)
reconstruct= add_image(lfreq,hfreq)
save_image(lfreq,"low-frequency")
save_image(hfreq,"high-frequency")
save_image(reconstruct,"reconstruct")

#sobel image
res=sobel_image(im)
mag=res[0]
feature_normalize(mag)
save_image(mag,"magnitude")

#sobel color
res=colorize_sobel(im)

rgb_to_hsv(res)

shift_image(res,1,1)

hsv_to_rgb(res)

save_image(res,"color_sobel")


