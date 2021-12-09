#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
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
         int c, rgb_comp_color;
         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }

         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
              exit(1);
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

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
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

PPMPixel* compute_pixel(PPMPixel* buff){
    PPMPixel* pix = (PPMPixel*)malloc(sizeof(PPMPixel));
    int a = ((int)buff[0].blue * (-1));
    int b = ((int)buff[1].blue * (-1));
    int c = ((int)buff[2].blue * (5));
    int d = ((int)buff[3].blue * (-1));
    int e = ((int)buff[4].blue * (-1));
    int blue = a + b + c + d + e; 
    blue = (blue > 255)? 255: blue;
    blue = (blue < 0)? 0 : blue;
    pix->blue = blue;

    a = ((int)buff[0].green * (-1));
    b = ((int)buff[1].green * (-1));
    c = ((int)buff[2].green * (5));
    d = ((int)buff[3].green * (-1));
    e = ((int)buff[4].green * (-1));
    int green = a + b + c + d + e; 
    green = (green > 255) ? 255: green;
    green = (green < 0)? 0 : green;
    pix->green = green;

    
    a = ((int)buff[0].red * (-1));
    b = ((int)buff[1].red * (-1));
    c = ((int)buff[2].red * (5));
    d = ((int)buff[3].red * (-1));
    e = ((int)buff[4].red * (-1));
    int red = a + b + c + d + e; 
    red = (red > 255) ? 255: red;
    red = (red < 0) ? 0 : red;
    pix->red = red;
    return pix;

}

PPMImage* applyKernel(PPMImage * img,PPMImage* img2){
    int i,j;
    if(img){
        for(i = 1;i < img->y-1;i++){
            for(j = 1;j < img->x-1;j++)
            {
                PPMPixel* buff = (PPMPixel*)calloc(5,sizeof(PPMPixel));
                //copying the row with respect to zero values in the kernel 

                //copying the first row corresponding to the kernel
                memcpy(buff,&img->data[(i-1)*img->x + j],sizeof(PPMPixel));

                // copying the second row coresponding to the kernel
                memcpy(&buff[1],&img->data[i * img->x + j - 1],3 * sizeof(PPMPixel));
                
                // copying the third row coresponding to the kernel 
                memcpy(&buff[4],&img->data[(i+1) * img->x + j],sizeof(PPMPixel));
                
                // compute the middle pixel represented by poz 
                PPMPixel* pix = compute_pixel(buff);
                int poz = i * img->x + j;
                img2->data[poz].red = pix->red;
                img2->data[poz].green = pix->green;
                img2->data[poz].blue = pix->blue;

            }
        }
        return img2;
    }
    return NULL;
}

unsigned short int* compute_grayscale_histogram(PPMImage* img)
{

    unsigned char* grey = (char*)calloc(sizeof(char)*img->x ,img->y);
    unsigned short int* histogram = (int*)calloc(5,sizeof(unsigned short int));
    int i;
    if(img){
        for(i = 0;i < img->x*img->y;i++){
            //round(0.2126*R + 0.7152*G + 0.0722*B)
            grey[i] = round( (double)(img->data[i].blue * 0.0722) + (double)(img->data[i].green *0.7152) + (double)(img->data[i].red * 0.2126)); 
            if(grey[i] < 51) histogram[0]++;
            else if(grey[i] < 102) histogram[1]++;
            else if(grey[i] < 153) histogram[2]++;
            else if(grey[i] < 204) histogram[3]++;
            else if(grey[i] < 256) histogram[4]++;
            
        }
    }
     for(int i = 0;i < 5;i++){
          printf(" %d ",histogram[i]);
     }
    return histogram;
}

void writeHistogram(unsigned short int* histogram,char* filename){
    FILE *fp;
    fp = fopen(filename, "w");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }
    fprintf(fp,"%d %d %d %d %d",histogram[0],histogram[1],histogram[2],histogram[3],histogram[4]);
    fclose(fp);
}

void print(PPMImage* img){
     for(int i = 0;i < 5;i++){
          for(int j = 0;j < 5;j++){
                    printf("\t%d",img->data[i*img->x + j].red);
          }
          printf("\n");
     }
     printf("\n");
}

int main(int argc,char** argv){
    PPMImage *image;
    PPMImage *image2;
    if(argc != 2){
		fprintf(stderr,"Usage: %s <file_name>",argv[0]);
	}

    image = readPPM(argv[1]);
    image2 = readPPM(argv[1]);
    
    image2 = applyKernel(image,image2);
    unsigned short int* histogram = compute_grayscale_histogram(image2);
    
    writeHistogram(histogram,"output.txt");
    writePPM("output.ppm",image2);
} 