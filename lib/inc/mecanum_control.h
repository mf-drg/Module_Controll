/*********************************************************************************************************************
 * @file    mecanum_control.h
 * @brief   Thư viện điều khiển động cơ Mecanum
 * @details Thư viện các hàm để điều khiển động cơ Mecanum,
 *          bao gồm việc khởi tạo, điều khiển tốc độ và hướng di chuyển của xe.
 *          Các hàm này sẽ sử dụng PWM để điều khiển tốc độ động cơ và GPIO để điều khiển hướng di chuyển.
 *          Các hàm này sẽ được gọi từ các hàm khác trong chương trình để điều khiển xe theo yêu cầu.
 * @version 4.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* =====================================================[ Guard ]====================================================*/
#ifndef __MECANUM_CONTROL_H__
#define __MECANUM_CONTROL_H__

/* ============================================[ INCLUDE FILE ]============================================*/
#include <math.h>               //**< Thư viện toán học                             >**/
#include <main.h>               //**< Thư viện chứa các định nghĩa GPIO và hàm HAL  >**/
#include "74HC595.h"            //**< Thư viện điều khiển 74HC595                   >**/

/*
 *  [0]--|||--[1]
 *   |         |
 *   |         |
 *   |         |
 *   |         |
 *  [3]-------[2]
 */
/* ============================================[ MACRO DEFINITIONS ]==========================================*/
#define IN4_MASK 	0x01                        //**< BIT 0 - 74HC595 >**/
#define IN3_MASK 	0x02                        //**< BIT 1 - 74HC595 >**/
#define IN2_MASK 	0x04                        //**< BIT 2 - 74HC595 >**/
#define IN1_MASK 	0x08                        //**< BIT 3 - 74HC595 >**/
#define IN8_MASK 	0x10                        //**< BIT 4 - 74HC595 >**/
#define IN7_MASK 	0x20                        //**< BIT 5 - 74HC595 >**/
#define IN6_MASK 	0x40                        //**< BIT 6 - 74HC595 >**/
#define IN5_MASK 	0x80                        //**< BIT 7 - 74HC595 >**/

/** 
 * Motor [0] 
 **/
#define MOTOR0_PWM0_PIN     GPIO_PIN_8          //**< GPIO - PWM - PIN  >**/
#define MOTOR0_PWM0_GPIO    GPIOA               //**< GPIO - PWM - PORT >**/

#define MOTOR0_PWM0_IN1		IN1_MASK	        //**< BIT 3 - 74HC595 >**/ 
#define MOTOR0_PWM0_IN2		IN2_MASK            //**< BIT 2 - 74HC595 >**/

#define TIM_HandlePWM0      htim1               //**< Handle Timer sử dụng cho động cơ 0 >**/
#define TIM_CHANNEL_PWM0    TIM_CHANNEL_1       //**< Kênh sử dụng cho động cơ 0         >**/

/** 
 * Motor [1] 
 **/
#define MOTOR1_PWM1_PIN     GPIO_PIN_9          //**< GPIO - PWM - PIN  >**/
#define MOTOR1_PWM1_GPIO    GPIOA               //**< GPIO - PWM - PORT >**/

#define TIM_HandlePWM1      htim1               //**< Handle Timer sử dụng cho động cơ 1 >**/
#define TIM_CHANNEL_PWM1    TIM_CHANNEL_2       //**< Kênh sử dụng cho động cơ 1         >**/

#define MOTOR1_PWM1_IN1		IN3_MASK	        //**< BIT 1 - 74HC595 >**/
#define MOTOR1_PWM1_IN2		IN4_MASK            //**< BIT 0 - 74HC595 >**/

/** 
 * Motor [2] 
 **/
#define MOTOR2_PWM2_PIN     GPIO_PIN_10         //**< GPIO - PWM - PIN  >**/
#define MOTOR2_PWM2_GPIO    GPIOA               //**< GPIO - PWM - PORT >**/

#define MOTOR2_PWM2_IN1		IN5_MASK            //**< BIT 7 - 74HC595 >**/	 
#define MOTOR2_PWM2_IN2		IN6_MASK            //**< BIT 6 - 74HC595 >**/

