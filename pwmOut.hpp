#pragma once
#include "main.h"
// #include 
namespace nanoshimarobot_hal_lib{

// typedef __uint32_t uint32_t;

#ifdef HAL_TIM_MODULE_ENABLED
class pwmOut{
    public:
        pwmOut(TIM_HandleTypeDef *handle, uint32_t ch, uint32_t max_value = 999):
            handle_(handle),
            ch_(ch),
            max_value_(max_value)
            {
                startup();
            }

        void write(float value){
            if(value < (float)0.0) value = 0.0;
            else if(value > (float)1.0) value = 1.0;

            uint32_t pulse = (uint32_t)(handle_->Init.Period * value);
            printf("%d\r\n",pulse);
            if(pulse > max_value_) pulse = max_value_;

            __HAL_TIM_SET_COMPARE(handle_, ch_, pulse);
        }

        void pulsewidth_us(int us){
            // uint
            float value = (float)us / (float)handle_->Init.Period;
            write(value);
        }
    
    protected:
        void startup(){
            __HAL_TIM_SET_COMPARE(handle_, ch_, 0);
            HAL_TIM_PWM_Start(handle_,ch_);
        }

    private:
        TIM_HandleTypeDef *handle_;
        uint32_t ch_;
        uint32_t max_value_;
};
#endif
}