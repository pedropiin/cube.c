#include <stdio.h>
#include <math.h>
#include <string.h>

#define K1 40;

float rotationx(int i, int j, int k, float A, float B, float C) {
    float fi = i * (cos(B) * cos(C));
    float fj = j * (sin(A) * sin(B) * cos(C) + cos(A) * sin(C));
    float fk = k * (-cos(A) * sin(B) * cos(C) + sin(A) * sin(C));

    return  fi + fj + fk;
}

float rotationy(int i, int j, int k, float A, float B, float C) {
    float fi = (-i) * cos(B) * sin(C);
    float fj = j * (-sin(A) * sin(B) * sin(C) + cos(A) * cos(C));
    float fk = k * (cos(A) * sin(B) * sin(C) * sin(A) * cos(C)); 

    return fi + fj + fk;
}

float rotationz(int i, int j, int k, float A, float B, float C) {
    float fi = i * sin(B);
    float fj = (-j) * sin(A) * cos(B);
    float fk = k * cos(A) * cos(B);

    return fi + fj + fk;
}

int main() {

    return 0;
}