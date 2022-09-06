//===================================================================
//
//		�r���{�[�h����[billboard.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_BILLBOARD		(128)		//�r���{�[�h�̍ő吔
#define BILLBOARD_WIDTH		(30.0f)		//�e�̕�
#define BILLBOARD_HEIGHT	(50.0f)		//�e�̍���

//======================================================
//	�r���{�[�h�̍\����
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
	int nIdxShadow;		//�e�̔ԍ�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;		//�g�p���Ă��邩�ǂ���
}billboard;

//======================================================
//	�O���[�o���錾
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//�e�N�X�`���ւ̃|�C���^
billboard g_abillboard[MAX_BILLBOARD];		//�r���{�[�h�̏��
int g_nZBuffer;		//Z�o�b�t�@�̐؂�ւ��t���O

//======================================================
//	�r���{�[�h�̏���������
//======================================================
void InitBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/friend000.png", &g_pTextureBillboard);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//�e���̏�����
	g_nZBuffer = 0;

	//�e���ڂ̏�����
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		g_abillboard[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abillboard[nCnt].bUse = false;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x - (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y + (BILLBOARD_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x + (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y + (BILLBOARD_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x - (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y - (BILLBOARD_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_abillboard[nCnt].pos.x + (BILLBOARD_WIDTH / 2), g_abillboard[nCnt].pos.y - (BILLBOARD_HEIGHT / 2), 0.0f);

		//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;
	}

	//�r���{�[�h�z�u
	SetBillboard(D3DXVECTOR3(190.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(150.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(100.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(50.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(-50.0f, 30.0f, 0.0f));

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse == true)
		{//�r���{�[�h���g�p����Ă���ꍇ�A�e��ݒ肷��
			g_abillboard[nCnt].nIdxShadow = SetShadow(D3DXVECTOR3(g_abillboard[nCnt].pos.x, 1.0f, g_abillboard[nCnt].pos.z), g_abillboard[nCnt].rot, 20.0f, 20.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboard->Unlock();
}

//======================================================
//	�r���{�[�h�̏I������
//======================================================
void UninitBillboard(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//======================================================
//	�r���{�[�h�̍X�V����
//======================================================
void UpdateBillboard(void)
{
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse)
		{
			//�e�̈ʒu�X�V
			SetPositionShadow(g_abillboard[nCnt].nIdxShadow, D3DXVECTOR3(g_abillboard[nCnt].pos.x, 1.0f, g_abillboard[nCnt].pos.z), g_abillboard[nCnt].rot);
		}
	}
}

//======================================================
//	�r���{�[�h�̕`�揈��
//======================================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X

	if (g_nZBuffer == 1)
	{//Z�o�b�t�@��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	if (g_nZBuffer == 0)
	{//Z�o�b�t�@�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse == true)
		{//�r���{�[�h���g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_abillboard[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X�̐ݒ�
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_abillboard[nCnt].mtxWorld._11 = mtxView._11;
			//g_abillboard[nCnt].mtxWorld._12 = mtxView._21;
			g_abillboard[nCnt].mtxWorld._13 = mtxView._31;
			//g_abillboard[nCnt].mtxWorld._21 = mtxView._12;
			//g_abillboard[nCnt].mtxWorld._22 = mtxView._22;
			//g_abillboard[nCnt].mtxWorld._23 = mtxView._32;
			g_abillboard[nCnt].mtxWorld._31 = mtxView._13;
			//g_abillboard[nCnt].mtxWorld._32 = mtxView._23;
			g_abillboard[nCnt].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_abillboard[nCnt].pos.x, g_abillboard[nCnt].pos.y, g_abillboard[nCnt].pos.z);
			D3DXMatrixMultiply(&g_abillboard[nCnt].mtxWorld, &g_abillboard[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_abillboard[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCnt * 4,	//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);			//�v���~�e�B�u�i�|���S���j��
		}
	}
	//���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�e�N�X�`�������ɖ߂�
	pDevice->SetTexture(0, NULL);

	//Z�o�b�t�@�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//======================================================
//	�r���{�[�h�̐ݒ菈��
//======================================================
void SetBillboard(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (g_abillboard[nCnt].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_abillboard[nCnt].pos = pos;
			g_abillboard[nCnt].bUse = true;

			break;
		}
	}
}