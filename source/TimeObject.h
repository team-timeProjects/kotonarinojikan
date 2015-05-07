#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_


// 時計・蝋燭・メトロノームの基底クラス
class TimeObject
{
public:
	virtual ~TimeObject() {}

	virtual void Update()=0;
	virtual void Render()=0;
	virtual bool CheckWithin(const POINT& p, const int& startX, const int& startY, const int size) { return false; };
	
};

#endif