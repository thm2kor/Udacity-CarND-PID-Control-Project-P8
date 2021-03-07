#ifndef PID_H
#define PID_H
#include <vector>

static constexpr double TWIDDLE_THRESHOLD = 0.001; 
enum twiddle_state {
  init = 0,
  param_incremented = 1,
  param_decremented = 2
};

class PID {
 public:
  /**
   * Constructor
   */
  PID();
  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init (double Kp_, double Ki_, double Kd_);
  /**
   * Destructor.
   */
  virtual ~PID();
  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  void UpdateError(double cte);

  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  double TotalError();
  
 private:
  /**
   * PID Errors
   */
  double p_error;
  double i_error;
  double d_error;

  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;
  
  /**
   * hold the previous cte for calculating the p error
   */ 
  double prev_cte;
  
  /**
   * parameters for twiddle
   */ 
  bool twiddle_mode;
  twiddle_state state;
  double errs_accumalated;
  int sample_size;
  int curr_param_index;
  double best_err; double current_err;
  /**
   * class members for twiddle algorithm
   */ 
  std::vector<double> parameter; //parameter vector
  std::vector<double> delta; //parameter delta vector

};

#endif  // PID_H