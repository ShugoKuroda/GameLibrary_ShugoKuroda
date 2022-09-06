//===================================================================
//
//		�G�t�F�N�g����[effect.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "effect.h"
#include "input.h"

//======================================================
//	�}�N����`
//======================================================	
#define MAX_EFFECT		(1240)		//�G�t�F�N�g�̍ő吔
#define MAX_EFFECT_TEX  (3)			//�G�t�F�N�g�̃e�N�X�`����
#define EFFECT_SIZE		(8.0f)		//�G�t�F�N�g�̃T�C�Y
#define EFFECT_LIFE		(10)		//�G�t�F�N�g�̎���

//======================================================
//	�G�t�F�N�g�\���̂̒�`
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 scale;		//�T�C�Y
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nLife;				//����
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_EFFECT_TEX] = { NULL };		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;					//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];										//�G�t�F�N�g�̏��

//======================================================
//	�G�t�F�N�g�̏���������
//======================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow001.png", &g_pTextureEffect[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hit.png", &g_pTextureEffect[2]);

	//�G�t�F�N�g�\���̂̏�����
	ZeroMemory(&g_aEffect, sizeof(g_aEffect));

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);

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

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_���W���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=======================================================
//	�G�t�F�N�g�̏I������
//=======================================================
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_EFFECT_TEX; nCnt++)
	{
		if (g_pTextureEffect[nCnt] != NULL)
		{
			g_pTextureEffect[nCnt]->Release();
			g_pTextureEffect[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=========================================================
//	�G�t�F�N�g�̍X�V����
//=========================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�G�t�F�N�g�̃��C�t����
			g_aEffect[nCntEffect].scale.x -= 0.05f;
			g_aEffect[nCntEffect].scale.y -= 0.05f;
			g_aEffect[nCntEffect].nLife--;

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;
	}
	//���_���W���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//===============================================================
//	�G�t�F�N�g�̕`�揈��
//===============================================================
void DrawEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X

	//Z�o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{//�e���g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X�̐ݒ�
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aEffect[nCnt].mtxWorld._11 = mtxView._11;
			g_aEffect[nCnt].mtxWorld._12 = mtxView._21;
			g_aEffect[nCnt].mtxWorld._13 = mtxView._31;
			g_aEffect[nCnt].mtxWorld._21 = mtxView._12;
			g_aEffect[nCnt].mtxWorld._22 = mtxView._22;
			g_aEffect[nCnt].mtxWorld._23 = mtxView._32;
			g_aEffect[nCnt].mtxWorld._31 = mtxView._13;
			g_aEffect[nCnt].mtxWorld._32 = mtxView._23;
			g_aEffect[nCnt].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCnt].pos.x, g_aEffect[nCnt].pos.y, g_aEffect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCnt].mtxWorld, &g_aEffect[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCnt].nType]);

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

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//===========================================================
//	�G�t�F�N�g�̐ݒ菈��
//===========================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 scale, int nType)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].scale = scale;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((+EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.x, (-EFFECT_SIZE / 2) * g_aEffect[nCntEffect].scale.y, 0.0f);

			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//���_���W���A�����b�N
	g_pVtxBuffEffect->Unlock();
}