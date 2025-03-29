/*********************************************************************************************************************
 * @file    ps2.h
 * @brief   Thư viện đọc điều khiển tay cầm PS2 bằng STM32
 * @details Thư viện này cung cấp các hàm để đọc điều khiển tay cầm PS2 thông qua giao thức SPI.
 *          Nó bao gồm các hàm để khởi tạo, cấu hình, đọc dữ liệu từ tay cầm PS2,
 *          và xử lý các sự kiện từ tay cầm.
 * @version 3.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* =====================================================[ Guard ]====================================================*/
#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

/* ============================================[ INCLUDE FILE ]============================================*/
#include <stdlib.h>                     //**< Thư viện chuẩn sử dụng hàm malloc >**/
#include <stdint.h>                     //**< Thư viện sử dụng kiểu dữ liệu uint >**/
#include "main.h"                       //**< Thư viện chứa các định nghĩa GPIO và hàm HAL >**/

/* ============================================[ MACRO DEFINITIONS ]==========================================*/
#define PS2_CS_PORT 	GPIOC           //**< GPIO - CS - SPI - PORT >**/
#define PS2_CS_PIN 		GPIO_PIN_4      //**< GPIO - CS - SPI - PIN  >**/

#define CS_LOW 		HAL_GPIO_WritePin(PS2_CS_PORT, PS2_CS_PIN, GPIO_PIN_RESET)      //**< CS LOW  >**/
#define CS_HIGH 	HAL_GPIO_WritePin(PS2_CS_PORT, PS2_CS_PIN, GPIO_PIN_SET)        //**< CS HIGH >**/

#define PS2_SPI_HANDLE  hspi1           //**< Handle SPI sử dụng cho PS2        >**/ 
#define PS2_TIM_COUNTER htim12          //**< Handle Timer sử dụng cho delay    >**/ 

/*
 * Last 6 bytes from 0x42 response (Analog Mode):
 *
 * 	Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
 * 	SLCT L3   R3   STRT UP   RGHT DOWN LEFT - Leftmost Byte
 * 	L2   R2    L1  R1   /\   O    X    |_|
 * 	Right Joy 0x00 = Left  0xFF = Right
 * 	Right Joy 0x00 = Up    0xFF = Down
 * 	Left Joy  0x00 = Left  0xFF = Right
 * 	Left Joy  0x00 = Up    0xFF = Down      - Rightmost Byte
*/

// Bitmask của các nút bấm
#define PSB_SELECT      0x0001          //**< BYTE 03 - BIT 1 >**/
#define PSB_L3          0x0002          //**< BYTE 03 - BIT 2 >**/ 
#define PSB_R3          0x0004          //**< BYTE 03 - BIT 3 >**/ 
#define PSB_START       0x0008          //**< BYTE 03 - BIT 4 >**/ 
#define PSB_PAD_UP      0x0010          //**< BYTE 03 - BIT 5 >**/ 
#define PSB_PAD_RIGHT   0x0020          //**< BYTE 03 - BIT 6 >**/ 
#define PSB_PAD_DOWN    0x0040          //**< BYTE 03 - BIT 7 >**/ 
#define PSB_PAD_LEFT    0x0080          //**< BYTE 03 - BIT 8 >**/ 

#define PSB_L2          0x0100          //**< BYTE 04 - BIT 1 >**/ 
#define PSB_R2          0x0200          //**< BYTE 04 - BIT 2 >**/ 
#define PSB_L1          0x0400          //**< BYTE 04 - BIT 3 >**/ 
#define PSB_R1          0x0800          //**< BYTE 04 - BIT 4 >**/ 
#define PSB_GREEN       0x1000          //**< BYTE 04 - BIT 5 >**/ 
#define PSB_RED         0x2000          //**< BYTE 04 - BIT 6 >**/ 
#define PSB_BLUE        0x4000          //**< BYTE 04 - BIT 7 >**/ 
#define PSB_PINK        0x8000          //**< BYTE 04 - BIT 8 >**/ 

#define PSB_TRIANGLE    0x1000          //**< BYTE 04 - BIT 5 >**/ 
#define PSB_CIRCLE      0x2000          //**< BYTE 04 - BIT 6 >**/ 
#define PSB_CROSS       0x4000          //**< BYTE 04 - BIT 7 >**/ 
#define PSB_SQUARE      0x8000          //**< BYTE 04 - BIT 8 >**/ 

#define PSB_RX          0xFF            //**< BYTE 05         >**/ 
#define PSB_RY          0x00FF          //**< BYTE 06         >**/ 
#define PSB_LX          0xFF            //**< BYTE 07         >**/ 
#define PSB_LY          0x00FF          //**< BYTE 08         >**/ 


