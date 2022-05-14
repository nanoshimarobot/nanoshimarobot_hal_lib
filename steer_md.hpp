#pragma once
#include "main.h"
#include "motordriver_base.hpp"
#include <map>
#include <string>
#include <array>
#include <vector>

namespace nanoshimarobot_hal_lib{
    class steer_md : public motordriver_base{
        using motordriver_base::motordriver_base;

        public:
            steer_md(TIM_HandleTypeDef *handle, 
                     std::array<uint32_t, 4> ch, 
                     std::map<std::string, uint8_t> motor_output_n = { { "steer", 0 }, { "drive", 1 } }):
            motordriver_base::motordriver_base(handle, ch, motor_output_n)
            {
                
            }

            virtual void write(float duty, const std::string output_inst){
                motordriver_base::write(duty, output_inst);
            }
        
        private:
            int32_t angle_;
            int32_t speed_;
    };
}