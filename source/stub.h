#ifndef _STUB_H_
#define _STUB_H_

#include "../IEX/iextreme.h"
#include "Utility.h"
#include <map>


bool Wait()// 疑似処理
{
	static int count = 0;
	if (count > 60)
	{
		count = 0;
		return true;
	}
	else
	{
		count++;
		return false;
	}
}

bool IsFirstboot()
{
	return true;
}

bool GameMain()// ゲーム終了ならtrue
{
	return !(KEY_Get(KEY_ENTER) == 3);
}

bool IsGameClear()
{
	return true;
}

bool SelectStage()
{
	if (KEY_Get(KEY_ENTER) == 3)
	{
		// 選択されたステージを保持
		return true;
	}
	else
	{
		return false;
	}
}

bool AddStage()
{
	return Wait();
}

bool FadeIn()
{
	return Wait();
}

bool FadeOut()
{
	return Wait();
}

//　画像用抽象クラス
class ImageHoge
{
public:
	iex2DObj*  image;

	ImageHoge() { image = nullptr; }
	virtual ~ImageHoge() { SafeDelete(image); }
	virtual void Render() = 0;
};

//　背景
class BackHoge :public ImageHoge
{
public:
	BackHoge()
	{
		image = new iex2DObj("DATA/title.png");
	}

	void Render()override
	{
		Campus::Inst()->Render(image, 0, 0, 1280, 720, 0, 0, 1280, 720);
	}
};

class BigClockHoge :public ImageHoge
{
public:
	BigClockHoge()
	{
		image = new iex2DObj("DATA/bigclock.png");
	}
	void Render()override
	{
		Campus::Inst()->Render(image, 850, 90, 256, 256, 0, 0, 512, 512);
		POINT mouse;
		mouse.x = Mouse::cursor.x;
		mouse.y = Mouse::cursor.y;
		mouse = Campus::Inst()->TransCampusPos(mouse);
		mouse = Campus::Inst()->TransCampusPos(mouse);
		image->Render(mouse.x, mouse.y, 50, 50, 0, 0, 512, 512);
	}
};

class ClockHoge :ImageHoge
{
public:
	int count;
	ClockHoge()
	{
		count = 0;
		image = new iex2DObj("DATA/clock.png");
	}
	void Render()override
	{
		Campus::Inst()->Render(image, (int)(855 + 128 - 25 + sinf((count - 1) * ((2 * PI) / 12)) * 100),
							   (int)(90 + 128 - 25 - cosf((count - 1)*((2 * PI) / 12)) * 100),
							   50, 50, 0, 0, 128, 128);

	}
	void RenderSelect()
	{
		for (int i = 0; i < count; i++)
		{
			// 大時計に沿うように配置
			Campus::Inst()->Render(image, (int)(855 + 128 - 25 + sinf(i * ((2 * PI) / 12)) * 100),
								   (int)(90 + 128 - 25 - cosf(i*((2 * PI) / 12)) * 100),
								   50, 50, 0, 0, 128, 128);
		}
	}
	void Append()
	{
		count = count < 12 ? count + 1 : count;
	}
	int GetCount()
	{
		return count;
	}
	POINT GetPos(int id)
	{
		// 各時計の中心点
		POINT ret;
		ret.x = (LONG)(855 + 128 + sinf(id * ((2 * PI) / 12)) * 100);
		ret.y = (LONG)(90 + 128 - cosf(id*((2 * PI) / 12)) * 100);
		return ret;
	}
};

class StageHoge :ImageHoge
{
public:
	StageHoge()
	{
		image = new iex2DObj("DATA/main.png");
	}
	void Render()override
	{
		image->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
	}
};

#endif //#ifndef _STUB_H_
