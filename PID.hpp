#include "main.h"
#include <cmath>
#include <vector>
#include <map>
#include <string>
// #include 

namespace nanoshimarobot_hal_lib{
    class PID{
        public:
            PID(float dt, std::array<float, 3> params):
            dt_(dt),
            params_({
                { "kp", params[0]},
                { "ki", params[1]},
                { "kd", params[2]}
            })
            {
                i_sum_ = 0.0;
            }

            float pos_type(float target_, float current_){
                float output = 0.0;
                float error = target_ - current_;
                i_sum_ += error *  dt_;
                output += params_.at("kp") * error;
                output += params_.at("ki") * i_sum_;
                output += params_.at("kd") * 
            }
            
            float vel_type(float target_, float current_){

            }
        private:

            std::map<std::string, float> params_;
            std::array<float, 3> error = {0.0, 0.0, 0.0};
            float dt_;
            float i_sum_;
    };
}