#ifndef SYSTEM_H
#define SYSTEM_H

// Declaración de una función
void set_initial_conditions(double *U);

void eval(double *U, double *V);

void jacobian(double x, double y, double z, double *J);

void print_header();


#endif // ROBERTSON_H