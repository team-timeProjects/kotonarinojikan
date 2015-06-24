#include "../IEX/iextreme.h"
#include "EDXLIB.h"

int EDX::EDX_Input::Click[3];

int EDX::MouseGet(int butontype){
	return  EDX::EDX_Input::GetClickModule(butontype);
}

BOOL EDX::EDX_Input::GetState(){
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)Click[EDX_CLICK_L]++;
	else Click[EDX_CLICK_L] = 0;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)Click[EDX_CLICK_R]++;
	else Click[EDX_CLICK_R] = 0;
	return TRUE;

}


unsigned int EDX::EDX_Input::GetClickModule(int BUTTON_TYPE){
	if (BUTTON_TYPE <= -1 || 3 <= BUTTON_TYPE)return 0;
	return Click[BUTTON_TYPE];
}