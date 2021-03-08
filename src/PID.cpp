#include "PID.h"
#include <iostream>
#include <cmath>
#include <numeric>
#include <limits>

// Initialize the controller with zero parameters.
// Use the overloaded constructor for better results
PID::PID() {
  Kp = 0.0;
  Ki = 0.0;
  Kd = 0.0;
  prev_cte = 0.0;
  // Initialize the errors
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  // Twiddle parameters
  best_err = std::numeric_limits<double>::max(); 
  twiddle_mode = false;
  curr_param_index = 0;
  current_err = 0.0;
  state = twiddle_state::init;
  // Initialize the data for twiddle algo
  parameter = {Kp, Ki, Kd};
  //suggestion from https://knowledge.udacity.com/questions/312174
  delta = {0.1 * Kp, 0.1 * Ki, 0.1 * Kd}; 

}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  // Initialise the gains. 
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  prev_cte = 0.0;
  // Initialize the errors
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  // Initialize the data for twiddle algo
  parameter = {Kp, Ki, Kd};
  //suggestion from https://knowledge.udacity.com/questions/312174
  delta = {0.1 * Kp, 0.1 * Ki, 0.1 * Kd}; 

  sample_size = 0;
  err_accumulated  = 0;
}

PID::~PID() {}

// Update the error parameters
void PID::UpdateError(double cte) {
  // set the error for the proportional part
  p_error = cte;
  // set the error for the differential part
  d_error = cte - prev_cte;
  // reset the prev_cte
  prev_cte = cte; 
  // set the error for integral part
  i_error += cte;
  
  if (twiddle_mode) {
    Twiddle(cte);
    std::cout << "Parameters [" << Kp << "," << Ki << "," << Kd << "] Error: " << current_err << std::endl;
  } 
}

// Update the  product sum of gain and error
double PID::TotalError() {
  return -(p_error * Kp) - (i_error * Ki) - (d_error * Kd);  
}

// Adapt the parameters using the twiddle algorithm
void PID::Twiddle(double cte) {   
  sample_size++;
  // accumulate the errors for deriving the minimum error.
  // fabs to compensate the effect of positive and negative cte
  err_accumulated += fabs(cte);  
  // twiddle every 300 samples
  if (sample_size % TWIDDLE_FREQ == 0 ) {      
    current_err = err_accumulated / sample_size;
    
    switch (state) {
      case twiddle_state::init:          
        best_err = current_err;  
        parameter[curr_param_index] += delta[curr_param_index];
        state = twiddle_state::param_incremented;
        break;
        
      case twiddle_state::param_incremented:
        if (current_err < best_err) {
          best_err = current_err;
          delta[curr_param_index] *= 1.1;
          //shift to next param index
          curr_param_index = (curr_param_index + 1) % 3; 
          parameter[curr_param_index] += delta[curr_param_index];
          state = twiddle_state::param_incremented;
        } else {
          parameter[curr_param_index] -= 2*delta[curr_param_index];
          if (parameter[curr_param_index] < 0) {
            parameter[curr_param_index] = 0;
            //shift to next param index
            curr_param_index = (curr_param_index + 1) % 3; 
          }
          state = twiddle_state::param_decremented;
        }
        break;
        
      case twiddle_state::param_decremented:
        if (current_err < best_err) {
          best_err = current_err;
          delta[curr_param_index] *= 1.1;
        } else {
          parameter[curr_param_index] += delta[curr_param_index];
          delta[curr_param_index] *= 0.9 ;
        }
        //shift to next param index
        curr_param_index = (curr_param_index + 1) % 3; 
        parameter[curr_param_index] += delta[curr_param_index];
        state = twiddle_state::param_incremented;
        break;
        
      default:
        break;
    } // end state check
    
    Kp = parameter[0];
    Ki = parameter[1];
    Kd = parameter[2];
    sample_size = 0;
    err_accumulated  = 0;
  }     
}