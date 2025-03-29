
#include "detectline.h"                  

State_Line stateLine;

//callback func xu lý cac truong hop detect duoc
void detect_Line(void (* handleLine[])(void)){
	uint8_t detectLine = ((READ_LINE5 ? 1 : 0) | (READ_LINE4 ? 2 : 0) | (READ_LINE3 ? 4 : 0) | (READ_LINE2 ? 8 : 0) | (READ_LINE1 ? 16 : 0));
	
	switch(detectLine){
		case LINE_GO_HEAD1:		// di thang
		case LINE_GO_HEAD2:
		case LINE_GO_HEAD3:
		case LINE_GO_HEAD4:
		case LINE_GO_HEAD5:
			handleLine[0]();
//			stateLine = STATE_HEAD;
			break;
		case LINE_GO_LEFT_SLOW1:	// lech trai
		case LINE_GO_LEFT_SLOW2:
		case LINE_GO_LEFT_SLOW3:
		case LINE_GO_LEFT_SLOW4:
		case LINE_GO_LEFT_SLOW5:
			handleLine[1]();
			stateLine = STATE_LEFT;
			break;
		case LINE_GO_RIGHT_SLOW1:	// lech phai
		case LINE_GO_RIGHT_SLOW2:
		case LINE_GO_RIGHT_SLOW3:
		case LINE_GO_RIGHT_SLOW4:
		case LINE_GO_RIGHT_SLOW5:
			handleLine[2]();
			stateLine = STATE_RIGHT;
			break;
		case LINE_GO_TURN:				// quay dau
		default:									// th khác
			if(stateLine == STATE_LEFT)
			{
				handleLine[3]();				// quay dau trai
			}else{
				handleLine[4]();					// quay dau phai
			}
			
			break;
	}
}





