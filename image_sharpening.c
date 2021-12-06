#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct{
    unsigned char red,green,blue;
}PPMPixel;

typedef struct{
    int x,y;
    PPMPixel *data;
}PPMImage;

double filter[10] ={
     0,-1, 0,
    -1, 5,-1,
     0,-1, 0
};

void split_pixel(int RGB,int* blue,int* red,int* green)
{
    *blue = (RGB >> 16) & 0xFF;
    *green = (RGB >> 8) & 0xFF;
    *red = RGB & 0xFF;
}

void open_image(FILE* f){
    f = fopen("/home/1465846/Documents/Tudi-master/image_sharpening_with_cache_optimization/reindeer.ppm","rb+");
    if (f== NULL){
            perror("cannot open image");
            exit(1);
    }

    fseek(f,16,SEEK_SET); // skip image format, we already know it's P6

}

void allocate_image(PPMImage** img){
    *img = (PPMImage*)malloc(sizeof(PPMImage));
    (*img)->data = (PPMPixel*)malloc(sizeof(PPMPixel));
    if(!(*img)){
        perror("Cannot allocate enough memory for image");
    }
}

void read_image(FILE*f , PPMImage* img){
    if(fscanf(f,"%d %d",&img->x,&img->y) != 2){
        perror("Cannot read width and weight correctly ");
    }
}

int main(int argc,char *argv[]){

    FILE* f = NULL;
    PPMImage* img = NULL;
    open_image(f);
    img = (PPMImage*)malloc(sizeof(PPMImage));
    read_image(f,img);
    printf("size of image is %lu\n",sizeof(img));
    return 0;
}