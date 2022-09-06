//===================================================================
//
//		�ǂ̏���[wall.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "wall.h"
#include "meshfield.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_WALL			(16)		//�e�̍ő吔

//======================================================
//	�O���[�o���錾
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//�e�N�X�`���ւ̃|�C���^
Wall g_aWall[MAX_WALL];		//�ǂ̏��

//======================================================
//	�ǂ̏���������
//======================================================
void InitWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wall001.jpg", &g_pTextureWall);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//�e���̏�����
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
		g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCnt].bUse = false;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(WALL_WIDTH / 2), 0.0f,(WALL_HEIGHT / 2));
		pVtx[1].pos = D3DXVECTOR3((WALL_WIDTH / 2), 0.0f,(WALL_HEIGHT / 2));
		pVtx[2].pos = D3DXVECTOR3(-(WALL_WIDTH / 2), 0.0f,-(WALL_HEIGHT / 2));
		pVtx[3].pos = D3DXVECTOR3(+(WALL_WIDTH / 2), 0.0f,-(WALL_HEIGHT / 2));

		//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffWall->Unlock();

	//�ǔz�u
	SetWall(D3DXVECTOR3(-(MESHFIELD_SIZE / 2), WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, -D3DX_PI / 2));
	SetWall(D3DXVECTOR3((MESHFIELD_SIZE / 2), WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, D3DX_PI / 2));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, (MESHFIELD_SIZE / 2)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, -D3DX_PI));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, -(MESHFIELD_SIZE / 2)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f));
}

//======================================================
//	�ǂ̏I������
//======================================================
void UninitWall(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//======================================================
//	�ǂ̍X�V����
//======================================================
void UpdateWall(void)
{
}

//======================================================
//	�ǂ̕`�揈��
//======================================================
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{//�e���g�p����Ă���ꍇ
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCnt].rot.y, g_aWall[nCnt].rot.x, g_aWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCnt].pos.x, g_aWall[nCnt].pos.y, g_aWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCnt * 4,	//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);			//�v���~�e�B�u�i�|���S���j��

		}
	}
	//�e�N�X�`�������ɖ߂�
	pDevice->SetTexture(0, NULL);
}

//======================================================
//	�ǂ̐ݒ菈��
//======================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aWall[nCnt].pos = pos;
			g_aWall[nCnt].rot = rot;
			g_aWall[nCnt].bUse = true;

			break;
		}
	}
}