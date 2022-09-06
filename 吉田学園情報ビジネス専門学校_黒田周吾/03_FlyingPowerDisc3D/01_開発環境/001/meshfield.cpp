//===================================================================
//
//		���b�V���t�B�[���h����[meshfield.cpp]
//		Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"

//======================================================
//	�}�N����`
//======================================================
#define MESHFIELD_X_BLOCK			(7)			//X�����̃u���b�N��
#define MESHFIELD_Z_BLOCK			(7)			//Z�����̃u���b�N��
#define MESHFIELD_VERTEX_NUM		((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1))			//���_��
#define MESHFIELD_INDEX_NUM			((MESHFIELD_X_BLOCK + 1) * 2 * MESHFIELD_Z_BLOCK + (MESHFIELD_Z_BLOCK - 1) * 2)		//�C���f�b�N�X��
#define MESHFIELD_PRIMITIVE_NUM		(MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 4)			//�|���S����

//======================================================
//	�O���[�o���錾
//======================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshField;		//�ʒu
D3DXVECTOR3 g_rotMeshField;		//����
D3DXMATRIX	g_mtxWorldMeshField;	//���[���h�}�g���b�N�X

//======================================================
//	���b�V���t�B�[���h�̏���������
//======================================================
void InitMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/glass000.jpg", &g_pTextureMeshField);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^
	WORD *pIdx;			//�C���f�b�N�X���ւ̃|�C���^

	g_posMeshField = D3DXVECTOR3(-MESHFIELD_SIZE / 4, 0.0f, MESHFIELD_SIZE / 4);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//�e���_���W�̐ݒ�
	for (int nCntBlockZ = 0; nCntBlockZ < MESHFIELD_Z_BLOCK + 1; nCntBlockZ++)
	{//Z�����̍��W�ݒ�
		for (int nCntBlockX = 0; nCntBlockX < MESHFIELD_X_BLOCK + 1; pVtx++, nCntBlockX++)
		{//X�����̍��W�ݒ�
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posMeshField.x + (MESHFIELD_SIZE / MESHFIELD_X_BLOCK) * nCntBlockX, g_posMeshField.y, g_posMeshField.z - (MESHFIELD_SIZE / MESHFIELD_Z_BLOCK) * nCntBlockZ);
			//�e���_�̖@���̐ݒ�(�x�N�g���̑傫���͂P�ɂ���K�v������)
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((1.0f / MESHFIELD_X_BLOCK) * nCntBlockX, (1.0f / MESHFIELD_Z_BLOCK) * nCntBlockZ);
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();

	int nCntIdxX;
	int nCntIdxZ;

	//�C���f�b�N�X�o�b�t�@�����b�N
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (nCntIdxZ = 0; nCntIdxZ < MESHFIELD_Z_BLOCK; pIdx += 2, nCntIdxZ++)
	{//Z�����̃C���f�b�N�X�ݒ�
		for (nCntIdxX = 0; nCntIdxX < MESHFIELD_X_BLOCK + 1; pIdx += 2, nCntIdxX++)
		{//X�����̃C���f�b�N�X�ݒ�
			pIdx[0] = nCntIdxX + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ) + MESHFIELD_X_BLOCK + 1;
			pIdx[1] = nCntIdxX + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ);
		}
		//�k�ރ|���S���p�̃C���f�b�N�X�ݒ�
		if (nCntIdxZ <= MESHFIELD_Z_BLOCK - 2)
		{//�Ō�̃C���f�b�N�X�ԍ��ł͂Ȃ���
			pIdx[0] = (nCntIdxX - 1) + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ);
			pIdx[1] = (nCntIdxX + (MESHFIELD_X_BLOCK + 1)) + ((MESHFIELD_X_BLOCK + 1) * nCntIdxZ);
		}
	}
	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshField->Unlock();
}

//======================================================
//	���b�V���t�B�[���h�̏I������
//======================================================
void UninitMeshField(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//======================================================
//	���b�V���t�B�[���h�̍X�V����
//======================================================
void UpdateMeshField(void)
{
	//�f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();

//	int nCntMesh;
	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();
}

//======================================================
//	���b�V���t�B�[���h�̕`�揈��
//======================================================
void DrawMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,			//�`�悷��ŏ��̃C���f�b�N�X
		0,			//�o�b�t�@�̍ŏ��l
		MESHFIELD_VERTEX_NUM,		//�`�悷��ŏ��̒��_�C���f�b�N�X
		0,			//�C���f�b�N�X�̍ŏ��̐�
		MESHFIELD_PRIMITIVE_NUM);	//�v���~�e�B�u�i�|���S���j��
}