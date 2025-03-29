
#include "handle_mecanum.h"                  


///LINE HANDLE
void lineHandle_Head(){
		carForward(30);
		HAL_Delay(20);
		carStop();
//		HAL_Delay(10);
}

void lineHandle_Left(){
		carTurnLeft(30);
		HAL_Delay(20);
		carStop();
}

void lineHandle_Right(){
		carTurnRight(30);
		HAL_Delay(20);
		carStop();
}

void lineHandle_Turn_Left(){
		carTurnLeft(35);
		HAL_Delay(20);
		carStop();
}
void lineHandle_Turn_Right(){
		carTurnRight(35);
		HAL_Delay(20);
		carStop();
}

//////// AUTO HANDLE/////////////////////////

void autoHandle_Stop(){
	carStop();
}
void autoHalde_Head(){
	carForward(25);
}

//RE TRAI
void autoHandle_Left(){
	carTurnLeft(50);
	HAL_Delay(380);
	carStop();
	HAL_Delay(500);
}
//RE PHAI
void autoHandle_Right(){
	carTurnRight(50);
	HAL_Delay(380);
	carStop();
	HAL_Delay(500);
}
//QUAY DAU
void autoHandle_Turn(){
	carTurnBack(40);
	HAL_Delay(850);
	carStop();
	HAL_Delay(500);
}
//TIEN LEN CHAM
void autoHandle_HeadSlow(){
	carForward(25);
	HAL_Delay(300);
	carStop();
}
//LUI CHAM
void autoHandle_Reverse(){
	carBackward(25);
	HAL_Delay(300);
	carStop();
}




