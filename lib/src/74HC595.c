/**********************************************************************************
 * @file    74HC595.c
 * @brief   Thư viện điều khiển 74HC595 và MAX7219
 * @details Triển khai các hàm để điều khiển 74HC595 và MAX7219,
 *          bao gồm việc gửi dữ liệu 8 bit và 16 bit đến các thiết bị này.
 *          Các hàm này sử dụng GPIO để giao tiếp với các thiết bị.
 * @version 3.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/
/* ============================================[ INCLUDE FILE ]============================================*/
#include "74HC595.h"


/* ========================================[ FUNCTION INPLEMENTATION ]======================================*/
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
void send_74HC595_8bit(uint8_t tx)
{
	uint8_t i ;

	for(i = 8 ; i > 0 ; i--)
	{
		((tx >> (i-1)) & 0x01) ? (DS_DATA_HIGH) : (DS_DATA_LOW);
		SH_CP_CLOCK_HIGH;       /**< SCK = 1               >**/
		SH_CP_CLOCK_LOW;        /**< SCK = 0               >**/
	}
  ST_CP_LATCH_HIGH;           	/**< chốt data             >**/
  ST_CP_LATCH_LOW;
}


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
void send_74HC595_Nbits(uint32_t num )
{
	uint8_t i ;

	for(i = BYTE_SIZE ; i > 0 ; i--)
	{
		((num >> (i-1)) & 0x01) ? (DS_DATA_HIGH) : (DS_DATA_LOW);
		SH_CP_CLOCK_HIGH;       	/**< SCK = 1          >**/
		SH_CP_CLOCK_LOW;        	/**< SCK = 0          >**/    	
	}
  ST_CP_LATCH_HIGH;           		/**< chốt data        >**/
  ST_CP_LATCH_LOW;
}


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
void send_MAX7219_16bit(uint8_t cmd, uint8_t tx)
{
	uint8_t i ;

	CS_LATCH_LOW;
	
	for(i = 8 ; i > 0 ; i--){
		((cmd >> (i-1)) & 0x01) ? (DIN_DATA_HIGH) : (DIN_DATA_LOW);
		CLK1_CLOCK_HIGH;                  /**< SCK = 1    	>**/   
		CLK1_CLOCK_LOW;                   /**< SCK = 0      >**/
	}
  
	for(i = 8 ; i > 0 ; i--){
		((tx >> (i-1)) & 0x01) ? (DIN_DATA_HIGH) : (DIN_DATA_LOW);
		CLK1_CLOCK_HIGH;                /**< SCK = 1     	>**/   
		CLK1_CLOCK_LOW;                 /**< SCK = 0  	    >**/
	}
	
	CS_LATCH_HIGH;						/**< chốt data    	>**/
}


