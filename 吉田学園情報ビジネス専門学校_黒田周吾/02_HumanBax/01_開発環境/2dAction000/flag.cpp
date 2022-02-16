#include "main.h"
#include "flag.h"
#include "particle.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureGameFlag[MAX_FLAG] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameFlag = NULL;			//���_�o�b�t�@�ւ̃|�C���^
FLAG g_aGameFlag[4];		//���Ԓn�_�̏��
int nPoint;

//======================================================
//		���Ԓn�_�̏���������
//======================================================
void InitGameFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	////�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/flag000.png",
	//	&g_pTextureGameFlag[0]);
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/TEXTURE/Exit000.png",
	//	&g_pTextureGameFlag[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameFlag,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameFlag->Lock(0, 0, (void**)&pVtx, 0);

	//�e���ڂ̏�����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		g_aGameFlag[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH + 400.0f, SCREEN_HEIGHT / 2, 0.0f);
		g_aGameFlag[nCnt].nType = 0;
		g_aGameFlag[nCnt].bUse = false;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);

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

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameFlag->Unlock();
}

//======================================================
//		���Ԓn�_�̏I������
//======================================================
void UninitGameFlag(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_FLAG; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureGameFlag[nCnt] != NULL)
		{
			g_pTextureGameFlag[nCnt]->Release();
			g_pTextureGameFlag[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGameFlag != NULL)
	{
		g_pVtxBuffGameFlag->Release();
		g_pVtxBuffGameFlag = NULL;
	}
}

//======================================================
//		���Ԓn�_�̍X�V����
//======================================================
void UpdateGameFlag(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_aGameFlag[nCnt].bUse == true)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameFlag->Unlock();
}

//======================================================
//		���Ԓn�_�̕`�揈��
//======================================================
void DrawGameFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameFlag, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGameFlag[g_aGameFlag[nCnt].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);						//�v���~�e�B�u�i�|���S���j��
	}
}

//======================================================
//		���Ԓn�_�̐ݒ菈��
//======================================================
void SetGameFlag(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_aGameFlag[nCnt].bUse == false)
		{
			g_aGameFlag[nCnt].pos = pos;
			g_aGameFlag[nCnt].nType = nType;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X, g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y, 0.0f);

			g_aGameFlag[nCnt].bUse = true;

			break;
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameFlag->Unlock();
}

//======================================================
//		���Ԓn�_�̐ݒ菈��
//======================================================
void CollisionFlag(D3DXVECTOR3 *pPos, float Width, float Height)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pPos->x + (Width / 2) >= g_aGameFlag[nCnt].pos.x - FLAG_SIZE_X
			&& pPos->x - (Width / 2) <= g_aGameFlag[nCnt].pos.x + FLAG_SIZE_X
			&& pPos->y >= g_aGameFlag[nCnt].pos.y - FLAG_SIZE_Y
			&& pPos->y - Height <= g_aGameFlag[nCnt].pos.y + FLAG_SIZE_Y)
		{
			for (int nCnt = 0; nCnt < 15; nCnt++)
			{
				SetParticle(g_aGameFlag[nCnt].pos);
			}
			if (nCnt == 0)
			{
				nPoint = 1;
			}
			if (nCnt == 1)
			{
				nPoint = 2;
			}
			if (nCnt == 2)
			{
				nPoint = 3;
			}
		}
	}
}

int FlagStage(void)
{
	return nPoint;
}

//======================================================
//		�v���C���[�̏����擾
//======================================================
FLAG *GetFlag(void)
{
	return &g_aGameFlag[0];
}