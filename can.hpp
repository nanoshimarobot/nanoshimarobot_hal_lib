#pragma once
#include "main.h"
#include <functional>
#include <array>
#include <queue>
#include <map>
#include <utility>

namespace nanoshimarobot_hal_lib{
    // #pragma region r_1
    #ifdef HAL_CAN_MODULE_ENABLED
    using CAN_HandleType = CAN_HandleTypeDef;
    using CAN_TxHeaderType = CAN_TxHeaderTypeDef;
    using CAN_RxHeaderType = CAN_RxHeaderTypeDef;
    using CAN_FilterType = CAN_FilterTypeDef;
    #elif defined(HAL_FDCAN_MODULE_ENABLED)
    using CAN_HandleType = FDCAN_HandleTypeDef;
    using CAN_TxHeaderType = FDCAN_TxHeaderTypeDef;
    using CAN_RxHeaderType = FDCAN_RxHeaderTypeDef;
    using CAN_FilterType = FDCAN_FilterTypeDef;
    #endif
    // #pragma endregion r_1
    class Can{
        public:
            Can(CAN_HandleType *handle, uint32_t filter, uint32_t filter_mask):
            handle_(handle),
            filter_(filter),
            filter_mask_(filter_mask)
            {
                CAN_FilterType sFilterConfig;
                #ifdef HAL_CAN_MODULE_ENABLED
                if(handle_->State == HAL_CAN_STATE_READY){
                    // CAN_FilterTypeDef sFilterConfig;
			        sFilterConfig.FilterBank = 0;
			        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
			        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		    	    sFilterConfig.FilterIdHigh = filter_<< 5;
			        sFilterConfig.FilterIdLow = filter_<< 21;
			        sFilterConfig.FilterMaskIdHigh = filter_mask_ << 5;
			        sFilterConfig.FilterMaskIdLow = filter_mask_ << 21;
			        sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
			        sFilterConfig.FilterActivation = ENABLE;
			        sFilterConfig.SlaveStartFilterBank = 14;

                    if(HAL_CAN_ConfigFilter(handle_, &sFilterConfig) != HAL_OK) Error_Handler();
                    if(HAL_CAN_ActivateNotification(handle_, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) Error_Handler();
                    if(HAL_CAN_Start(handle_) != HAL_OK) Error_Handler();
                }
                #elif defined(HAL_FDCAN_MODULE_ENABLED)
                if(handle_->State == HAL_FDCAN_STATE_READY){
                    // FDCAN_FilterTypeDef sFilterConfig;
                    sFilterConfig.IdType = FDCAN_STANDARD_ID;
                    sFilterConfig.FilterIndex = 0;
                    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
                    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
                    sFilterConfig.FilterID1 = filter_;
                    sFilterConfig.FilterID2 = filter_mask_;

                    if(HAL_FDCAN_ConfigFilter(handle_, &sFilterConfig) != HAL_OK) Error_Handler();
                    if(HAL_FDCAN_ConfigGlobalFilter(handle_, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK) Error_Handler();
                    if(HAL_FDCAN_ActivateNotification(handle_, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0) != HAL_OK) Error_Handler();
                    if(HAL_FDCAN_Start(handle_) != HAL_OK) Error_Handler();
                }
                #endif
            }
        
            void write(uint32_t id, uint8_t *data, uint32_t len){
                CAN_TxHeaderType TxHeader;
                #ifdef HAL_CAN_MODULE_ENABLED
                // CAN_TxHeaderTypeDef TxHeader;
                TxHeader.StdId = id;
                TxHeader.IDE = CAN_ID_STD;
                TxHeader.RTR = CAN_RTR_DATA;
                TxHeader.DLC = len;
                TxHeader.TransmitGlobalTime = DISABLE;
                uint32_t TxMailbox;
                if(HAL_CAN_AddTxMessage(handle_, &TxHeader, data, &TxMailbox)) Error_Handler();
                
                #elif defined(HAL_FDCAN_MODULE_ENABLED)
                // FDCAN_TxHeaderTypeDef TxHeader;
                TxHeader.Identifier = id;
                TxHeader.IdType = FDCAN_STANDARD_ID;
                TxHeader.TxFrameType = FDCAN_DATA_FRAME;
                TxHeader.DataLength = len;
                TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
                TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
                TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
                TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
                TxHeader.MessageMarker = 0;
                if(HAL_FDCAN_AddMessageToTxFifoQ(handle_, &TxHeader, data)) Error_Handler();
                #endif
            }

            void attach(std::function<void(CAN_RxHeaderType& RxHeader, std::array<uint8_t, 8>&& data)> func){
                rx_callback_list_.insert(std::make_pair(handle_, func));
            }

            static void dummy_rx_callback_list_(CAN_HandleType *handle, CAN_RxHeaderType RxHeader, std::array<uint8_t,8> data){
                if(rx_callback_list_.find(handle) != rx_callback_list_.end()) rx_callback_list_.at(handle)(RxHeader, std::move(data));
            }
        private:
            CAN_HandleType *handle_;
            uint32_t filter_;
            uint32_t filter_mask_;
            inline static std::map<CAN_HandleType*, std::function<void(CAN_RxHeaderType&, std::array<uint8_t,8>&&)>> rx_callback_list_;
    };
}

extern "C"{
    #ifdef HAL_CAN_MODULE_ENABLED
    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *handle){
        /*
        nanoshimarobot_hal_lib::CAN_RxHeaderType RxHeader;
        std::array<uint8_t, 8> data;
        HAL_CAN_GetRxMessage(handle, CAN_RX_FIFO0, &RxHeader, data.data());
        nanoshimarobot_hal_lib::Can::dummy_rx_callback_list_(handle, RxHeader, data);
        */
        printf("CAN received msg\r\n");
    }

    #elif defined(HAL_FDCAN_MODULE_ENABLED)
    void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *handle, uint32_t RxFifo0ITs){
        [[maybe_unused]] FDCAN_RxHeaderTypeDef RxHeader;
        printf("FDCAN received msg\r\n");
    }
    #endif
}