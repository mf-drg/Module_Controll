/*********************************************************************************************************************
 * @file    mecanum_control.c
 * @brief   Thư viện điều khiển động cơ Mecanum
 * @details Triển khai các hàm để điều khiển động cơ Mecanum,
 *          bao gồm việc khởi tạo, điều khiển tốc độ và hướng di chuyển của xe.
 *          Các hàm này sẽ sử dụng PWM để điều khiển tốc độ động cơ và GPIO để điều khiển hướng di chuyển.
 *          Các hàm này sẽ được gọi từ các hàm khác trong chương trình để điều khiển xe theo yêu cầu.
 * @version 4.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* ===============================================[ INCLUDE FILE ]============================================*/
#include "mecanum.h"                          //**< Thư viện chứa các hàm điều khiển động cơ Mecanum >**/
#include <stm32f4xx_hal.h>                    //**< Thư viện HAL cho STM32F4 >**/

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo động cơ Mecanum
 * @details Hàm này sẽ khởi tạo các thông số cần thiết cho động cơ Mecanum,
 *         bao gồm việc cấu hình GPIO và Timer cho các động cơ.
 * @param   void
 * @return  void
 **/
void carBegin() {
	HAL_TIM_Base_Start(&TIM_HandlePWM0);
	
    // Khởi tạo PWM cho động cơ
  HAL_TIM_PWM_Start(&TIM_HandlePWM0, TIM_CHANNEL_PWM0);
	HAL_TIM_PWM_Start(&TIM_HandlePWM1, TIM_CHANNEL_PWM1);
	HAL_TIM_PWM_Start(&TIM_HandlePWM2, TIM_CHANNEL_PWM2);
	HAL_TIM_PWM_Start(&TIM_HandlePWM3, TIM_CHANNEL_PWM3);
}


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
void carForward(int16_t power)       	{ carMove(   0  , power	, 0		, 0); }
void carBackward(int16_t power)      	{ carMove( 180  , power	, 0		, 0); }
void carLeft(int16_t power)          	{ carMove( -90  , power	, 0		, 0); }
void carRight(int16_t power)         	{ carMove(  90  , power	, 0		, 0); }
void carTurnLeft(int16_t power)      	{ carMove(   0  , 0		, power	, 0); }
void carTurnRight(int16_t power)     	{ carMove(   0  , 0		, -power, 0); }
void carLeftForword(int16_t power)   	{ carMove( -45  , power	, 0		, 0); }
void carRightForword(int16_t power)  	{ carMove(  45  , power	, 0		, 0); }
void carLeftBackward(int16_t power)  	{ carMove(-135  , power	, 0		, 0); }
void carRightBackward(int16_t power) 	{ carMove( 135  , power	, 0		, 0); }
void carTurnBack(int16_t power)			{ carMove( 0	, 0		, power	, 0); }
void carLeftHead(int16_t power)		    { carMove( 0	, power	, power	, 1); }
void carRightHead(int16_t power)		{ carMove( 0	, power	, power	, 1); }
void carTurnLeftDrift(int16_t power) 	{ carMove( 90	, power	, power	, 2); }
void carTurnRightDrift(int16_t power)	{ carMove( -90  , power	, power	, 2); }
void carStop(void)                      { carMove(   0  , 0		, 0		, 0); }



/**
 * @brief   Hàm nội bộ chuyển đổi giá trị từ khoảng này sang khoảng khác  
 * @details Hàm này sẽ chuyển đổi giá trị từ khoảng minValue - maxValue sang khoảng minPWM - maxPWM.
 *          Nó sẽ sử dụng công thức để tính toán giá trị mới dựa trên tỷ lệ giữa các khoảng.
 * @param   value   Giá trị cần chuyển đổi
 * @param   minValue  Giá trị tối thiểu của khoảng đầu vào
 * @param   maxValue  Giá trị tối đa của khoảng đầu vào
 * @param   minPWM    Giá trị tối thiểu của khoảng đầu ra
 * @param   maxPWM    Giá trị tối đa của khoảng đầu ra
 * @return  uint32_t  Giá trị đã chuyển đổi sang khoảng PWM
 **/
static uint32_t map(int32_t value,uint32_t minValue, uint32_t maxValue, uint32_t minPWM, uint32_t maxPWM){
    value = (value < 0) ? (-value) : value;
    if(value == 0)
        return 0;
    return (value - minValue) * (maxPWM - minPWM) / (maxValue - minValue) + minPWM;
}


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
void PWMControlMotor0(uint8_t dir, int16_t powerPWM){
    dataDirMotor &= ~(MOTOR0_PWM0_IN1 | MOTOR0_PWM0_IN2);                   //**< clear bit IN1 và IN2    >**/
    dataDirMotor |= (dir ? MOTOR0_PWM0_IN1 : MOTOR0_PWM0_IN2);              //**< set lại bit IN1 và IN2  >**/
		
    __HAL_TIM_SET_COMPARE(&TIM_HandlePWM0, TIM_CHANNEL_PWM0, powerPWM);     //**< set PWM >**/
}


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
void PWMControlMotor1(uint8_t dir, int16_t powerPWM){
    dataDirMotor &= ~(MOTOR1_PWM1_IN1 | MOTOR1_PWM1_IN2);;                  //**< clear bit IN1 và IN2    >**/ 
    dataDirMotor |= (dir ? MOTOR1_PWM1_IN1 : MOTOR1_PWM1_IN2);              //**< set lại bit IN1 và IN2  >**/
		
    __HAL_TIM_SET_COMPARE(&TIM_HandlePWM1, TIM_CHANNEL_PWM1, powerPWM);     //**< set PWM >**/
}


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
void PWMControlMotor2(uint8_t dir, int16_t powerPWM){
    dataDirMotor &= ~(MOTOR2_PWM2_IN1 | MOTOR2_PWM2_IN2);                   //**< clear bit IN1 và IN2    >**/
    dataDirMotor |= (dir ? MOTOR2_PWM2_IN1 : MOTOR2_PWM2_IN2);              //**< set lại bit IN1 và IN2  >**/
		
    __HAL_TIM_SET_COMPARE(&TIM_HandlePWM2, TIM_CHANNEL_PWM2, powerPWM);     //**< set PWM >**/
}


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
void PWMControlMotor3(uint8_t dir, int16_t powerPWM){  
    dataDirMotor &= ~(MOTOR3_PWM3_IN1 | MOTOR3_PWM3_IN2);                   //**< clear bit IN1 và IN2    >**/
    dataDirMotor |= (dir ? MOTOR3_PWM3_IN1 : MOTOR3_PWM3_IN2);              //**< set lại bit IN1 và IN2  >**/
		
    __HAL_TIM_SET_COMPARE(&TIM_HandlePWM3, TIM_CHANNEL_PWM3, powerPWM);     //**< set PWM >**/
}


