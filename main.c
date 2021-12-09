#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
typedef struct {
     unsigned char red,green,blue;
} Pixel;

typedef struct {
     int x, y;
     Pixel *data;
} Image;


Image* read_file(const char *filename)
{
         FILE *fp;
         char buff[16];
         Image *image = (Image *)malloc(sizeof(Image));;
         int rgb;

         fp = fopen(filename, "rb");
         if (!fp) {
              perror("Cannot open file");
              exit(1);
         }

         if (!fgets(buff, sizeof(buff), fp)) { // we always read a P6 format 
              perror(filename);
              exit(1);
         }
          if (fscanf(fp, "%d %d", &image->x, &image->y) != 2) {
               perror("Wrong sizes for width and height");
               exit(1);
          }

          if (fscanf(fp, "%d", &rgb) != 1) {
               perror("Cannot read rgb component");
               exit(1);
          }

          if (rgb!= 255) {
               perror("Invalid rgb component -> should be 255");
               exit(1);
          }

          image->data = (Pixel*)malloc(image->x * image->y * sizeof(Pixel));

          //we need to get rid of spaces before reading the pixels 
          while (fgetc(fp) != '\n') ;

          if (fread(image->data, 3 * image->x, image->y, fp) != image->y) {
               perror("Cannot read width*height pixels correctly");
               exit(1);
          }

          fclose(fp);
    return image;
}

void write_file(const char *filename, Image *img)
{
    FILE *fp;
    fp = fopen(filename, "wb");
    if (!fp) {
         perror("Cannot write in ppm file");
         exit(1);
    }

    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n",img->x,img->y);
    fprintf(fp, "%d\n",255);
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
}

Pixel* compute_pixel(Pixel* buff){
    Pixel* pix = (Pixel*)malloc(sizeof(Pixel));
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

Image* apply_kernel(Image * img,Image* img2){
    int i,j;
    if(img){
        for(i = 1;i < img->y-1;i++){
            for(j = 1;j < img->x-1;j++)
            {
                Pixel* buff = (Pixel*)calloc(5,sizeof(Pixel));
                //copying the row with respect to zero values in the kernel 

                //copying the first row corresponding to the kernel
                memcpy(buff,&img->data[(i-1)*img->x + j],sizeof(Pixel));

                // copying the second row coresponding to the kernel
                memcpy(&buff[1],&img->data[i * img->x + j - 1],3 * sizeof(Pixel));
                
                // copying the third row coresponding to the kernel 
                memcpy(&buff[4],&img->data[(i+1) * img->x + j],sizeof(Pixel));
                
                // compute the middle pixel represented by poz 
                Pixel* pix = compute_pixel(buff);
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

unsigned short int* compute_grayscale_histogram(Image* img)
{

    unsigned char* grey = (unsigned char*)calloc(sizeof(char)*img->x ,img->y);
    unsigned short int* histogram = (unsigned short int*)calloc(5,sizeof(unsigned short int));
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
     // for(int i = 0;i < 5;i++){
     //      printf(" %d ",histogram[i]);
     // }
    return histogram;
}

void write_Histogram(unsigned short int* histogram,char* filename){
    FILE *fp;
    fp = fopen(filename, "w");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }
    fprintf(fp,"%d %d %d %d %d",histogram[0],histogram[1],histogram[2],histogram[3],histogram[4]);
    fclose(fp);
}

void print(Image* img,Image* img2){
     for(int i = 0;i < img->y;i++){
          for(int j = 0;j < img->x;j++){
               if(img->data[i*img->x + j].green != img2->data[i*img->x + j].green)
                    printf("\t%d :\t%d",img->data[i*img->x + j].red,img2->data[i*img->x + j].red);
                    // printf("\t[%d]:\t%d",i*img->x + j,img->data[i*img->x + j].red);
          }
          // printf("\n");
     }
     // printf("\n");
}


int main(int argc,char* argv[]){ 
    Image *image;
    Image *image2;
    Image *image3;
    if(argc != 2){
		fprintf(stderr,"Usage: %s <file_name>",argv[0]);
	}

    image = read_file(argv[1]);
    image2 = read_file(argv[1]);
    image3 = read_file("corect_output.ppm");

    image2 = apply_kernel(image,image2);

    print(image2,image3);
    unsigned short int* histogram = compute_grayscale_histogram(image2);
    
    write_Histogram(histogram,"output.txt");
    write_file("output.ppm",image2);
} 