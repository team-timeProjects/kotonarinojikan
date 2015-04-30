#ifndef _SCENETITLE_H_
#define _SCENETITLE_H_

class sceneTitle :public Scene
{
	//----------- field -----------
private:

	//----------- method ----------
public:
	sceneTitle();
	~sceneTitle();
	bool Initialize()override;
	void Update()override;
	void Render()override;
};

#endif //#ifndef _SCENETITLE_H_
