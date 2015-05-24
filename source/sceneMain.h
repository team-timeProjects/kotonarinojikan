#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include <deque>
#include	"Game.h"
class StageMNG;

class sceneMain :public Scene
{
	//-------- field ---------
private:
	StageMNG* stage;

	//-------- method ---------
public:
	sceneMain();
	~sceneMain();
	bool Initialize()override;
	void Update()override;
	void Render()override;

};

#endif //#ifndef _SCENEMAIN_H_
