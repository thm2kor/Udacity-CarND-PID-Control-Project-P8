#ifndef PID_H
#define PID_H
#include <vector>
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
  PID(double Kp_, double Ki_, double Kd_);
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
  /**
   * Calculate the best possible values for control parameters .
   * @output none
   */
  void Twiddle();
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
   * class members for twiddle algorithm
   */ 
  std::vector<double> parameter; //parameter vector
  std::vector<double> delta; //parameter delta vector

};

#endif  // PID_H