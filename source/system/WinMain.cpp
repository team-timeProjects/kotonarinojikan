#include	"iextreme.h"
#include	"Framework.h"
#include	"sceneMain.h"
#include	"Control.h"
#include	"../../EDX/EDXLIB.h"
#include	"../sceneTitle.h"
#include	"../DataOwner.h"
#include	"../TransitionBat.h"
#include	"../Pumpkin.h"
#include	"../Sound.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//*****************************************************************************************************************************
//
//

//
//*****************************************************************************************************************************

char*	AppTitle = "IEX2010";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode = SCREEN720p;

Framework*	MainFrame = NULL;


//*****************************************************************************************************************************
//		アプリケーション初期化
//*****************************************************************************************************************************

BOOL	InitApp(HWND hWnd)
{
	//	IEXシステム初期化
	IEX_Initialize(hWnd, bFullScreen, ScreenMode);
	IEX_InitAudio();
	IEX_InitInput();

	//	システムの初期化
	SYSTEM_Initialize();

	Sound::Initialize();
	//	メインフレームワーク生成
	MainFrame = new Framework();
	//	初期シーン登録
	MainFrame->ChangeScene(new sceneTitle());
	//MainFrame->ChangeScene(new sceneMain());

	return TRUE;
}

//*****************************************************************************************************************************
//		ウィンドウプロシージャ
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int nWheelFraction = 0;
	int zDelta = 0;
	int Notch = 0;
	switch (message)
	{
	case WM_ACTIVATE:
		nWheelFraction = 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONUP:
		Mouse::lbClick = true;
		break;
	case WM_RBUTTONUP:
		Mouse::rbClick = true;
		break;
	case WM_MOUSEWHEEL:
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		zDelta += nWheelFraction;
		Notch = zDelta / WHEEL_DELTA;
		nWheelFraction = zDelta%WHEEL_DELTA;

		EDX::EDX_Input::Click[EDX::EDX_WHEEL] = Notch;
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
		case VK_F1:		iexSystem::OpenDebugWindow(); return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//*****************************************************************************************************************************
//		ウィンドウ作成
//*****************************************************************************************************************************
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND			hWnd;

	//	スクリーンサイズ取得
	RECT	WindowSize;
	iexSystem::GetScreenRect(ScreenMode, WindowSize);

	//	ウィンドウクラス設定
	WNDCLASS	wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "IEX2010";
	RegisterClass(&wc);

	//	ウィンドウ作成
	if (!bFullScreen){
		AdjustWindowRect(&WindowSize, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow("IEX2010", AppTitle, WS_OVERLAPPEDWINDOW,
			0, 0, WindowSize.right - WindowSize.left, WindowSize.bottom - WindowSize.top,
			NULL, NULL, hInstance, NULL);
	}
	else {
		hWnd = CreateWindow("IEX2010", AppTitle, WS_POPUP, 0, 0, WindowSize.right, WindowSize.bottom, NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);
	}
	if (!hWnd) return NULL;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	_CrtSetBreakAlloc(543);

	MSG		msg;
	HWND	hWnd;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) bFullScreen = TRUE;


	hWnd = InitWindow(hInstance, nCmdShow);
	InitApp(hWnd);

	DataOwner::GetInst()->Init();
	TransitionBat::GetInst()->Init();
	Pumpkin::GetInst()->Init();

	//	メインループ
	for (;;){
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (MainFrame->Update()) MainFrame->Render();
		}
	}

	TransitionBat::GetInst()->Release();
	Pumpkin::GetInst()->Release();
	//	全解放	
	delete	MainFrame;
	SYSTEM_Release();
	IEX_ReleaseAudio();
	iexParticle::Release();
	iexSystem::CloseDebugWindow();
	IEX_ReleaseInput();
	IEX_Release();

	return 0;
}
