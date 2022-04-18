//========================================================
//
//						�r���{�[�h
//					AUTHOR:�z�{���V
//
//========================================================
#include "main.h"
#include "billboard.h"
#include "player.h"

//�}�N����`
#define BILLBOARD_SIZE (10)												//�r���{�[�h�̃T�C�Y
#define MAX_BILLBOARD (256)												//�r���{�[�h�̍ő吔
#define MAX_TEXTURE (16)												//�e�N�X�`���̍ő吔

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;													//�ʒu
	D3DXVECTOR3 rot;													//����
	D3DXMATRIX mtxWorld;												//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 pTexture;										//�r���{�[�h�̃e�N�X�`���̃|�C���^
	float nSize;														//�T�C�Y
	int nTextureType;													//�e�N�X�`���̎��
	int nCntTime;														//�\�����鎞��
	bool bUse;															//�g�p����Ă��邩�ǂ���
} Billboard;

//�O���[�o���ϐ�	
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;						//�r���{�[�h�̒��_�o�b�t�@�ւ̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];									//�r���{�[�h�̏����i�[
int g_nCntBillTime;														//�r���{�[�h���\�������܂ł̃J�E���^�[

//========================================================
//					�r���{�[�h����������
//========================================================
void InitBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���{�[�h�̏��̏�����
	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		g_aBillboard[nCntBill].pTexture = NULL;
		g_aBillboard[nCntBill].nTextureType = -1;
		g_aBillboard[nCntBill].nSize = 0;
		g_aBillboard[nCntBill].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aBillboard[nCntBill].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏�����
	}
	g_nCntBillTime = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL);

	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		//�e���_�ʒu�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, BILLBOARD_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f);

		//�e���_�̖@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//�e���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBillboard->Unlock();
}

//========================================================
//					�r���{�[�h�I������
//========================================================
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (g_aBillboard[nCntTexture].pTexture != NULL)
		{
			g_aBillboard[nCntTexture].pTexture->Release();
			g_aBillboard[nCntTexture].pTexture = NULL;
		}
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//========================================================
//					�r���{�[�h�X�V����
//========================================================
void UpdateBillboard(void)
{
	if (g_nCntBillTime < 20)
	{
		g_nCntBillTime++;
		if (g_nCntBillTime >= 20)
		{
			SetBillboard(D3DXVECTOR3(0.0f, 100.0f, FIELD_SIZE_Z-200.0f), 20.0f, 180, "data/TEXTURE/5pts000.png");
			SetBillboard(D3DXVECTOR3(70.0f, 100.0f, FIELD_SIZE_Z - 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
			SetBillboard(D3DXVECTOR3(-70.0f, 100.0f, FIELD_SIZE_Z - 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
			SetBillboard(D3DXVECTOR3(0.0f, 100.0f, -FIELD_SIZE_Z + 200.0f), 20.0f, 180, "data/TEXTURE/5pts000.png");
			SetBillboard(D3DXVECTOR3(70.0f, 100.0f, -FIELD_SIZE_Z + 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
			SetBillboard(D3DXVECTOR3(-70.0f, 100.0f, -FIELD_SIZE_Z + 200.0f), 20.0f, 180, "data/TEXTURE/3pts000.png");
		}
	}

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].nCntTime != -1)
		{//�\���J�E���^�[��-1�̏ꍇ�A��ɕ\������

			if (g_aBillboard[nCntBill].nCntTime <= 0)
			{//�J�E���^�[0�Ŕ�\��	
				g_aBillboard[nCntBill].bUse = false;
			}
			else
			{
				g_aBillboard[nCntBill].nCntTime--;		//�\���J�E���^�[���Z
			}
		}
	}
}

//========================================================
//					�r���{�[�h�`�揈��
//========================================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�o�b�t�@�@�̗L����
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntBill = 0; nCntBill < MAX_BILLBOARD; nCntBill++)
	{
		if (g_aBillboard[nCntBill].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCntBill].mtxWorld);

			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aBillboard[nCntBill].mtxWorld._11 = mtxView._11;
			g_aBillboard[nCntBill].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBill].mtxWorld._13 = mtxView._31;
			g_aBillboard[nCntBill].mtxWorld._21 = mtxView._12;
			g_aBillboard[nCntBill].mtxWorld._22 = mtxView._22;
			g_aBillboard[nCntBill].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBill].mtxWorld._31 = mtxView._13;
			g_aBillboard[nCntBill].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBill].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBill].pos.x, g_aBillboard[nCntBill].pos.y, g_aBillboard[nCntBill].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBill].mtxWorld, &g_aBillboard[nCntBill].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBill].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aBillboard[nCntBill].pTexture);

			//���e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			//���e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}

	//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Z�o�b�t�@�@��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//�J������L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetTexture(0, NULL);
}

//========================================================
//					�r���{�[�h�̐ݒ菈��
//========================================================
void SetBillboard(D3DXVECTOR3 pos, float fSize, int nCntTime, char *pFilename)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pFilename, &g_aBillboard[nCntBillboard].pTexture);

			g_aBillboard[nCntBillboard].nSize = fSize;
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].nCntTime = nCntTime;

			//�e���_�ʒu�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].nSize, g_aBillboard[nCntBillboard].nSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].nSize, g_aBillboard[nCntBillboard].nSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBillboard[nCntBillboard].nSize, -g_aBillboard[nCntBillboard].nSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].nSize, -g_aBillboard[nCntBillboard].nSize, 0.0f);

			g_aBillboard[nCntBillboard].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBillboard->Unlock();

}
