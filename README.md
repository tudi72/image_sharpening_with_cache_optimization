# image_sharpening_with_cache_optimization

Applying a kernel to an image and optimize the result using cache memory.
For the implementation we used the following function of reading a ppm file from:
* https://stackoverflow.com/a/2699908/10573740

As for the image sharpening, we used the kernel specified for sharpening :
* https://en.wikipedia.org/wiki/Kernel_(image_processing)

The function for applying the kernel is making a gaussian sum of each 9 elements and makes a matrix convolution with the image;

for source code image_sharpening.c: 
```
gcc -Wall image_sharpening.c -o h
./h.exe
```
