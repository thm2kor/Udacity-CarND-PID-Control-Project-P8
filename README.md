# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program
[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)
![cover_image](./images/result_pid_controller.gif)

[//]: # (Image References)

[image1]: ./images/plot_cte_step1.jpg "Gain selection - Step 1"
[image2]: ./images/plot_cte_step2.jpg "Gain selection - Step 2"
[image3]: ./images/plot_cte_step3.jpg "Gain selection - Step 3"
[image4]: ./images/plot_cte_step4.jpg "Gain selection - Step 4"

---
## Objective
The objective of this project is to develop a PID (Proportional, Integral and Differential) controller to maneuvre a vehicle around a virtual track. Udacity provided a [simulator](https://github.com/udacity/self-driving-car-sim/releases) which sends the error component for the steering angle, the vehicle's current speed (in mph) and current steering angle. The program has to calculate the optimum steering angle for the next cycle so that the vehicle drives within the road boundaries.

### Error
The term **Error** refers to the deviation of an actual value of a control variable(eg: current steering angle) with respect to a target variable(eg: target steering angle). The PID controller monitors this error and utilize a combination of 3 corrective terms (proportional, integral and derivate) to calculate output values which attempts to reduce the error, which in terms moves the control variable closer to the target variable.

In the project, the error component for optimising the steering angle is called as CTE - Cross Track Error, which is the lateral distance between the position of the car and the reference trajectory. The project calculates the total error by adding the responses from three corrective terms and returns the new corrective steering angle back to the simulator.

### Proportional Term
The Proportional term produces an output value that is proportional to the current error value. The proportional response can be adjusted by multiplying the error by a constant K<sub>p</sub>, called the proportional gain constant. In principle, the proportional term of the controller tries to steer the car toward the centre of the road. When only the proportional term is configured, it tends to overshoot after a certain time.

### Integral Term
The Integral term in a PID controller is the sum of the instantaneous error over time and gives the accumulated offset that should have been corrected previously. The accumulated error is then multiplied by the integral gain K<sub>i</sub> and added to the controller output. The integral term tries to eliminate possible bias, if any, on the controlled system that could prevent the error to be eliminated.

### Derivative Term
The derivative term determines the slope of the error over time and multiplying this rate of change by the derivative gain K<sub>d</sub>.The differential term helps to counteract the proportional trend to overshoot the centre line by smoothing the approach to it.

### Total Error
The Total Error is calculated as the sum of all the 3 terms discussed above multiplied by their respective gains.

```c++
double PID::TotalError() {
  return -(p_error * Kp) - (i_error * Ki) - (d_error * Kd);  
}
```
## Definition of K<sub>p</sub> , K<sub>i</sub> , K<sub>d</sub>
The gain components are determined manually using the method discussed [here](https://udacity-reviews-uploads.s3.amazonaws.com/_attachments/41330/1493863065/pid_control_document.pdf). The application supports a set of 3 command line arguments which accepts three parameters, one each for the gain components.

```sh
./pid <Kp> <Ki> <Kd>
```
For example, to run the simulator with **only Proportional** gain factor of 0.15:
```sh
./pid 0.15 0 0 >> output.txt
```
If the compiler definition **LOG_CTE is set**, the program logs the CTE's to an output file mentioned in the command line. These values are then processed by a python [script](./plotter.py) to generate a chart with the CTE's on the y-axis and time counters on the x-axis. Since the CTE values could be positive and negative values, the mean squared error (MSE) is selected as the key evaluation metric. The CTE values are captured as long as the vehicle stays on course. A CTE value of 1.5 or higher indicates that the car is out of the road surface. During the gain definition phase, the speed of the car was not adapted. The speed of the car is determined by a control variable named **throttle**. It was set to a default **0.3**. With this default value, the car reached a **maximum speed of 34 - 35 mph**.

### Step 1: Set K<sub>p</sub> , K<sub>i</sub> , K<sub>d</sub> to 0
When the gain parameters are set to 0, the errors, as expected, increased rapidly resulting in the car leaving the track in few seconds. This could be visually understood from the below plot, which shows the errors increasing rapidly from 1.0 to 4.0 within 500 time steps.
![step1][image1]

### Step 2: Increase K<sub>p</sub> until the error is fairly small
Keeping K<sub>i</sub> , K<sub>d</sub> as 0, the K<sub>p</sub> alone was adjusted until the car stayed on course for a fair amount of time. The car was able to negotiate the first curve for K<sub>p</sub> between 0.1 and 0.2. The mean squared errors remained almost the same for these values. Since there was no clear winner, I settled for a **mid point of 1.5 for K<sub>p</sub>**.
![step2][image2]

### Step 3: Increase K<sub>d</sub> until any overshoot is fairly minimal
During this step, when the K<sub>d</sub> were configured between 2.0 and 3.0, the vehicle was able to cross the bridge and negotiate the subsequent left and rights turns with a little bit of difficulty.
![step3][image3]
As shown in the above graph, the K<sub>d</sub> value of 0.3 resulted in the best minimization of the error. There were still some minor overstepping around the curves even with the best possible values of K<sub>d.

### Step 4: Increase K<sub>i</sub> until any error that is still existing is eliminated.
During this step, there was a slight improvement in the performance around the corners. But the values of K<sub>i</sub> were very sensitive to the changes.
![step4][image4]
As shown above, the best performance of the car was achieved when the K<sub>p</sub> , K<sub>i</sub> , K<sub>d</sub> were set to **0.15, 0.0001 and 3.0** respectively.

### Step 5: Fine tune the gains using live twiddle
To further finetune the performance, as an optional step, a twiddle algorithm was implemented. This twiddle function, when enabled, runs every 300 timesteps (`TWIDDLE_FREQ`) and adapts the gain parameters as discussed in the class. I referred to the c++ implementation as mentioned [here](https://martin-thoma.com/twiddle/). Since the simulator had to run to derive the CTE's, I switched from the 'while .. if loop' to a state machine based implementation. But the algorithm implementation is in principle  the same as discussed in the class.

Since twiddle as a method for deriving the parameter is an optional step, it is disabled by default. To enable the twiddle algorithm, the program should be run with the --twiddle option.
```sh
./pid --twiddle
```

## Results
The results of the PID controller with **K<sub>p</sub> = 0.15, K<sub>i</sub> = 0.0001 and K<sub>p</sub> = 3.0** without twiddle algorithm could be found [here](./images/result_pid_controller.mov)

---
## Dependencies

* cmake >= 3.5  
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

---
## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.
---

## References
1. https://en.wikipedia.org/wiki/PID_controller
2. https://martin-thoma.com/twiddle/
3. https://youtu.be/2uQ2BSzDvXs - Twiddle Algorithm discussed during the class
4. https://udacity-reviews-uploads.s3.amazonaws.com/_attachments/41330/1493863065/pid_control_document.pdf
