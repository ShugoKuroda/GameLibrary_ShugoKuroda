#include "bullet.h"
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLET_TYPE] = { NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];									//�e�̏��
D3DXVECTOR3 g_PosPlayer;										//�v���C���[�̈ʒu���
int g_nCntScore;												//�X�R�A���Z�p

//================================================
//			�e�̏���������
//================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntBullet;

	pDevice = GetDevice();			//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gobo.png", &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet002.png", &g_pTextureBullet[1]);

	g_nCntScore = 0;

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//======================================================
//		�e�̏I������
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
//		�e�̍X�V����
//======================================================
void UpdateBullet(void)
{
	int nCntBullet;		
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//���ˏ���
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			Enemy *pEnemy;		//�G�l�~�[�̃|�C���^
			Player *pPlayer;	//�v���C���[�̃|�C���^
			int nCntEnemy;

			//�G���擾
			pEnemy = GetEnemy();

			//�v���C���[�����擾
			pPlayer = GetPlayer();

			//�e�̈ʒu�̍X�V
			if (g_aBullet[nCntBullet].nType == 0)
			{
				//�ʒu���X�V����
				g_aBullet[nCntBullet].pos += D3DXVECTOR3(15.0f, 0.0f, 0.0f);

			}
			else if (g_aBullet[nCntBullet].nType == 1)
			{
				g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].pos.z)*BULLET_MOVE;
				g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].pos.z)*BULLET_MOVE;

				//�ʒu���X�V����
				g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
			}

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);

			g_aBullet[nCntBullet].g_nCounterAnimBullet++;		//�J�E���^�[�����Z
			if (g_aBullet[nCntBullet].g_nCounterAnimBullet % 3 == 0)
			{
				g_aBullet[nCntBullet].g_nCounterAnimBullet = 0;			//�J�E���^�[���Z�b�g
				g_aBullet[nCntBullet].g_nPatternAnimBullet++;			//�A�j���[�V������i�߂�

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((0.25f*g_aBullet[nCntBullet].g_nPatternAnimBullet), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((0.25f*(g_aBullet[nCntBullet].g_nPatternAnimBullet + 1)), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((0.25f*g_aBullet[nCntBullet].g_nPatternAnimBullet), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((0.25f*(g_aBullet[nCntBullet].g_nPatternAnimBullet + 1)), 1.0f);
			}

			if (g_aBullet[nCntBullet].pos.x < 0.0f || g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.y < 0.0f || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{//�e����ʂ̊O�ɏo����
				g_aBullet[nCntBullet].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//�G���o���e�̓����蔻��
			if (g_aBullet[nCntBullet].nType == 1)
			{
				if (pPlayer->bUse == true)
				{//�v���C���[��������
					if (pPlayer->posPlayer.x + PLAYER_SIZE_X / 2 > pVtx[0].pos.x && pPlayer->posPlayer.x - PLAYER_SIZE_X / 2 < pVtx[1].pos.x &&pPlayer->posPlayer.y + PLAYER_SIZE_Y / 2 > pVtx[1].pos.y && pPlayer->posPlayer.y - PLAYER_SIZE_Y / 2 < pVtx[2].pos.y)
					{//�v���C���[�ɒe������������(�������œ����蔻�蒲��)
					 //�v���C���[�̃q�b�g����
						HitPlayer();
						g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
					}
				}
			}

			//�v���C���[���o���e�̓����蔻��
			if (g_aBullet[nCntBullet].nType == 0)
			{
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						if (pEnemy->posEnemy.x + pEnemy->nSize / 1.5 > pVtx[0].pos.x 
							&& pEnemy->posEnemy.x - pEnemy->nSize / 1.5 < pVtx[1].pos.x
							&&pEnemy->posEnemy.y + pEnemy->nSize / 1.5 > pVtx[1].pos.y
							&& pEnemy->posEnemy.y - pEnemy->nSize / 1.5 < pVtx[2].pos.y)
						{//�G�ɒe������������(�������œ����蔻�蒲��)

							//�G�̃q�b�g����
							HitEnemy(nCntEnemy, 20);		//�����Ń_���[�W�ʂ̒���

							g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
						}
					}
				}
			}
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//======================================================
//		�e�̕`�揈��
//======================================================
void DrawBullet(void)
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nType]);
			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCntBullet * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);		//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//======================================================
//		�e�̐ݒ菈��
//======================================================
void SetBullet(D3DXVECTOR3 pos, int Type)
{
	int nCntBullet;
	int nCntType;
	nCntType = Type;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y - BULLET_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE, g_aBullet[nCntBullet].pos.y + BULLET_SIZE, 0.0f);

			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].nType = nCntType;
			g_aBullet[nCntBullet].g_nCounterAnimBullet = 0;
			g_aBullet[nCntBullet].g_nPatternAnimBullet = 0;

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}