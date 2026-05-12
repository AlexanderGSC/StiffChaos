#include "system.h"
#include <stdio.h>

const double k_1 = 0.04;
const double k_2 = 3e7;
const double k_3 = 1e4;

void set_initial_conditions(double *U) {
    U[0] = 1.0;
    U[1] = 0.0;
    U[2] = 0.0;
}

void eval(double *U, double *V) {
    //dx/dt = -k_1x + k_3yz
    //dy/dt = k_1x - k_3yz - k_2y^2
    //dz/dt = k_2y^2
    V[0] = -k_1 * U[0] + k_3 * U[1] * U[2];
    V[1] =  k_1 * U[0] - k_3 * U[1] * U[2] - k_2 * U[1] * U[1];
    V[2] =  k_2 * U[1] * U[1]; 
}

void jacobian(double x, double y, double z, double *J) {
    //dx/dt = -k_1x + k_3yz
    //dy/dt = k_1x - k_3yz - k_2y^2
    //dz/dt = k_2y^2
    J[0] = -k_1;
    J[1] = k_3*z;
    J[2] = k_3*y;
    J[3] = k_1;
    J[4] = -k_3*z - 2*k_2*y;
    J[5] = -k_3*y;
    J[6] = 0.0;
    J[7] = 2*k_2*y;
    J[8] = 0.0;
}

void print_header(void) {
    printf("-------------------ROBERTSON ECUATIONS SIMULATION---------------------------\n");
    printf("PARAMS  K1 = %.2f K2 = %.2f  K3 = %.2f\n", k_1,k_2,k_3);
}
