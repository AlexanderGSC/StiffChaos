#ifndef ROS3_H
#define ROS3_H

extern double       H_STEP;
extern const double H_MIN;
extern const double H_MAX;
extern const double a_tol;
extern const double r_tol;
extern double relative_error;
extern double global_time;

// Declaración de una función
int ros3(double *U);
#endif // ROS3_H
