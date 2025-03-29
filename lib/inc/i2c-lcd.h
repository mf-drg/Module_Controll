/**********************************************************************************
 * @file    i2c-lcd.h
 * @brief   Thư viện điều khiển LCD I2C
 * @details Thư viện các hàm để điều khiển LCD I2C,
 *          bao gồm việc khởi tạo, gửi lệnh và dữ liệu đến LCD,
 *          gửi chuỗi và thiết lập con trỏ đến vị trí mong muốn trên LCD.
 * @version 2.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/

/* =====================================================[ Guard ]====================================================*/
#ifndef I2C_LCD_H
#define I2C_LCD_H

/* =========================================[ MACRO DEFINITIONS ]==========================================*/
#define SLAVE_ADDRESS_LCD   0x4E        //**< Địa chỉ I2C của LCD I2C >**/
#define LCD_COLUMS			16          //**< Số cột của LCD >**/
#define LCD_ROWS		    2           //**< Số hàng của LCD >**/

/* ============================================[ INCLUDE FILE ]============================================*/
#include "main.h"                       //**< Thư viện chứa các định nghĩa GPIO và hàm HAL >**/

/* ==========================================[ TYPE DEFINITIONS ]==========================================*/
extern I2C_HandleTypeDef hi2c1;         //**< Handle I2C sử dụng cho LCD I2C >**/


/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo LCD I2C
 * @details Hàm này sẽ khởi tạo LCD I2C bằng cách gửi các lệnh cấu hình ban đầu đến LCD.
 *          Nó sẽ thiết lập chế độ 4 bit, số dòng và kiểu ký tự. 
 * @param   void    
 * @return  void
 **/
void lcd_init (void);

/**
 * @brief   Hàm gửi lệnh đến LCD I2C
 * @details Hàm này sẽ gửi lệnh đến LCD I2C bằng cách sử dụng giao thức I2C.
 *          Nó sẽ gửi lệnh theo định dạng 4 bit và 8 bit.
 * @note    Hàm này sẽ được gọi để gửi lệnh đến LCD.
 * @param   cmd    Lệnh cần gửi đến LCD I2C.
 * @return  void
 **/
void lcd_send_cmd (char cmd);  // send command to the lcd

/**
 * @brief   Hàm gửi dữ liệu đến LCD I2C
 * @details Hàm này sẽ gửi dữ liệu đến LCD I2C bằng cách sử dụng giao thức I2C.
 *          Nó sẽ gửi dữ liệu theo định dạng 4 bit và 8 bit.
 * @note    Hàm này sẽ được gọi để gửi dữ liệu đến LCD.
 * @param   data   Dữ liệu cần gửi đến LCD I2C.
 * @return  void
 **/
void lcd_send_data (char data);  // send data to the lcd


/**
 * @brief   Hàm gửi chuỗi đến LCD I2C
 * @details Hàm này sẽ gửi chuỗi đến LCD I2C bằng cách sử dụng giao thức I2C.
 *          Nó sẽ gửi từng ký tự trong chuỗi đến LCD.
 * @note    Hàm này sẽ được gọi để gửi chuỗi đến LCD.
 * @param   str    Chuỗi cần gửi đến LCD I2C.
 * @return  void
 **/
void lcd_send_string (char *str);  // send string to the lcd


/**
 * @brief   Hàm thiết lập con trỏ đến vị trí mong muốn trên LCD I2C
 * @details Hàm này sẽ thiết lập con trỏ đến vị trí mong muốn trên LCD I2C.
 *          Nó sẽ gửi lệnh để di chuyển con trỏ đến vị trí X, Y được chỉ định.
 * @note    Hàm này sẽ được gọi để thiết lập con trỏ trên LCD.
 * @param   Xpos   Vị trí cột (X) cần thiết lập con trỏ.
 * @param   Ypos   Vị trí hàng (Y) cần thiết lập con trỏ.
 * @return  void
 **/
void lcd_set_cursor(uint8_t Xpos, uint8_t Ypos); //poiter to X,Y


/**
 * @brief   Hàm xóa màn hình LCD I2C
 * @details Hàm này sẽ gửi dữ liệu đến LCD I2C bằng cách sử dụng giao thức I2C.
 * 		 	Nó sẽ gửi dữ liệu toàn bộ là khoảng trống
 * @note    Hàm này sẽ được gọi để xóa dữ liệu trên LCD.
 * @param   void
 * @return  void
 **/
void lcd_clear (void);  

/* =====================================================[ Guard ]====================================================*/
#endif