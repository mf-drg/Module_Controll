/**********************************************************************************
 * @file    74HC595.c
 * @brief   Thư viện điều khiển 74HC595 và MAX7219
 * @details Thư viện các hàm để điều khiển 74HC595 và MAX7219,
 *          bao gồm việc gửi dữ liệu 8 bit và 16 bit đến các thiết bị này.
 *          Các hàm này sử dụng GPIO để giao tiếp với các thiết bị.
 * @version 3.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/

/* =====================================================[ Guard ]====================================================*/
#ifndef __74HC595_H__
#define __74HC595_H__

/* ============================================[ INCLUDE FILE ]============================================*/
#include "main.h"                               //**< Thư viện chứa các định nghĩa GPIO và hàm HAL >**/

/* ==========================================[ MACRO DEFINITIONS ]==========================================*/
#define DS_Pin 				GPIO_PIN_15         //**< DS chân dữ liệu 74HC595 >**/
#define DS_GPIO_Port 		GPIOA               //**< DS chân dữ liệu 74HC595 >**/
#define ST_CP_Pin 			GPIO_PIN_11         //**< ST_CP chân latch 74HC595 >**/
#define ST_CP_GPIO_Port     GPIOC               //**< ST_CP chân latch 74HC595 >**/
#define SH_CP_Pin 			GPIO_PIN_2          //**< SH_CP chân clock 74HC595 >**/
#define SH_CP_GPIO_Port     GPIOD               //**< SH_CP chân clock 74HC595 >**/

#define ST_CP_LATCH_HIGH    HAL_GPIO_WritePin(ST_CP_GPIO_Port, ST_CP_Pin, GPIO_PIN_SET)     //**< Latch HIGH    >**/
#define ST_CP_LATCH_LOW     HAL_GPIO_WritePin(ST_CP_GPIO_Port, ST_CP_Pin, GPIO_PIN_RESET)   //**< Latch LOW     >**/
#define SH_CP_CLOCK_HIGH    HAL_GPIO_WritePin(SH_CP_GPIO_Port, SH_CP_Pin, GPIO_PIN_SET)     //**< SCK HIGH      >**/
#define SH_CP_CLOCK_LOW     HAL_GPIO_WritePin(SH_CP_GPIO_Port, SH_CP_Pin, GPIO_PIN_RESET)   //**< SCK LOW       >**/
#define DS_DATA_HIGH        HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET)           //**< DS HIGH       >**/
#define DS_DATA_LOW         HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET)         //**< DS LOW        >**/


#define CS_Pin          GPIO_PIN_13             //**< CS chân chip select MAX7219 >**/
#define CS_GPIO_Port    GPIOC                   //**< CS chân chip select MAX7219 >**/
#define DIN_Pin         GPIO_PIN_3              //**< DIN chân dữ liệu MAX7219 >**/
#define DIN_GPIO_Port   GPIOB                   //**< DIN chân dữ liệu MAX7219 >**/
#define CLK1_Pin        GPIO_PIN_5              //**< CLK chân clock MAX7219 >**/
#define CLK1_GPIO_Port  GPIOB                   //**< CLK chân clock MAX7219 >**/

#define CS_LATCH_HIGH       HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)           //**< CS HIGH       >**/
#define CS_LATCH_LOW        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)         //**< CS LOW        >**/
#define CLK1_CLOCK_HIGH     HAL_GPIO_WritePin(CLK1_GPIO_Port, CLK1_Pin, GPIO_PIN_SET)       //**< CLK HIGH      >**/
#define CLK1_CLOCK_LOW      HAL_GPIO_WritePin(CLK1_GPIO_Port, CLK1_Pin, GPIO_PIN_RESET)     //**< CLK LOW       >**/
#define DIN_DATA_HIGH       HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_SET)         //**< DIN HIGH      >**/
#define DIN_DATA_LOW        HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_RESET)       //**< DIN LOW       >**/

#define BYTE_SIZE 32                            //**< Kích thước byte cần truyền >**/


/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Gửi dữ liệu 8 bit đến 74HC595
 * @details Hàm này gửi dữ liệu 8 bit đến 74HC595 bằng cách sử dụng GPIO.
 *          Dữ liệu được gửi theo thứ tự MSB (Most Significant Bit) trước.
 *          Sau khi gửi xong, hàm sẽ chốt dữ liệu bằng cách kích hoạt chân ST_CP.
 * @note    Chân DS sử dụng để gửi dữ liệu, chân SH_CP sử dụng để tạo clock,
 *          và chân ST_CP sử dụng để chốt dữ liệu.
 * @param   tx   Dữ liệu 8 bit cần gửi đến 74HC595.
 * @return  void
 **/
void send_74HC595_8bit(uint8_t tx);

/**
 * @brief   Gửi dữ liệu N bit đến 74HC595
 * @details Hàm này gửi dữ liệu N bit đến 74HC595 bằng cách sử dụng GPIO.
 *          Dữ liệu được gửi theo thứ tự MSB (Most Significant Bit) trước.
 *          Sau khi gửi xong, hàm sẽ chốt dữ liệu bằng cách kích hoạt chân ST_CP.
 * @note    Chân DS sử dụng để gửi dữ liệu, chân SH_CP sử dụng để tạo clock,
 *          và chân ST_CP sử dụng để chốt dữ liệu.
 * @param   tx   Dữ liệu 8 bit cần gửi đến 74HC595.
 * @return  void
 **/
void send_74HC595_Nbits (uint32_t num );


/**
 * @brief   Gửi dữ liệu 16 bit đến MAX7219
 * @details Hàm này gửi dữ liệu 16 bit đến MAX7219 bằng cách sử dụng GPIO.
 *          Dữ liệu được gửi theo thứ tự MSB (Most Significant Bit) trước.
 * 			Dữ liệu được tách thành 2 byte: byte đầu tiên là lệnh (command) và byte thứ hai là dữ liệu (data).
 *          Sau khi gửi xong, hàm sẽ chốt dữ liệu bằng cách kích hoạt chân CS.
 * @note    Chân DIN sử dụng để gửi dữ liệu, chân CLK1 sử dụng để tạo clock,
 * 			và chân CS sử dụng để chốt dữ liệu.
 * @param   cmd  Lệnh cần gửi đến MAX7219.
 * @param   tx   Dữ liệu 8 bit cần gửi đến MAX7219.
 * @return  void
 **/
void send_MAX7219_16bit(uint8_t cmd, uint8_t tx);

#endif


