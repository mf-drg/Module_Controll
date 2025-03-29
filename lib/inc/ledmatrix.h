/*********************************************************************************************************************
 * @file    ledmatrix.h
 * @brief   Thư viện điều khiển ma trận LED 8x8
 * @details Triển khai các hàm để điều khiển ma trận LED 8x8,  
 *          bao gồm việc khởi tạo, hiển thị dữ liệu và chạy hiệu ứng trên ma trận LED.
 * @version 2.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* =====================================================[ Guard ]====================================================*/
#ifndef __LEDMATRIX_H_
#define __LEDMATRIX_H_

/* ============================================[ INCLUDE FILE ]============================================*/
#include "stdlib.h"         //**< Thư viện chứa các hàm toán học và chuỗi       >**/
#include "main.h"           //**< Thư viện chứa các định nghĩa GPIO và hàm HAL  >**/
#include "74HC595.h"        //**< Thư viện điều khiển 74HC595                   >**/
#include "string.h"         //**< Thư viện chứa các hàm chuỗi                   >**/

uint8_t chu_chay[8][8] = {	{0x81,0xc3,0xbd,0x81,0x81,0x81,0x81,0x81},        
							{0x3c,0x42,0x81,0x81,0x81,0x81,0x42,0x3c},
							{0xfe,0x81,0x81,0xfe,0x81,0x81,0xfe,0x00},
							{0xff,0x18,0x18,0x18,0x18,0x18,0x18,0xff},
							{0xff,0x80,0x80,0x80,0xff,0x80,0x80,0x80},
							{0x3c,0x42,0x81,0x81,0x81,0x81,0x42,0x3c},
							{0x81,0xc1,0xa1,0x91,0x89,0x85,0x83,0x81},
							{0xff,0x80,0x80,0xff,0xff,0x80,0x80,0xff}
						};

uint8_t m[8] 		= 	{0x81,0xc3,0xbd,0x81,0x81,0x81,0x81,0x81};          //**< Hình chữ M        >**/
uint8_t left[8] 	= 	{0x00,0x10,0x3e,0x7e,0x3e,0x10,0x00,0x00};          //**< Hình mũi tên trái >**/
uint8_t right[8] 	= 	{0x00,0x08,0x7c,0x7e,0x7c,0x08,0x00,0x00};          //**< Hình mũi tên phải >**/
uint8_t up[8] 		= 	{0x00,0x10,0x38,0x7c,0x38,0x38,0x38,0x00};          //**< Hình mũi tên lên  >**/
uint8_t down[8]		= 	{0x00,0x1c,0x1c,0x1c,0x3e,0x1c,0x08,0x00};          //**< Hình mũi tên xuống >**/
uint8_t turnBack[8] = 	{0x00,0xe6,0x61,0xa5,0x86,0x67,0x00,0x00};          //**< Hình quay lại     >**/

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo ma trận LED 8x8
 * @details Hàm này sẽ khởi tạo ma trận LED 8x8 bằng cách 
 * 			gửi các lệnh cấu hình ban đầu đến ma trận LED.
 *          Nó sẽ thiết lập chế độ hoạt động và số dòng của ma trận LED.
 * @note    Hàm này sẽ được gọi để khởi tạo ma trận LED trước khi sử dụng.
 * @param   void    Chân GPIO của nút bấm được nhấn
 * @return  void
 **/
void Led_Matrix_Init(void);


/**
 * @brief   Hàm hiển thị dữ liệu trên ma trận LED 8x8
 * @details Hàm này sẽ hiển thị dữ liệu trên ma trận LED 8x8 bằng cách gửi dữ liệu đến ma trận LED.
 *          Nó sẽ gửi dữ liệu theo định dạng 16 bit để điều khiển các dòng và cột của ma trận LED.
 * @note    Hàm này sẽ được gọi để hiển thị dữ liệu trên ma trận LED.
 * @param   data    Dữ liệu cần hiển thị trên ma trận LED.
 * @return  void
 **/
void display_Led(const char data[]);


/**
 * @brief   Hàm chạy hiệu ứng chữ chạy trên ma trận LED 8x8
 * @details Hàm này sẽ chạy hiệu ứng chữ chạy trên ma trận LED 8x8 bằng cách 
 * 			sử dụng dữ liệu đã được định nghĩa trước.
 *          Nó sẽ hiển thị từng khung hình của chữ chạy và xử lý việc dịch chuyển các bit.
 * @note    Hàm này sẽ được gọi để chạy hiệu ứng chữ chạy trên ma trận LED.
 * @param   void    Chân GPIO của nút bấm được nhấn
 * @return  void
 **/
void display_Led_Running_Column(void);



#endif

