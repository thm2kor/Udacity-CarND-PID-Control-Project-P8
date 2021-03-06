#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

//overloading << for displaying vectors in the form [x,y,z]
std::ostream& operator<<(std::ostream& strm, const std::vector<double> &d) {
  strm << "[" ;
  for (unsigned int i = 0; i<d.size(); i++) {
    strm << d[i];
    if (i != d.size() - 1)
         strm << ", ";
  }
  return strm << "]";
}

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main( int argc, char *argv[] ) {
  uWS::Hub h;
  PID pid_angle;
  // The application supports either ./pid --twiddle flag or  ./pid <Kp> <Ki> <Kd>
  if (argc == 2) {
    std::string flag = argv[1]; // check if we are in twiddle mode.
    if (flag.compare("--twiddle") == 0){
      pid_angle.twiddle_mode = true;
      std::cout << "Twiddle mode enabled " << std::endl;
    }
  }
  // default values. ./pid <Kp> <Ki> <Kd>
  std::vector<double> parameters = {0.15, 0.0001, 3.0}; // default values.
  if (argc == 4) { // Simple check to see if the parameters are sent on command line
    for ( int i = 1 ; i < argc ; i++){
      parameters[i-1] = std::stod(argv[i]);
    }
  }
  std::cout << "Running PID controller with parameters " << parameters << std::endl;
  //Initializing the parameters for the PID controller
  pid_angle.Init (parameters[0], parameters[1], parameters[2]);
  h.onMessage([&pid_angle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          //double speed = std::stod(j[1]["speed"].get<string>());
          //double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value = 0.0;
          // send the current cte to the PID object
          pid_angle.UpdateError(cte);
          // send the returned value to the simulator
          steer_value = pid_angle.TotalError();
          // limit the steering value to [-1, 1]
          if (steer_value < -1) {
            steer_value = -1;
          } else if (steer_value > 1) {
            steer_value = 1;
          }
          double throttle=0.4;
          if (fabs(cte) > 1) {
            throttle = 0.25;
          } else {
            throttle = fmin(1 / fabs(steer_value), 50);
            throttle = ((1 - 0.45) * (throttle - 0)) / (50 - 0) + 1;
          }
#ifdef LOG_CTE
          std::cout << cte <<  std::endl;
#endif
          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
#ifdef DEBUG
          std::cout << msg << std::endl;
#endif
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }

  h.run();
}
