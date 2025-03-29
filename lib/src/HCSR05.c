/**********************************************************************************
 * @file    HCSR05.h
 * @brief   Thư viện điều khiển cảm biến siêu âm HCSR05
 * @details Triển khai các hàm để điều khiển cảm biến siêu âm HCSR05,
 *          bao gồm việc khởi tạo, kích hoạt cảm biến,
 *          tính toán khoảng cách và trả về hoặc hiển thị kết quả trên LCD.
 * @version 2.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/
/* ============================================[ INCLUDE FILE ]============================================*/
#include "HCSR05.h"

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm sử dụng để xử lý ngắt khi có sự kiện capture từ Timer
 * @details Hàm này sẽ được gọi khi có sự kiện capture từ Timer HCSR05.
 *          Nó sẽ đọc giá trị thời gian từ Timer và tính toán khoảng cách dựa trên thời gian xung.
 * @note    Hàm này sẽ được gọi tự động khi có sự kiện capture từ Timer.  
 * @param   htim    Handle của Timer HCSR05
 * @return  void
 **/
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM_HCSR05) {  									// Ki?m tra d�ng Timer
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {  // K�nh dang d�ng
            if (capture_flag == 0) {
                // (Rising Edge)
                rising_edge = HAL_TIM_ReadCapturedValue(htim, TIM_HCSR05_CHANNEL);
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_HCSR05_CHANNEL, TIM_INPUTCHANNELPOLARITY_FALLING);
                capture_flag = 1;
            } else {
                // (Falling Edge)
                falling_edge = HAL_TIM_ReadCapturedValue(htim, TIM_HCSR05_CHANNEL);
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_HCSR05_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
                capture_flag = 0;

                // T�nh to�n
                if (falling_edge > rising_edge) {
                    pulse_width = falling_edge - rising_edge;
										distance_measure = (pulse_width * 0.0343) / 2.0;
                } else {
                    pulse_width = (htim->Init.Period - rising_edge) + falling_edge;
										distance_measure = (pulse_width * 0.0343) / 2.0;
                }
            }
        }
    }
}


/**
 * @brief   Hàm khởi tạo cảm biến HCSR05   
 * @details Hàm này sẽ khởi tạo Timer và cấu hình các thông số cần thiết cho cảm biến HCSR05. 
 * @param   void   
 * @return  void
 **/
void HCSR05_Init(){
	HAL_TIM_IC_Start_IT(&TIM_HANDLE_HCSR05, TIM_HCSR05_CHANNEL);
	HAL_TIM_Base_Start(&TIM_HANDLE_HCSR05);
}


/**
 * @brief   Hàm kích hoạt cảm biến HCSR05 để bắt đầu quá trình đo khoảng cách   
 * @details Hàm này sẽ gửi tín hiệu kích hoạt đến chân Trigger của cảm biến HCSR05 
 *          để bắt đầu quá trình đo khoảng cách. 
 *          Sau khi kích hoạt, cảm biến sẽ gửi sóng siêu âm và chờ phản hồi từ vật thể.
 * @note    Hàm này sẽ được gọi để bắt đầu quá trình đo khoảng cách. 
 * @param   htim   
 * @return  void
 **/
void HCSR05_Trigger() {
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);  	// Set Trigger
    delay_us(10);                                     						// 1ms
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET); 	// Reset Trigger
}


/**
 * @brief   Hàm tính toán khoảng cách dựa trên thời gian xung đã đo được từ cảm biến HCSR05  
 * @details Hàm này sẽ tính toán khoảng cách dựa trên thời gian xung đã đo được từ cảm biến HCSR05.
 *          Công thức tính khoảng cách là: distance = (pulse_width * 0.0343) / 2.0 (đơn vị: cm).   
 * @param   void   
 * @return  float   Khoảng cách đã tính toán (đơn vị: cm)
 **/
float Calculate_Distance() {
    return (pulse_width * 0.0343) / 2.0;  // pw tinh bang us => pw/1.000.000 *34.300 cm /2 quang duong
}


/**
 * @brief   Hàm hiển thị khoảng cách đo được trên LCD  
 * @details Hàm này sẽ hiển thị khoảng cách đo được trên LCD.
 *          Nó sẽ sử dụng hàm sprintf để định dạng chuỗi và gửi đến LCD.
 * @note    Hàm này sẽ được gọi để hiển thị kết quả đo được trên LCD.    
 * @param   void   
 * @return  void
 **/
void HCSR05_LCD(){
		char buf[16];
		lcd_set_cursor(1,1);
		sprintf(buf,"Distance=%.1fcm",Calculate_Distance());
		lcd_send_string(buf);
}


/**
 * @brief   Hàm cập nhật khoảng cách đo được từ cảm biến HCSR05   
 * @details Hàm này sẽ kích hoạt cảm biến HCSR05 và chờ đợi kết quả đo được.
 *          Sau khi đo xong, nó sẽ trả về khoảng cách đã đo được.
 * @param   void   
 * @return  float   Khoảng cách đã đo được (đơn vị: cm)
 **/
float HCSR05_update(){
		HCSR05_Trigger();
		HAL_Delay(10);
//		HCSR05_LCD();
//		return (pulse_width * 0.0343) / 2.0;  // pw tinh bang us => pw/1.000.000 *34.300 cm /2 quang duong
		return distance_measure;
}


