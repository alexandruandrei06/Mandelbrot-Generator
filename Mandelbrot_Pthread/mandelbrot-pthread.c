#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "util.h"

void calculateMandelbrot(ppm_bitmap *bitmap, params *param, int id, float *Q, float *P)
{
    int iter;
    float x, x_square, y, y_square;

    /*
     * For each row and each column set real and imag parts of the complex
     * number to be used in iteration
     */

    int col_start = id * (double)bitmap->height / param->threads_number;
    int col_end = MIN((id + 1) * (double)bitmap->height / param->threads_number, bitmap->height);
    
    for(int col = col_start; col < col_end; col++){
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

typedef struct thread_args {
    params *param;
    ppm_bitmap *bitmap;
    int id;
    float *Q, *P;
} thread_args;


void *thread_function(void *arg){
    struct thread_args *args = (struct thread_args *) arg;

    calculateMandelbrot(args->bitmap, args->param, args->id, args->Q, args->P);

    return NULL;
}

void generate_Q_P(float *Q, float *P, params *param, ppm_bitmap *bitmap){
    Q[0] = param->y_max;
    P[0] = param->x_min;

    float delta_x = (param->x_max - param->x_min) / bitmap->width;
    float delta_y = (param->y_max - param->y_min) / bitmap->height;

    for (int row = 1; row < bitmap->height; row++)
        Q[row] = Q[row - 1] - delta_y;
    for (int col = 1; col < bitmap->width; col++ )
        P[col] = P[col - 1] + delta_x;
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

    float *Q = (float *)calloc(bitmap->height, sizeof(float));
    if (Q == NULL) {
		printf("Eroare la alocare!\n");
		exit(1);
	}

    float *P = (float *)calloc(bitmap->height, sizeof(float));
    if (P == NULL) {
		printf("Eroare la alocare!\n");
		exit(1);
	}

    generate_Q_P(Q, P, param, bitmap);

    pthread_t threads[param->threads_number];
    int r;
    void *status;

    thread_args **args = (thread_args **)calloc(param->threads_number, sizeof(thread_args *));
    if (args == NULL) {
		printf("Eroare la alocare\n");
		exit(1);
	}

    for(int id = 0; id < param->threads_number; id++){
        args[id] = (thread_args *)calloc(1, sizeof(thread_args));
        if (args[id] == NULL) {
            printf("Eroare la alocare\n");
            exit(1);
        }

        args[id]->id = id;
        args[id]->bitmap = bitmap;
        args[id]->param = param;
        args[id]->Q = Q;
        args[id]->P = P;

        r = pthread_create(&threads[id], NULL, thread_function, args[id]);
        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            exit(-1);
        }
    }

    for (int id = 0; id < param->threads_number; id++) {
        r = pthread_join(threads[id], &status);

        free(args[id]);
        if (r) {
            printf("Eroare la asteptarea thread-ului %d\n", id);
            exit(-1);
        }
    }

    printf("\n%s -> PTHREADS -> DONE", argv[4]);

    FILE *img_fd = fopen(param->output_file, "wb");
    if(img_fd == NULL){
        printf("Nu s-a putut deschide fisierul de output");
        exit(1);
    }

    write_picture(img_fd, bitmap);

    // free memory
    free_bitmap(bitmap);
    fclose(img_fd);
    free_params(param);
    free(args);
    free(Q);
    free(P);
    return 0;    
}
