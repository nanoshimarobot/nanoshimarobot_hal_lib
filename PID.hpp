#include "main.h"
#include <cmath>
#include <vector>
#include <map>
#include <string>

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
            }

            float pos_type(float target_, float current_){
                output = 0.0;
                error[0] = target_ - current_;
                if(fabs(error[0]) < 1.0) i_sum_ = 0;
                i_sum_ += error[0] * dt_;
                output += params_.at("kp") * error[0];
                output += params_.at("ki") * i_sum_;
                output += params_.at("kd") * (error[0] - error[1])/dt_;
                error[2] = error[1];
                error[1] = error[0];

                return output;
            }
            
            float vel_type(float target_, float current_){
                error[0] = target_ - current_;
                output += params_.at("kp") * (error[0] - error[1]);
                output += params_.at("ki") * error[0] * dt_;
                output += params_.at("kd") * (error[0] - 2.0 * error[1] + error[2]) / dt_;

                return output;
            }
        private:
            void _swap_error(void){
                error[2] = error[1];
                error[1] = error[0];
            }

            std::map<std::string, float> params_;
            std::array<float, 3> error = {0.0, 0.0, 0.0};
            float dt_;
            float i_sum_ = 0;
            float output = 0.0;
    };
}