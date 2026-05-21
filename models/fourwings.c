#include <stdio.h>

const double a = 0.2;
const double b = -0.01;
const double c = 1.0;
const double d = -0.4;
const double e = -1.0;
const double f = -1.0;

void set_initial_conditions(double *U) {
    U[0] = 1.0;
    U[1] = 1.0;
    U[2] = 1.0;
}

void eval(double *U, double *V) {
    //dx/dt = ax + cyz
    //dy/dt = bx + dy - xz
    //dz/dt = ez + fxy
    V[0] =  a * U[0] + c * U[1] * U[2];
    V[1] =  b * U[0] + d * U[1] - U[0] * U[2];
    V[2] =  e * U[2] + f * U[0] * U[1]; 
}

void jacobian(double x, double y, double z, double *J) {
    //dx/dt = ax + cyz
    //dy/dt = bx + dy - xz
    //dz/dt = ez + fxy
    J[0] = a;
    J[1] = c*z;
    J[2] = c*y;
    J[3] = b - z;
    J[4] = d;
    J[5] = -x;
    J[6] = f*y;
    J[7] = f*x;
    J[8] = e;
}

void print_header(void) {
    printf("-------------------FOUR WINGS ATTRACTOR---------------------------\n");
    printf("PARAMS  A = %.2f B = %.2f  C = %.2f  D = %.2f  E= %.2f  F= %.2f\n", a,b,c,d,e,f);
}