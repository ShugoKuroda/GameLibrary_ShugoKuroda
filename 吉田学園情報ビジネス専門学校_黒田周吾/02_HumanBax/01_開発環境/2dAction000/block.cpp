#include "main.h"
#include "block.h"
#include "player.h"
#include "switch.h"
#include "particle.h"
#include "flag.h"
#include "fade.h"
#include "sound.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBlock[MAX_BLOCK_TEX] = { NULL };			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Block g_aBlock[MAX_BLOCK];							//�u���b�N�̏��
int g_nFlag;		//���Ԓn�_�̒ʉߔԍ�

//================================================
//			�u���b�N�̏���������
//================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();			//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.png", &g_pTextureBlock[0]);	//�ʏ�u���b�N
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block001.png", &g_pTextureBlock[1]);	//���u���b�N
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block002.png", &g_pTextureBlock[2]);	//���u���b�N(�S�[���u���b�N)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block003.png", &g_pTextureBlock[3]);	//�Q�[�g�u���b�N
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block004.png", &g_pTextureBlock[4]);	//���u���b�N(�Ƃ��t��)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block005.png", &g_pTextureBlock[5]);	//�Ƃ��u���b�N
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Exit000.png", &g_pTextureBlock[6]);	//�o��

	//�u���b�N�̏��̏�����
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].Airmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].nMoveCntMax = 0;
		g_aBlock[nCntBlock].nMoveCnt = 0;
		g_aBlock[nCntBlock].nMoveCntSwitch = 0;
		g_aBlock[nCntBlock].nType = 0;
		g_aBlock[nCntBlock].nTypeSwitch = 0;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);

		//rhw�̐ݒ�(1.0f�ŌŒ�)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	g_pVtxBuffBlock->Unlock();
}

//======================================================
//		�u���b�N�̏I������
//======================================================
void UninitBlock(void)
{
	for (int nCnt = 0; nCnt > MAX_BLOCK_TEX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBlock != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//======================================================
//		�u���b�N�̍X�V����
//======================================================
void UpdateBlock(void)
{
	Player *pPlayer;

	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{//�g�p���Ă���ꍇ

			//�O��̈ʒu��ۑ�
			g_aBlock[nCnt].posOld = g_aBlock[nCnt].pos;

			//�d�͕��ׂ�������
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				g_aBlock[nCnt].move.y += 1.0f;
			}

			//�����u���b�N�̓��쏈��
			g_aBlock[nCnt].nMoveCnt++;
			if (g_aBlock[nCnt].nMoveCnt == g_aBlock[nCnt].nMoveCntMax)
			{
				g_aBlock[nCnt].nMoveCnt = 0;
				g_aBlock[nCnt].Airmove *= -1.0f;
			}

			//���X�N���[������
			GameScroll(0);

			//�ʒu�X�V
			g_aBlock[nCnt].pos += g_aBlock[nCnt].Airmove;
			g_aBlock[nCnt].pos += g_aBlock[nCnt].move;
			g_aBlock[nCnt].pos += g_aBlock[nCnt].BlockMove;

			//�����蔻��
			CollisionPlayer(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);
			CollisionMoveBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);

			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].BlockMove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
			}
			if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 5)
			{
				CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].BlockMove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 1);
			}
			if (g_aBlock[nCnt].nType == 2)
			{
				CollisionFlag(&g_aBlock[nCnt].pos, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);
			}

			//���_���W�̍X�V
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
			}
			else if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 3 || g_aBlock[nCnt].nType == 5 || g_aBlock[nCnt].nType == 6)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCnt].fTexY);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, g_aBlock[nCnt].fTexY);
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//======================================================
//		�u���b�N�̕`�揈��
//======================================================
void DrawBlock(void)
{
	int nCntBlock;
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�e���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].nType]);
			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCntBlock * 4,								//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);		//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//======================================================
