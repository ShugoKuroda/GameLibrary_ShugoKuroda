#include "main.h"
#include "player.h"
#include "input.h"
#include "block.h"
#include "switch.h"
#include "particle.h"
#include "flag.h"
#include "sound.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Player g_Player;									//�v���C���[�̏��

//======================================================
//		�v���C���[�̏���������
//======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player000.png", &g_pTexturePlayer);

	g_Player.nCounterAnim = 0;		//�J�E���^�[������������
	g_Player.nPatternAnim = 0;		//�p�^�[��No.������������
	g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f);		//�ʒu������������
	g_Player.posOld = D3DXVECTOR3(200.0f, 400.0f, 0.0f);			//�O��̈ʒu������������
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ړ��ʂ�����������
	g_Player.BlockMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//���ݏ���Ă���u���b�N�ԍ��̏�����
	g_Player.bUse = true;											//�g�p�󋵂�����������
	g_Player.bIsJumping = false
		;										//�v���C���[���W�����v�����������������
	g_Player.nDirectionMove = 0;									//�v���C���[�̌�������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�(���S���W�͉��ӂ̒��S)
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_Player.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_Player.pos.y;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�(1.0f�ŌŒ�)
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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//======================================================
//		�v���C���[�̏I������
//======================================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	} 
}

//======================================================
//		�v���C���[�̍X�V����
//======================================================
void UpdatePlayer(void)
{
	//�e�|�C���^�[��錾
	Block *pBlock;

	//�e���̎擾
	pBlock = GetBlock();

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	if (g_Player.bUse == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ
			g_Player.move.x += sinf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.move.y += cosf(-D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.nDirectionMove = 1;
		}

		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ
			g_Player.move.x += sinf(D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.move.y += cosf(D3DX_PI * 0.5f)*PLAYER_MOVE;
			g_Player.nDirectionMove = 0;
		}

		if (g_Player.bIsJumping == false)
		{
			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{//�X�y�[�X�L�[�������ꂽ
				g_Player.move.y = PLAYER_MOVEUP;

				g_Player.bIsJumping = true;

				if (g_Player.nPatternAnim == 0 || g_Player.nPatternAnim == 2)
				{
					g_Player.nPatternAnim++;
				}

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * g_Player.nDirectionMove);
				pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * g_Player.nDirectionMove);
				pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * (g_Player.nDirectionMove + 1));
				pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * (g_Player.nDirectionMove + 1));

				if (g_Player.nPatternAnim >= 4)
				{//�p�^�[��No.�������l�ɖ߂�
					g_Player.nPatternAnim = 0;
				}
			}

			if (g_Player.move.x >= 1.0f || g_Player.move.x <= -1.0f)
			{//�v���C���[���ړ���
				g_Player.nCounterAnim++;		//�J�E���^�[�����Z
				if (g_Player.nCounterAnim > 6)
				{
					g_Player.nCounterAnim = 0;

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * g_Player.nDirectionMove);
					pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * g_Player.nDirectionMove);
					pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * (g_Player.nDirectionMove + 1));
					pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * (g_Player.nDirectionMove + 1));

					g_Player.nPatternAnim++;

					if (g_Player.nPatternAnim >= 4)
					{//�p�^�[��No.�������l�ɖ߂�
						g_Player.nPatternAnim = 0;
					}
				}
			}
			else
			{//�v���C���[����~���̏ꍇ
				if (g_Player.nPatternAnim == 1 || g_Player.nPatternAnim == 3)
				{
					g_Player.nPatternAnim++;
				}
				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * g_Player.nDirectionMove);
				pVtx[1].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * g_Player.nDirectionMove);
				pVtx[2].tex = D3DXVECTOR2(0.25f * g_Player.nPatternAnim, 0.5f * (g_Player.nDirectionMove + 1));
				pVtx[3].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim + 1), 0.5f * (g_Player.nDirectionMove + 1));

				if (g_Player.nPatternAnim >= 4)
				{//�p�^�[��No.�������l�ɖ߂�
					g_Player.nPatternAnim = 0;
				}
			}
		}

		//�O��̈ʒu��ۑ�
		g_Player.posOld = g_Player.pos;

		//�d�͕��ׂ�������
		g_Player.move.y += PLAYER_MOVEDOWN;

		//���X�N���[������
		GameScroll(0);

		//�ʒu�X�V
		g_Player.pos += g_Player.move;
		if (g_Player.bIsJumping == false)
		{
			g_Player.pos.x += g_Player.BlockMove.x;
		}

		//�u���b�N�̓����蔻��
		g_Player.bIsJumping = !CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.BlockMove, PLAYER_SIZE_X, PLAYER_SIZE_Y, 2);
		CollisionSwitch(&g_Player.pos, &g_Player.posOld, &g_Player.move, PLAYER_SIZE_X, PLAYER_SIZE_Y);

		//�ړ��ʍX�V(����)
		g_Player.move.x += -g_Player.move.x * 0.1f;			//(�ړI�̒l - ���݂̒l) * �����W��

		//��ʉ��̎��S����
		if (g_Player.pos.y >= SCREEN_HEIGHT + PLAYER_SIZE_Y)
		{
			//�p�[�e�B�N������
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_SIZE_Y, g_Player.pos.z));
			}
			g_Player.bUse = false;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
	}

	//���_���W�̐ݒ�(���S���W�͉��ӂ̒��S)
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x - PLAYER_SIZE_X / 2;
	pVtx[2].pos.y = g_Player.pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + PLAYER_SIZE_X / 2;
	pVtx[3].pos.y = g_Player.pos.y;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//======================================================
