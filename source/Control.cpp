#include	"Control.h"

POINT	Mouse::cursor;
bool	Mouse::lbClick = false;

bool	Mouse::Click()
{
	if (lbClick == true)
	{
		Mouse::ResetClickFlag();
		return true;
	}
	return false;
}

void	Mouse::Update()
{

}

void	Mouse::Render()
{
	
}
