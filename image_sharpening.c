<<<<<<< HEAD
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

#define RGB_COMPONENT_COLOR 255
static PPMImage *readPPM(const char *filename)
{
         char buff[16];
         PPMImage *img;
         FILE *fp;
         int  rgb_comp_color;
         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }

         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
         }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(1);
    }
    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp);
    return img;
}

void writePPM(const char *filename, PPMImage *img)
{
    FILE *fp;
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }

    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n",img->x,img->y);
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}

void applyKernel(PPMImage * img,PPMImage* img2){
    int i,j;
    if(img){
        for(i = 0;i < img->x-2;i++){
            for(j = 0;j < img->y-2;j++)
            {
                int rand = i * img->y;
                int poz = rand + j + 1;
                PPMPixel buff[10];
                memcpy(buff,&img->data[rand + j],3 * sizeof(PPMPixel));
                rand = rand + img->y;
                memcpy(&buff[3],&img->data[rand + j],3 * sizeof(PPMPixel));
                rand = rand + img->y;
                memcpy(&buff[6],&img->data[rand + j],3 * sizeof(PPMPixel));

                img2->data[poz].blue = buff[1].blue * (-1) + buff[3].blue * (-1) + buff[4].blue * 5 + buff[5].blue * (-1) +  buff[7].blue * (-1);
                //img2->data[poz].blue = img->data[poz].blue > 255 ? 255: (img->data[poz].blue < 0 ? 0 : img->data[poz].blue);
                
                img2->data[poz].green = buff[1].green * (-1) + buff[3].green * (-1) + buff[4].green * 5 + buff[5].green * (-1) +  buff[7].green * (-1);
               // img2->data[poz].green = img->data[poz].green > 255 ? 255: (img->data[poz].green < 0 ? 0 : img->data[poz].green);
                
                img2->data[poz].red = buff[1].red * (-1) + buff[3].red * (-1) + buff[4].red * 5 + buff[5].red * (-1) +  buff[7].red * (-1);
               // img2->data[poz].red = img->data[poz].red > 255 ? 255: (img->data[poz].red < 0 ? 0 : img->data[poz].red);
            }
        }
    }
}

int main(){
    PPMImage *image;
    PPMImage *image2;
    image = readPPM("lazar.ppm");
    image2 = readPPM("lazar.ppm");
    applyKernel(image,image2);
    writePPM("lazarand.ppm",image2);
    printf("Press any key...");
    getchar();
=======
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
>>>>>>> aeb15174d226b10eca9251bc39f111f8180fac92
}