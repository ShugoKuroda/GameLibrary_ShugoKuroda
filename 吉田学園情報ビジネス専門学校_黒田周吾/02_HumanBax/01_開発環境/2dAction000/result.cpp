#include "main.h"
#include "result.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult[MAX_RESULT] = { NULL };	//���_�o�b�t�@�ւ̃|�C���^
bool g_GameResultPush;		//���U���g���o�̌J��Ԃ��h�~

//���U���g���o�p�I�u�W�F�N�g���
D3DXVECTOR3 g_PlayerPos2;		//�ʒu������������
D3DXVECTOR3 g_BlockPos2;			//�ʒu������������
D3DXVECTOR3 g_PlayerMove2;		//�ړ��ʂ�����������
int g_PlayerDirectionMove2;		//�v���C���[�̌�������������
int g_PlayerCounterAnim2;		//�J�E���^�[������������
int g_PlayerPatternAnim2;		//�p�^�[��No.������������

//======================================================
//		���U���g��ʂ̏���������
//======================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result000.png",
		&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block002.png",
		&g_pTextureResult[2]);

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResult[nCnt],
			NULL);
	}

	g_PlayerPos2 = D3DXVECTOR3(-200.0f, 470.0f, 0.0f);	//�ʒu������������
	g_BlockPos2 = D3DXVECTOR3(-170.0f, 435.0f, 0.0f);	//�ʒu������������
	g_PlayerMove2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ�����������
	g_PlayerDirectionMove2 = 0;		//�v���C���[�̌�������������
	g_PlayerCounterAnim2 = 0;		//�J�E���^�[������������
	g_PlayerPatternAnim2 = 0;		//�p�^�[��No.������������

	g_GameResultPush = false;			//�I�����o�J��Ԃ��h�~�̏�����

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult[0]->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�(���S���W�͉��ӂ̒��S)
	pVtx[0].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos2.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos2.y;
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
	g_pVtxBuffResult[1]->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[2]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);

	//rhw�̐ݒ�(1.0f�ŌŒ�)
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
	g_pVtxBuffResult[2]->Unlock();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//======================================================
//		���U���g��ʂ̏I������
//======================================================
void UninitResult(void)
{
	//�T�E���h��~
	StopSound();

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffResult != NULL)
		{
			g_pVtxBuffResult[nCnt]->Release();
			g_pVtxBuffResult[nCnt] = NULL;
		}
	}
}

//======================================================
//		���U���g��ʂ̍X�V����
//======================================================
void UpdateResult(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	if (g_GameResultPush == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//�G���^�[�L�[�������ꂽ��
			//���[�h�ڍs
			SetFade(MODE_TITLE);

			//���U���g���o�̌J��Ԃ��h�~
			g_GameResultPush = true;
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[1]->Lock(0, 0, (void**)&pVtx, 0);

	g_PlayerMove2.x -= sinf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);
	g_PlayerMove2.y -= cosf(-D3DX_PI * 0.5f)*(PLAYER_MOVE - 0.3f);

	g_PlayerCounterAnim2++;		//�J�E���^�[�����Z
	if (g_PlayerCounterAnim2 > 6)
	{
		g_PlayerCounterAnim2 = 0;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim2, 0.5f * g_PlayerDirectionMove2);
		pVtx[1].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim2 + 1), 0.5f * g_PlayerDirectionMove2);
		pVtx[2].tex = D3DXVECTOR2(0.25f * g_PlayerPatternAnim2, 0.5f * (g_PlayerDirectionMove2 + 1));
		pVtx[3].tex = D3DXVECTOR2(0.25f * (g_PlayerPatternAnim2 + 1), 0.5f * (g_PlayerDirectionMove2 + 1));

		g_PlayerPatternAnim2++;

		if (g_PlayerPatternAnim2 >= 4)
		{//�p�^�[��No.�������l�ɖ߂�
			g_PlayerPatternAnim2 = 0;
		}
	}

	//�ʒu�X�V
	g_PlayerPos2 += g_PlayerMove2;
	g_BlockPos2.x += 4.0f;

	//�ړ��ʍX�V(����)
	g_PlayerMove2.x += -g_PlayerMove2.x * 0.1f;			//(�ړI�̒l - ���݂̒l) * �����W��

	//���_���W�̐ݒ�(���S���W�͉��ӂ̒��S)
	pVtx[0].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_PlayerPos2.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_PlayerPos2.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_PlayerPos2.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_PlayerPos2.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_PlayerPos2.y;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult[1]->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[2]->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y - (70.0f / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_BlockPos2.x - (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_BlockPos2.x + (70.0f / 2), g_BlockPos2.y + (70.0f / 2), 0.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult[2]->Unlock();
}

//======================================================
//		���U���g��ʂ̕`�揈��
//======================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffResult[nCnt], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[nCnt]);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);		//�v���~�e�B�u�i�|���S���j��
	}
}