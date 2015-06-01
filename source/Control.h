#ifndef __CONTROL_H__
#define	__CONTROL_H__

#include	"iextreme.h"

class Mouse
{
private:
	
public:
	enum{
		LBUTTON, RBUTTON
	};
	static	POINT	cursor;
	static	bool	lbClick;
	static	bool	rbClick;
	static  int		WheelNotch;

	Mouse();
	~Mouse();

	static	bool	Click(int buttontype);
	static	void	ResetClickFlag(int buttontype){
		switch (buttontype){
		case LBUTTON:
			lbClick = false;
			break;
		case RBUTTON:
			rbClick = false;
			break;
		}
	}

	static void	Update();
	void	Render();

};



#endif