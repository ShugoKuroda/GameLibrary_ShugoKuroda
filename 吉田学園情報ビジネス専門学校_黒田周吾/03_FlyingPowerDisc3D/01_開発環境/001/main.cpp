//===================================================================
//
//		3Dゲーム[main.cpp]
//		Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include <stdio.h>
#include <time.h>
#include "camera.h"
#include "light.h"
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "billboard.h"
#include "meshfield.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "load.h"
#include "title.h"
#include "pause.h"
#include "tutorial.h"
#include "fade.h"
#include "game.h"
#include "sound.h"

//======================================================
//	グローバル変数宣言
//======================================================
LPDIRECT3D9	g_pD3D = NULL;						//Directx3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Directx3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;						//現在のモード
HWND g_hWnd;									//ウインドウハンドル(識別子)

#ifdef _DEBUG
LPD3DXFONT g_pFont = NULL;		//フォントへのポインタ
#endif							//DEBUG
int g_nCountFPS;				//FPSカウンタ

//==================================================
//	メイン関数
//==================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		//ファイルのアイコン
	};

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExeLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	MSG msg;				//メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			//画面サイズの構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPED, FALSE);

	//ウインドウを生成
	g_hWnd = CreateWindowEx(
		0,								//拡張ウィンドウスタイル
		CLASS_NAME,						//ウィンドウクラスの名前
		WINDOW_NAME,					//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
		CW_USEDEFAULT,					//ウィンドウの左上x座標
		CW_USEDEFAULT,					//ウィンドウの左上ｙ座標
		(rect.right - rect.left),		//ウィンドウの幅
		(rect.bottom - rect.top),		//ウィンドウの高さ
		NULL,							//親ウィンドウのハンドル
		NULL,							//メニューハンドルまたは子ウィンドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウィンドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, g_hWnd, TRUE)))	//ウインドウ表示：TRUE,フルスクリーン表示：FALSE
	{//初期化が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化する
	dwExeLastTime = timeGetTime();		//現在時刻を取得(保存)
	dwFPSLastTime = timeGetTime();
	dwFrameCount = 0;

	//ウインドウの表示
	ShowWindow(g_hWnd, nCmdShow);			//ウィンドウの表示状態を設定
	UpdateWindow(g_hWnd);					//クライアント領域の更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{//メッセージの設定
				TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージに変換
				DispatchMessage(&msg);		//ウィンドウプロシージャにメッセージを送出
			}
		}
		else
		{//Directxの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒ごとに実行
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExeLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExeLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	//終了処理
	Uninit();

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//============================================================
//	ウインドウプロシージャ
//============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:		//ウインドウ破壊のメッセージ

	//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//ウインドウの破壊
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//==========================================================
//	初期化処理
//==========================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//directx3Dデバイス生成(描画処理をハードウェア頂点処理をCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//directx3Dデバイス生成(描画処理頂点処理をCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートパラメータの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//αブレンディング処理設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		//最初のα引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		//２番目のα引数

#ifdef _DEBUG
		//デバッグ情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 23, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
#endif // _DEBUG

	//ランダム種の初期化
	srand((unsigned int)time(0));

	if (FAILED(InitInput(hInstance, hWnd)))
	{//入力処理の初期化
		return E_FAIL;
	}

	//サウンドの初期化
	InitSound(hWnd);

	//フェードの設定
	InitFade(g_mode);

	//ロード処理
	LoadSetModel(hWnd);
	LoadSetPlayer(hWnd);

	return S_OK;
}

