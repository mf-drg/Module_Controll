#ifndef __DETECTLINE_H__
#define __DETECTLINE_H__


#include "main.h"


//BYTE4//BYTE3//BYTE2//BYTE1//BYTE0//
//LINE1//LINE2//LINE3//LINE4//LINE5//

/*			LINE
 *  	1-2-3-4-5
 *  [0]--|||--[1]
 *   |         |
 *   |         |
 *   |         |
 *   |         |
 *  [3]-------[2]
 */

#define LINE_GO_HEAD1				0x19		// 11001
#define LINE_GO_HEAD2				0x13		// 10011
#define LINE_GO_HEAD3				0x11		// 10001
#define LINE_GO_HEAD4				0x00		// 00000
#define LINE_GO_HEAD5				0x1b		// 11011


#define LINE_GO_LEFT_SLOW1	0x0f		// 01111
#define LINE_GO_LEFT_SLOW2	0x07		// 00111
#define LINE_GO_LEFT_SLOW3	0x03		// 00011
#define LINE_GO_LEFT_SLOW4	0x01		// 00001
#define LINE_GO_LEFT_SLOW5	0x17		// 10111

#define LINE_GO_RIGHT_SLOW1	0x1c		// 11110
#define LINE_GO_RIGHT_SLOW2	0x1e		// 11100
#define LINE_GO_RIGHT_SLOW3	0x18		// 11000
#define LINE_GO_RIGHT_SLOW4	0x10		// 10000
#define LINE_GO_RIGHT_SLOW5	0x1d		// 11101

#define	LINE_GO_TURN				0x1f		// 11111


#define READ_LINE1	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)
#define READ_LINE2	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define READ_LINE3	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)
#define READ_LINE4	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)
#define READ_LINE5	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)

typedef enum {
	STATE_HEAD = 0,
	STATE_LEFT = 1,
	STATE_RIGHT = 2
} State_Line;


void detect_Line(void (* handleLine[])(void));

#endif