/* =============================================[ TYPE DEFINITIONS ]==========================================*/
/**
 * @brief   Cấu trúc dữ liệu lưu trữ trạng thái của các nút nhấn trên PS2.
 **/
typedef union {
    struct {
        uint8_t SELECT : 1;
        uint8_t L3 : 1;
        uint8_t R3 : 1;
        uint8_t START : 1;
        uint8_t UP : 1;
        uint8_t RIGHT : 1;
        uint8_t DOWN : 1;
        uint8_t LEFT : 1;
        uint8_t L2 : 1;
        uint8_t R2 : 1;
        uint8_t L1 : 1;
        uint8_t R1 : 1;
        uint8_t TRIANGLE : 1;
        uint8_t CIRCLE : 1;
        uint8_t CROSS : 1;
        uint8_t SQUARE : 1;
        uint8_t RX;
        uint8_t RY;
        uint8_t LX;
        uint8_t LY;
    }button;
	struct{
        uint16_t buttonData;
        uint16_t coordinatesR;
        uint16_t coordinatesL;
    }data;	
}PS2;

extern SPI_HandleTypeDef PS2_SPI_HANDLE;            //**< Handle SPI sử dụng cho PS2            >**/ 
extern TIM_HandleTypeDef PS2_TIM_COUNTER;           //**< Handle Timer sử dụng cho delay        >**/

uint8_t ps2_response[9] ={0x00};                    //**< Mảng toàn cục để lưu dữ liệu PS2      >**/

PS2 *ps2 = NULL;                                    //**< Lưu trữ trạng thái toàn bộ nút bấm    >**/

uint8_t PS2_MODE;                                   //**< Lưu trữ chế độ của tay cầm PS2        >**/


/*
 * NON-CONFIG MODE
 */
/* Main polling command */
uint8_t main_polling_42[9] = { 0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* Exit Config Mode */
uint8_t exit_config_43[9] = { 0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };

/*
 * CONFIG MODE RESPONSES
 */
/* Find out what buttons are included in poll responses. */
 uint8_t find_polling_41[9] = { 0x01, 0x41, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };

/* Enter Config Mode, */
 uint8_t enter_config_43[9] = { 0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* Switch modes between digital and analog */
 uint8_t switch_mode_44[9] = { 0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 };

/* Get more status info */
 uint8_t read_more_info_45[9] = { 0x01, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* Read an unknown constant value from controller */
 uint8_t type_read_46[2][9] = {{ 0x01, 0x46, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A },
								{ 0x01, 0x46, 0x00, 0x01, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A }};

/* Read an unknown constant value from controller */
 uint8_t type_read_47[9] = { 0x01, 0x47, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };

/* Read an unknown constant value from controller */
 uint8_t type_read_4c[2][9] = {{ 0x01, 0x4C, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A },
								{ 0x01, 0x4C, 0x00, 0x01, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A }};

 uint8_t enable_rumble_4d[9] = { 0x01, 0x4D, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };

 uint8_t set_bytes_large_4f[9] = { 0x01, 0x4F, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00 };


/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo động cơ Mecanum
 * @details Hàm này sẽ khởi tạo các thông số cần thiết cho động cơ Mecanum,
 *          bao gồm việc cấu hình GPIO và Timer cho các động cơ.
 * @param   void
 * @return  void
 **/
void delay_us(uint16_t us);


/**
 * @brief   Hàm khởi tạo giao tiếp PS2
 * @details Hàm này sẽ truyền các lệnh setup và cấu hình chế độ PS2
 *          0x42 (READ_DATA) -> 0X43 (ENTER_CONFIG_MODE) -> 0X44 (ANALOG_MODE) -> 0X43 (EXIT_CONFIG_MODE)
 * @param   void
 * @return  void
 **/
void PS2_Init(void);


/**
 * @brief   Hàm đọc trạng thái của các nút bấm
 * @details Hàm này sẽ xử lý data trạng thái nút bấm được PS2 trả về trong ps2_response,
 *          sau đó lưu trạng thái nút vào ps2
 * @param   void
 * @return  void
 **/
void PS2_ButtonPressed(void);


/**
 * @brief   Hàm cập nhật trạng thái của PS2
 * @details Hàm này sẽ gửi lệnh để nhận trạng thái của PS2
 *          Sau đó sẽ gọi hàm đọc trạng thái nút để lưu lại
 * @param   void
 * @return  void
 **/
void PS2_Update(void);

#endif