//		�u���b�N�̐ݒ菈��
//======================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nMoveCntMax, int nType,int nTypeSwitch, float width, float height, float TexSizeX, float TexSizeY)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == false)
		{//�u���b�N���g�p����Ă��Ȃ��ꍇ

			g_aBlock[nCnt].pos = pos;
			g_aBlock[nCnt].Airmove = move;
			g_aBlock[nCnt].nMoveCntMax = nMoveCntMax;
			g_aBlock[nCnt].nMoveCnt = 0;
			g_aBlock[nCnt].nType = nType;
			g_aBlock[nCnt].nTypeSwitch = nTypeSwitch;
			g_aBlock[nCnt].fWidth = width;
			g_aBlock[nCnt].fHeight = height;
			g_aBlock[nCnt].fTexX = TexSizeX;
			g_aBlock[nCnt].fTexY = TexSizeY;

			//���_���W�̍X�V
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y, 0.0f);
			}
			else if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 3 || g_aBlock[nCnt].nType == 5 || g_aBlock[nCnt].nType == 6)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2), g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2), 0.0f);
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCnt].fTexY);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt].fTexX, g_aBlock[nCnt].fTexY);

			g_aBlock[nCnt].bUse = true;

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//======================================================
//		�u���b�N�̓����蔻��
//======================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pMoveBlock, float Width, float Height, int nType)
{
	bool bIsLanding = false;		//���n���Ă��邩�ǂ���

	//�v���C���[�����擾
	Player *pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			if (g_aBlock[nCnt].nType == 0 || g_aBlock[nCnt].nType == 5)
			{//�ʏ�̃u���b�N�̏ꍇ
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPosOld->y <= g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2)
					&& pPos->y > g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2))
				{//�ォ��u���b�N�ɒ��n�����ꍇ
					if (g_aBlock[nCnt].nType == 5)
					{//�Ƃ��u���b�N�ɏ������
						if (nType == 2)
						{
							//�T�E���h�Đ�
							PlaySound(SOUND_LABEL_SE_EXPLOSION);

							//�v���C���[����������
							pPlayer->bUse = false;

							//�p�[�e�B�N������
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - (PLAYER_SIZE_Y / 2), pPlayer->pos.z));
							}
							break;
						}
					}
					pPos->y = g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2);
					bIsLanding = true;
					pMove->y = 0.0f;
					*pMoveBlock = g_aBlock[nCnt].Airmove;

					//���t�g�u���b�N�ɏ�����ꍇ
					if (g_aBlock[nCnt].Airmove.x < 0.0f || g_aBlock[nCnt].Airmove.x > 0.0f)
					{
						//���X�N���[������
						GameScroll(1);
					}
				}
			}
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{//���u���b�N�̏ꍇ
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPosOld->y <= g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
					&& pPos->y > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight)
				{//�ォ��u���b�N�ɒ��n�����ꍇ
					if (g_aBlock[nCnt].nType == 4)
					{//�Ƃ��t���u���b�N�ɏ������
						if (nType == 2)
						{
							//�v���C���[����������
							pPlayer->bUse = false;

							//�p�[�e�B�N������
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - (PLAYER_SIZE_Y / 2), pPlayer->pos.z));
							}

							//�T�E���h�Đ�
							PlaySound(SOUND_LABEL_SE_EXPLOSION);
							break;
						}
					}
					pPos->y = g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight;
					bIsLanding = true;
					pMove->y = 0.0f;
					*pMoveBlock = g_aBlock[nCnt].Airmove;
				}
			}
			if (nType == 0 || nType == 2)
			{
				if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
				{//���u���b�N�̏ꍇ
					//������u���b�N�ɂԂ������ꍇ
					if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y
						&& pPosOld->y - Height >= g_aBlock[nCnt].pos.y)
					{
						pPos->y = g_aBlock[nCnt].pos.y + Height;
						pMove->y = 0.0f;
					}
				}
				else
				{
					//������u���b�N�ɂԂ������ꍇ
					if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2)
						&& pPosOld->y - Height >= g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2))
					{
						pPos->y = g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2) + Height;
						pMove->y = 0.0f;
					}
				}
			}

			//if (nType == 1)
			//{
			//	//������u���b�N�ɂԂ������ꍇ
			//	if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
			//		&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
			//		&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
			//		&& pPosOld->y - (Height / 2) >= g_aBlock[nCnt].pos.y)
			//	{
			//		CollisionMoveBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight);
			//	}
			//}

			if (nType == 0 || nType == 2)
			{//���u���b�N�A�v���C���[�̏ꍇ
				if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
				{
					//�E����u���b�N�ɂԂ��������ꍇ
					if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
						&& pPos->y - Height < g_aBlock[nCnt].pos.y)
					{
						g_aBlock[nCnt].posOld.x = g_aBlock[nCnt].pos.x;
						g_aBlock[nCnt].pos.x = pPos->x - (Width / 2) - (g_aBlock[nCnt].fWidth / 2) - 3;
						CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
						GameScroll(2);
						pPos->x = g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2) + (Width / 2);
						pMove->x = 0.0f;
					}
				}
				else
				{
					//�E����u���b�N�ɂԂ��������ꍇ
					if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2))
					{
						pPos->x = g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2) + (Width / 2);
						pMove->x = 0.0f;
					}
				}
			}

			if (nType == 1)
			{//�ʏ�u���b�N(���t�g)�̏ꍇ
			 //�E����u���b�N�ɂԂ��������ꍇ
				if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
					&& pPos->y + (Height / 2) < g_aBlock[nCnt].pos.y)
				{
					pPos->x = g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2) + (Width / 2);
				}
			}

			if (nType == 0 || nType == 2)
			{
				if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
				{
					//������u���b�N�ɂԂ��������ꍇ
					if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
						&& pPos->y - Height < g_aBlock[nCnt].pos.y)
					{
						g_aBlock[nCnt].posOld.x = g_aBlock[nCnt].pos.x;
						g_aBlock[nCnt].pos.x = pPos->x + (Width / 2) + (g_aBlock[nCnt].fWidth / 2) + 3;
						CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].Airmove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
						GameScroll(2);
						pPos->x = g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2) - (Width / 2);
						pMove->x = 0.0f;
					}
				}
				else
				{
					//������u���b�N�ɂԂ��������ꍇ
					if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
						&& pPos->y > g_aBlock[nCnt].pos.y - (g_aBlock[nCnt].fHeight / 2)
						&& pPos->y - Height < g_aBlock[nCnt].pos.y + (g_aBlock[nCnt].fHeight / 2))
					{
						if (nType == 0)
						{
							if (g_aBlock[nCnt].nType == 6)
							{//�o���ɐG�ꂽ��
								//���[�h�ڍs
								SetFade(MODE_RESULT);

								g_aBlock[nCnt].bUse = false;
							}
						}
						pPos->x = g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2) - (Width / 2);
						pMove->x = 0.0f;
					}
				}
			}

			if (nType == 1)
			{
				//������u���b�N�ɂԂ��������ꍇ
				if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight
					&& pPos->y + (Height / 2) < g_aBlock[nCnt].pos.y)
				{
					pPos->x = g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2) - (Width / 2);
				}
			}

		}
	}
	return bIsLanding;
}

