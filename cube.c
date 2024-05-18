#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define SCREEN_W 160
#define SCREEN_H 44
#define CUBE_W 20
#define K2 100  //distance of the cube from the viewer
#define K1 40 //fixed z1 coordinate
#define ROTATION_SPEED 0.045
#define INCREMENT_SPEED 0.6
#define HOR_OFFSET (-1 * CUBE_W)

/*
FUN FACT:
While testing the best/most pleasing values for K1 and K2,
realized that using Andy Sloane's (a1kon) formula plotted 
the cube as if the user was inside it.

K2 = 5
K1 = SCREEN_W * K2 * 3 / (8 * CUBE_W)
*/

char output[SCREEN_W * SCREEN_H];
float zbuffer[SCREEN_W * SCREEN_H];

void setup_buffers() {
    memset(output, ' ', SCREEN_W * SCREEN_H);
    memset(zbuffer, 0, 4 * (SCREEN_W * SCREEN_H));
}

float rotated_x(int i, int j, int k, float A, float B, float C) {
    float fi = i * (cos(B) * cos(C));
    float fj = j * (sin(A) * sin(B) * cos(C) + cos(A) * sin(C));
    float fk = k * (-cos(A) * sin(B) * cos(C) + sin(A) * sin(C));

    return  fi + fj + fk;
}

float rotated_y(int i, int j, int k, float A, float B, float C) {
    float fi = (-i) * cos(B) * sin(C);
    float fj = j * (-sin(A) * sin(B) * sin(C) + cos(A) * cos(C));
    float fk = k * (cos(A) * sin(B) * sin(C) + sin(A) * cos(C)); 

    return fi + fj + fk;
}

float rotated_z(int i, int j, int k, float A, float B, float C) {
    float fi = i * sin(B);
    float fj = (-j) * sin(A) * cos(B);
    float fk = k * cos(A) * cos(B);

    return fi + fj + fk;
}

float new_coordinates_xy(int i, int j, int k, float A, float B, float C, int *xprojection, int *yprojection) {
    float x = rotated_x(i, j, k, A, B, C);
    float y = rotated_y(i, j, k, A, B, C);
    float z = rotated_z(i, j, k, A, B, C) + K2;

    *xprojection = (int) (SCREEN_W/2 + HOR_OFFSET + K1 * x * 2 / z);
    *yprojection = (int) (SCREEN_H/2 + K1 * y / z);

    return z;
}

void luminosity(int xprojection, int yprojection, float z, char ascii_char) {
    int lum_index = xprojection + yprojection * SCREEN_W;
    float ooz = 1/z;
    if (lum_index >= 0 && lum_index < SCREEN_W * SCREEN_H) {
        if (ooz > zbuffer[lum_index]) {
            zbuffer[lum_index] = ooz;
            output[lum_index] = ascii_char;
        }
    }
}

void calculate_point(int i, int j, int k, char ascii_char, float A, float B, float C)
{
    int xprojection, yprojection;
    float z = new_coordinates_xy(i, j, k, A, B, C, &xprojection, &yprojection);
    luminosity(xprojection, yprojection, z, ascii_char);
}

int main() {
    float A = 0, B = 0, C = 0;

    while (1) {
        setup_buffers();
        for (float posx = - CUBE_W; posx < CUBE_W; posx += INCREMENT_SPEED) {
            for (float posy = -CUBE_W; posy < CUBE_W; posy += INCREMENT_SPEED) {
                calculate_point(posx, posy, -CUBE_W, '@', A, B, C);
                calculate_point(CUBE_W, posy, posx, '$', A, B, C);
                calculate_point(-CUBE_W, posy, -posx, '~', A, B, C);
                calculate_point(-posx, posy, CUBE_W, '#', A, B, C);
                calculate_point(posx, -CUBE_W, -posy, ';', A, B, C);
                calculate_point(posx, CUBE_W, posy, '+', A, B, C);
            }
        }

        printf("\x1b[H");
        for (int i = 0; i < SCREEN_W * SCREEN_H; i++) {
            putchar(i % SCREEN_W ? output[i] : '\n');
        }

        A += ROTATION_SPEED;
        B += ROTATION_SPEED;
        C += 0.01;
        usleep(8000 * 2);
    }
    return 0;
}
