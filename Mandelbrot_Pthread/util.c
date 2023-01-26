#include "util.h"

// functie care scrie imaginea finala in format ppm
void write_picture(FILE *img_fd, ppm_bitmap *bitmap){
    write_ppm_header(img_fd, bitmap->width, bitmap->height);

    for (uint32_t y = 0; y < bitmap->height; ++y) {
        for (uint32_t x = 0; x < bitmap->width; ++x){
            write_pixel(img_fd, bitmap->pixels[XY_TO_BYTE(x, y, bitmap->width)]);
        }
    }

    fflush(img_fd);
}

// functie care scrie header-ul pentru o imagine de tip ppm
void write_ppm_header(FILE *img_fd, int width, int height){
    fprintf(img_fd, "P3\n%d %d\n%d\n", width, height, 255);
}

// functie care scrie valoarea unui pixel
void write_pixel(FILE *img_fd, ppm_color_t pixel){
    // Writing the RGB values in the 2D array to the file
            fprintf(img_fd, "%d %d %d\n", pixel.red, pixel.green, pixel.blue);
}

// citeste fisierul de intrare
void read_input_file(params* param)
{
	FILE *file = fopen(param->input_file, "r");
	if (file == NULL) {
		printf("Eroare la deschiderea fisierului de intrare!\n");
		exit(1);
	}

	fscanf(file, "%lf %lf %lf %lf",
			&param->x_min, &param->x_max, &param->y_min, &param->y_max);
	fscanf(file, "%lf", &param->resolution);
	fscanf(file, "%d", &param->iterations);

	fclose(file);
}

// citeste argumentele programului
void get_args(int argc, char **argv, params *param)
{
	if (argc < 4) {
		printf("Numar insuficient de parametri:\n\t"
				"./exec input_file output_file threads_number");
		exit(1);
	}

    strcpy(param->input_file, argv[1]);
    strcpy(param->output_file, argv[2]);
    param->threads_number = atoi(argv[3]);
	
}

// aloca memorie pentru parametrii
params *alloc_params(){
    params *param = (params *)calloc(1, sizeof(params));
    if(param == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    param->input_file = (char*)calloc(50, sizeof(char));
    if(param->input_file == NULL){
        free(param);
        printf("Eroare la alocare");
        exit(1);
    }

    param->output_file = (char*)calloc(50, sizeof(char));
    if(param->output_file == NULL){
        free(param->input_file);
        free(param);
        printf("Eroare la alocare");
        exit(1);
    }

    return param;
}

// dealoca memoria alocata pentru parametrii
void free_params(params *param){
    free(param->input_file);
    free(param->output_file);
    free(param);
}


ppm_bitmap *alloc_bitmap(int width, int height){
    ppm_bitmap *bitmap = (ppm_bitmap *)calloc(1, sizeof(ppm_bitmap));
    if(bitmap == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    bitmap->width = width;
    bitmap->height = height;

    bitmap->pixels = (ppm_color_t *)calloc(height * width, sizeof (ppm_color_t));
    if(bitmap->pixels == NULL){
        free(bitmap);
        printf("Eroare la alocare");
        exit(1);
    }   

    return bitmap;
}

// Functie pentru dealocarea memoriei pentru bitmap
void free_bitmap(ppm_bitmap *bitmap){
    free(bitmap->pixels);
    free(bitmap);
}