//======================================================
//		���u���b�N�̓����蔻��
//======================================================
void CollisionMoveBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			//���u���b�N�̏ꍇ
			if (g_aBlock[nCnt].nType == 1 || g_aBlock[nCnt].nType == 2 || g_aBlock[nCnt].nType == 4)
			{
				//�ォ��u���b�N�ɒ��n�����ꍇ
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPosOld->y - (Height / 2) <= g_aBlock[nCnt].pos.y
					&& pPos->y - (Height / 2) > g_aBlock[nCnt].pos.y)
				{//���u���b�N�����~������
					g_aBlock[nCnt].posOld = g_aBlock[nCnt].pos;
					g_aBlock[nCnt].pos.y = pPos->y - (Height / 2);
					CollisionBlock(&g_aBlock[nCnt].pos, &g_aBlock[nCnt].posOld, &g_aBlock[nCnt].move, &g_aBlock[nCnt].BlockMove, g_aBlock[nCnt].fWidth, g_aBlock[nCnt].fHeight, 0);
				}

				//�ォ��u���b�N�ɒ��n�����ꍇ
				if (pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
					&& pPosOld->y - (Height / 2) >= g_aBlock[nCnt].pos.y)
				{//���u���b�N�㏸������
					g_aBlock[nCnt].pos.y = pPos->y - (Height / 2);
				}

				//�E����u���b�N�ɂԂ��������ꍇ
				if (pPosOld->x + (Width / 2) <= g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x + (Width / 2) > g_aBlock[nCnt].pos.x - (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
					&& pPos->y + (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight)
				{//�u���b�N�̉E�ɉ����o��
					g_aBlock[nCnt].pos.x = pPos->x + (Width / 2) + (g_aBlock[nCnt].fWidth / 2);
				}

				//������u���b�N�ɂԂ��������ꍇ
				if (pPosOld->x - (Width / 2) >= g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->x - (Width / 2) < g_aBlock[nCnt].pos.x + (g_aBlock[nCnt].fWidth / 2)
					&& pPos->y - (Height / 2) < g_aBlock[nCnt].pos.y
					&& pPos->y + (Height / 2) > g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight)
				{//�u���b�N�̍��ɉ����o��
					g_aBlock[nCnt].pos.x = pPos->x - (Width / 2) - (g_aBlock[nCnt].fWidth / 2);
				}
			}
		}
	}
}

//======================================================
//		�X�C�b�`�̃Q�[�g�M�~�b�N����
//======================================================
int OpenGate(int nTypeSwitch)
{
	int IsnPushing = 1;

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			//���u���b�N�̏ꍇ
			if (g_aBlock[nCnt].nType == 3)
			{
				if (nTypeSwitch == 0)
				{
					if (g_aBlock[nCnt].nTypeSwitch == 1)
					{
						g_aBlock[nCnt].pos.y -= 4.0f;
						g_aBlock[nCnt].nMoveCntSwitch++;
						if (g_aBlock[nCnt].nMoveCntSwitch == 30.0f)
						{
							IsnPushing = 2;
						}
					}
				}
				if (nTypeSwitch == 1)
				{
					if (g_aBlock[nCnt].nTypeSwitch == 2)
					{
						g_aBlock[nCnt].pos.y -= 4.0f;
						g_aBlock[nCnt].nMoveCntSwitch++;
						if (g_aBlock[nCnt].nMoveCntSwitch == 30.0f)
						{
							IsnPushing = 2;
						}
					}
				}
				if (nTypeSwitch == 2)
				{
					if (g_aBlock[nCnt].nTypeSwitch == 3)
					{
						g_aBlock[nCnt].pos.y -= 4.0f;
						g_aBlock[nCnt].nMoveCntSwitch++;
						if (g_aBlock[nCnt].nMoveCntSwitch == 30.0f)
						{
							IsnPushing = 2;
						}
					}
				}
			}
		}
	}
	return IsnPushing;
}

//======================================================
//		�u���b�N�̏����擾
//======================================================
Block *GetBlock(void)
{
	return &g_aBlock[0];
}