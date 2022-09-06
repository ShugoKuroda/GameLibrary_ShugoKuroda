//=============================================================================
//
// ���b�V��(��)���� [meshcylinder.cpp]
// Author : SHUGO KURODA
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "meshcylinder.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define CYLINDER_MAX				(256)		//���̂̍ő吔

//-----------------------------------------------------------------------------
//�\���̂̒�`
//-----------------------------------------------------------------------------
//���b�V���\����
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXCOLOR col;						//�J���[
	float fRadiusX; float fRadiusZ;		//�e���c��
	int nMeshX; int nMeshZ;				//�c���̃��b�V�����̐�
	int nPolygon;						//�|���S����
	int nTop;							//���_
	int nIdx;							//�C���f�b�N�X��
	D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
	TexturetCylinder Texturetstate;	//�e�N�X�`���̏��
	bool bUse;							//�g�p���Ă��邩�ǂ���
}CylinderStructure;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ��錾
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCylinder = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCylinder = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCylinder[CYLINDER_TEXTURE_MAX] = { NULL };//�e�N�X�`���ւ̃|�C���^
CylinderStructure g_aCylinder[CYLINDER_MAX];//���b�V���\����

//*****************************************************************************
//	���b�V��������
//*****************************************************************************
void InitMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���b�V���̃e�N�X�`���̏�����
	ZeroMemory(&g_pTextureCylinder[0], sizeof(g_pTextureCylinder));

	//�e�N�X�`���̓ǂݍ���(0�Ԗڂ�NULL)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky000.jpg",
		&g_pTextureCylinder[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * CYLINDER_MAX,//�m�ۂ��钸�_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCylinder,
		NULL);

	//���_�̃C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4 * CYLINDER_MAX,//�m�ۂ��钸�_�̐�
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pIdxBuffCylinder,
		NULL);

	//���b�V���\���̂̏�����
	ZeroMemory(&g_aCylinder[0], sizeof(g_aCylinder));

	//�������b�V���̔z�u
	SetCylinder(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		3000.0f, 3000.0f, 10, 10, CYLINDER_TEXTURE_1);
}

//*****************************************************************************
//	���b�V���̏I������
//*****************************************************************************
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < CYLINDER_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureCylinder[nCnt] != NULL)
		{
			g_pTextureCylinder[nCnt]->Release();
			g_pTextureCylinder[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCylinder != NULL)
	{
		g_pVtxBuffCylinder->Release();
		g_pVtxBuffCylinder = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffCylinder != NULL)
	{
		g_pIdxBuffCylinder->Release();
		g_pIdxBuffCylinder = NULL;
	}
}

//*****************************************************************************
//	���b�V���̍X�V����
//*****************************************************************************
void UpdateMeshCylinder(void)
{
	for (int nCnt = 0; nCnt < CYLINDER_MAX; nCnt++)
	{
		if (g_aCylinder[nCnt].bUse)
		{//�g�p���Ă�����
			g_aCylinder[nCnt].rot.x += 0.001f;
		}
	}
}

