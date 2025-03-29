#include "handle.h"

/////////// CONFIG MODE ///////////
extern Mode mode;
extern Set set;
extern uint8_t distance;


///// PS2 CONTROLLER MODE ////////
extern PS2 *ps2;

///// AUTO MOVING MODE ////////
uint8_t flag_obstacle = 0;		//vat can
uint8_t flag_turn_car = 0;		//trang thai quay dau


/////////// CONFIG MODE /////////////////
void updateAll(){
	switch(mode){
		case CONTROL:
			PS2_Update();
			control_PS2();
			swap_Mode_PS2();
			break;
		case AUTO:
			PS2_Update();
			swap_Mode_PS2();
			update_status_car();
			break;
		case LINE:
			PS2_Update();
			swap_Mode_PS2();
			update_Detect_Line();
			break;
		default:
			break;
	} 
}

// config mode with ps2
void swap_Mode_PS2(void){
	if(ps2->button.CIRCLE){					// MODE
		if(mode < 3)
				mode++;
		if(mode >= 3)
				mode = CONTROL;
		set = NOT;
	}else if(ps2->button.TRIANGLE){	//UP
		if(mode == AUTO){
			if(distance < DISTANCE_MAX)
				distance += 5;
			if(distance >= DISTANCE_MAX)
				distance = DISTANCE_MIN;
		}	
	}else if(ps2->button.CROSS){		//DOWN
		if(mode == AUTO){
			if(distance > DISTANCE_MIN)
					distance -= 5;
			if(distance < DISTANCE_MIN)
					distance = DISTANCE_MIN;
		}
	}else if(ps2->button.SQUARE){
		
	}
}

void display_LCD(){
	char buf[16];
	if(mode == NONE)
	{
			lcd_clear();
			lcd_set_cursor(1,1);
			lcd_send_string("  PRESS BUTTON  ");
	}else if(mode == CONTROL){
			lcd_clear();
			lcd_set_cursor(1,1);
			lcd_send_string("CONTROL WITH PS2");
	}else if(mode == AUTO){
			lcd_clear();
			lcd_set_cursor(1,1);
			lcd_send_string("AUTO MOVING MODE");
			lcd_set_cursor(1,2);
			sprintf (buf, "  DISTANCE:%d  ", distance);
			lcd_send_string(buf);
	}else if(mode == LINE){
			lcd_clear();
			lcd_set_cursor(1,1);
			lcd_send_string("DETECT LINE MODE");
	}
}
////////////////////////////////////////////////////////



/*
//	MODE1: CONTROLLER WITH PS2
//  
*/
void control_PS2(void){
	if(ps2->button.UP){
		if(ps2->button.LEFT){
			carLeftForword(50);
		}else if(ps2->button.RIGHT){
			carRightForword(50);
		}else{
			carForward(50);
		}
	}
	else if(ps2->button.DOWN){
		if(ps2->button.LEFT){
			carLeftBackward(50);
		}else if(ps2->button.RIGHT){
			carRightBackward(50);
		}else{
			carBackward(50);
		}
	}
	else if(ps2->button.LEFT){
		carLeft(50);
	}
	else if(ps2->button.RIGHT){
		carRight(50);
	}
//	else if(ps2->button.L1){
//		carTurnLeftDrift(80);
//	}
	else if(ps2->button.R1){
		carTurnRightDrift(80);
	}
	else
		carStop();
}
///////////////////////////////////




/*
// MODE 2: AUTO MOVE
//
*/


// dieu khien xe
// Di thang
// b1: neu distance < 20cm => dung xe, quay trai do kc trai luu lai, quay phai do kc phai luu lai, tra servo ve dang truoc
// b1.1 neu ca 2 deu < 20 => lui xe va tiep tuc do lai tai b1
// b1.2 1.3 neu phai >= trai => re phai else re trái
// di thang

void update_status_car(){
	float disLeft, disRight;
//	float dis;

	if(flag_obstacle == 0){
//		dis = HCSR05_update();
		if(HCSR05_update() < distance){
			// xu ly dung xe
			autoHandle_Stop();
			// BAT CO CO VAT CAN
			flag_obstacle = 1;		
		}
		else{
			// xu ly di thang
			autoHalde_Head();
		}
		//NHAY RA NGOAI
	}else{ // flag_obstacle == 1
		// DO TRAI PHAI
		SERVO_LEFT;
		HAL_Delay(1000);
		disLeft = HCSR05_update(); 
		SERVO_RIGHT;
		HAL_Delay(1000);
		disRight = HCSR05_update();
		SERVO_FRONT;
		HAL_Delay(1000);
		
		// TH 1.1
		if((disLeft < 30) && (disRight < 30)){
			// TH 1.1.1
			if((disLeft > 15) && (disRight > 15) && (flag_turn_car == 0)){
				// XY LY QUAY DAU
				autoHandle_Turn();
				// BAT CO QUAY DAU LEN 1
				flag_turn_car = 1;
			}else if(flag_turn_car == 1){
				// xy ly di thang 1 doan ngan
				autoHandle_HeadSlow();			
			}else{	// TH 1.1.2	
				// xy ly di lui
				autoHandle_Reverse();
			}
		}else if(disRight >= disLeft){	// TH 1.2
			// xy ly re phai
			autoHandle_Right();
			//clear het cac co
			flag_obstacle = 0;
			flag_turn_car = 0;
		}else{													// TH 1.3
			// xy ly re trai
			autoHandle_Left();
			//clear het cac co
			flag_obstacle = 0;
			flag_turn_car = 0;
		}
	}
}



/*
//	MODE3: Line Detection
//  
*/

void (* handleLine[])(void) = {lineHandle_Head, lineHandle_Left, lineHandle_Right, lineHandle_Turn_Left, lineHandle_Turn_Right};

void update_Detect_Line(){
	detect_Line(handleLine);
}



