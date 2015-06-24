#include	"Control.h"

POINT	Mouse::cursor;
bool	Mouse::lbClick = false;
bool	Mouse::rbClick = false;
int		Mouse::WheelNotch = 0;

bool	Mouse::Click(int buttontype)
{
	switch (buttontype){
	case LBUTTON:
		if (lbClick == true)
		{
			Mouse::ResetClickFlag(buttontype);
			return true;
		}
		break;
	case RBUTTON:
		if (rbClick == true)
		{
			Mouse::ResetClickFlag(buttontype);
			return true;
		}
		break;
	}
	return false;
}

void	Mouse::Update()
{
	const float leftWndFrame = 8.0f;
	const float topWndFrame = 29;
	//à íuçXêV
	GetCursorPos(&cursor);
	ScreenToClient(iexSystem::Window, &cursor);
	
}

void	Mouse::Render()
{
	
}
