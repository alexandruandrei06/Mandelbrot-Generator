#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "util.h"

void calculateMandelbrot(ppm_bitmap *bitmap, params *param)
{
    int iter;
    float x, x_square, y, y_square;

    /*
     * For each row and each column set real and imag parts of the complex
     * number to be used in iteration
     */
    float delta_x = (param->x_max - param->x_min) / bitmap->width;
    float delta_y = (param->y_max - param->y_min) / bitmap->height;

    float Q[bitmap->height];
    float P[bitmap->width];
    Q[0] = param->y_max;
    P[0] = param->x_min;

    for (int row = 1; row < bitmap->height; row++)
        Q[row] = Q[row - 1] - delta_y;
    for (int col = 1; col < bitmap->width; col++ )
        P[col] = P[col - 1] + delta_x;
    
    
    for(int col = 0; col < bitmap->height; col++){
        for(int row = 0; row < bitmap->width; row++){
            x = 0;
            y = 0;
            x_square = 0.0;
            y_square = 0.0;

            iter = 1;
        

            while((iter < param->iterations) && ((x_square + y_square) < 4)){
                x_square = x * x;
                y_square = y * y;
                y = 2 * x * y + Q[row];
                x = x_square - y_square + P[col];
                iter++;
            }
            
            if (col >= 0 || row >= 0 || col < bitmap->width || row < bitmap->height){
                if(col < bitmap->width && row < bitmap->height){
                    int pos = XY_TO_BYTE(col, row, bitmap->width);
                    bitmap->pixels[pos].red = (unsigned char)(sin(0.0585 * iter + 6) * 230 + 25);
                    bitmap->pixels[pos].green = (unsigned char)(sin(0.068 * iter + 63) * 230 + 25);
                    bitmap->pixels[pos].blue = (unsigned char)(sin(0.123 * iter + 44) * 230 + 25);
                }
            }

        }   
    }
}

int main(int argc, char *argv[])
{
    // alocare memorie pentru parametri
    params *param = alloc_params();

    // se citesc argumentele programului
	get_args(argc, argv, param);
    read_input_file(param);

    // calculate width and height
    int width, height;

    width = (param->x_max - param->x_min) / param->resolution;
	height = (param->y_max - param->y_min) / param->resolution;

    ppm_bitmap *bitmap = alloc_bitmap(width, height);

    FILE *img_fd = fopen(param->output_file, "wb");
    if(img_fd == NULL){
        printf("Nu s-a putut deschide fisierul de output");
        exit(1);
    }

    //Calculates Mandelbrot set
    calculateMandelbrot(bitmap, param);
 
    printf("\n%s -> SERIAL -> DONE", argv[3]);

    write_picture(img_fd, bitmap);

    // free memory
    free_bitmap(bitmap);
    fclose(img_fd);
    free_params(param);

    return 0;    
}
