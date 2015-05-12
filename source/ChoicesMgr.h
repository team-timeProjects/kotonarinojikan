#ifndef _CHOICESMGR_H_
#define _CHOICESMGR_H_

#include <vector>

class TimeObject;// ŽžŒv‚â‚ë‚¤‚»‚­‚ÌŠî’êƒNƒ‰ƒX

class ChoicesMgr
{
	//------------ field ---------------
private:
	std::vector<TimeObject*> choicesList;

	//------------ method ----------------
public:
	ChoicesMgr();
	~ChoicesMgr();
	void Init();
	void Update();
	void Render();
};

#endif