//		�v���C���[�̕`�揈��
//======================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Player.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�v���~�e�B�u(�|���S��)��
	}
}

//======================================================
//		�����u���b�N�̓����蔻��
//======================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height)
{
	//�u���b�N�̃|�C���^�[��錾
	Block *pBlock;

	//�u���b�N�̏����擾
	pBlock = GetBlock();

	bool bIsMoving = false;		//�����Ă��邩�ǂ���

	//�ォ��u���b�N�ɒ��n�����ꍇ
	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPosOld->y - (Height / 2) <= g_Player.pos.y
		&& pPos->y - (Height / 2) > g_Player.pos.y)
	{//�v���C���[�����~������
		g_Player.posOld = g_Player.pos;
		g_Player.pos.y = pPos->y - (Height / 2);
		CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.BlockMove, PLAYER_SIZE_X, PLAYER_SIZE_Y, 0);
	}

	//�ォ��u���b�N�ɒ��n�����ꍇ
	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->y - (Height / 2) < g_Player.pos.y
		&& pPosOld->y - (Height / 2) >= g_Player.pos.y)
	{//�v���C���[���㏸������
		g_Player.pos.y = pPos->y - (Height / 2);
	}

	//�����瓮���u���b�N�ɋ��܂����ꍇ
	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_SIZE_Y
		&& pPosOld->y + (Height / 2) <= g_Player.pos.y - PLAYER_SIZE_Y)
	{
		if (g_Player.bIsJumping == false)
		{
			//�p�[�e�B�N������
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - (PLAYER_SIZE_Y / 2), g_Player.pos.z));
			}
			g_Player.bUse = false;
			g_Player.pos = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
	}

	//�E����u���b�N�ɂԂ��������ꍇ
	if (pPosOld->x + (Width / 2) <= g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_SIZE_X / 2)
		&& pPos->y - (Height / 2) < g_Player.pos.y
		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_SIZE_Y)
	{//�u���b�N�̉E�ɉ����o��
		g_Player.pos.x = pPos->x + (Width / 2) + (PLAYER_SIZE_X / 2);

		//���X�N���[������
		GameScroll(1);
	}
	
	//������u���b�N�ɂԂ��������ꍇ
	if (pPosOld->x - (Width / 2) >= g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_SIZE_X / 2)
		&& pPos->y - (Height / 2) < g_Player.pos.y
		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_SIZE_Y)
	{//�u���b�N�̍��ɉ����o��
		g_Player.pos.x = pPos->x - (Width / 2) - (PLAYER_SIZE_X / 2);

		//�ʒu�X�V
		GameScroll(1);
	}
	return bIsMoving;
}

