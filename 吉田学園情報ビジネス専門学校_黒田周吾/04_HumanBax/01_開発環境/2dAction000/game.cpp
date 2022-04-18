#include "main.h"
#include "game.h"
#include "player.h"
#include "bg.h"
#include "time.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "block.h"
#include "switch.h"
#include "particle.h"
#include "game_tutorial.h"
#include "flag.h"

//�}�N����`
#define	PAUSE_IMAGE_MAX			(5)			//�|�[�Y��ʂɕK�v�ȉ摜��
#define	PAUSE_SELECT_MAX		(4)			//�|�[�Y��ʂɕK�v�ȑI��
#define PAUSE_UI_SIZE_X			(1000.0f)	//�|�[�Y���UI��X�T�C�Y
#define PAUSE_UI_SIZE_Y			(400.0f)	//�|�[�Y���UI��Y�T�C�Y
#define PAUSE_SELECT_SIZE_X		(160.0f)	//�|�[�Y��ʂ̑I�����ڂ�X�T�C�Y
#define PAUSE_SELECT_SIZE_Y		(200.0f)	//�|�[�Y��ʂ̑I�����ڂ�Y�T�C�Y

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_IMAGE_MAX] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause[3] = { NULL };			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_PausePos[2];		//�|�[�Y��ʂ̈ʒu
bool g_bPause;					//�Q�[���̃|�[�Y(�ꎞ��~)����
bool g_bDiePlayer;				//�v���C���[�̎��S����
float g_fColPauseA[PAUSE_IMAGE_MAX + 1];	//�|�[�Y�A���t�@�l
int g_nPauseSelecting;						//�|�[�Y��ʒ��ɑI���������ڂ̕ϐ�
int g_nCntCol;			//�|�[�Y��ʂ̃t�F�[�h�̏o�����p
int g_nColPoseCnt;		//�|�[�Y��ʒ��ɑI�����Ă��鍀�ڂ��`�J�`�J������p�ϐ�
int g_nFade;			//�t�F�[�h�܂ł̗]�C
int g_nCntSet;			//�u���b�N�̔z�u�^�C�~���O���炵�p�ϐ�

