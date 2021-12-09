# image_sharpening_with_cache_optimization

Applying a kernel to an image and optimize the result using cache memory.
For the implementation we used the following function of reading a ppm file from:
* https://stackoverflow.com/a/2699908/10573740

As for the image sharpening, we used the kernel specified for sharpening :
* https://en.wikipedia.org/wiki/Kernel_(image_processing)

The implementation contains a function for computing each pixel with the kernel( excepting for the ones on the border). 
Then it's computed an array of grayscale values and into a histogram for counting values between [0,50][51,101][102,152][153,203][204,255]
for source code image_sharpening.c: 
```
gcc -Wall image_sharpening.c -o h
./h.exe <file.ppm>
```

In this function we are reading the ppm file only if it's type P6, else we generate an error.
```
static PPMImage *readPPM(const char *filename)
```


