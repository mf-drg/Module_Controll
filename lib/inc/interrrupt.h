/*********************************************************************************************************************
 * @file    interrrupt.c
 * @brief   Thư viện xử lý ngắt cho các nút bấm
 * @details Thư viện các hàm để xử lý ngắt cho các nút bấm trên hệ thống.
 *          Các nút bấm được sử dụng để điều khiển chế độ hoạt động của hệ thống.
 * @version 1.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* =====================================================[ Guard ]====================================================*/
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

/* ============================================[ INCLUDE FILE ]============================================*/
#include "main.h"					//**< Thư viện chứa các định nghĩa GPIO và hàm HAL >**/

/* =========================================[ MACRO DEFINITIONS ]==========================================*/
#define DISTANCE_MIN 	20			//**< Khoảng cách tối thiểu >**/
#define DISTANCE_MAX	80			//**< Khoảng cách tối đa >**/

/* ===========================================[ TYPE DEFINITIONS ]==========================================*/
/**
 * @brief   Kiểu dữ liệu cho chế độ hoạt động của hệ thống
 * @details Chế độ hoạt động của hệ thống có thể là CONTROL, LINE, AUTO hoặc NONE.
 **/
typedef enum{
	CONTROL = 0,
	LINE = 1,
	AUTO = 2,
	NONE = 3
} Mode;

/**
 * @brief   Kiểu dữ liệu cho chế độ sử dụng
 * @details Chế độ sử dụng có thể là NOT, DISTANCE hoặc AAA.
 **/
typedef enum{
	NOT = 0,
	DISTANCE = 1,
	AAA = 2
} Set;

Mode mode = CONTROL;				//**< Chế độ hoạt động của hệ thống >**/
Set set = NOT;						//**< Chế độ sử dụng 				>**/

uint8_t distance = DISTANCE_MIN;	//**< Khoảng cách đo thiết lập 		>**/

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm xử lý ngắt cho các nút bấm
 * @details Hàm này sẽ được gọi khi có sự kiện ngắt từ các nút bấm trên hệ thống.
 *          Nó sẽ kiểm tra nút nào được nhấn và thực hiện hành động tương ứng.
 * @note    Hàm này sẽ được gọi tự động khi có sự kiện ngắt từ các nút bấm.
 * @param   GPIO_Pin    Chân GPIO của nút bấm được nhấn
 * @return  void
 **/
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* =====================================================[ Guard ]====================================================*/
#endif


