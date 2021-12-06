#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

int main(){
    PPMPixel buff[9] = {{1,1,1},{2,2,2},{3,4,5},
    {6,7,8},{9,10,11},{12,13,14},
    {15,61,17},{18,19,1},{1,1,1}};
    PPMPixel buff2[9];

    memcpy(buff2,buff,3 * sizeof(PPMPixel));
    memcpy(&buff2[3],&buff[3],3 * sizeof(PPMPixel));
    memcpy(&buff2[6],&buff[6],3 * sizeof(PPMPixel));

    for(int i = 0; i< 9;i++){
        printf("{%d,%d,%d} ",buff2[i].red,buff2[i].green,buff2[i].blue);
    }
}