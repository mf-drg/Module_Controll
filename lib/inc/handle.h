#ifndef __HANDLE_H__
#define __HANDLE_H__

#include "74HC595.h"
#include "ps2x.h"
#include "HCSR05.h"
#include "servo.h"
#include "interrrupt.h"
#include "ledmatrix.h"
#include "i2c-lcd.h"

#include "handle_mecanum.h"    
#include "detectline.h" 



void updateAll(void);
void display_LCD(void);

void swap_Mode_PS2(void);
void control_PS2(void);


void update_status_car(void);
void update_Detect_Line(void);

#endif


