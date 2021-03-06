#include "PID.h"
#include <iostream>
#include <numeric>

PID::PID() {
  
}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) { //TODO: Why not move this to the constructor?
  // Initialise the gains. 
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  prev_cte = 0.0;
  // Initialize the errors
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  // Twiddle parameters
  parameter = {0.0, 0.0, 0.0};
  delta = {1.0, 1.0, 1.0};
}

void PID::UpdateError(double cte) {
  // set the error for the proportional part
  p_error = cte;
  // set the error for the differential part
  d_error = cte - prev_cte;
  // reset the prev_cte. 
  prev_cte = cte; 
  // set the error for integral part
  i_error += cte;
}

double PID::TotalError() {
  return (p_error * Kp) + (i_error * Ki) + (d_error * Kd);  
}

void PID::Twiddle() {
/*  double best_err = 0.0;
  double threshold = 0.001;*/
  double sum = std::accumulate(parameter.begin(), parameter.end(), 0);

}