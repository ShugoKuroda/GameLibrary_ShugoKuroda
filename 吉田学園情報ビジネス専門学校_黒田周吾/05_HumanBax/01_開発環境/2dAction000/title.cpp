#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[MAX_TITLE] = { NULL };		//���_�o�b�t�@�ւ̃|�C���^
bool g_GameStartPush;			//�X�^�[�g���o�̌J��Ԃ��h�~

//�^�C�g�����o�p�v���C���[���
D3DXVECTOR3 g_PlayerPos;		//�ʒu������������
D3DXVECTOR3 g_PlayerMove;		//�ړ��ʂ�����������
int g_PlayerDirectionMove;		//�v���C���[�̌�������������
int g_PlayerCounterAnim;		//�J�E���^�[������������
int g_PlayerPatternAnim;		//�p�^�[��No.������������
int g_nCntSetPlayer;			//�v���C���[�̃��[�v�p�ϐ�

//======================================================
//		�^�C�g����ʂ̏���������
//======================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title000.png",
		&g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title001.png",
		&g_pTextureTitle[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title002.png",
		&g_pTextureTitle[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_pTextureTitle[4]);

	//���_�o�b�t�@�̐���
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTitle[nCnt],
			NULL);
	}

	g_PlayerPos = D3DXVECTOR3(SCREEN_WIDTH + 200.0f , 470.0f, 0.0f);	//�ʒu������������
	g_PlayerMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ړ��ʂ�����������
	g_nCntSetPlayer = 0;			//�v���C���[�̃��[�v�p�ϐ�
	g_PlayerDirectionMove = 1;		//�v���C���[�̌�������������
	g_PlayerCounterAnim = 0;		//�J�E���^�[������������
	g_PlayerPatternAnim = 0;		//�p�^�[��No.������������

	g_GameStartPush = false;		//�X�^�[�g���o�J��Ԃ��h�~�̏�����

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[0]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[1]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[2]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[2]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[3]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[3]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[4]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�(���S���W�͉��ӂ̒��S)
	pVtx[0].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos.y;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�(1.0f�ŌŒ�)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[4]->Unlock();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//======================================================
//		�^�C�g����ʂ̏I������
//======================================================
void UninitTitle(void)
{
	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffTitle[nCnt] != NULL)
		{
			g_pVtxBuffTitle[nCnt]->Release();
			g_pVtxBuffTitle[nCnt] = NULL;
		}
	}
}

//======================================================
//		�^�C�g����ʂ̍X�V����
//======================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[4]->Lock(0, 0, (void**)&pVtx, 0);

	if (g_GameStartPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//�G���^�[�L�[�������ꂽ��
			//���[�h�ڍs
			SetFade(MODE_TUTORIAL);

			//�X�^�[�g���o�̌J��Ԃ��h�~
			g_GameStartPush = true;
		}
	}

	g_nCntSetPlayer++;
	if (g_nCntSetPlayer == 900)
	{
		g_PlayerPos = D3DXVECTOR3(SCREEN_WIDTH + 200.0f, 470.0f, 0.0f);
		g_nCntSetPlayer = 0;
	}

	g_PlayerMove.x += sinf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);
	g_PlayerMove.y += cosf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);

	g_PlayerCounterAnim++;		//�J�E���^�[�����Z
	if (g_PlayerCounterAnim > 6)
	{
		g_PlayerCounterAnim = 0;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim, 0.5f * g_PlayerDirectionMove);
		pVtx[1].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim + 1), 0.5f * g_PlayerDirectionMove);
		pVtx[2].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim, 0.5f * (g_PlayerDirectionMove + 1));
		pVtx[3].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim + 1), 0.5f * (g_PlayerDirectionMove + 1));

		g_PlayerPatternAnim++;

		if (g_PlayerPatternAnim >= 4)
		{//�p�^�[��No.�������l�ɖ߂�
			g_PlayerPatternAnim = 0;
		}
	}

	//�ʒu�X�V
	g_PlayerPos += g_PlayerMove;

	//�ړ��ʍX�V(����)
	g_PlayerMove.x += -g_PlayerMove.x * 0.1f;			//(�ړI�̒l - ���݂̒l) * �����W��

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[4]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�(���S���W�͉��ӂ̒��S)
	pVtx[0].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos.y;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle[4]->Unlock();
}

//======================================================
//		�^�C�g����ʂ̕`�揈��
//======================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle[nCnt], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);		//�v���~�e�B�u�i�|���S���j��
	}
}