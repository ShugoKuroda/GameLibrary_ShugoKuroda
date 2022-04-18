//===================================================================
//
//	�p�[�e�B�N������[particle.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "particle.h"

//======================================================
//	�}�N����`
//======================================================	
#define MAX_PARTICLE		(1240)		//�p�[�e�B�N���̍ő吔
#define MAX_PARTICLE_TEX	(2)			//�p�[�e�B�N���̃e�N�X�`����
#define PARTICLE_SIZE		(8.0f)		//�p�[�e�B�N���̃T�C�Y
#define PARTICLE_LIFE		(60)		//�p�[�e�B�N���̎���
#define PARTICLE_MOVEDOWN	(0.1f)		//�p�[�e�B�N���̏d��

//======================================================
//	�p�[�e�B�N���\���̂̒�`
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
}Particle;

//======================================================
//	�O���[�o���ϐ�
//======================================================
LPDIRECT3DTEXTURE9 g_pTextureParticle[MAX_PARTICLE_TEX] = { NULL };		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;						//���_�o�b�t�@�ւ̃|�C���^
Particle g_aParticle[MAX_PARTICLE];		//�p�[�e�B�N���̏��

//======================================================
//	�p�[�e�B�N���̏���������
//======================================================
void InitParticle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow001.png", &g_pTextureParticle[0]);		//���`
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureParticle[1]);		//�ی^

	//�p�[�e�B�N���\���̂̏�����
	ZeroMemory(&g_aParticle, sizeof(g_aParticle));

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_PARTICLE; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntEffect].scale.y, 0.0f);

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
	g_pVtxBuffParticle->Unlock();
}

//=======================================================
//	�p�[�e�B�N���̏I������
//=======================================================
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_PARTICLE_TEX; nCnt++)
	{
		if (g_pTextureParticle[nCnt] != NULL)
		{
			g_pTextureParticle[nCnt]->Release();
			g_pTextureParticle[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=========================================================
//	�p�[�e�B�N���̍X�V����
//=========================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{//�p�[�e�B�N�����g�p���̏ꍇ

			//�ړ��ʂ̉��Z
			g_aParticle[nCntParticle].move.y -= PARTICLE_MOVEDOWN;		//�d�͕��ׂ�������
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//�G�t�F�N�g�̃��C�t����
			g_aParticle[nCntParticle].scale.x -= 0.01f;
			g_aParticle[nCntParticle].scale.y -= 0.01f;
			g_aParticle[nCntParticle].nLife--;

			//����
			g_aParticle[nCntParticle].move.x -= g_aParticle[nCntParticle].move.x * 0.1f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
		pVtx += 4;
	}
	//���_���W���A�����b�N
	g_pVtxBuffParticle->Unlock();
}

//===============================================================
//	�p�[�e�B�N���̕`�揈��
//===============================================================
void DrawParticle(void)
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

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == true)
		{//�e���g�p����Ă���ꍇ
			 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aParticle[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X�̐ݒ�
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aParticle[nCnt].mtxWorld._11 = mtxView._11;
			g_aParticle[nCnt].mtxWorld._12 = mtxView._21;
			g_aParticle[nCnt].mtxWorld._13 = mtxView._31;
			g_aParticle[nCnt].mtxWorld._21 = mtxView._12;
			g_aParticle[nCnt].mtxWorld._22 = mtxView._22;
			g_aParticle[nCnt].mtxWorld._23 = mtxView._32;
			g_aParticle[nCnt].mtxWorld._31 = mtxView._13;
			g_aParticle[nCnt].mtxWorld._32 = mtxView._23;
			g_aParticle[nCnt].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCnt].pos.x, g_aParticle[nCnt].pos.y, g_aParticle[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCnt].mtxWorld, &g_aParticle[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureParticle[g_aParticle[nCnt].nType]);

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
//	�p�[�e�B�N���̐ݒ菈��
//===========================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scale, int nType)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;		//�ʒu
			g_aParticle[nCntParticle].move = move;		//�ړ���
			g_aParticle[nCntParticle].col = col;		//�F
			g_aParticle[nCntParticle].scale = scale;	//�傫��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((+PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.x, (-PARTICLE_SIZE / 2) * g_aParticle[nCntParticle].scale.y, 0.0f);

			g_aParticle[nCntParticle].nType = nType;			//���
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;	//����
			g_aParticle[nCntParticle].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//���_���W���A�����b�N
	g_pVtxBuffParticle->Unlock();
}