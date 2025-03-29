/*********************************************************************************************************************
 * @file    ps2.c
 * @brief   Thư viện đọc điều khiển tay cầm PS2 bằng STM32
 * @details Triển khai các hàm để đọc điều khiển tay cầm PS2 thông qua giao thức SPI.
 *          Nó bao gồm các hàm để khởi tạo, cấu hình, đọc dữ liệu từ tay cầm PS2,
 *          và xử lý các sự kiện từ tay cầm.
 * @version 3.0
 * @date    2024-11-25
 * @author  LongTruong
 *********************************************************************************************************************/
/* ============================================[ INCLUDE FILE ]============================================*/
#include "ps2.h"            //**< Thư viện đọc điều khiển tay cầm PS2 bằng STM32 >**/




/**
 * @brief   Hàm sử dụng để tạo độ trễ trong micro giây      
 * @details Hàm này sẽ tạo độ trễ trong khoảng thời gian được chỉ định bằng micro giây.  
 * @param   us    Thời gian trễ cần tạo, tính bằng micro giây  
 * @return  void
 **/
void delay_us(uint16_t us) {
	__HAL_TIM_SET_COUNTER(&htim12, 0);
	while (__HAL_TIM_GET_COUNTER(&htim12) < us);
}


/**
 * @brief   Hàm nội bộ Truyền và nhận dữ liệu thông qua SPI  
 * @details Hàm này sẽ truyền dự liệu băng SPI với data được cấu hình sẵn theo từng yêu cầu
 *          Sau đó sẽ nhận dữ liệu phản hồi từ SP2 và lưu vào ps2_response
 * @param   command   Mảng dữ liệu cấu hình sẵn cho 1 chức năng nhất định
 * @param   length    Kích thước dữ liệu truyền
 * @return  void
 **/
static void PS2_SendCommand(const uint8_t command[], uint8_t length){
    CS_LOW;
    delay_us(15);  
    for (uint8_t i = 0; i < length; i++) {
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&command[i], &ps2_response[i], sizeof(uint8_t), 1);
	    delay_us(15);
    }
    CS_HIGH;      
}

// Các hàm truyền lệnh đặc thù cho từng CMD
/**
 * @brief   Các hàm truyền lệnh đặc thù cho từng CMD
 * @details Các hàm này sẽ truyền lệnh đặc thù để giao tiếp với SP2 
 *          Như: cấu hình PS2, chuyển đổi trạng thái, đọc dữ liệu điều khiển,..
 * @note    Hàm này sẽ gọi Truyền và nhận dữ liệu thông qua SPI 
 * @param   mode    Chế độ đọc TYPEREAD
 * @param   void
 * @return  void
 **/
static void PS2_MainPolling(void)       { PS2_SendCommand(main_polling_42,9);       }
static void PS2_Polling(void)           { PS2_SendCommand(main_polling_42, 5);      }
static void PS2_ExitConfig(void)        { PS2_SendCommand(exit_config_43, 9);       }
static void PS2_FindPolling(void)       { PS2_SendCommand(find_polling_41, 9);      }
static void PS2_EnterConfig(void)       { PS2_SendCommand(enter_config_43, 5);      }
static void PS2_SwitchMode(void)        { PS2_SendCommand(switch_mode_44, 9);       }
static void PS2_ReadMoreInfo(void)      { PS2_SendCommand(read_more_info_45, 9);    }
static void PS2_TypeRead46(uint8_t mode){ PS2_SendCommand(type_read_46[mode], 9);   }
static void PS2_TypeRead47(void)        { PS2_SendCommand(type_read_47, 9);         }
static void PS2_TypeRead4C(uint8_t mode){ PS2_SendCommand(type_read_4c[mode], 9);   }
static void PS2_EnableRumble4D(void)    { PS2_SendCommand(enable_rumble_4d, 9);     }
static void PS2_SetBytesLarge4F(void)   { PS2_SendCommand(set_bytes_large_4f, 9);   }


/**
 * @brief   Hàm khởi tạo giao tiếp PS2
 * @details Hàm này sẽ truyền các lệnh setup và cấu hình chế độ PS2
 *          0x42 (READ_DATA) -> 0X43 (ENTER_CONFIG_MODE) -> 0X44 (ANALOG_MODE) -> 0X43 (EXIT_CONFIG_MODE)
 * @param   void
 * @return  void
 **/
void PS2_Init(void){ 
    PS2_Polling();
    PS2_EnterConfig();                          //**< vào CONFIG MODE       >**/
    PS2_SwitchMode();                           //**< ANALOG MODE           >**/
	PS2_Polling();
    PS2_ExitConfig();                           //**< thoát CONFIG MODE     >**/
}

/**
 * @brief   Hàm đọc trạng thái của các nút bấm
 * @details Hàm này sẽ xử lý data trạng thái nút bấm được PS2 trả về trong ps2_response,
 *          sau đó lưu trạng thái nút vào ps2
 * @param   void
 * @return  void
 **/
void PS2_ButtonPressed(void){
    if (ps2 == NULL) {                           //**< Chỉ cấp phát nếu ps2 chưa được cấp phát >**/
        ps2 = (PS2 *)malloc(sizeof(PS2));
    }
		if(ps2_response[1] == 0x73)
		{
			PS2_MODE = 1;
		}
		else if (ps2_response[1] != 0x73)
		{
			PS2_MODE= 0;
		}
    if(ps2_response[1] == 0x41 | ps2_response[1] == 0x73){
        ps2->data.buttonData    = ~(ps2_response[3] | (ps2_response[4] << 8));
        ps2->data.coordinatesR  = ps2_response[5] | (ps2_response[6] << 8);
        ps2->data.coordinatesL  = ps2_response[7] | (ps2_response[8] << 8);
    }
}


/**
 * @brief   Hàm cập nhật trạng thái của PS2
 * @details Hàm này sẽ gửi lệnh để nhận trạng thái của PS2
 *          Sau đó sẽ gọi hàm đọc trạng thái nút để lưu lại
 * @param   void
 * @return  void
 **/
void PS2_Update(void){
    PS2_MainPolling();
    PS2_ButtonPressed();
	HAL_Delay(10);
}