/**
 * @brief   Hàm cập nhật dữ liệu điều khiển hướng quay động cơ
 * @details Hàm này sẽ gửi dữ liệu điều khiển động cơ thông qua giao thức 74HC595.
 *          Nó sẽ gửi dữ liệu bit điều khiển hướng quay của động cơ đến khối động cơ.
 * @param   void
 * @return  void
 **/
void DirControlMotorUpdate(void){
	send_74HC595_8bit(dataDirMotor);
}



/**
 * @brief   Hàm điều khiển động cơ Mecanum
 * @details Hàm này sẽ điều khiển động cơ Mecanum dựa trên các tham số đầu vào,
 *          bao gồm công suất và hướng quay của từng động cơ.
 * @param   power0   Công suất động cơ 0 (0 - 1000)
 * @param   power1   Công suất động cơ 1 (0 - 1000)
 * @param   power2   Công suất động cơ 2 (0 - 1000)
 * @param   power3   Công suất động cơ 3 (0 - 1000)
 * @return  void
 **/
void carSetMotors(int16_t power0, int16_t power1, int16_t power2, int16_t power3) {
    uint8_t dir[4];                                              //**< Hướng quay của động cơ >**/
    int16_t power[4] = {power0, power1, power2, power3};         //**< Công suất động cơ      >**/
    int16_t PWM[4];                                              //**< PWM động cơ            >**/

    for (uint8_t i = 0; i < 4; i++) {
        dir[i] = power[i] > 0;

        if (MOTOR_DIRECTIONS[i])                                //**< Nếu motor đảo chiều, thay đổi hướng >**/
            dir[i] = !dir[i];

        if (power[i] == 0) {                                    //**< Nếu công suất bằng 0, không điều khiển động cơ >**/     
            PWM[i] = 0;
        } else {
            PWM[i] = map(power[i], 0, 100, MOTOR_POWER_PWM_MIN, MOTOR_POWER_PWM_MAX);
        }    

        if (PWM[i] != 0 && PWM[i] < MOTOR_POWER_PWM_MIN) {      //**< Nếu PWM nhỏ hơn giá trị tối thiểu, điều khiển động cơ với giá trị tối thiểu >**/
            controlMotor[i](dir[i], MOTOR_POWER_PWM_MIN);
        }
				else if(PWM[i] > MOTOR_POWER_PWM_MAX){
						controlMotor[i](dir[i], MOTOR_POWER_PWM_MAX);
				}
        else{
            controlMotor[i](dir[i], PWM[i]);
        }
        //delayMs(200);
    }
		DirControlMotorUpdate();                                    //**< Cập nhật dữ liệu điều khiển động cơ >**/
}


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
void carMove(int16_t angle, int16_t power, int8_t rot, uint8_t drift) {
    int16_t power_0, power_1, power_2, power_3;
    float ratio;

    angle += 90;                      //**< Chuyển đổi góc về hướng tiến lên là 0 >**/
    float rad = angle * 3.1416 / 180; //**< Chuyển đổi góc sang radian            >**/
		
		if(drift){
			ratio = 0.3	;
		}
		else{
			ratio = 0.5;
		}
			
		if((angle % 90) != 0){
				power /= sqrt(2);	
		}
		
		if(rot != 0){
			power = power * (1-ratio);
			if(power){
				rot /= 2;
			}			
		}

    // tính công suất các động cơ
    if (drift == 1) {
        power_0 = (power * sin(rad) - power * cos(rad)) - rot * cos(rad) * ratio ;
        power_1 = (power * sin(rad) + power * cos(rad)) + rot * cos(rad) * ratio ;
        power_2 = (power * sin(rad) - power * cos(rad)) ;
        power_3 = (power * sin(rad) + power * cos(rad)) ; 
    } else if(drift == 2) {
        power_0 = (power * sin(rad) - power * cos(rad)*cos(rad + 3.1416)) ;
        power_1 = (power * sin(rad) + power * cos(rad)*cos(rad )) ;
        power_2 = (power * sin(rad) - power * cos(rad)) - rot * cos(rad) * ratio;
        power_3 = (-power * sin(rad) + power * cos(rad)) + rot * cos(rad) * ratio;
    } else{
        power_0 = (power * sin(rad) - power * cos(rad)) - rot * ratio * 2;
        power_1 = (power * sin(rad) + power * cos(rad)) + rot * ratio * 2;
        power_2 = (power * sin(rad) - power * cos(rad)) + rot * ratio * 2;
        power_3 = (power * sin(rad) + power * cos(rad)) - rot * ratio * 2;
    }
  
    carSetMotors(power_0, power_1, power_2, power_3);             //**< Gọi hàm điều khiển động cơ >**/
}