//======================================================
//		�Q�[���S�̂̏���������
//======================================================
void InitGame(void)
{
	//�e�|�C���^�[��錾
	Block *pBlock;
	Switch *pSwitch;
	FLAG *pFlag;

	//�e���̎擾
	pBlock = GetBlock();
	pSwitch = GetSwitch();
	pFlag = GetFlag();

	//�w�i�̏���������
	InitBg();

	//�Q�[�����̃`���[�g���A���摜�̏�����
	InitGameTutorial();

	//�X�C�b�`�̏���������
	InitSwitch();

	//�v���C���[�̏���������
	InitPlayer();

	//�u���b�N�̏���������
	InitBlock();

	//���Ԓn�_�̏���������
	InitGameFlag();

	//�u���b�N�z�u(	�ʒu / �ړ��� / �ړ������J�E���^�[ / ��� / �M�~�b�N�ԍ� / �� / ���� / �e�N�X�`���̑傫��(X) / �e�N�X�`���̑傫��(Y))
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH / 1.2f, SCREEN_HEIGHT - 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),	 0,	 0, 0, SCREEN_WIDTH + 500.0f,	  100.0f,			 1.0f, 1.0f);				//�n��
	SetBlock(D3DXVECTOR3(-200.0f, SCREEN_HEIGHT / 2, 0.0f),				D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  0, 0, SCREEN_WIDTH / 6,  SCREEN_HEIGHT * 2, 1.0f, 1.0f);						//����
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH -100.0f, 500.0f, 0.0f),				D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  2, 0, 70.0f,			  70.0f,			 1.0f, 1.0f);			//���u���b�N(�S�[��)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 400.0f, 0.0f),				D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  0, 0, 100.0f,			  160.0f,			 1.0f, 1.0f);				//�󒆂ɕ����Ă�
	SetBlock(D3DXVECTOR3((SCREEN_WIDTH * 1.2f) + 75.0f, 400.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  0, 0, 50.0f,			  50.0f,			 1.0f, 1.0f);			//�󒆂ɕ����Ă�
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.6f + 10.0f, 520.0f, 0.0f),				 D3DXVECTOR3(0.0f, 1.0f, 0.0f), 140,  0, 0, 200.0f,			  50.0f,			 1.0f, 1.0f);	//���t�g(�c)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.9f - 20.0f, SCREEN_HEIGHT - 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 500.0f, 220.0f, 1.0f, 1.0f);				//�n��2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.2f - 100.0f, SCREEN_HEIGHT - 60.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 110.0f, 100.0f, 1.0f, 1.0f);				//�n��2��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.3f - 20.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 200.0f, 40.0f, 1.0f, 1.0f);						//�󒆂ɕ����Ă�2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.3f - 20.0f, SCREEN_HEIGHT / 3, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 0, 100.0f, 100.0f, 1.0f, 1.0f);					//���u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.4f - 50.0f, SCREEN_HEIGHT - 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 500.0f, 220.0f, 1.0f, 1.0f);				//�n��2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.7f - 80.0f, 250.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 160, 0, 0, 200.0f, 50.0f, 1.0f, 1.0f);								//���t�g(�c)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 80.0f, SCREEN_HEIGHT - 130.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 300.0f, 50.0f, 1.0f, 1.0f);				//�n��2�R��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.8f - 80.0f, SCREEN_HEIGHT - 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 40.0f, 300.0f, 1.0f, 1.0f);				//�n��2(��)�R
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 70.0f, SCREEN_HEIGHT - 425.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 300.0f, 50.0f, 1.0f, 1.0f);				//�n��2�R��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.0f - 0.0f, SCREEN_HEIGHT - 35.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 110.0f, 40.0f, 1.0f, 1.0f);					//�n��2��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 40.0f, SCREEN_HEIGHT - 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 0, 100.0f, 100.0f, 1.0f, 1.0f);				//���u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.1f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 160, 0, 0, 100.0f, 50.0f, 1.0f, 1.0f);								//���t�g(�c)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 60.0f, SCREEN_HEIGHT - 600.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 4, 0, 100.0f, 100.0f, 1.0f, 1.0f);				//���u���b�N(�Ƃ��t��)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.1f - 70.0f, SCREEN_HEIGHT - 75.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH * 2, 160.0f, 1.0f, 1.0f);		//�n��3
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.5f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 120, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.7f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), 90, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.9f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 11.0f, 0.0f), 40, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)
	//SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.7f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 2, 0, 70.0f, 70.0f, 1.0f, 1.0f);											//���u���b�N(�S�[��)��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.8f - 10.0f, SCREEN_HEIGHT - 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH * 1.5, 50.0f, 1.0f, 1.0f);		//�n��4��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.2f - 120.0f, 220.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 190, 0, 0, 145.0f, 50.0f, 1.0f, 1.0f);								//���t�g(�c)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.0f - 100.0f, SCREEN_HEIGHT / 3, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH * 1.5, 50.0f, 1.0f, 1.0f);			//�n��4��
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.3f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 100.0f, 100.0f, 1.0f, 1.0f);				//�Ƃ��u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.5f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 145.0f, 100.0f, 1.0f, 1.0f);				//�Ƃ��u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.8f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 250.0f, 100.0f, 1.0f, 1.0f);				//�Ƃ��u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.8f - 0.0f, SCREEN_HEIGHT - 170.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 40.0f, 40.0f, 1.0f, 1.0f);				//�󒆂ɕ����Ă�2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.1f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 300.0f, 100.0f, 1.0f, 1.0f);				//�Ƃ��u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.0f + 10.0f, 525.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 60, 0, 0, 50.0f, 50.0f, 1.0f, 1.0f);							//���t�g(��)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.4f - 80.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 300.0f, 100.0f, 1.0f, 1.0f);			//�Ƃ��u���b�N
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.4f + 10.0f, 525.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 60, 0, 0, 50.0f, 50.0f, 1.0f, 1.0f);							//���t�g(��)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.6f + 20.0f, SCREEN_HEIGHT / 1.5f + 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 100.0f, 550.0f, 1.0f, 1.0f);				//��(�Ō�)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.7f + 20.0f, SCREEN_HEIGHT / 3, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 160.0f, 50.0f, 1.0f, 1.0f);						//�n��(�Ō��)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.9f + 20.0f, SCREEN_HEIGHT - 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH / 1.5f, 200.0f, 1.0f, 1.0f);		//�n��(�Ō�)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 7.1f, SCREEN_HEIGHT - 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 6, 0, 100.0f, 100.0f, 1.0f, 1.0f);			//�o��

	//�X�C�b�`�z�u( �ʒu / �� / ����)
	SetSwitch(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 310.0f, 0.0f), 50.0f, 30.0f);
	SetSwitch(D3DXVECTOR3(SCREEN_WIDTH * 6.0f - 90.0f, SCREEN_HEIGHT - 55.0f, 0.0f), 50.0f, 30.0f);
	SetSwitch(D3DXVECTOR3(SCREEN_WIDTH * 6.6f - 120.0f, SCREEN_HEIGHT - 55.0f, 0.0f), 50.0f, 30.0f);

	//�Q�[�g�z�u( �ʒu / �ړ��� / �ړ������J�E���^�[ / ��� / �M�~�b�N�ԍ� / �� / ���� / �e�N�X�`���̑傫��(X) / �e�N�X�`���̑傫��(Y))
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 550.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3, 1, 50.0f, 150.0f, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.3f - 0.0f, SCREEN_HEIGHT / 4 - 45.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3, 2, 50.0f, 150.0f, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.5f - 0.0f, SCREEN_HEIGHT / 4 - 45.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3, 3, 50.0f, 150.0f, 1.0f, 1.0f);

	//���Ԓn�_�z�u
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 310.0f, 0.0f), 0);
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 1.4f, 310.0f, 0.0f), 0);
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 5.2f, 310.0f, 0.0f), 0);
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 7.2f, 310.0f, 0.0f), 1);

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�^�C���̏���������
	InitTime();

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (FlagStage() == 1)
	{

	}

	//�e���ڂ̏�����
	g_PausePos[0] = D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, 0.0f);				//�|�[�Y��ʂ̑I�����ڈʒu�̏�����
	g_PausePos[1] = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / 2.0f) - 20.0f, 0.0f);		//�|�[�Y���UI�̈ʒu�̏�����
	g_nCntCol = 0;			//�|�[�Y��ʂ̃t�F�[�h�̏o�����p
	g_nFade = 0;			//�t�F�[�h�܂ł̗]�C
	g_nPauseSelecting = 2;	//�Q�[���𑱂����I���ɏ�����
	g_nColPoseCnt = 0;		//�|�[�Y��ʒ��ɑI�����Ă��鍀�ڂ��`�J�`�J������p�̏�����
	g_bPause = false;		//�|�[�Y����
	g_bDiePlayer = false;	//�v���C���[�̎��S����
	g_nCntSet = 0;			//�u���b�N�̔z�u�^�C�~���O���炵�p�ϐ��̏�����

	for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
	{
		g_fColPauseA[nCnt] = 0.0f;	//�|�[�Y�A���t�@�l������
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause000.png",
		&g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause002.png",
		&g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause001.png",
		&g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause003.png",
		&g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/PauseUI000.png",
		&g_pTexturePause[4]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//��ʑS�̂̃o�b�t�@
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[0],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//�|�[�YUI�̃o�b�t�@
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[1],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_SELECT_MAX,		//�|�[�Y�I�����ڂ̃o�b�t�@
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[2],
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause[0]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_PausePos[1].x - (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y - (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PausePos[1].x + (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y - (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PausePos[1].x - (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y + (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PausePos[1].x + (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y + (PAUSE_UI_SIZE_Y / 2), 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause[1]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) - (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y - (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) + (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y - (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) - (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y + (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) + (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y + (PAUSE_SELECT_SIZE_Y / 2), 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause[2]->Unlock();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//======================================================
//		�Q�[���S�̂̏I������
//======================================================
void UninitGame(void)
{
	//�T�E���h��~
	StopSound();

	//�w�i�̏I������
	UninitBg();

	//�Q�[�����̃`���[�g���A���摜�̏I������
	UninitGameTutorial();

	//�X�C�b�`�̏I������
	UninitSwitch();

	//�v���C���[�̏I������
	UninitPlayer();

	//�u���b�N�̏I������
	UninitBlock();

	//���Ԓn�_�̏I������
	UninitGameFlag();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�X�R�A�̏I������
	UninitTime();

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX; nCnt++)
	{
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffPause[nCnt] != NULL)
		{
			g_pVtxBuffPause[nCnt]->Release();
			g_pVtxBuffPause[nCnt] = NULL;
		}
	}
}

//======================================================
//		�Q�[���S�̂̍X�V����
//======================================================
void UpdateGame(void)
{
	Player *pPlayer;	//�v���C���[�̃|�C���^

	//�v���C���[�����擾
	pPlayer = GetPlayer();

	//���t�g�u���b�N�����炵�Ĕz�u
	if (g_nCntSet < 65)
	{
		g_nCntSet++;
		if (g_nCntSet == 5)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.2f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)�A��
		}
		if (g_nCntSet == 10)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.3f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)�A��
		}
		if (g_nCntSet == 15)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.4f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)�A��
		}
		if (g_nCntSet == 20)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.5f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)�A��
		}
		if (g_nCntSet == 25)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.6f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)�A��
		}
		if (g_nCntSet == 30)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.7f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//���t�g(�c)�A��
		}
	}

	//P�L�[�������ꂽ�ꍇ
	if (GetKeyboardTrigger(DIK_P) == true && g_bPause == false)
	{	
		//�Q�[�����~�߂�
		g_nCntCol = 0;
		g_bPause = true;
	}

	//�|�[�Y���̏ꍇ
	if (g_bPause == true)
	{
		if (g_nCntCol == 0)
		{//�o�b�N�̃Q�[����ʂ��Â�����
			g_fColPauseA[0] += 0.1f;
			if (g_fColPauseA[0] > 0.5f)
			{
				g_fColPauseA[0] = 0.5f;
				g_nCntCol = 1;
			}
		}
		else if (g_nCntCol == 1)
		{//�|�[�Y��ʂ��o��
			for (int nCnt = 1; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
			{
				g_fColPauseA[nCnt] += 0.1f;
				if (g_fColPauseA[nCnt] > 1.0f)
				{
					g_fColPauseA[nCnt] = 1.0f;
					g_nCntCol = 2;
				}
			}
		}
		else if (g_nCntCol == 2)
		{//�|�[�Y�I�����
			if (GetKeyboardTrigger(DIK_A)== true)
			{
				g_nPauseSelecting--;
				//�I�����Ă��Ȃ����ڂ̃��Z�b�g
				g_fColPauseA[2] = 1.0f;
				g_fColPauseA[3] = 1.0f;
				g_fColPauseA[4] = 1.0f;
				g_fColPauseA[5] = 1.0f;
				if (g_nPauseSelecting < 2)
				{
					g_nPauseSelecting = 5;
				}
			}
			else if (GetKeyboardTrigger(DIK_D) == true)
			{
				g_nPauseSelecting++;
				//�I�����Ă��Ȃ����ڂ̃��Z�b�g
				g_fColPauseA[2] = 1.0f;
				g_fColPauseA[3] = 1.0f;
				g_fColPauseA[4] = 1.0f;
				g_fColPauseA[5] = 1.0f;
				if (g_nPauseSelecting > 5)
				{
					g_nPauseSelecting = 2;
				}
			}
			else if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) == true)
			{
				if (g_nPauseSelecting == 2)
				{//�Q�[���𑱂���
					g_bPause = false;
				}
				else if (g_nPauseSelecting == 3)
				{//���g���C
					//DiedPlayer();
					SetMode(MODE_GAME);
				}
				else if (g_nPauseSelecting == 4)
				{//���^�C�A
					g_bPause = false;
				}
				else if (g_nPauseSelecting == 5)
				{//�^�C�g���ɖ߂�
					//DiedPlayer();
					SetFade(MODE_TITLE);
				}
			}

			if (g_nColPoseCnt == 0)
			{//�I�𒆂̍��ڂ��`�J�`�J������
				g_fColPauseA[g_nPauseSelecting] -= 0.05f;
				if (g_fColPauseA[g_nPauseSelecting] < 0.0f)
				{
					g_fColPauseA[g_nPauseSelecting] = 0.0f;
					g_nColPoseCnt = 1;
				}
			}
			else if (g_nColPoseCnt == 1)
			{
				g_fColPauseA[g_nPauseSelecting] += 0.05f;
				if (g_fColPauseA[g_nPauseSelecting] > 1.0f)
				{
					g_fColPauseA[g_nPauseSelecting] = 1.0f;
					g_nColPoseCnt = 0;
				}
			}
		}
	}
	else
	{//�|�[�Y��ʂ��o���Ă��Ȃ��Ƃ�
		for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
		{
			g_fColPauseA[nCnt] -= 0.1f;
			if (g_fColPauseA[nCnt] < 0.0f)
			{
				g_fColPauseA[nCnt] = 0.0f;
			}
		}
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause[0]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause[1]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause[2]->Unlock();

	//�v���C���[�����S�����ꍇ
	g_bDiePlayer = DiePlayer();

	if (g_bDiePlayer == false)
	{
		if (g_nFade > 100)
		{
			SetMode(MODE_GAME);
		}
		else
		{
			g_nFade++;
		}
	}

	if (g_bPause == false)
	{//�|�[�Y���łȂ����

		//�w�i�̍X�V����
		UpdateBg();

		//�Q�[�����̃`���[�g���A���摜�̍X�V����
		UpdateGameTutorial();

		//�X�C�b�`�̍X�V����
		UpdateSwitch();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�u���b�N�̍X�V����
		UpdateBlock();

		//���Ԓn�_�̍X�V����
		UpdateGameFlag();

		//�p�[�e�B�N���̍X�V����
		UpdateParticle();

		//�X�R�A�̍X�V����
		UpdateTime();
	}
}

//======================================================
//		�Q�[���S�̂̕`�揈��
//======================================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBg();

	//�Q�[�����̃`���[�g���A���摜�̕`�揈��
	DrawGameTutorial();

	//�X�C�b�`�̕`�揈��
	DrawSwitch();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�u���b�N�̕`�揈��
	DrawBlock();

	//���Ԓn�_�̕`�揈��
	DrawGameFlag();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//�X�R�A�̕`�揈��
	//DrawTime();

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause[0], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);		//�v���~�e�B�u�i�|���S���j��


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause[1], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePause[4]);
	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);		//�v���~�e�B�u�i�|���S���j��


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause[2], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);
		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			4 * nCnt,	//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);			//�v���~�e�B�u�i�|���S���j��
	}
}