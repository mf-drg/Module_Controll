/*********************************************************************************************************************
 * @file    ledmatrix.h
 * @brief   Thư viện điều khiển ma trận LED 8x8
 * @details Thư viện các hàm để điều khiển ma trận LED 8x8,  
 *          bao gồm việc khởi tạo, hiển thị dữ liệu và chạy hiệu ứng trên ma trận LED.
 * @version 2.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* ============================================[ INCLUDE FILE ]============================================*/
#include "ledmatrix.h"			

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
void Led_Matrix_Init(){
	send_MAX7219_16bit(0x0A, 0x0F);		//**< Do sang 		>**/
	HAL_Delay(1);
	send_MAX7219_16bit(0x0B, 0x07);		//**< Quét 8 dòng 	>**/
	HAL_Delay(1);
}


/**
 * @brief   Hàm hiển thị dữ liệu trên ma trận LED 8x8
 * @details Hàm này sẽ hiển thị dữ liệu trên ma trận LED 8x8 bằng cách gửi dữ liệu đến ma trận LED.
 *          Nó sẽ gửi dữ liệu theo định dạng 16 bit để điều khiển các dòng và cột của ma trận LED.
 * @note    Hàm này sẽ được gọi để hiển thị dữ liệu trên ma trận LED.
 * @param   data    Dữ liệu cần hiển thị trên ma trận LED.
 * @return  void
 **/
void display_Led(const char data[]){
		for(uint8_t i = 0; i < 8; i++){
				send_MAX7219_16bit(i,data[i]);
		}
}


/**
 * @brief   Hàm chạy hiệu ứng chữ chạy trên ma trận LED 8x8
 * @details Hàm này sẽ chạy hiệu ứng chữ chạy trên ma trận LED 8x8 bằng cách 
 * 			sử dụng dữ liệu đã được định nghĩa trước.
 *          Nó sẽ hiển thị từng khung hình của chữ chạy và xử lý việc dịch chuyển các bit.
 * @note    Hàm này sẽ được gọi để chạy hiệu ứng chữ chạy trên ma trận LED.
 * @param   void    Chân GPIO của nút bấm được nhấn
 * @return  void
 **/
void display_Led_Running_Column(){
	char run_word[8];
	char cache[8];
	uint8_t numVol = 1;						//**< Khoang cach giua 2 chu cai 				>**/
	
	for(uint8_t i = 0; i < sizeof(chu_chay) / 8; i++){
		//luu chu dau tien vao runword de chuan bi in ra va chu tiep theo vao bo nho dem de chuan bi dich
		for(uint8_t j = 0; j < 8; j++){
			run_word[j] = chu_chay[i][j];
			if(i < 7){
				cache[j] = chu_chay[i+1][j];
			}
			else{
				cache[j] = 0;
			}
		}
		
		display_Led(run_word); 				//**< in ra khung hinh dau tien truoc khi dich 	>**/
		HAL_Delay(100);

		// Xu ly chu chay
		// neu khoang trang la 1 cot thi sau khi dich 1 lan se khong chen th�m g�, den lan thu 2 se bat dau xuat hien cot bit cao nhat cua chu thiep theo
		// vay neu n cot trang thi den lan thu n (tinh tu 0) se bat dau xuat hien cot bit cao nhat cua chu thiep theo
		for(uint8_t j = 0; j < 7 + numVol; j++){			//**< j < 7 + numVol de khong bi vuot qua khoang trang 				>**/
			for(uint8_t t = 0; t < 8; t++){					//**< t < 8 de khong bi vuot qua 8 cot cua ma tran LED 				>**/
				if( j < numVol){							//**< neu j < numVol thi khong chen them bit nao 					>**/
					run_word[t] = run_word[t] << 1;
				}else{ 										//**< neu j >= numVol thi chen them bit cao nhat cua chu tiep theo 	>**/					
					run_word[t] = run_word[t] << 1 | ( (cache[t] >> (7 + numVol - j)) & 0x01) ;
				}
			}		
			display_Led(run_word);
			HAL_Delay(100);
		}		
	}	
}


