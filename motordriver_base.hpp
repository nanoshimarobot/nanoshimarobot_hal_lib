#pragma once
#include "main.h"
#include "pwmOut.hpp"
#include <vector>
#include <array>
#include <cmath>
#include <tuple>

#define pwm_ch_num 4

namespace nanoshimarobot_hal_lib{
    class motordriver_base{
        public:
            motordriver_base(TIM_HandleTypeDef *handle, uint32_t ch[])
            {
                motor_output_ch_.at(0) = { pwmOut(handle, ch[0]), pwmOut(handle, ch[1]) };
                motor_output_ch_.at(1) = { pwmOut(handle, ch[2]), pwmOut(handle, ch[3]) };
                // std::array<std::tuple<pwmOut, pwmOut>, 2> motor_output_ch = {
                //     {pwmOut(handle_, ch[0]), pwmOut(handle_, ch[1])},
                //     {pwmOut(handle_, ch[0]), pwmOut(handle_, ch[1])}
                // };
            }

            void write(float duty, uint8_t motor_output_n){
                if(duty >= 0){
                    std::get<0>(motor_output_ch_[motor_output_n]).write(duty);
                    std::get<1>(motor_output_ch_[motor_output_n]).write(0);
                }else{
                    std::get<0>(motor_output_ch_[motor_output_n]).write(0);
                    std::get<1>(motor_output_ch_[motor_output_n]).write(std::fabs(duty));
                }
            }

        private:
            std::array<std::tuple<pwmOut, pwmOut>, 2> motor_output_ch_;
    };
}