#define TIM_HandlePWM2      htim1               //**< Handle Timer sử dụng cho động cơ 2 >**/
#define TIM_CHANNEL_PWM2    TIM_CHANNEL_3       //**< Kênh sử dụng cho động cơ 2         >**/

/** 
 * Motor [3] 
 **/
#define MOTOR3_PWM3_PIN     GPIO_PIN_11         //**< GPIO - PWM - PIN  >**/
#define MOTOR3_PWM3_GPIO    GPIOA               //**< GPIO - PWM - PORT >**/

#define MOTOR3_PWM3_IN1		IN7_MASK            //**< BIT 5 - 74HC595 >**/	 
#define MOTOR3_PWM3_IN2		IN8_MASK            //**< BIT 4 - 74HC595 >**/

#define TIM_HandlePWM3      htim1               //**< Handle Timer sử dụng cho động cơ 3 >**/
#define TIM_CHANNEL_PWM3    TIM_CHANNEL_4       //**< Kênh sử dụng cho động cơ 3         >**/


#define MOTOR_POWER_PWM_MIN 100   // PWM tối thiểu cho động cơ
#define MOTOR_POWER_PWM_MAX 999   // PWM tối đa cho động cơ

#define MOTOR_START_POWER 100     // PWM khởi động cho động cơ

/** Set hướng quay cho từng bánh */
/** Front-Left  ->  [0] ->  GPIO - PIN
 *  Front-Right ->  [1] ->  GPIO - PIN
 *  Rear-Right  ->  [2] ->  GPIO - PIN
 *  Rear-Left   ->  [3] ->  GPIO - PIN
 */ 
#define MOTOR_DIRECTIONS (uint8_t[4]){0, 1, 1, 0}       //**< Hướng quay của động cơ >**/  

#define CAR_DEFAULT_POWER 80                            //**< Tốc độ mặc định của động cơ >**/      

/* =============================================[ TYPE DEFINITIONS ]==========================================*/
uint8_t dataDirMotor = 0;                               //**< Dữ liệu hướng điều khiển động cơ  >**/

extern TIM_HandleTypeDef TIM_HandlePWM0;                //**< Handle Timer sử dụng cho PWM      >**/   

//**< Mảng con trỏ hàm truyền dữ liệu động cơ >**/
void (*controlMotor[4])(uint8_t dir, int16_t power) = {PWMControlMotor0, PWMControlMotor1, PWMControlMotor2, PWMControlMotor3}; 

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo động cơ Mecanum
 * @details Hàm này sẽ khởi tạo các thông số cần thiết cho động cơ Mecanum,
 *          bao gồm việc cấu hình GPIO và Timer cho các động cơ.
 * @param   void
 * @return  void
 **/
void carBegin(void);


/**
 * @brief   Các hàm điều khiển động cơ Mecanum
 * @details Các hàm này sẽ điều khiển động cơ Mecanum 
 *          dựa trên các trường hợp cấu hình sẵn khác nhau.
 *          Nó sẽ điều khiển các động cơ theo hướng và 
 *          tốc độ được chỉ định theo các hàm đã cấu hình sẵn.
 * @note    Hàm này sẽ gọi 1 hàm khác để xử lý thông số của từng trường hợp cụ thể.
 * @param   power   Công suất động cơ (0 - 1000)
 * @return  void
 **/
void carSetMotors(int16_t power0, int16_t power1, int16_t power2, int16_t power3);
void carForward(int16_t power);
void carBackward(int16_t power);
void carTurnLeft(int16_t power);
void carTurnRight(int16_t power);
void carLeftForword(int16_t power);
void carRightForword(int16_t power);
void carLeftBackward(int16_t power);
void carRightBackward(int16_t power);
void carLeft(int16_t power);
void carRight(int16_t power);
void carTurnBack(int16_t power);
void carTurnLeftDrift(int16_t power);
void carTurnRightDrift(int16_t power);
void carStop(void);


/**
 * @brief   Hàm truyền dữ liệu điều khiển động cơ 0  
 * @details Hàm này sẽ truyền dữ liệu điều khiển động cơ 0 thông qua giao thức 74HC595.
 *          Tính toán xung PWM cho công suất của động cơ 0.
 *          Và sử dụng hàm send_74HC595_8bit để gửi dữ liệu bit
 *          điều khiển hướng quay của bánh xe đến khối động cơ.
 * @param   dir       Hướng quay của động cơ (1: tiến, 0: lùi)
 * @param   powerPWM  Tốc độ động cơ theo PWM (0 - 1000)
 * @return  void
 **/
