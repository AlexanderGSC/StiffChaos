#include <stdio.h>
#include <math.h>

const double a = 1.4;

void eval(double *U, double *V) {
    //dx/dt = -ax - 4y - 4z - y^2 
    //dy/dt = -ay - 4z - 4x - z^2
    //dz/dt = -az - 4x - 4y - x^2
    V[0] = -a*U[0] - 4.0*U[1] - 4.0*U[2] - U[1]*U[1];
    V[1] = -a*U[1] - 4.0*U[2] - 4.0*U[0] - U[2]*U[2];
    V[2] = -a*U[2] - 4.0*U[0] - 4.0*U[1] - U[0]*U[0];  ;
}

void set_initial_conditions(double *U) {
    U[0] = 1.0;
    U[1] = 0.0;
    U[2] = 0.0;
}


void jacobian(double x, double y, double z, double *J) {
    // J es un array de 9 floats, orden row-major
    //dx/dt = -ax - 4y - 4z - y^2 
    //dy/dt = -ay - 4z - 4x - z^2
    //dz/dt = -az - 4x - 4y - x^2
    J[0] = -a;                                             // d(f1)/dx
    J[1] = -4.0 - 2.0*y;                                         // d(f1)/dy
    J[2] = -4.0;                                         // d(f1)/dz

    J[3] = -4.0;                                             // d(f2)/dx
    J[4] = -a;                                      // d(f2)/dy
    J[5] = -4.0 - 2.0*z;                                         // d(f2)/dz

    J[6] = -4.0-2.0*x;                                            // d(f3)/dx
    J[7] = -4.0;                                          // d(f3)/dy
    J[8] = -a;                                             // d(f3)/dz
}

void print_header(void) {
    printf("-------------------HALVORSEN ATRACTOR ECUATIONS SIMULATION---------------------------\n");
    printf("PARAMS  A = %.2f\n", a);
}