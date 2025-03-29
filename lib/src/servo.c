
#include "servo.h"

/**
 * @brief   Hàm khởi tạo Servo
 * @details Hàm này sẽ bắt đầu xung PWM và chỉnh góc Servo về phía trước
 * @param   void
 * @return  void
 **/
void PWM_Servo_Start(void){
	HAL_TIM_PWM_Start(&SERVO_TIM_HANDLE, SERVO_TIM_CHANNEL);
	SERVO_FRONT;
}

/**
 * @brief   Hàm điểu khiển góc quay Servo
 * @details Hàm này sẽ điểu khiển góc quay Servo bằng dutyCycle
 * @param   angle	Góc muốn xuay ( tính theo độ )
 * @return  void
 **/
void ServoTurn(uint8_t angle)
{
	uint16_t dutyCycle = angle* (SERVO_ANGLE_180 - SERVO_ANGLE_0) / 180 + SERVO_ANGLE_0;
    __HAL_TIM_SET_COMPARE(&SERVO_TIM_HANDLE, SERVO_TIM_CHANNEL, dutyCycle);
    HAL_Delay(SERVO_DELAY_TIME_MS);
}