//*****************************************************************************
//	���b�V���̕`�揈��
//*****************************************************************************
void DrawMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�J�����OOFF
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCnt = 0; nCnt < CYLINDER_MAX; nCnt++)
	{
		if (g_aCylinder[nCnt].bUse)
		{
			D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCylinder[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aCylinder[nCnt].rot.y, g_aCylinder[nCnt].rot.x, g_aCylinder[nCnt].rot.z);

			D3DXMatrixMultiply(&g_aCylinder[nCnt].mtxWorld, &g_aCylinder[nCnt].mtxWorld, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aCylinder[nCnt].pos.x, g_aCylinder[nCnt].pos.y, g_aCylinder[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aCylinder[nCnt].mtxWorld, &g_aCylinder[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCylinder[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffCylinder, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffCylinder);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCylinder[g_aCylinder[nCnt].Texturetstate]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
				0,							//�C���f�b�N�X�̍ŏ��l
				g_aCylinder[nCnt].nTop,		//���_�̐�
				0,							//�C���f�b�N�X�̍ŏ��̐�
				g_aCylinder[nCnt].nPolygon);		//�v���~�e�B�u�i�|���S���j��
		}
	}
	//�e�N�X�`�������ɖ߂�
	pDevice->SetTexture(0, NULL);

	//�J�����O�����ɖ߂�(ON)
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------------------
//	�Z�b�g���b�V������
//-----------------------------------------------------------------------------
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, int nMeshX, int nMeshZ, TexturetCylinder Texturetstate)
{
	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CYLINDER_MAX; nCnt++)
	{
		if (!g_aCylinder[nCnt].bUse)
		{//�������U��������

			//�\���̂��g�p��Ԃɂ���
			g_aCylinder[nCnt].bUse = true;
			g_aCylinder[nCnt].pos = pos;
			g_aCylinder[nCnt].rot = rot;
			g_aCylinder[nCnt].col = col;
			g_aCylinder[nCnt].fRadiusX = fRadiusX;
			g_aCylinder[nCnt].fRadiusZ = fRadiusZ;
			g_aCylinder[nCnt].nMeshX = nMeshX;
			g_aCylinder[nCnt].nMeshZ = nMeshZ;
			g_aCylinder[nCnt].Texturetstate = Texturetstate;
			g_aCylinder[nCnt].nTop = (nMeshX + 1) * (nMeshZ + 1);
			g_aCylinder[nCnt].nIdx = (nMeshX + 1) * 2 * nMeshZ + (nMeshZ - 1) * 2;
			g_aCylinder[nCnt].nPolygon = nMeshX * nMeshZ * 2 + (nMeshZ - 1) * 4;

			for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
			{
				float fData = fRadiusZ * cosf((D3DX_PI/ nMeshZ * nCntZ) - D3DX_PI / 2);
				for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
				{
					pVtx[0].pos.x = fData * sinf(D3DX_PI * 2 / nMeshX * nCntX);
					pVtx[0].pos.y = fRadiusZ * sinf((D3DX_PI / nMeshZ  * nCntZ) - D3DX_PI / 2);
					pVtx[0].pos.z = fData * cosf(D3DX_PI * 2 / nMeshX * nCntX);

					//�e���_�̖@���̐ݒ�i�x�N�g���̑傫���͂P�ɂ���K�v������j
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//���_�J���[�̐ݒ�
					pVtx[0].col = g_aCylinder[nCnt].col;

					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((1.0f / nMeshX) * nCntX, (1.0f / nMeshZ) * nCntZ);

					pVtx++; //�f�[�^��1���i�߂�
				}
			}

			//�C���f�b�N�X�o�b�t�@�����b�N
			WORD* pIdx;
			g_pIdxBuffCylinder->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntZ = 0; nCntZ <= nMeshZ; nCntZ++)
			{
				for (int nCntX = 0; nCntX <= nMeshX; nCntX++)
				{
					//�C���f�b�N�X�̐ݒ�
					pIdx[0] = (WORD)(nCntX + (nMeshX + 1) * (nCntZ + 1));
					pIdx[1] = (WORD)(nCntX + (nMeshX + 1) * nCntZ);
					pIdx += 2;//�C���f�b�N�X�̃f�[�^���Q�����߂�
				}

				if (nCntZ < nMeshZ - 1)
				{
					//�C���f�b�N�X�̐ݒ�
					pIdx[0] = pIdx[-1];
					pIdx[1] = (WORD)((nMeshX + 1) * (nCntZ + 2));
					pIdx += 2;//�C���f�b�N�X�̃f�[�^���Q�����߂�
				}
			}
			g_pIdxBuffCylinder->Unlock();
			break;
		}

		pVtx += 4; //�f�[�^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffCylinder->Unlock();
}