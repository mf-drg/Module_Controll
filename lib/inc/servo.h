/*********************************************************************************************************************
 * @file    servo.h
 * @brief   Thư viện đọc điều khiển Servo SG90 bằng STM32
 * @details Thư viện các hàm để điều khiển Servo SG90 bằng STM32
 * @version 1.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* =====================================================[ Guard ]====================================================*/
#ifndef __SERVO_H__
#define __SERVO_H__

/**
*  Servo SG90 
*   f = 50 Hz, T = 20ms
*   xung PWM 0.5 ms -> 2.5 ms tương ứng 0 - 180 độ
**/ 

/* ============================================[ INCLUDE FILE ]============================================*/
#include "main.h"                   //**< Thư viện chứa các định nghĩa GPIO và hàm HAL >**/


/* ============================================[ MACRO DEFINITIONS ]==========================================*/
#define SERVO_FREQUENCY	    50                          //**< Tần số hoạt động của Servo    >**/
#define SERVO_PERIOD		1000                        //**< Chu kỳ config để tạo xung     >**/
//PRESCALE = CLOCK / ( SERVO_FREQUENCY * SERVO_PERIOD) - 1

#define SERVO_ANGLE_180	    SERVO_PERIOD * 25 / 200     //**< Độ rộng xung tại góc 180 độ   >**/
#define SERVO_ANGLE_0		SERVO_PERIOD * 5 / 200      //**< Độ rộng xung tại góc 0 độ     >**/

#define SERVO_LEFT          ServoTurn(180)              //**< Servo quay trái               >**/
#define SERVO_FRONT         ServoTurn(90)               //**< Servo hướng phía trước        >**/
#define SERVO_RIGHT         ServoTurn(0)                //**< Servo quay phải               >**/

#define	SERVO_DELAY_TIME_MS	100                         //**< Thời gian delay ms            >**/
#define	SERVO_TIM_HANDLE    htim3                       //**< Handle Timer sử dụng tạo xung >**/
#define	SERVO_TIM_CHANNEL   TIM_CHANNEL_1               //**< Kênh sử dụng cho servo        >**/

/* =============================================[ TYPE DEFINITIONS ]==========================================*/
extern TIM_HandleTypeDef SERVO_TIM_HANDLE;              //**< Handle Timer sử dụng tạo xung >**/

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo Servo
 * @details Hàm này sẽ bắt đầu xung PWM và chỉnh góc Servo về phía trước
 * @param   void
 * @return  void
 **/
void PWM_Servo_Start(void);


/**
 * @brief   Hàm điểu khiển góc quay Servo
 * @details Hàm này sẽ điểu khiển góc quay Servo bằng dutyCycle
 * @param   angle	Góc muốn xuay ( tính theo độ )
 * @return  void
 **/
void ServoTurn(uint8_t angle);

#endif


