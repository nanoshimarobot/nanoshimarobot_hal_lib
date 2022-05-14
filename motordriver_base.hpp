#pragma once
#include "main.h"
#include "pwmOut.hpp"
#include <vector>
#include <array>
#include <cmath>
#include <tuple>
#include <functional>
#include <string>

namespace nanoshimarobot_hal_lib{
    class motordriver_base{
        public:
            motordriver_base(TIM_HandleTypeDef *handle,
                             std::array<uint32_t, 4> ch, 
                             std::map<std::string, uint8_t> motor_output_n = { { "M0", 0 }, { "M1", 1 } }):
            pwm_out_ch_({
                { pwmOut(handle, ch[0]), pwmOut(handle, ch[1]) },
                { pwmOut(handle, ch[2]), pwmOut(handle, ch[3]) }
            }),
            motor_output_n_(motor_output_n)
            {
            }

            ~motordriver_base(){};

            virtual void write(float duty, const std::string output_inst){
                if(duty >= 0){
                    std::get<0>(pwm_out_ch_[motor_output_n_.at(output_inst)]).write(0);
                    std::get<1>(pwm_out_ch_[motor_output_n_.at(output_inst)]).write(duty);
                }else{
                    std::get<0>(pwm_out_ch_[motor_output_n_.at(output_inst)]).write(std::fabs(duty));
                    std::get<1>(pwm_out_ch_[motor_output_n_.at(output_inst)]).write(0);
                }
            }

        private:
            std::vector<std::tuple<pwmOut, pwmOut>> pwm_out_ch_;
            std::map<std::string, uint8_t> motor_output_n_;
    };
}