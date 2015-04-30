#ifndef __CONTROL_H__
#define	__CONTROL_H__

#include	"iextreme.h"

class Mouse
{
private:
	
public:
	static	POINT	cursor;
	static	bool	lbClick;

	Mouse();
	~Mouse();

	static	bool	Click();
	static	void	ResetClickFlag(){ lbClick = false; }

	void	Update();
	void	Render();

};



#endif