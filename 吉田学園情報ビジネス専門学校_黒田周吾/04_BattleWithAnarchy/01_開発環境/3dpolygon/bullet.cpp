//===================================================================
//
//	�e�̏���[bullet.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "particle.h"
#include "function.h"
#include "score.h"
#include "sound.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_BULLET			(128)		//�e�̍ő吔
#define BULLET_SIZE			(8.0f)		//�e�̃T�C�Y
#define BULLET_TYPE			(1)			//�e�̎��
#define BULLET_LIFE			(180)		//�e�̎���
#define BULLET_COLLISION	(5.0f)		//�e�̓����蔻��(��)�̔��a		

//================================================
//	�O���[�o���ϐ�
//================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLET_TYPE] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];									//�e�̏��

//================================================
//	�e�̏���������
//================================================
void InitBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet[0]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//�e�\���̂̏�����
	ZeroMemory(&g_aBullet, sizeof(g_aBullet));

	//�e���ڂ̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y + (BULLET_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y + (BULLET_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y - (BULLET_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_SIZE / 2), g_aBullet[nCntBullet].pos.y - (BULLET_SIZE / 2), 0.0f);

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

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//======================================================
//	�e�̏I������
//======================================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < BULLET_TYPE; nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//======================================================
//	�e�̍X�V����
//======================================================
void UpdateBullet(void)
{
	//���ˏ���
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�G���̎擾
		Enemy *pEnemy = GetEnemy();

		if (g_aBullet[nCntBullet].bUse)
		{//�e���g�p����Ă���ꍇ

			g_aBullet[nCntBullet].nLife--;

			//�e�̈ʒu�̍X�V
			switch (g_aBullet[nCntBullet].nType)
			{
			case 0:
				//�ʒu���X�V����
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;
				break;

			case 1:
				g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].pos.z) * BULLET_MOVE;
				g_aBullet[nCntBullet].move.z = cosf(g_aBullet[nCntBullet].pos.z) * BULLET_MOVE;

				//�ʒu���X�V����
				g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.z -= g_aBullet[nCntBullet].move.z;
				break;

			default:
				break;
			}
			for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
			{
				if (pEnemy->bUse)
				{//�G���g�p����Ă���ꍇ

					//�e�ƓG�Ƃ̓����蔻��
					if (SphereCollision(pEnemy->pos, g_aBullet[nCntBullet].pos, ENEMY_COLLISION + 10.0f, BULLET_COLLISION))
					{
						pEnemy->nLife -= 5.0f;		//�G�̗̑͌���
						g_aBullet[nCnt].bUse = false;		//�e������
						AddScore(2);			//�X�R�A���Z

						//�T�E���h�Đ�
						PlaySound(SOUND_LABEL_SE_HIT);

						//�p�[�e�B�N������
						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							SetParticle(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z),		//�ʒu
								D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//�ړ���
									tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
									cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
								D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f),		//�F
								D3DXVECTOR3(1.0f, 1.0f, 1.0f),			//�傫��
								0);				//�傫��
						}
					}
				}
			}

			//�G�t�F�N�g�̐ݒ�
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0);		//�i���W�A�F�A�傫���A�����A��ށj

			if (g_aBullet[nCntBullet].nLife <= 0)
			{//�e�̎����������Ȃ���
				g_aBullet[nCntBullet].bUse = false;
			}
		}
	}
}

//======================================================
//	�e�̕`�揈��
//======================================================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{//�e���g�p����Ă���ꍇ
			 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X�̐ݒ�
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aBullet[nCnt].mtxWorld._11 = mtxView._11;
			g_aBullet[nCnt].mtxWorld._12 = mtxView._21;
			g_aBullet[nCnt].mtxWorld._13 = mtxView._31;
			g_aBullet[nCnt].mtxWorld._21 = mtxView._12;
			g_aBullet[nCnt].mtxWorld._22 = mtxView._22;
			g_aBullet[nCnt].mtxWorld._23 = mtxView._32;
			g_aBullet[nCnt].mtxWorld._31 = mtxView._13;
			g_aBullet[nCnt].mtxWorld._32 = mtxView._23;
			g_aBullet[nCnt].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet[0]);

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
}

//======================================================
//	�e�̐ݒ菈��
//======================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int Type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nType = Type;
			g_aBullet[nCntBullet].nLife = BULLET_LIFE;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
	}
}