void PWMControlMotor0(uint8_t dir, int16_t powerPWM);

/**
 * @brief   Hàm truyền dữ liệu điều khiển động cơ 1  
 * @details Hàm này sẽ truyền dữ liệu điều khiển động cơ 1 thông qua giao thức 74HC595.
 *          Tính toán xung PWM cho công suất của động cơ 1.
 *          Và sử dụng hàm send_74HC595_8bit để gửi dữ liệu bit
 *          điều khiển hướng quay của bánh xe đến khối động cơ.
 * @param   dir       Hướng quay của động cơ (1: tiến, 0: lùi)
 * @param   powerPWM  Tốc độ động cơ theo PWM (0 - 1000)
 * @return  void
 **/
void PWMControlMotor1(uint8_t dir, int16_t powerPWM);

/**
 * @brief   Hàm truyền dữ liệu điều khiển động cơ 2  
 * @details Hàm này sẽ truyền dữ liệu điều khiển động cơ 2 thông qua giao thức 74HC595.
 *          Tính toán xung PWM cho công suất của động cơ 2.
 *          Và sử dụng hàm send_74HC595_8bit để gửi dữ liệu bit
 *          điều khiển hướng quay của bánh xe đến khối động cơ.
 * @param   dir       Hướng quay của động cơ (1: tiến, 0: lùi)
 * @param   powerPWM  Tốc độ động cơ theo PWM (0 - 1000)
 * @return  void
 **/
void PWMControlMotor2(uint8_t dir, int16_t powerPWM);

/**
 * @brief   Hàm truyền dữ liệu điều khiển động cơ 3  
 * @details Hàm này sẽ truyền dữ liệu điều khiển động cơ 3 thông qua giao thức 74HC595.
 *          Tính toán xung PWM cho công suất của động cơ 3.
 *          Và sử dụng hàm send_74HC595_8bit để gửi dữ liệu bit
 *          điều khiển hướng quay của bánh xe đến khối động cơ.
 * @param   dir       Hướng quay của động cơ (1: tiến, 0: lùi)
 * @param   powerPWM  Tốc độ động cơ theo PWM (0 - 1000)
 * @return  void
 **/
void PWMControlMotor3(uint8_t dir, int16_t powerPWM);


/**
 * @brief   Hàm cập nhật dữ liệu điều khiển động cơ
 * @details Hàm này sẽ gửi dữ liệu điều khiển động cơ thông qua giao thức 74HC595.
 *          Nó sẽ gửi dữ liệu bit điều khiển hướng quay của động cơ đến khối động cơ.
 * @param   void
 * @return  void
 **/
void DirControlMotorUpdate(void);


/**
 * @brief   Hàm điều khiển xe theo hướng góc và tốc độ
 * @details Hàm này sẽ điều khiển động cơ Mecanum dựa trên các tham số đầu vào,
 *          bao gồm góc, tốc độ và chế độ điều khiển.
 * @param   angle   Hướng di chuyển (0-360 độ)
 * @param   power   Tốc độ di chuyển (0-100%)
 * @param   rot     Góc quay cố định của ô tô trong quá trình chuyển động
 * @param   drift   Chế độ drift (0: không drift, 1: drift trái, 2: drift phải)
 * @return  void
 **/
void carMove(int16_t angle, int16_t power, int8_t rot, uint8_t drift);


/**
 * @brief   Hàm điều khiển xe theo hướng góc và tốc độ
 * @details Hàm này sẽ điều khiển động cơ Mecanum dựa trên các tham số đầu vào,
 *          bao gồm góc, tốc độ và chế độ điều khiển.
 * @param   angle   Hướng di chuyển (0-360 độ)
 * @param   power   Tốc độ di chuyển (0-100%)
 * @param   rot     Góc quay cố định của ô tô trong quá trình chuyển động
 * @param   drift   Chế độ drift (0: không drift, 1: drift trái, 2: drift phải)
 * @return  void
 **/
void carSetMotors(int16_t power0, int16_t power1, int16_t power2, int16_t power3);

/* =====================================================[ Guard ]====================================================*/
#endif // __CAR_CONTROL_H__

