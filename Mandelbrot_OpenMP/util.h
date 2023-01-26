#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define XY_TO_BYTE(x, y, width)                 \
    ((y) * (width) + (x))

#define MIN(a, b) (a > b) ? b : a

// structura pentru parametrii unui fisier de intrare
typedef struct params {
	int iterations;
	double x_min, x_max, y_min, y_max;
    double resolution;
    char *input_file;
    char *output_file;
} params;

// structura pentru retinerea valorilor RGB al unui pixel
typedef struct ppm_color_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    int col;
    int row;
} ppm_color_t;

// structura pentru retinerea valorilor legate de bitmap
typedef struct ppm_bitmap {
    uint16_t width;
    uint16_t height;
    ppm_color_t *pixels;
} ppm_bitmap;

// functie care scrie imaginea finala in format ppm
void write_picture(FILE *img_fd, ppm_bitmap *bitmap);

// functie care scrie header-ul pentru o imagine de tip ppm
void write_ppm_header(FILE *img_fd, int width, int height);

// functie care scrie valoarea unui pixel
void write_pixel(FILE *img_fd, ppm_color_t pixel);

// citeste fisierul de intrare
void read_input_file(params* par);

// citeste argumentele programului
void get_args(int argc, char **argv, params *param);

// aloca memorie pentru parametrii
params *alloc_params();

// dealoca memoria alocata pentru parametrii
void free_params(params *param);

// Functie pentru alocarea memoriei pentru bitmap
ppm_bitmap *alloc_bitmap(int width, int height);

// Functie pentru dealocarea memoriei pentru bitmap
void free_bitmap(ppm_bitmap *bitmap);

#endif /* UTIL_H */