//======================================================
//	終了処理
//======================================================
void Uninit(void)
{
	//分解能を戻す
	timeEndPeriod(1);

	//入力処理の終了処理
	UninitInput();

#ifdef _DEBUG
	if (g_pFont != NULL)
	{//デバッグ情報表示用フォントを設定
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif // _DEBUG

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//======================================================
//	更新処理
//======================================================
void Update(void)
{
	//入力処理の更新処理
	UpdateInput();

	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UpdateGame();
		break;
	}

	//フェード更新処理
	UpdateFade();
}

//======================================================
//	描画処理
//======================================================
void Draw(void)
{
	//カメラ情報の取得
	Camera *pCamera = GetCamera();

	//画面クリア(バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面

			//カメラビューポートの設定
			pCamera->viewport.X = 0;
			pCamera->viewport.Y = 0;
			pCamera->viewport.Width = SCREEN_WIDTH;
			pCamera->viewport.Height = SCREEN_HEIGHT;
			pCamera->viewport.MinZ = 10.0f;
			pCamera->viewport.MaxZ = 1000.0f;
			g_pD3DDevice->SetViewport(&pCamera->viewport);

			DrawTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			DrawTutorial();
			break;

		case MODE_GAME:			//ゲーム画面

			//カメラビューポートの設定(1P側)
			pCamera->viewport.X = 0;
			pCamera->viewport.Y = 0;
			pCamera->viewport.Width = SCREEN_WIDTH / 2;
			pCamera->viewport.Height = SCREEN_HEIGHT;
			pCamera->viewport.MinZ = 10.0f;
			pCamera->viewport.MaxZ = 1000.0f;
			g_pD3DDevice->SetViewport(&pCamera->viewport);

			//画面クリア(バックバッファ＆Zバッファのクリア)
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			//ゲーム画面の描画
			DrawGame(CAMERA_1);
			//フェード描画処理
			DrawFade();
			//カメラのポインタ加算
			pCamera++;

			//カメラビューポートの設定(2P側)
			pCamera->viewport.X = SCREEN_WIDTH / 2;
			pCamera->viewport.Y = 0;
			pCamera->viewport.Width = SCREEN_WIDTH / 2;
			pCamera->viewport.Height = SCREEN_HEIGHT;
			pCamera->viewport.MinZ = 10.0f;
			pCamera->viewport.MaxZ = 1000.0f;
			g_pD3DDevice->SetViewport(&pCamera->viewport);

			//画面クリア(バックバッファ＆Zバッファのクリア)
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			//ゲーム画面の描画
			DrawGame(CAMERA_2);

			break;
		}
		//フェード描画処理
		DrawFade();

#ifdef _DEBUG
		//FPS表示
		DrawFPS();
#endif // _DEBUG

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//=======================================================
//	FPS表示処理
//=======================================================
void DrawFPS(void)
{
	////カメラ情報の取得
	//Camera *pCamera = GetCamera();

	//RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char str[256];

	//sprintf(str, "FPS:%d\nPosVX:%.1f\nPosVY:%.1f\nPosVZ:%.1f\nPosRX:%.1f\nPosRY:%.1f\nPosRZ:%.1f\n\nJoyPadStickLeft_x:%.1f\nJoyPadStickLeft_y:%.1f\nJoyPadStickLeft_z:%.1f",
	//	g_nCountFPS, pCamera->posV.x, pCamera->posV.y, pCamera->posV.z, pCamera->posR.x, pCamera->posR.y, pCamera->posR.z,
	//	GetJoypadStick(JOYKEY_LEFT_STICK,1).x, GetJoypadStick(JOYKEY_LEFT_STICK, 1).y, GetJoypadStick(JOYKEY_LEFT_STICK, 1).z);

	////テキスト描画
	//g_pFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
}
#endif

//=======================================================
//	デバイスの取得
//=======================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=======================================================
//	モードの設定
//=======================================================
void SetMode(MODE mode)
{
	//現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UninitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UninitGame();
		break;
	}

	//新しい画面の初期化
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		InitGame();
		break;
	}

	g_mode = mode;		//現在の画面を切り替える
}

//=======================================================
//	デバイスの取得
//=======================================================
HWND GetHWnd(void)
{
	return g_hWnd;
}