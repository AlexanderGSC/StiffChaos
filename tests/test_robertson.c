#include "system.h"
#include "ros3.h"
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define EXIT_FAILURE -1
#define EXIT_SUCCESS  0

int main(void) {
    double U[3]; // condiciones iniciales
    set_initial_conditions(U);
    double time     = 0.0; //total time simulated
    double global_time=0.0;
    double delta_t2 = 0.0;//time between prints
    double delta_t  = 0.0; //time between saves!!
    double final_time = 100000.0;
    int updated;
    int accepted = 0;
    double min_step = 1.e-9;
    double max_step = 1.e-3;
    int NSTEPS = 0;
    FILE *csv_file = fopen("simulation.csv", "w");
    if (!csv_file) {
        perror("Error opening CSV file.");
        return EXIT_FAILURE;
    }
    fprintf(csv_file, "time,x,y,z\n");
    fprintf(csv_file, "%.3f,%.3f,%.3f,%.3f\n", time, U[0], U[1], U[2]);

    print_header();

    printf("INITIAL CONDITIONS: X = %.6f Y = %.6f Z = %.6f \n INITIAL H STEP: %.4e MAX H STEP: %.4e MIN HSTEP: %4.e ATOL: %.4e RTOL: %.4e\n", U[0], U[1], U[2], H_STEP, H_MAX, H_MIN, a_tol, r_tol);
    printf("-----------------------------------------------------------\n"); 
    clock_t start_time = clock();
    while (time < final_time) {
        updated = ros3(U);
        if (updated == 1) {
            time += H_STEP;
            delta_t2 += H_STEP;
            delta_t  += H_STEP;
            accepted += 1;
        } else {
            printf("STEP: %d HSTEP %.4e  RELATIVE ERROR: %4e\n",NSTEPS, H_STEP,relative_error);
        }
        if (H_STEP < min_step) min_step = H_STEP;
        if (H_STEP > max_step) max_step = H_STEP;
        if (delta_t2 >= final_time / 100) {
            //NSTEPS = 0;
            double sum = U[0] + U[1] + U[2];
            printf("(TIME %.2f) X: %.12f Y: %.12f Z: %.12f SUM: %.18f H STEP: %.6e ERR/TOL: %.12e\n", time, U[0], U[1], U[2], sum, H_STEP,relative_error);
            delta_t2 = 0;
        }
        if (delta_t >= 0.2) {
            fprintf(csv_file, "%.10f,%.10f,%.10f,%.10f\n", time, U[0], U[1], U[2]);
            delta_t = 0;
        }
        NSTEPS++;
        global_time = time;
    }
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fclose(csv_file);
    printf("(TIME %.2f) X: %.6f Y: %.6f Z: %.6f H STEP: %.12f ERR/TOL: %.6e\n", time, U[0], U[1], U[2], H_STEP,relative_error);
    printf("----------------------------------------------------------------------\n");   
    printf("FINAL RESULTS\n");
    printf("Elapsed time: %.4f (secs)\n", elapsed_time);
    printf("Steps accepted: %d  Percentage steps accepted: %.2f\n", accepted, 100.0f * (float)accepted / (float)NSTEPS);
    printf("Steps rejected: %d  Percentage steps rejected: %.2f\n", NSTEPS - accepted, 100.0f * (float)(NSTEPS - accepted) / (float)NSTEPS);
    printf("Total simulated time (SUM h accepted): %.4f\n", time);
    printf("NUMBER OF STEPS: %d   Max H_STEP used: %.4e   Min H STEP used %.4e\n",NSTEPS, max_step, min_step);
    printf("Simulation saved on simulation.csv with elapsed time %.3f\n",0.1f);
    printf("---------------------END OF SIMULATION--------------------------------------\n");  
    return EXIT_SUCCESS;
}
