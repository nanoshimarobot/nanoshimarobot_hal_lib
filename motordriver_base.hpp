#pragma once
#include "main.h"
#include "pwmOut.hpp"
#include <vector>
#include <array>
#include <cmath>
#include <tuple>

#define pwm_ch_num 4

namespace nanoshimarobot_hal_lib{
    #define motor_output_0_ 0
    #define motor_output_1_ 1
    class motordriver_base{
        public:
            template <typename T>
            motordriver_base(TIM_HandleTypeDef *handle, uint32_t ch[])
            {
                // motor_output_ch_[0] = std::make_tuple(pwmOut(handle_, ch_[0]), pwmOut(handle_, ch_[1]));
                // motor_output_ch_[1] = std::make_tuple(pwmOut(handle_, ch_[2]), pwmOut(handle_, ch_[3]));
                motor_output_ch_[0] = { pwmOut(handle_, ch[0]), pwmOut(handle_, ch[1]) };
                motor_output_ch_[1] = { pwmOut(handle_, ch[2]), pwmOut(handle_, ch[3]) };
                // for(uint8_t i = 0; i < pwm_ch_num; ++i){
                //     pwm_handler_[i] = pwmOut(handle, ch[i]);
                //     pwm_handler_[i].write(0.0);
                // }
                // motor_output_ch_[0] = std::make_tuple(pwm_handler_[0], pwm_handler_[1]);
                // motor_output_ch_[1] = std::make_tuple(pwm_handler_[2], pwm_handler_[3]);
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
            TIM_HandleTypeDef *handle_;
            std::array<std::tuple<pwmOut, pwmOut>, 2> motor_output_ch_;
    };
}