//======================================================
//		�v���C���[�̎��S����
//======================================================
bool DiePlayer(void)
{
	return g_Player.bUse;
}

//======================================================
//		��ʃX�N���[������
//======================================================
void GameScroll(int nType)
{
	//�e�|�C���^�[��錾
	Block *pBlock;
	Switch *pSwitch;
	FLAG *pFlag;

	//�e���̎擾
	pBlock = GetBlock();
	pSwitch = GetSwitch();
	pFlag = GetFlag();

	//�ʒu�X�V
	if (g_Player.pos.x > SCREEN_WIDTH * 0.7f)
	{//�E�[�ɍs�����Ƃ����ꍇ
		g_Player.pos.x = SCREEN_WIDTH * 0.7f;
		for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++, pBlock++)
		{
			pBlock->pos.x -= g_Player.move.x;

			////�f�o�b�O��
			//pBlock->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//���t�g�ňړ�����ꍇ
				pBlock->pos.x -= 4.0f;
			}

			else if (nType == 2)
			{//���u���b�N�ړ���
				pBlock->pos.x -= 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++, pSwitch++)
		{
			pSwitch->pos.x -= g_Player.move.x;

			////�f�o�b�O��
			//pSwitch->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//���t�g�ňړ�����ꍇ
				pSwitch->pos.x -= 4.0f;
			}

			else if (nType == 2)
			{//���u���b�N�ړ���
				pSwitch->pos.x -= 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pFlag->pos.x -= g_Player.move.x;

			////�f�o�b�O��
			//pSwitch->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//���t�g�ňړ�����ꍇ
				pFlag->pos.x -= 4.0f;
			}

			else if (nType == 2)
			{//���u���b�N�ړ���
				pFlag->pos.x -= 3.0f;
			}
		}
	}

	else if (g_Player.pos.x < SCREEN_WIDTH * 0.3f)
	{//���[�ɍs�����Ƃ����ꍇ
		g_Player.pos.x = SCREEN_WIDTH * 0.3f;
		for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++, pBlock++)
		{
			pBlock->pos.x -= g_Player.move.x;

			////�f�o�b�O��
			//pBlock->pos.x += PLAYER_MOVE;

			if (nType == 1)
			{//���t�g�ňړ�����ꍇ
				pBlock->pos.x += 4.0f;
			}

			else if (nType == 2)
			{//���u���b�N�ړ���
				pBlock->pos.x += 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < MAX_SWITCH; nCnt++, pSwitch++)
		{
			pSwitch->pos.x -= g_Player.move.x;

			////�f�o�b�O��
			//pSwitch->pos.x += PLAYER_MOVE;

			if (nType == 1)
			{//���t�g�ňړ�����ꍇ
				pSwitch->pos.x += 4.0f;
			}

			else if (nType == 2)
			{//���u���b�N�ړ���
				pSwitch->pos.x += 3.0f;
			}
		}
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pFlag->pos.x -= g_Player.move.x;

			////�f�o�b�O��
			//pSwitch->pos.x -= PLAYER_MOVE;

			if (nType == 1)
			{//���t�g�ňړ�����ꍇ
				pFlag->pos.x += 4.0f;
			}

			else if (nType == 2)
			{//���u���b�N�ړ���
				pFlag->pos.x += 3.0f;
			}
		}
	}
}

//======================================================
//		�v���C���[�̏����擾
//======================================================
Player *GetPlayer(void)
{
	return &g_Player;
}