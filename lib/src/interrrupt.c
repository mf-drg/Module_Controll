/**********************************************************************************
 * @file    interrrupt.c
 * @brief   Thư viện xử lý ngắt cho các nút bấm
 * @details Triển khai các hàm để xử lý ngắt cho các nút bấm trên hệ thống.
 *          Các nút bấm được sử dụng để điều khiển chế độ hoạt động của hệ thống.
 * @version 1.0
 * @date    2024-11-25
 * @author  LongTruong
 **********************************************************************************/
/* ============================================[ INCLUDE FILE ]============================================*/
#include "interrrupt.h"

/* ============================================[ FUNCTION PROTOTYPES ]========================================*/
/**
 * @brief   Hàm xử lý ngắt cho các nút bấm
 * @details Hàm này sẽ được gọi khi có sự kiện ngắt từ các nút bấm trên hệ thống.
 *          Nó sẽ kiểm tra nút nào được nhấn và thực hiện hành động tương ứng.
 * @note    Hàm này sẽ được gọi tự động khi có sự kiện ngắt từ các nút bấm.
 * @param   GPIO_Pin    Chân GPIO của nút bấm được nhấn
 * @return  void
 **/
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		if (GPIO_Pin == GPIO_PIN_5 )  // NUT 1 - Mode
    {
        // SWAP MODE
    }
    else if (GPIO_Pin == GPIO_PIN_0)  // nut 2 - Up
    {
        if(mode == AUTO){
					if(distance < DISTANCE_MAX)
						distance += 5;
					if(distance >= DISTANCE_MAX)
						distance = DISTANCE_MIN;
				}	
    }
    else if (GPIO_Pin == GPIO_PIN_2)  // nut3 down
    {
        if(mode == AUTO){
					if(distance > DISTANCE_MIN)
						distance -= 5;
					if(distance < DISTANCE_MIN)
						distance = DISTANCE_MIN;
				}
    }
    else if (GPIO_Pin == GPIO_PIN_1)  // NUT 4 MODE
    {
        if(mode < 3)
					mode++;
				if(mode >= 3)
					mode = CONTROL;
					set = NOT;
    }
}


