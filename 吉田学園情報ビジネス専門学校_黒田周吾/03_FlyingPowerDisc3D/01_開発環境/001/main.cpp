//===================================================================
//
//		3D�Q�[��[main.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
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
//	�O���[�o���ϐ��錾
//======================================================
LPDIRECT3D9	g_pD3D = NULL;						//Directx3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Directx3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;						//���݂̃��[�h
HWND g_hWnd;									//�E�C���h�E�n���h��(���ʎq)

#ifdef _DEBUG
LPD3DXFONT g_pFont = NULL;		//�t�H���g�ւ̃|�C���^
#endif							//DEBUG
int g_nCountFPS;				//FPS�J�E���^

//==================================================
//	���C���֐�
//==================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�v���V�[�W��
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)		//�t�@�C���̃A�C�R��
	};

	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExeLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	MSG msg;				//���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			//��ʃT�C�Y�̍\����

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPED, FALSE);

	//�E�C���h�E�𐶐�
	g_hWnd = CreateWindowEx(
		0,								//�g���E�B���h�E�X�^�C��
		CLASS_NAME,						//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�B���h�E�̍���x���W
		CW_USEDEFAULT,					//�E�B���h�E�̍��゙���W
		(rect.right - rect.left),		//�E�B���h�E�̕�
		(rect.bottom - rect.top),		//�E�B���h�E�̍���
		NULL,							//�e�E�B���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL);							//�E�B���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hInstance, g_hWnd, TRUE)))	//�E�C���h�E�\���FTRUE,�t���X�N���[���\���FFALSE
	{//�����������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//����������
	dwExeLastTime = timeGetTime();		//���ݎ������擾(�ۑ�)
	dwFPSLastTime = timeGetTime();
	dwFrameCount = 0;

	//�E�C���h�E�̕\��
	ShowWindow(g_hWnd, nCmdShow);			//�E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(g_hWnd);					//�N���C�A���g�̈�̍X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
				DispatchMessage(&msg);		//�E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o
			}
		}
		else
		{//Directx�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b���ƂɎ��s
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExeLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExeLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//============================================================
//	�E�C���h�E�v���V�[�W��
//============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:		//�E�C���h�E�j��̃��b�Z�[�W

	//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//�E�C���h�E�̔j��
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//==========================================================
//	����������
//==========================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//directx3D�f�o�C�X����(�`�揈�����n�[�h�E�F�A���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//directx3D�f�o�C�X����(�`�揈�����_������CPU�ōs��)
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
	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//���u�����f�B���O�����ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		//�ŏ��̃�����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		//�Q�Ԗڂ̃�����

#ifdef _DEBUG
		//�f�o�b�O���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 23, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
#endif // _DEBUG

	//�����_����̏�����
	srand((unsigned int)time(0));

	if (FAILED(InitInput(hInstance, hWnd)))
	{//���͏����̏�����
		return E_FAIL;
	}

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�t�F�[�h�̐ݒ�
	InitFade(g_mode);

	//���[�h����
	LoadSetModel(hWnd);
	LoadSetPlayer(hWnd);

	return S_OK;
}

//======================================================
//	�I������
//======================================================
void Uninit(void)
{
	//����\��߂�
	timeEndPeriod(1);

	//���͏����̏I������
	UninitInput();

#ifdef _DEBUG
	if (g_pFont != NULL)
	{//�f�o�b�O���\���p�t�H���g��ݒ�
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif // _DEBUG

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//======================================================
//	�X�V����
//======================================================
void Update(void)
{
	//���͏����̍X�V����
	UpdateInput();

	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		UpdateTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		UpdateGame();
		break;
	}

	//�t�F�[�h�X�V����
	UpdateFade();
}

//======================================================
//	�`�揈��
//======================================================
void Draw(void)
{
	//�J�������̎擾
	Camera *pCamera = GetCamera();

	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		switch (g_mode)
		{
		case MODE_TITLE:		//�^�C�g�����

			//�J�����r���[�|�[�g�̐ݒ�
			pCamera->viewport.X = 0;
			pCamera->viewport.Y = 0;
			pCamera->viewport.Width = SCREEN_WIDTH;
			pCamera->viewport.Height = SCREEN_HEIGHT;
			pCamera->viewport.MinZ = 10.0f;
			pCamera->viewport.MaxZ = 1000.0f;
			g_pD3DDevice->SetViewport(&pCamera->viewport);

			DrawTitle();
			break;

		case MODE_TUTORIAL:		//�`���[�g���A�����
			DrawTutorial();
			break;

		case MODE_GAME:			//�Q�[�����

			//�J�����r���[�|�[�g�̐ݒ�(1P��)
			pCamera->viewport.X = 0;
			pCamera->viewport.Y = 0;
			pCamera->viewport.Width = SCREEN_WIDTH / 2;
			pCamera->viewport.Height = SCREEN_HEIGHT;
			pCamera->viewport.MinZ = 10.0f;
			pCamera->viewport.MaxZ = 1000.0f;
			g_pD3DDevice->SetViewport(&pCamera->viewport);

			//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			//�Q�[����ʂ̕`��
			DrawGame(CAMERA_1);
			//�t�F�[�h�`�揈��
			DrawFade();
			//�J�����̃|�C���^���Z
			pCamera++;

			//�J�����r���[�|�[�g�̐ݒ�(2P��)
			pCamera->viewport.X = SCREEN_WIDTH / 2;
			pCamera->viewport.Y = 0;
			pCamera->viewport.Width = SCREEN_WIDTH / 2;
			pCamera->viewport.Height = SCREEN_HEIGHT;
			pCamera->viewport.MinZ = 10.0f;
			pCamera->viewport.MaxZ = 1000.0f;
			g_pD3DDevice->SetViewport(&pCamera->viewport);

			//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
			g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			//�Q�[����ʂ̕`��
			DrawGame(CAMERA_2);

			break;
		}
		//�t�F�[�h�`�揈��
		DrawFade();

#ifdef _DEBUG
		//FPS�\��
		DrawFPS();
#endif // _DEBUG

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//=======================================================
//	FPS�\������
//=======================================================
void DrawFPS(void)
{
	////�J�������̎擾
	//Camera *pCamera = GetCamera();

	//RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char str[256];

	//sprintf(str, "FPS:%d\nPosVX:%.1f\nPosVY:%.1f\nPosVZ:%.1f\nPosRX:%.1f\nPosRY:%.1f\nPosRZ:%.1f\n\nJoyPadStickLeft_x:%.1f\nJoyPadStickLeft_y:%.1f\nJoyPadStickLeft_z:%.1f",
	//	g_nCountFPS, pCamera->posV.x, pCamera->posV.y, pCamera->posV.z, pCamera->posR.x, pCamera->posR.y, pCamera->posR.z,
	//	GetJoypadStick(JOYKEY_LEFT_STICK,1).x, GetJoypadStick(JOYKEY_LEFT_STICK, 1).y, GetJoypadStick(JOYKEY_LEFT_STICK, 1).z);

	////�e�L�X�g�`��
	//g_pFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
}
#endif

//=======================================================
//	�f�o�C�X�̎擾
//=======================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=======================================================
//	���[�h�̐ݒ�
//=======================================================
void SetMode(MODE mode)
{
	//���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		UninitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		UninitGame();
		break;
	}

	//�V������ʂ̏�����
	switch (mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		InitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		InitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		InitGame();
		break;
	}

	g_mode = mode;		//���݂̉�ʂ�؂�ւ���
}

//=======================================================
//	�f�o�C�X�̎擾
//=======================================================
HWND GetHWnd(void)
{
	return g_hWnd;
}