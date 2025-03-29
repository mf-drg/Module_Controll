/**********************************************************************************
 * @file    i2c-lcd.c
 * @brief   Thư viện điều khiển LCD I2C
 * @details Triển khai các hàm để điều khiển LCD I2C,
 *          bao gồm việc khởi tạo, gửi lệnh và dữ liệu đến LCD,
 *          gửi chuỗi và thiết lập con trỏ đến vị trí mong muốn trên LCD.
 * @version 2.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/
/* ============================================[ INCLUDE FILE ]============================================*/
#include "i2c-lcd.h"

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm khởi tạo LCD I2C
 * @details Hàm này sẽ khởi tạo LCD I2C bằng cách gửi các lệnh cấu hình ban đầu đến LCD.
 *          Nó sẽ thiết lập chế độ 4 bit, số dòng và kiểu ký tự.
 * @note    Hàm này sẽ được gọi để khởi tạo LCD trước khi sử dụng.
 * @param   void    
 * @return  void
 **/
void lcd_init (void)
{
	HAL_Delay(50);  			//**< Đợi trên 40ms >**/
	lcd_send_cmd (0x30);		//**< Gửi lệnh 0x30 >**/
	HAL_Delay(5);  				//**< Đợi trên 4.1ms >**/
	lcd_send_cmd (0x30);
	HAL_Delay(1);  				//**< Đợi trên 100us >**/
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  		//**< Chế độ 4 bit >**/
	HAL_Delay(10);

	//Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
  // dislay initialisation
	lcd_send_cmd (0x28);	//**< Chế độ 4 bit (DL = 0), 2 dòng (N = 1), 5x8 ký tự (F = 0) 			>**/ 
	HAL_Delay(1);
	lcd_send_cmd (0x08); 	//**< Display on/off control --> D=0,C=0, B=0  ---> display off 		>**/
	HAL_Delay(1);
	lcd_send_cmd (0x01);  	//**< Clear display --> D=0,C=0, B=0  ---> display off 					>**/
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); 	//**< Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift) 	>**/
	HAL_Delay(1);
	lcd_send_cmd (0x0C); 	//**< Display on/off control --> D=1,C=0, B=0  ---> display on 			>**/
}

/**
 * @brief   Hàm gửi lệnh đến LCD I2C
 * @details Hàm này sẽ gửi lệnh đến LCD I2C bằng cách sử dụng giao thức I2C.
 *          Nó sẽ gửi lệnh theo định dạng 4 bit và 8 bit.
 *          Lệnh được gửi sẽ được xử lý để thiết lập chế độ 4 bit, số dòng và kiểu ký tự.
 * @note    Hàm này sẽ được gọi để khởi tạo LCD trước khi sử dụng.
 * @param   cmd    Lệnh cần gửi đến LCD I2C.
 * @return  void
 **/
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  		//**<en=1, rs=0 >**/
	data_t[1] = data_u|0x08;  		//**<en=0, rs=0 >**/
	data_t[2] = data_l|0x0C;  		//**<en=1, rs=0 >**/
	data_t[3] = data_l|0x08;  		//**<en=0, rs=0 >**/
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

/**
 * @brief   Hàm gửi dữ liệu đến LCD I2C
 * @details Hàm này sẽ gửi dữ liệu đến LCD I2C bằng cách sử dụng giao thức I2C.
 *          Nó sẽ gửi dữ liệu theo định dạng 4 bit và 8 bit.
 * 			Dữ liệu được gửi sẽ được xử lý để hiển thị trên LCD.
 * @note    Hàm này sẽ được gọi để gửi dữ liệu đến LCD.
 * @param   data   Dữ liệu cần gửi đến LCD I2C.
 * @return  void
 **/
void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  			//**<en=1, rs=1 >**/
	data_t[1] = data_u|0x09;  			//**<en=0, rs=1 >**/
	data_t[2] = data_l|0x0D;  			//**<en=1, rs=1 >**/
	data_t[3] = data_l|0x09;  			//**<en=0, rs=1 >**/
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}


/**
 * @brief   Hàm xóa màn hình LCD I2C
 * @details Hàm này sẽ gửi dữ liệu đến LCD I2C bằng cách sử dụng giao thức I2C.
 * 		 	Nó sẽ gửi dữ liệu toàn bộ là khoảng trống
 * @note    Hàm này sẽ được gọi để xóa dữ liệu trên LCD.
 * @param   void
 * @return  void
 **/
void lcd_clear (void)
{
	lcd_send_cmd (0x00);
	for (int i=0; i<100; i++)
	{
		lcd_send_data (' ');
	}
}


/**
 * @brief   Hàm thiết lập con trỏ đến vị trí mong muốn trên LCD I2C
 * @details Hàm này sẽ thiết lập con trỏ đến vị trí mong muốn trên 
 *          Nó sẽ gửi lệnh để di chuyển con trỏ đến vị trí X, Y được chỉ định.
 * @note    Hàm này sẽ được gọi để thiết lập vị trí con trỏ trên LCD.
 * @param   Xpos   Tọa độ X của vị trí con trỏ (0 ~ 15).
 * @param   Ypos   Tọa độ Y của vị trí con trỏ (0 ~ 3).
 * @return  void
 **/
void lcd_set_cursor(uint8_t Xpos, uint8_t Ypos){
	uint8_t DRAM_ADDRESS = 0x00;
	
	Xpos = (Xpos >= LCD_COLUMS) ? (LCD_COLUMS - 1) : Xpos - 1;
	Ypos = (Ypos >= LCD_ROWS) ? (LCD_ROWS -1) : Ypos - 1;
	
	if(Ypos == 0 | Ypos == 1)
	{
		DRAM_ADDRESS = 0x00 + (Ypos)*0x40 + Xpos;
	}
	else if(Ypos == 2 | Ypos == 3)
	{
		DRAM_ADDRESS = 0x14 + (Ypos - 2)*0x40 + Xpos;
	}
	lcd_send_cmd(0x80|DRAM_ADDRESS);
}


/**
 * @brief   Hàm gửi chuỗi đến LCD I2C
 * @details Hàm này sẽ gửi chuỗi đến LCD I2C bằng cách sử dụng giao thức I2C.
 *          Nó sẽ gửi từng ký tự trong chuỗi đến LCD.
 * @note    Hàm này sẽ được gọi để gửi chuỗi đến LCD.
 * @param   str    Chuỗi cần gửi đến LCD I2C.
 * @return  void
 **/
void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}


