#include "main.h"
#include "switch.h"
#include "block.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureSwitch[MAX_SWITCH_TEX] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSwitch[MAX_SWITCH_TEX] = { NULL };		//���_�o�b�t�@�ւ̃|�C���^
Switch g_aSwitch[MAX_SWITCH];

//======================================================
//		�X�C�b�`��ʂ̏���������
//======================================================
void InitSwitch(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/switch000.png",
		&g_pTextureSwitch[0]);

	//���_�o�b�t�@�̐���
	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SWITCH,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffSwitch[nCnt],
			NULL);
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSwitch[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		g_aSwitch[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSwitch[nCnt].bUse = false;
		g_aSwitch[nCnt].nPushCnt = 0;
		g_aSwitch[nCnt].fWidth = 0.0f;
		g_aSwitch[nCnt].fHeight = 0.0f;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSwitch[nCnt].fWidth, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, g_aSwitch[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSwitch[nCnt].fWidth, g_aSwitch[nCnt].fHeight, 0.0f);

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
	g_pVtxBuffSwitch[0]->Unlock();
}

//======================================================
//		�X�C�b�`��ʂ̏I������
//======================================================
void UninitSwitch(void)
{
	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSwitch[nCnt] != NULL)
		{
			g_pTextureSwitch[nCnt]->Release();
			g_pTextureSwitch[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffSwitch[nCnt] != NULL)
		{
			g_pVtxBuffSwitch[nCnt]->Release();
			g_pVtxBuffSwitch[nCnt] = NULL;
		}
	}
}

//======================================================
//		�X�C�b�`��ʂ̍X�V����
//======================================================
void UpdateSwitch(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSwitch[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		if (g_aSwitch[nCnt].bUse == true)
		{
			if (g_aSwitch[nCnt].nPushCnt == 1)
			{
				g_aSwitch[nCnt].nPushCnt = OpenGate(nCnt);
			}

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffSwitch[0]->Unlock();
}

//======================================================
//		�X�C�b�`��ʂ̕`�揈��
//======================================================
void DrawSwitch(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SWITCH_TEX; nCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffSwitch[nCnt], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSwitch[nCnt]);

		for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
		{
			if (g_aSwitch[nCnt].bUse == true)
			{
				//�v���C���[�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
					nCnt * 4,		//�`�悷��ŏ��̒��_�C���f�b�N�X
					2);		//�v���~�e�B�u�i�|���S���j��
			}
		}
	}
}

//======================================================
//		�X�C�b�`�̐ݒ菈��
//======================================================
void SetSwitch(D3DXVECTOR3 pos, float Width, float Height)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSwitch[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		if (g_aSwitch[nCnt].bUse == false)
		{//�u���b�N���g�p����Ă��Ȃ��ꍇ

			g_aSwitch[nCnt].pos = pos;
			g_aSwitch[nCnt].fWidth = Width;
			g_aSwitch[nCnt].fHeight = Height;

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2), g_aSwitch[nCnt].pos.y + (g_aSwitch[nCnt].fHeight / 2), 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_aSwitch[nCnt].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSwitch[0]->Unlock();
}

//======================================================
//		�X�C�b�`�̓����蔻��
//======================================================
void CollisionSwitch(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height)
{
	for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++)
	{
		if (g_aSwitch[nCnt].bUse == true)
		{
			if (g_aSwitch[nCnt].nPushCnt == 0)
			{
				if (pPos->x + (Width / 2) > g_aSwitch[nCnt].pos.x - (g_aSwitch[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aSwitch[nCnt].pos.x + (g_aSwitch[nCnt].fWidth / 2)
					&& pPosOld->y <= g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2)
					&& pPos->y > g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2))
				{//�ォ��u���b�N�ɒ��n�����ꍇ
					pPos->y = g_aSwitch[nCnt].pos.y - (g_aSwitch[nCnt].fHeight / 2);
					pMove->y = 0.0f;
					g_aSwitch[nCnt].pos.y += 10.0f;
					g_aSwitch[nCnt].nPushCnt = 1;
				}
			}
		}
	}
}



//======================================================
//		�X�C�b�`�̏����擾
//======================================================
Switch *GetSwitch(void)
{
	return &g_aSwitch[0];
}