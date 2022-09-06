#include "main.h"
#include "explosion.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];					//�\���̂̐錾

//===============================================
//		�����̏���������
//===============================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		g_aExplosion[nCnt].nSizeExplosion = 100;
		g_aExplosion[nCnt].bUse = false;
		//�A�j���[�V����������
		g_aExplosion[nCnt].nCounterAnimExplosion = 0;
		g_aExplosion[nCnt].nPatternAnimExplosion = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.25f);

		pVtx += 4; //�f�[�^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//===============================================
//		�����̏I������
//===============================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//===============================================
//		�����̍X�V����
//===============================================
void UpdateExplosion(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == true)
		{
			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);

			g_aExplosion[nCnt].nCounterAnimExplosion++;//�J�E���^�[�����Z
			if (g_aExplosion[nCnt].nCounterAnimExplosion % 2 == 0)
			{
				g_aExplosion[nCnt].nPatternAnimExplosion = (g_aExplosion[nCnt].nPatternAnimExplosion + 1) % 16;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion % 4) + 1), 0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion / 4));
				pVtx[1].tex = D3DXVECTOR2(0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion % 4), 0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion / 4));
				pVtx[2].tex = D3DXVECTOR2(0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion % 4) + 1), 0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion / 4) + 1));
				pVtx[3].tex = D3DXVECTOR2(0.25f * (g_aExplosion[nCnt].nPatternAnimExplosion % 4), 0.25f * ((g_aExplosion[nCnt].nPatternAnimExplosion / 4) + 1));

				if (g_aExplosion[nCnt].nPatternAnimExplosion >= 15)
				{
					g_aExplosion[nCnt].bUse = false;
				}
			}
		}
		pVtx += 4; //�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//===============================================
//		�����̕`�揈��
//===============================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == true)
		{
			//�����̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCnt * 4,	//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);			//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//===============================================
//		�����̐ݒ菈��
//===============================================
void SetExplosion(D3DXVECTOR3 pos,float Size)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == false)
		{
			//�ʒu�̎擾
			g_aExplosion[nCnt].posExplosion = pos;
			g_aExplosion[nCnt].nSizeExplosion = Size;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y - g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x - g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCnt].posExplosion.x + g_aExplosion[nCnt].nSizeExplosion, g_aExplosion[nCnt].posExplosion.y + g_aExplosion[nCnt].nSizeExplosion, 0.0f);

			g_aExplosion[nCnt].bUse = true;	//�g�p���Ă����Ԃɂ���
			g_aExplosion[nCnt].nCounterAnimExplosion = 0;
			g_aExplosion[nCnt].nPatternAnimExplosion = 0;

			break;
		}
		pVtx += 4; //�f�[�^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}