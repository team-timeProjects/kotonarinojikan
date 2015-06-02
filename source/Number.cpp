#include "TimeObject.h"
#include "Campus.h"

Number::~Number()
{
	delete image;
}

void Number::Init()
{
	const int SIZE = 128;
	image = new iex2DObj("DATA/ƒQ[ƒ€‰æ–Ê/suuji.png");
	param[MINUS_1] = ImageParam(1 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[MINUS_2] = ImageParam(0 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[MINUS_3] = ImageParam(7 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_4] = ImageParam(6 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_5] = ImageParam(5 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_6] = ImageParam(4 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_7] = ImageParam(3 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_8] = ImageParam(2 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_9] = ImageParam(1 * SIZE, 0 * SIZE, SIZE, SIZE);
	param[MINUS_10] = ImageParam(0 * SIZE, 0 * SIZE, SIZE, SIZE);

	param[PLUS_1] = ImageParam(2 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[PLUS_2] = ImageParam(3 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[PLUS_3] = ImageParam(4 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[PLUS_4] = ImageParam(5 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[PLUS_5] = ImageParam(6 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[PLUS_6] = ImageParam(7 * SIZE, 1 * SIZE, SIZE, SIZE);
	param[PLUS_7] = ImageParam(0 * SIZE, 2 * SIZE, SIZE, SIZE);
	param[PLUS_8] = ImageParam(1 * SIZE, 2 * SIZE, SIZE, SIZE);
	param[PLUS_9] = ImageParam(2 * SIZE, 2 * SIZE, SIZE, SIZE);
	param[PLUS_10] = ImageParam(3 * SIZE, 2 * SIZE, SIZE, SIZE);
}

void Number::Render(int num, int x, int y, float scale, bool campus)
{
	if(campus)
		Campus::GetInst()->Add(image,
		x - param[num + 10].w*scale / 2, y - param[num + 10].h*scale / 2, param[num + 10].w*scale, param[num + 10].h*scale,
		param[num + 10].x, param[num + 10].y, param[num + 10].w, param[num + 10].h);
	else
		image->Render(x - param[num + 10].w*scale / 2, y - param[num + 10].h*scale / 2, param[num + 10].w*scale, param[num + 10].h*scale,
		param[num + 10].x, param[num + 10].y, param[num + 10].w, param[num + 10].h);
}
