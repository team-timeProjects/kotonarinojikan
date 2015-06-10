#include "TimeObject.h"
#include "Campus.h"

Number::Number():WIDTH(128), HEIGHT(128)
{}

Number::~Number()
{
	delete image;
}

void Number::Init()
{
	const int SIZE = 128;
	image = new iex2DObj("DATA/ゲーム画面/_suuji.png");
}

void Number::RenderCC(int num, int x, int y, float scale, float alpha, bool campus)
{
	//桁数判定
	int digit = num == 0 ? 1 : (int)log10(abs(num)) + 1;
	if(num < 0)digit += 1;// マイナス符号分

	//右端座標位置
	int startX = x + WIDTH*scale*(digit / 2 - 1)
		+ digit % 2 * (WIDTH*scale / 2);// 奇数個の数字なら画像半分だけオフセット

	if(num < 0)digit -= 1;

	//各桁描画
	int n = abs(num);
	int i = 0;// 一ケタの数字
	int dx = startX;
	int dy = y - HEIGHT*scale / 2;
	do
	{
		i = n % 10;
		Render(i, dx, dy, scale, alpha, campus);
		dx -= WIDTH*scale;
		n /= 10;
	} while(n != 0);
	if(num < 0)
		Render(MINUS, dx, dy, scale, alpha, campus);
}

void Number::Render(int n, int x, int y, float scale, float alpha, bool campus)
{
	ImageParam source = {0, 0, WIDTH, HEIGHT};
	switch(n)
	{
		case MINUS:
			source.x = WIDTH * 2;
			source.y = HEIGHT * 1;
			break;
		default:
			source.x = n % 8 * WIDTH;
			source.y = n / 8 * HEIGHT;
			break;
	}
	if(campus)
	{
		Campus::GetInst()->Add(image,
							   x, y,
							   WIDTH*scale, HEIGHT*scale,
							   source.x, source.y, source.w, source.h,
							   GetPoint(0, 0), 0.0f, 1.0f, 0UL, ARGB((int)(0xFF * alpha), 0xFF, 0xFF, 0xFF));
	}
	else
	{
		image->Render(x, y,
					  WIDTH*scale, HEIGHT*scale,
					  source.x, source.y, source.w, source.h,
					  GetPoint(0, 0), 0.0f, 1.0f, 0UL, ARGB((int)(0xFF * alpha), 0xFF, 0xFF, 0xFF));
	}
}

inline int Number::idxCalc(int base, int exponent)
{
	if(exponent != 0)
	{
		return (int)(idxCalc(base, exponent > 0 ? exponent - 1 : exponent + 1)*
					 exponent > 0 ? exponent : 1.0f / exponent);
	}
	else
		return 1;
}
