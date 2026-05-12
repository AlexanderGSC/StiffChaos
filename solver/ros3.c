#include <math.h>
#include <stdio.h>
#include "system.h"

double       H_STEP   = 1e-3;
const double H_MIN    = 1e-9;
const double H_MAX    = 1e-3;
const double a_tol    = 1e-7;
const double r_tol    = 1e-7;
double relative_error = 0.0;
double global_time    = 0.0;

const double ros3_gamma = 0.4358665215;
const double c2 = 0.4358665215;
const double c3 = 1.0;

const double a21 = 0.4358665215;
const double a31 = 1.208496649;
const double a32 = -0.644363171;

const double m1 = 0.25;
const double m2 = 0.4597037304;
const double m3 = 0.2892962696;

void swap_rows(double *A, int r1, int r2) {
    for (int i = 0; i < 3; ++i) {
        double temp = A[r1*3 + i];
        A[r1*3 + i] = A[r2*3 + i];
        A[r2*3 + i] = temp;
    }
}

int decompositionLU_pivot(double *A, double *L, double *U, int *P) {
    double M[9];
    for (int i=0; i<9; ++i) M[i] = A[i];
    for (int i=0; i<3; ++i) P[i] = i;

    for (int k=0; k<3; ++k) {
        int maxRow = k;
        double maxVal = fabs(M[k*3 + k]);
        for (int i=k+1; i<3; ++i) {
            double val = fabs(M[i*3 + k]);
            if (val > maxVal) {
                maxVal = val;
                maxRow = i;
            }
        }
        if (maxVal < 1e-18) return -1;

        if (maxRow != k) {
            swap_rows(M, k, maxRow);
            int tmp = P[k]; P[k] = P[maxRow]; P[maxRow] = tmp;
        }

        for (int i=k+1; i<3; ++i) {
            M[i*3 + k] /= M[k*3 + k];
            for (int j=k+1; j<3; ++j) {
                M[i*3 + j] -= M[i*3 + k] * M[k*3 + j];
            }
        }
    }

    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            if (i > j) { L[i*3 + j] = M[i*3 + j]; U[i*3 + j] = 0.0; }
            else if (i == j) { L[i*3 + j] = 1.0; U[i*3 + j] = M[i*3 + j]; }
            else { L[i*3 + j] = 0.0; U[i*3 + j] = M[i*3 + j]; }
        }
    }
    return 0;
}

void forwardSubstitution(double *L, double *b, double *y) {
    y[0] = b[0];
    y[1] = b[1] - L[3] * y[0];
    y[2] = b[2] - L[6] * y[0] - L[7] * y[1];
}

void backwardSubstitution(double *U, double *y, double *x) {
    x[2] = y[2] / U[8];
    x[1] = (y[1] - U[5] * x[2]) / U[4];
    x[0] = (y[0] - U[1] * x[1] - U[2] * x[2]) / U[0];
}

void solveLU_pivot(double *L, double *U, int *P, double *b, double *x) {
    double b_permuted[3], y[3];
    for (int i=0; i<3; ++i) b_permuted[i] = b[P[i]];
    forwardSubstitution(L, b_permuted, y);
    backwardSubstitution(U, y, x);
}
double min(double a, double b) {
    if (a < b) return a;
    else return b;
}
double max(double a, double b) {
    if (a > b) return a;
    else return b;
}
int ros3(double *U) {
    double Jacobian[9], A[9], A_L[9], A_U[9], K1[3], K2[3], K3[3];
    int P[3];
    double F_eval[3];
    double Y0[3],Y1[3],Y2[3];
    double error[3], tol[3];
    double h_update;
    jacobian(U[0],U[1],U[2], Jacobian);

    //Y0 = F(U)
    eval(U,Y0);
    
    // A = I - h * γ * J
    for (int i = 0; i < 9; ++i) A[i] = -Jacobian[i] * H_STEP * ros3_gamma;
    A[0] += 1.0f;
    A[4] += 1.0f;
    A[8] += 1.0f;

    if (decompositionLU_pivot(A, A_L, A_U, P) != 0) {
        printf("Error: singular LU matrix\n");
        //Manage here a singular matrix
    }

    // STEP 2: Obtaining K1 coeffs
    for (int i=0; i<3; i++) F_eval[i] = H_STEP * Y0[i];
    solveLU_pivot(A_L,A_U,P,F_eval, K1);
    // STEP3: Obtaining K2 coeffs
    for (int i=0; i<3; i++) Y1[i]  = U[i] + a21 * K1[i];
    eval(Y1,F_eval);
    for (int i=0; i<3; i++) F_eval[i] = H_STEP * F_eval[i] + c2  * K1[i];
    solveLU_pivot(A_L,A_U,P,F_eval,K2);
    //STEP 4: Obtaining K3 coeffs
    for (int i=0; i<3; i++) Y2[i]  = U[i] + a31 * K1[i] + a32 * K2[i];
    eval(Y2,F_eval);
    for (int i=0; i<3; i++) F_eval[i] = H_STEP * F_eval[i] + c3  * K1[i] + (1 - 2*c3)*K2[i];
    solveLU_pivot(A_L,A_U,P,F_eval,K3);

    for (int i = 0; i < 3; ++i) {
        F_eval[i] = U[i] + m1 * K1[i] + m2 * K2[i] + m3 * K3[i];
        error[i]  = fabs(m3 * K3[i]);
        tol[i]    = fabs(U[i])  * r_tol + a_tol; 
    }
    int update = 1;
    relative_error = max(error[0]/tol[0], max(error[1]/tol[1], error[2]/tol[2]));
    if (relative_error > 1.0) update = 0;

    h_update = 0.9 * pow(1.0 / relative_error, 1.0 / (4.0)); // order of the method = 3
    H_STEP = min(H_MAX, max(H_MIN, min(H_STEP * h_update, H_STEP * 2)));
    if (update == 1) {
        U[0] = F_eval[0]; U[1] = F_eval[1]; U[2] = F_eval[2];
    } 
    return update;
}