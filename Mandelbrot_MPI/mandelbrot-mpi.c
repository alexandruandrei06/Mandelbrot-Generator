#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include "util.h"

typedef struct input_data {
	int iterations;
	double x_min, x_max, y_min, y_max;
    int width;
    int height;
} input_data;

/* Functie pentru alocarea memoriei pentru transmiterea 
   datelor de input catre toate procesele
*/
input_data *alloc_input_data(int iterations, double x_min, double x_max, 
                             double y_min, double y_max, int width, int height){
    input_data *in_data = (input_data *)calloc(1, sizeof(input_data));
    if(in_data == NULL){
        printf("Eroare la alocare\n");
        exit(1);
    }

    in_data->iterations = iterations;
    in_data->x_min = x_min;
    in_data->x_max = x_max;
    in_data->y_max = y_max;
    in_data->y_min = y_min;
    in_data->width = width;
    in_data->height = height;

    return in_data;
}

// Functie pentru dealocarea memoriei folosita de in_data
void free_input_data(input_data *in_data){
    free(in_data);
}

void calculateMandelbrot(ppm_color_t *pixels, input_data in_data, int col_start, int col_end)
{
    int iter;
    float x, x_square, y, y_square;

    /*
     * For each row and each column set real and imag parts of the complex
     * number to be used in iteration
     */
    float delta_x = (in_data.x_max - in_data.x_min) / in_data.width;
    float delta_y = (in_data.y_max - in_data.y_min) / in_data.height;

    float Q[in_data.height];
    float P[in_data.width];
    Q[0] = in_data.y_max;
    P[0] = in_data.x_min;

    for (int row = 1; row < in_data.height; row++)
        Q[row] = Q[row - 1] - delta_y;
    for (int col = 1; col < in_data.width; col++ )
        P[col] = P[col - 1] + delta_x;
    
    int i, j;
    i = 0;
    for(int col = col_start; col < col_end; col++){
        j = 0;
        for(int row = 0; row < in_data.width; row++){
            x = 0;
            y = 0;
            x_square = 0.0;
            y_square = 0.0;

            iter = 1;
        

            while((iter < in_data.iterations) && ((x_square + y_square) < 4)){
                x_square = x * x;
                y_square = y * y;
                y = 2 * x * y + Q[row];
                x = x_square - y_square + P[col];
                iter++;
            }
            
            if (col >= 0 || row >= 0 || col < in_data.width || row < in_data.height){
                if(col < in_data.width && row < in_data.height){
                    int pos = i * in_data.width + j;
                    pixels[pos].red = (unsigned char)(sin(0.0585 * iter + 6) * 230 + 25);
                    pixels[pos].green = (unsigned char)(sin(0.068 * iter + 63) * 230 + 25);
                    pixels[pos].blue = (unsigned char)(sin(0.123 * iter + 44) * 230 + 25);
                    pixels[pos].col = col;
                    pixels[pos].row = row;
                }
            }

            j++;
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    int rank, numtasks;
    input_data in_data;
    int width, height;
    params *param;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // The current process ID / Rank.

    ppm_bitmap *bitmap;
    ppm_color_t *process_pixels;
    ppm_color_t *pixels;
    int CHUNK_SIZE;

    //Procesul master
    if(rank == 0){
        // alocare memorie pentru parametri
        param = alloc_params();

        // se citesc argumentele programului
        get_args(argc, argv, param);
        read_input_file(param);

        // calculate width and height

        width = (param->x_max - param->x_min) / param->resolution;
        height = (param->y_max - param->y_min) / param->resolution;

        CHUNK_SIZE = width * height / numtasks;

        bitmap = alloc_bitmap(width, height);

        pixels = (ppm_color_t *)calloc(width * height, sizeof (ppm_color_t));

        input_data *in_data_master = alloc_input_data(param->iterations, param->x_min, param->x_max,
                                                param->y_min, param->y_max, width, height);
        
        in_data.x_max = param->x_max;
        in_data.x_min = param->x_min;
        in_data.y_max = param->y_max;
        in_data.y_min = param->y_min;
        in_data.iterations = param->iterations;
        in_data.height = height;
        in_data.width = width;
        //sending input data for problem
        for(int i = 1; i < numtasks; i++){
            MPI_Send(in_data_master, sizeof(input_data), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        free_input_data(in_data_master);

    }else{
        MPI_Status status;

        MPI_Recv(&in_data, sizeof(input_data), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        CHUNK_SIZE = in_data.width * in_data.height / numtasks;
    }

    int start = rank * (double)(in_data.width * in_data.height) / numtasks;
    int end = MIN((rank + 1) * (double)(in_data.width * in_data.height) / numtasks, in_data.width * in_data.height);
    int size = end - start;
    process_pixels = (ppm_color_t *)calloc(size, sizeof (ppm_color_t));
    if(process_pixels == NULL){
        free(process_pixels);
        printf("Eroare la alocare");
        exit(1);
    }

    int col_start = rank * (double)(in_data.height) / numtasks;
    int col_end = MIN((rank + 1)* (double)(in_data.height) / numtasks, in_data.height);


    MPI_Scatter(pixels, CHUNK_SIZE * sizeof(ppm_color_t), MPI_CHAR, process_pixels, CHUNK_SIZE * sizeof(ppm_color_t), MPI_CHAR, 0, MPI_COMM_WORLD);

    calculateMandelbrot(process_pixels, in_data, col_start, col_end);

    MPI_Gather(process_pixels, CHUNK_SIZE * sizeof(ppm_color_t), MPI_CHAR, pixels, CHUNK_SIZE * sizeof(ppm_color_t), MPI_CHAR, 0, MPI_COMM_WORLD);

    if(rank == 0){
        printf("\n%s - MPI  -> DONE", argv[3]);

        FILE *img_fd = fopen(param->output_file, "wb");
        if(img_fd == NULL){
            printf("Nu s-a putut deschide fisierul de output");
            exit(1);
        }

        for(int i = 0; i < in_data.height; i++){
            for(int j = 0; j < in_data.width; j++){
                int pos = XY_TO_BYTE(pixels[i*in_data.width + j].col, pixels[i*in_data.width + j].row, bitmap->width);
                bitmap->pixels[pos].red = pixels[i*in_data.width + j].red;
                bitmap->pixels[pos].green = pixels[i*in_data.width + j].green;
                bitmap->pixels[pos].blue = pixels[i*in_data.width + j].blue;
            }
        }

        write_picture(img_fd, bitmap);

        // free memory
        free_bitmap(bitmap);
        fclose(img_fd);
        free_params(param);
    }

    free(process_pixels);

    MPI_Finalize();

    return 0;
}