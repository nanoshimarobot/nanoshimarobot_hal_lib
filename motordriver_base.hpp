#pragma once
#include "main.h"
#include "pwmOut.hpp"
#include <vector>
#include <array>
#include <cmath>
#include <tuple>
// #include <paire

#define pwm_ch_num 4

namespace nanoshimarobot_hal_lib{
    class motordriver_base{
        public:

            const uint8_t motor_output_0_ = 0;
            const uint8_t motor_output_1_ = 1;

            motordriver_base(TIM_HandleTypeDef *handle, std::array<uint32_t, 4> ch)
            {
                motor_output_ch_[0] = pwmOut(handle, TIM_CHANNEL_1);
                motor_output_ch_[1] = pwmOut(handle, TIM_CHANNEL_2);
                // moto
                // motor_output_ch_.push_back({ pwmOut(handle, ch[0]), pwmOut(handle, ch[1]) });
                // motor_output_ch_.push_back({ pwmOut(handle, ch[2]), pwmOut(handle, ch[3]) });
                // motor_output_ch_.at(0) = { pwmOut(handle, ch[0]), pwmOut(handle, ch[1]) };
                // motor_output_ch_.at(1) = { pwmOut(handle, ch[2]), pwmOut(handle, ch[3]) };
                // std::array<std::tuple<pwmOut, pwmOut>, 2> motor_output_ch = {
                //     {pwmOut(handle, ch[0]), pwmOut(handle, ch[1])},
                //     {pwmOut(handle, ch[2]), pwmOut(handle, ch[3])}
                // };
                // motor_output_ch_.at(0) = std::make_pair(pwmOut(handle, ch[0]), pwmOut(handle, ch[1]));
                // motor_output_ch_.at(1) = std::make_pair(pwmOut(handle, ch[2]), pwmOut(handle, ch[3]));
            }

            void write(float duty, uint8_t motor_output_n){
                if(duty >= 0){
                    // std::get<0>(motor_output_ch_[motor_output_n]).write(duty);
                    // std::get<1>(motor_output_ch_[motor_output_n]).write(0);
                    // std::get<0>(motor_output_ch_[0]).write(1);
                    // std::get<1>(motor_output_ch_[0]).write(0);
                    motor_output_ch_[0].write(1);
                    motor_output_ch_[1].write(0);
                }else{
                    // std::get<0>(motor_output_ch_[motor_output_n]).write(0);
                    // std::get<1>(motor_output_ch_[motor_output_n]).write(std::fabs(duty));
                    // std::get<0>(motor_output_ch_[0]).write(0);
                    // std::get<1>(motor_output_ch_[0]).write(1);
                    motor_output_ch_[0].write(0);
                    motor_output_ch_[1].write(1);
                }
            }

        private:
            TIM_HandleTypeDef *handle_;
            // std::vector<std::tuple<pwmOut, pwmOut>> motor_output_ch_;
            // std::map<uint8_t, pwmOut> motor_output_ch_;
            std::array<pwmOut, 2> motor_output_ch_ = default;
    };
}