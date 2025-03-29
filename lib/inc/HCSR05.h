/**********************************************************************************
 * @file    HCSR05.h
 * @brief   Thư viện điều khiển cảm biến siêu âm HCSR05
 * @details Thư viện các hàm để điều khiển cảm biến siêu âm HCSR05,
 *          bao gồm việc khởi tạo, kích hoạt cảm biến,
 *          tính toán khoảng cách và trả về hoặc hiển thị kết quả trên LCD.
 * @version 2.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/

/* =====================================================[ Guard ]====================================================*/
#ifndef __HCSR05_H__
#define __HCSR05_H__

/* ============================================[ INCLUDE FILE ]============================================*/
#include "main.h"                   //**< Thư viện chứa các định nghĩa GPIO và hàm HAL >**/
#include "stdio.h"                  //**< Thư viện chứa hàm sprintf >**/

/* ==========================================[ MACRO DEFINITIONS ]==========================================*/
#define	TIM_HCSR05			TIM2                //**< Timer sử dụng cho cảm biến HCSR05         >**/
#define	TIM_HCSR05_CHANNEL	TIM_CHANNEL_1       //**< Kênh sử dụng cho cảm biến HCSR05          >**/
#define TIM_HANDLE_HCSR05	htim2               //**< Handle Timer sử dụng cho cảm biến HCSR05  >**/
#define TIM_HANDLE_COUNTER 	htim12              //**< Handle Timer sử dụng cho cảm biến HCSR05  >**/

/* ===========================================[ TYPE DEFINITIONS ]==========================================*/
extern TIM_HandleTypeDef TIM_HANDLE_HCSR05;     //**< Handle Timer sử dụng cho cảm biến HCSR05  >**/
extern TIM_HandleTypeDef TIM_HANDLE_COUNTER;    //**< Handle Timer sử dụng cho cảm biến HCSR05  >**/

volatile uint32_t   rising_edge = 0;            //**< Thời gian rising edge         >**/
volatile uint32_t   falling_edge = 0;           //**< Thời gian falling edge        >**/
volatile uint32_t   pulse_width = 0;            //**< Thời gian xung                >**/
volatile uint32_t   distance_measure = 0;       //**< Khoảng cách đo được           >**/
volatile uint8_t    capture_flag = 0;           //**< Cờ để xác định trạng thái đo  >**/

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm sử dụng để xử lý ngắt khi có sự kiện capture từ Timer
 * @details Hàm này sẽ được gọi khi có sự kiện capture từ Timer HCSR05.
 *          Nó sẽ đọc giá trị thời gian từ Timer và tính toán khoảng cách dựa trên thời gian xung.
 * @note    Hàm này sẽ được gọi tự động khi có sự kiện capture từ Timer.  
 * @param   htim    Handle của Timer HCSR05
 * @return  void
 **/
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

/**
 * @brief   Hàm sử dụng để tạo độ trễ trong micro giây      
 * @details Hàm này sẽ tạo độ trễ trong khoảng thời gian được chỉ định bằng micro giây.  
 * @param   us    Thời gian trễ cần tạo, tính bằng micro giây  
 * @return  void
 **/
extern void delay_us(uint16_t us);

/**
 * @brief   Hàm khởi tạo cảm biến HCSR05   
 * @details Hàm này sẽ khởi tạo Timer và cấu hình các thông số cần thiết cho cảm biến HCSR05. 
 * @param   void   
 * @return  void
 **/
void HCSR05_Init(void);

/**
 * @brief   Hàm kích hoạt cảm biến HCSR05 để bắt đầu quá trình đo khoảng cách   
 * @details Hàm này sẽ gửi tín hiệu kích hoạt đến chân Trigger của cảm biến HCSR05 
 *          để bắt đầu quá trình đo khoảng cách. 
 *          Sau khi kích hoạt, cảm biến sẽ gửi sóng siêu âm và chờ phản hồi từ vật thể.
 * @note    Hàm này sẽ được gọi để bắt đầu quá trình đo khoảng cách. 
 * @param   htim   
 * @return  void
 **/
void HCSR05_Trigger(void);

/**
 * @brief   Hàm tính toán khoảng cách dựa trên thời gian xung đã đo được từ cảm biến HCSR05  
 * @details Hàm này sẽ tính toán khoảng cách dựa trên thời gian xung đã đo được từ cảm biến HCSR05.
 *          Công thức tính khoảng cách là: distance = (pulse_width * 0.0343) / 2.0 (đơn vị: cm).
 * @note    Hàm này sẽ được gọi để tính toán khoảng cách sau khi đã kích hoạt cảm biến.
 *          Kết quả sẽ được lưu trữ trong biến distance_measure.    
 * @param   void   
 * @return  float   Khoảng cách đã tính toán (đơn vị: cm)
 **/
float Calculate_Distance(void);

/**
 * @brief   Hàm hiển thị khoảng cách đo được trên LCD  
 * @details Hàm này sẽ hiển thị khoảng cách đo được trên LCD.
 *          Nó sẽ sử dụng hàm sprintf để định dạng chuỗi và gửi đến LCD.
 * @note    Hàm này sẽ được gọi để hiển thị kết quả đo được trên LCD.    
 * @param   void   
 * @return  void
 **/
void HCSR05_LCD(void);


/**
 * @brief   Hàm cập nhật khoảng cách đo được từ cảm biến HCSR05   
 * @details Hàm này sẽ kích hoạt cảm biến HCSR05 và chờ đợi kết quả đo được.
 *          Sau khi đo xong, nó sẽ trả về khoảng cách đã đo được.
 * @param   void   
 * @return  void
 **/
float HCSR05_update(void);

/* =====================================================[ Guard ]====================================================*/
#endif


