//===================================================================
//
//	�v���C���[����[player.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "bullet.h"
#include "model.h"
#include "sound.h"
#include "inputMouse.h"
#include "function.h"
#include "enemy.h"
#include "particle.h"
#include "score.h"
#include "logo.h"

//======================================================
//	�}�N����`
//======================================================
#define PLAYER_WIDTH			(50.0f)		//�v���C���[�̕�
#define PLAYER_HEIGHT			(8.0f)		//�v���C���[�̍���
#define PLAYER_MOVEDOWN			(0.3f)		//�v���C���[�ւ̏d��
#define PLAYER_MOVEATTACK		(1.2f)		//�U�����̑O�ړ�
#define PLAYER_SWORDCOLLISION	(50.0f)		//�v���C���[�̌��̓����蔻��T�C�Y(��)

//-----------------------------------------------------------------------------
//	�\���̂̒�`
//-----------------------------------------------------------------------------
//���[�V�����ԍ����̍\����
typedef struct
{
	int nFrame;					//���݂̃t���[����
	int nKeySetIdx;				//�Đ����̃L�[�Z�b�g�ԍ�
	int nMotionIdx;				//�Đ����̃��[�V�����ԍ�
}PlayerMotionIdx;

//���f�����̍\����
typedef struct
{
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;				//�}�e���A�����̐�
	bool bUse;						//�g�p���Ă��邩�ǂ���
}PlayerPartsInfo;

//======================================================
//	�O���[�o���錾
//======================================================
D3DXMATRIX g_PlayerPartsmtxWorld[MAX_PARTS];	//���[���h�}�g���b�N�X(�v���C���[�̃p�[�c)
PlayerPartsInfo g_aPartsInfo[MAX_PARTS];		//���f��(�p�[�c)�̏��
PlayerMotionIdx	g_PlayerMotionIdx;				//�v���C���[�̃��[�V�����ԍ����
Player g_Player;		//�v���C���[�̏��

//======================================================
//	�v���C���[�̏���������
//======================================================
void InitPlayer(void)
{
	//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
	ZeroMemory(&g_PlayerPartsmtxWorld, sizeof(g_PlayerPartsmtxWorld));
	//�v���C���[�\���̂̏�����
	ZeroMemory(&g_aPartsInfo, sizeof(g_aPartsInfo)); 
	ZeroMemory(&g_Player, sizeof(Player));
	//�v���C���[�̃��[�V�����ԍ����̏�����
	ZeroMemory(&g_PlayerMotionIdx, sizeof(PlayerMotionIdx));

	//�v���C���[�̏����ʒu�ݒ�
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);

	//�e�̐ݒ�
	g_Player.nIdxShadow = SetShadow(D3DXVECTOR3(g_Player.pos.x, 1.0f, g_Player.pos.z), g_Player.rot, 30.0f, 30.0f);
}

//======================================================
//	�v���C���[�̏I������
//======================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		//���b�V���̔j��
		if (g_aPartsInfo[nCnt].pMesh != NULL)
		{
			g_aPartsInfo[nCnt].pMesh->Release();
			g_aPartsInfo[nCnt].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aPartsInfo[nCnt].pBuffMat != NULL)
		{
			g_aPartsInfo[nCnt].pBuffMat->Release();
			g_aPartsInfo[nCnt].pBuffMat = NULL;
		}
	}
}

//======================================================
//	�v���C���[�̍X�V����
//======================================================
void UpdatePlayer(void)
{
	//�O��̈ʒu��ۑ�
	g_Player.posOld = g_Player.pos;

	//�J�������̎擾
	Camera *pCamera = GetCamera();

	//�G���̎擾
	Enemy *pEnemy = GetEnemy();

	//����̈ʒu�����擾(���[���h�}�g���b�N�X���烏�[���h���W���擾)
	D3DXVECTOR3 SwordPos = D3DXVECTOR3(
		g_PlayerPartsmtxWorld[g_Player.aParts[17].nIndex]._41,
		g_PlayerPartsmtxWorld[g_Player.aParts[17].nIndex]._42,
		g_PlayerPartsmtxWorld[g_Player.aParts[17].nIndex]._43);

	if (g_Player.bUse)
	{
		if (!g_Player.bIsDie)
		{//�v���C���[�����S��Ԃ̂Ƃ��͑���ł��Ȃ�
			if (!g_Player.bIsDamage)
			{//�v���C���[���_���[�W��Ԃ̎��͑���ł��Ȃ�
				if (!g_Player.bIsAttackingSword)
				{//�v���C���[���ߐڍU�����̏ꍇ�͑���ł��Ȃ�
					if (GetKeyboardPress(DIK_LSHIFT) == true)
					{//�_�b�V��
						g_Player.bIsDashing = true;			//�_�b�V�������L���ɂ���
						g_Player.fMoveDash = 0.4f;			//�ړ��ʂ𑝉�
					}
					else
					{//�_�b�V������𖳌��ɂ���
						g_Player.bIsDashing = false;
						g_Player.fMoveDash = 0.0f;
					}

					if (GetKeyboardPress(DIK_A) == true)
					{//���ړ�
						if (GetKeyboardPress(DIK_W) == true)
						{
							g_Player.move.x += sinf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z += cosf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (D3DX_PI / 1.5f) + pCamera->rot.y;
						}
						else if (GetKeyboardPress(DIK_S) == true)
						{
							g_Player.move.x += sinf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z += cosf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (D3DX_PI / 4) + pCamera->rot.y;
						}
						else
						{
							g_Player.move.x += sinf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z += cosf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (D3DX_PI / 2) + pCamera->rot.y;
						}
					}

					else if (GetKeyboardPress(DIK_D) == true)
					{//�E�ړ�
						if (GetKeyboardPress(DIK_W) == true)
						{
							g_Player.move.x -= sinf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z -= cosf(pCamera->rot.y - (D3DX_PI / 1.5f)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (-D3DX_PI / 1.5f) + pCamera->rot.y;
						}
						else if (GetKeyboardPress(DIK_S) == true)
						{
							g_Player.move.x -= sinf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z -= cosf(pCamera->rot.y - (D3DX_PI / 4)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (-D3DX_PI / 4) + pCamera->rot.y;
						}
						else
						{
							g_Player.move.x -= sinf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.move.z -= cosf(pCamera->rot.y - (D3DX_PI / 2)) * (g_Player.fMove + g_Player.fMoveDash);
							g_Player.rotDest.y = (-D3DX_PI / 2) + pCamera->rot.y;
						}
					}

					else if (GetKeyboardPress(DIK_W) == true)
					{//�O�ړ�
						g_Player.move.x += sinf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.move.z += cosf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.rotDest.y = D3DX_PI + pCamera->rot.y;
					}

					else if (GetKeyboardPress(DIK_S) == true)
					{//��ړ�
						g_Player.move.x -= sinf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.move.z -= cosf(pCamera->rot.y) * (g_Player.fMove + g_Player.fMoveDash);
						g_Player.rotDest.y = 0.0f + pCamera->rot.y;
					}

					if (!g_Player.bIsJumping)
					{//�v���C���[���W�����v���łȂ��ꍇ
						if (GetKeyboardTrigger(DIK_SPACE) == true)
						{//�W�����v
							g_Player.move.y = g_Player.fJump;

							//�v���C���[���W�����v������ɂ���
							g_Player.bIsJumping = true;
						}
					}
				}
			}
			if (g_Player.bIsDamage)
			{//�v���C���[���_���[�W��Ԃ̏ꍇ
				g_Player.nCntDamage++;			//�_���[�W��Ԑ؂�ւ��J�E���^�[�̉��Z

				g_Player.move.x += g_Player.moveKnockBack.x;
				g_Player.move.z += g_Player.moveKnockBack.z;

				if (g_PlayerMotionIdx.nMotionIdx != 9)
				{//������у��[�V�����̍Đ�
					g_PlayerMotionIdx.nMotionIdx = 9;
					g_PlayerMotionIdx.nKeySetIdx = 0;
					g_PlayerMotionIdx.nFrame = 0;
				}
				if (g_Player.nCntDamage == 1)
				{
					g_Player.move.y = (g_Player.fJump / 3);
				}
				if (g_Player.nCntDamage >= 30)
				{//�J�E���^�[�����ȏ�	
					g_Player.bIsDamage = false;			//�v���C���[�̃_���[�W��Ԃ�����
					g_Player.nCntDamage = 0;			//�_���[�W�J�E���^�[�̃��Z�b�g
				}
			}
			if (!g_Player.bIsDamage)
			{//��_�����͍U���܂��͕���؂�ւ����ł��Ȃ�
				if (GetMouseTrigger(CLICK_RIGHT) == true)
				{//�E�N���b�N��(�g���K�[)
					switch (g_Player.Weapon)
					{//����̐؂�ւ�
					case PLAYERWEAPON_GUN:			//�v���C���[���e���������Ă���ꍇ
						g_Player.Weapon = PLAYERWEAPON_SWORD;			//���ɐ؂�ւ���
						g_Player.AttackCombo = PLAYERATTACK_1COMBO;		//�R���{�󋵂̏�����
						g_Player.bIsAttacking = false;					//�U���t���O����
						break;
					case PLAYERWEAPON_SWORD:		//�v���C���[�������������Ă���ꍇ
						//�e�ɐ؂�ւ���
						g_Player.Weapon = PLAYERWEAPON_GUN;
						g_Player.bIsAttackingSword = false;				//�ߐڍU���t���O����
						break;
					}
				}
				switch (g_Player.Weapon)
				{//���킲�Ƃ̍U��
				case PLAYERWEAPON_GUN:			//�v���C���[���e���������Ă���ꍇ
					if (GetMousePress(CLICK_LEFT) == true)
					{//���N���b�N��(�v���X)
						g_Player.nCntShot++;				//�ˌ��Ԋu�̃J�E���^�[
						g_Player.bIsAttacking = true;		//�v���C���[���U�����̃t���O�𗧂Ă�(���̃��[�V������Đ��h�~)

						if (g_PlayerMotionIdx.nMotionIdx != 5)
						{//�ˌ����[�V�����̍Đ�
							g_PlayerMotionIdx.nMotionIdx = 5;
							g_PlayerMotionIdx.nKeySetIdx = 0;
							g_PlayerMotionIdx.nFrame = 0;
						}

						if (g_Player.nCntShot > 20)
						{//�ˌ��J�E���^�[�����ȏ�Ŏˌ�

							g_Player.nCntShot = 0;		//�ˌ��J�E���^�[���Z�b�g

							 //�e�̐ݒu(�ړ���(�x�N�g��)�̓v���C���[�̌���)
							SetBullet(D3DXVECTOR3((sinf(g_Player.rot.y) * -42.0f) + g_Player.pos.x, g_Player.pos.y + 57.0f, (cosf(g_Player.rot.y) * -42.0f) + g_Player.pos.z),
								D3DXVECTOR3(sinf(g_Player.rot.y) * -BULLET_MOVE, 0.0f, cosf(g_Player.rot.y) * -BULLET_MOVE),
								0);

							//�T�E���h�Đ�
							PlaySound(SOUND_LABEL_SE_SHOT);
						}
						//�p�[�e�B�N���̐ݒu(�o�b�N�p�b�N�̕��ˌ��̉�)
						SetParticle(D3DXVECTOR3((sinf(g_Player.rot.y) * 10.0f) + g_Player.pos.x, g_Player.pos.y + 50.0f, (cosf(g_Player.rot.y) * 10.0f) + g_Player.pos.z),	//�ʒu
							D3DXVECTOR3(sinf((float)(rand() % 629) / 200) * (float)(rand() % 629 - 314) / 100.0f,		//�ړ���
								-(float)(rand() % 629) / 200.0f,
								cosf((float)(rand() % 629) / 200) * (float)(rand() % 629 - 314) / 100.0f),
							D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f),		//�F
							D3DXVECTOR3(0.7f, 0.7f, 0.7f),			//�傫��
							1);			//���
					}
					else
					{//���N���b�N����Ă��Ȃ�
						g_Player.nCntShot = 0;			//�J�E���^�[�̃��Z�b�g
						g_Player.bIsAttacking = false;	//�v���C���[�̍U������𖳌��ɂ���
					}
					break;
				case PLAYERWEAPON_SWORD:		//�v���C���[�������������Ă���ꍇ
					if (GetMouseTrigger(CLICK_LEFT) == true)
					{//���N���b�N��(�g���K�[)
						g_Player.bIsAttacking = true;			//�v���C���[���U�����̃t���O�𗧂Ă�(���̃��[�V������Đ��h�~)
						g_Player.bIsAttackingSword = true;		//�v���C���[���ߐڍU�����̃t���O�𗧂Ă�(�U�����̃L�[���͖����p)
						switch (g_Player.AttackCombo)
						{
						case PLAYERATTACK_1COMBO:		//0�R���{�̏ꍇ
							if (g_PlayerMotionIdx.nMotionIdx != 6)
							{//�ߐڍU�����[�V�����̍Đ�(�ꌂ��)
								g_PlayerMotionIdx.nMotionIdx = 6;
								g_PlayerMotionIdx.nKeySetIdx = 0;
								g_PlayerMotionIdx.nFrame = 0;
								g_Player.nCntAttack = 0;			//�J�E���^�[���Z�b�g
							}
							//�R���{��Ԃ�i�߂�(�񌂖�)
							g_Player.AttackCombo = PLAYERATTACK_2COMBO;
							break;
						case PLAYERATTACK_2COMBO:			//1�R���{�̏ꍇ
							if (g_Player.nCntAttack >= 20)
							{//�A���U���h�~�J�E���^�[
								if (g_PlayerMotionIdx.nMotionIdx != 7)
								{//�ߐڍU�����[�V�����̍Đ�(�񌂖�)
									g_PlayerMotionIdx.nMotionIdx = 7;
									g_PlayerMotionIdx.nKeySetIdx = 0;
									g_PlayerMotionIdx.nFrame = 0;
									g_Player.nCntAttack = 0;			//�J�E���^�[���Z�b�g
								}
								//�R���{��Ԃ�i�߂�(�O����)
								g_Player.AttackCombo = PLAYERATTACK_3COMBO;
							}
							break;
						case PLAYERATTACK_3COMBO:			//2�R���{�̏ꍇ
							if (g_Player.nCntAttack >= 20)
							{//�A���U���h�~�J�E���^�[
								if (g_PlayerMotionIdx.nMotionIdx != 8)
								{//�ߐڍU�����[�V�����̍Đ�(�O����)
									g_PlayerMotionIdx.nMotionIdx = 8;
									g_PlayerMotionIdx.nKeySetIdx = 0;
									g_PlayerMotionIdx.nFrame = 0;
									g_Player.nCntAttack = 0;			//�J�E���^�[���Z�b�g
								}
								//�R���{��Ԃ��J��Ԃ���ԂɈڍs����
								g_Player.AttackCombo = PLAYERATTACK_LOOP;
							}
							break;
						case PLAYERATTACK_LOOP:			//�R���{���I������ꍇ
							if (g_Player.nCntAttack >= 30)
							{
								//�R���{��Ԃ��ŏ��ɖ߂�(�ꌂ��)
								g_Player.AttackCombo = PLAYERATTACK_1COMBO;
							}
						}
					}
					if (g_Player.bIsAttacking)
					{//�v���C���[���U�����̏ꍇ
						g_Player.nCntAttack++;
						if (g_Player.nCntAttack >= 15 && g_Player.nCntAttack < 20)
						{//�U�����[�V�������Ō���U�艺�낵�Ă��鎞
							g_Player.move.x += sinf(g_Player.rot.y) * -PLAYER_MOVEATTACK;
							g_Player.move.z += cosf(g_Player.rot.y) * -PLAYER_MOVEATTACK;

							for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
							{
								if (pEnemy->bUse)
								{//�G���g�p����Ă���ꍇ
									if (!pEnemy->bIsSwordHit)
									{//���U����1�񓖂�������A���̍U���܂ōU����������Ȃ�
										if (SphereCollision(SwordPos, pEnemy->pos, PLAYER_SWORDCOLLISION, ENEMY_COLLISION))
										{//�G�ƃv���C���[�̌��̓����蔻��

											//�T�E���h�Đ�
											PlaySound(SOUND_LABEL_SE_HIT);

											pEnemy->bIsSwordHit = true;			//�G�����U��Hit��Ԃɂ���
											pEnemy->nCntPlayerAttack = 0;		//�G�̑��i���U���h�~�J�E���^�[�̃��Z�b�g

											pEnemy->bIsDamage = true;		//�G���_���[�W��Ԃɂ���

											//�G�ƃv���C���[�̋���������ۑ�(�ړI�̈ʒu - ���݂̈ʒu)
											D3DXVECTOR3 vecToPlayer = pEnemy->pos - g_Player.pos;

											//�G����v���C���[�ւ̃x�N�g��(�ړ���)�ɕϊ�����
											D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);

											//�ړ��ʂ̍X�V
											pEnemy->moveKnockBack.x = vecToPlayer.x;
											pEnemy->moveKnockBack.z = vecToPlayer.z;

											//�v���C���[�̌����X�V
											pEnemy->rotDest.y = atan2f(vecToPlayer.x, vecToPlayer.z);

											if (g_PlayerMotionIdx.nMotionIdx == 6 || g_PlayerMotionIdx.nMotionIdx == 7)
											{//�v���C���[�̍U����1�A2�i�K�ڂ̏ꍇ
												pEnemy->nLife -= 20.0f;		//�G�̗̑͌���

												//�X�R�A���Z����
												AddScore(0);

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
											else if (g_PlayerMotionIdx.nMotionIdx == 8)
											{//�v���C���[�̍U����3�i�K�ڂ̏ꍇ
												pEnemy->nLife -= 30.0f;		//�G�̗̑͌���

												//�X�R�A���Z
												AddScore(1);

												//�p�[�e�B�N������
												for (int nCnt = 0; nCnt < 20; nCnt++)
												{
													SetParticle(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z),		//�ʒu
														D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//�ړ���
															tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
															cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
														D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f),		//�F
														D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//�傫��
														0);		//���
												}
											}
										}
									}
								}
							}
						}
						if (g_Player.nCntAttack >= 40)
						{//��莞�Ԍo������(�R���{���ɍU�����Ȃ�������)
							g_Player.bIsAttacking = false;			//�U���t���O����
							g_Player.bIsAttackingSword = false;		//�ߐڍU���t���O����
							g_Player.nCntAttack = 0;		//�J�E���^�[���Z�b�g
							g_Player.AttackCombo = PLAYERATTACK_1COMBO;			//�R���{�󋵂����Z�b�g
						}
					}
					break;
				}
			}
			g_Player.move.y -= PLAYER_MOVEDOWN;		//�d�͕��ׂ�������
			g_Player.pos += g_Player.move;			//�ʒu�X�V

			//�u���b�N�̓����蔻��
			g_Player.bIsJumping = !CollisionModel(&g_Player.pos, &g_Player.posOld, &g_Player.move, PLAYER_WIDTH, PLAYER_HEIGHT, 0);

			//�ړ��ʂ̌���(�ړI�̒l - ���݂̒l) * �����W��
			g_Player.move.x += -g_Player.move.x * 0.2f;
			g_Player.move.z += -g_Player.move.z * 0.2f;

			//�ǁE���̓����蔻�菈��
			if (g_Player.pos.x - (PLAYER_WIDTH / 2) <= -2000.0f / 2)
			{//����
				g_Player.pos.x = (-2000.0f / 2) + (PLAYER_WIDTH / 2);
			}
			else if (g_Player.pos.x + (PLAYER_WIDTH / 2) >= 2000.0f / 2)
			{//�E��
				g_Player.pos.x = (2000.0f / 2) - (PLAYER_WIDTH / 2);
			}
			if (g_Player.pos.z + (PLAYER_WIDTH / 2) >= 2000.0f / 2)
			{//����
				g_Player.pos.z = (2000.0f / 2) - (PLAYER_WIDTH / 2);
			}
			else if (g_Player.pos.z - (PLAYER_WIDTH / 2) <= -2000.0f / 2)
			{//��O��
				g_Player.pos.z = (-2000.0f / 2) + (PLAYER_WIDTH / 2);
			}
			if (g_Player.pos.y - (PLAYER_HEIGHT / 2) <= 0.0f)
			{//��
				g_Player.pos.y = (PLAYER_HEIGHT / 2);
				g_Player.bIsJumping = false;
				g_Player.move.y = 0.0f;			//�ړ���Y�̏�����
			}
		}

		if (g_Player.bIsDie)
		{//�v���C���[�����S������
			g_Player.nCntDie++;		//�J�E���^�[���Z
			if (g_PlayerMotionIdx.nMotionIdx != 10)
			{//���ꃂ�[�V�����̍Đ�
				g_PlayerMotionIdx.nMotionIdx = 10;
				g_PlayerMotionIdx.nKeySetIdx = 0;
				g_PlayerMotionIdx.nFrame = 0;
			}
			if (g_Player.nCntDie <= 100)
			{//��莞�ԉ��ɒ��܂���
				g_Player.pos.y -= 0.35f;
			}
			if (g_Player.nCntDie == 101)
			{//���[�V�����Đ����I�������
				g_Player.bUse = false;		//�v���C���[������

				//�p�[�e�B�N������
				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z),		//�ʒu
						D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//�ړ���
							tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
							cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
						D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f),		//�F
						D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//�傫��
						0);			//���

					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}

				//�v���C���[�̔s�k���S���o��������
				SetLogo(3);
			}
		}

		if (!g_Player.bIsDamage)
		{//�v���C���[���_���[�W��ԂłȂ��ꍇ
			if (!g_Player.bIsAttacking)
			{//�v���C���[���U�����łȂ��ꍇ
				if (g_Player.bIsJumping)
				{//�v���C���[���W�����v���̏ꍇ
					if (g_PlayerMotionIdx.nMotionIdx != 3)
					{//�W�����v���[�V�����̍Đ�
						g_PlayerMotionIdx.nMotionIdx = 3;
						g_PlayerMotionIdx.nKeySetIdx = 0;
						g_PlayerMotionIdx.nFrame = 0;
					}
				}
				else if (!g_Player.bIsJumping)
				{//�v���C���[���W�����v���łȂ��ꍇ
					if (g_Player.move.x > 1.0f
						|| g_Player.move.x < -1.0f
						|| g_Player.move.z > 1.0f
						|| g_Player.move.z < -1.0f)
					{//�v���C���[���ړ���
						if (g_Player.bIsDashing)
						{//�v���C���[���_�b�V�����̏ꍇ
							if (g_PlayerMotionIdx.nMotionIdx != 2)
							{//�_�b�V�����[�V�����̍Đ�
								g_PlayerMotionIdx.nMotionIdx = 2;
								g_PlayerMotionIdx.nKeySetIdx = 0;
								g_PlayerMotionIdx.nFrame = 0;
							}
						}
						else if (!g_Player.bIsDashing)
						{//�v���C���[���_�b�V�����Ă��Ȃ��ꍇ
							if (g_PlayerMotionIdx.nMotionIdx != 1)
							{//���s���[�V�����̍Đ�
								g_PlayerMotionIdx.nMotionIdx = 1;
								g_PlayerMotionIdx.nKeySetIdx = 0;
								g_PlayerMotionIdx.nFrame = 0;
							}
						}
					}
					else
					{//�~�܂��Ă���ꍇ
						if (g_PlayerMotionIdx.nMotionIdx != 0)
						{//�j���[�g�������[�V�����̍Đ�
							g_PlayerMotionIdx.nMotionIdx = 0;
							g_PlayerMotionIdx.nKeySetIdx = 0;
							g_PlayerMotionIdx.nFrame = 0;
						}
					}
				}
			}
		}

		//�p�x�̍X�V
		if (g_Player.rotDest.y - g_Player.rot.y > D3DX_PI)
		{
			g_Player.rotDest.y -= D3DX_PI * 2;
		}
		if (g_Player.rotDest.y - g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rotDest.y += D3DX_PI * 2;
		}
		g_Player.rot.y += (g_Player.rotDest.y - g_Player.rot.y) * 0.1f;

		//�p�x�̐��K��
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y -= D3DX_PI * 2;
		}
		if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += D3DX_PI * 2;
		}

		//�e�̈ʒu�X�V
		SetPositionShadow(g_Player.nIdxShadow, D3DXVECTOR3(g_Player.pos.x, 1.0f, g_Player.pos.z), g_Player.rot);

		//���[�V�����Đ�
		MotionPlayer();
	}
}

//======================================================
//	�v���C���[�̕`�揈��
//======================================================
void DrawPlayer(void)
{
	if (g_Player.bUse)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		D3DXMATRIX mtxRotParent;		//�e�̃��[���h�}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

		//���[���h�}�g���b�N�X�̏������i�e�j
		D3DXMatrixIdentity(&mtxRotParent);

		//�v���C���[�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

		D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxRot);

		//�v���C���[�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

		D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxTrans);

		switch (g_Player.Weapon)
		{
		case PLAYERWEAPON_GUN:			//�v���C���[���e���������Ă���ꍇ
			//���̕`�����߁A�e��`�悷��
			g_Player.aParts[15].bUse = false;
			g_Player.aParts[16].bUse = true;
			break;
		case PLAYERWEAPON_SWORD:		//�v���C���[�������������Ă���ꍇ
			//�e�̕`�����߁A����`�悷��
			g_Player.aParts[15].bUse = true;
			g_Player.aParts[16].bUse = false;
			break;
		}

		for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
		{
			if (g_Player.aParts[nCnt].bUse)
			{
				D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
				D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

				//���[���h�}�g���b�N�X�̏������i�q�j
				D3DXMatrixIdentity(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex]);

				//�����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_Player.aParts[nCnt].rot.y, g_Player.aParts[nCnt].rot.x, g_Player.aParts[nCnt].rot.z);

				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &mtxRot);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans,
					g_Player.aParts[nCnt].pos.x, g_Player.aParts[nCnt].pos.y, g_Player.aParts[nCnt].pos.z);

				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &mtxTrans);

				//�����̐e���f���̃}�g���b�N�X�Ƃ̊|���Z
				if (g_Player.aParts[nCnt].nParent < 0)
				{
					//���f���̃}�g���b�N�X �� �e�̃��[���h�}�g���b�N�X
					D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &mtxRotParent);
				}
				else
				{
					//���f���̃}�g���b�N�X �� �e�̃��[���h�}�g���b�N�X
					D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nParent]);
				}

				if (g_aPartsInfo[g_Player.aParts[nCnt].nIndex].pMesh == NULL)
				{//�p�[�c�̃��f����NULL��������
					continue;		//for���̍ŏ��ɖ߂�
				}

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_PlayerPartsmtxWorld[g_Player.aParts[nCnt].nIndex]);
				
				//���݂̃}�e���A����ێ�
				pDevice->GetMaterial(&matDef);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aPartsInfo[g_Player.aParts[nCnt].nIndex].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPartsInfo[g_Player.aParts[nCnt].nIndex].nNumMat; nCntMat++)
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					 
					//���f���p�[�c�̕`��
					g_aPartsInfo[g_Player.aParts[nCnt].nIndex].pMesh->DrawSubset(nCntMat);
				}

				//�ێ����Ă����}�e���A����߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//���f���^�C�v�ǂݍ��ݏ���
//-----------------------------------------------------------------------------
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < MAX_PARTS; nCntPatn++)
	{
		if (!g_aPartsInfo[nCntPatn].bUse)
		{
			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aPartsInfo[nCntPatn].pBuffMat,
				NULL,
				&g_aPartsInfo[nCntPatn].nNumMat,
				&g_aPartsInfo[nCntPatn].pMesh);

			g_aPartsInfo[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	MessageBox(hWnd, "�ǂݍ��݉\���f�����𒴂��Ă��܂�", "�x���I", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//�v���C���[�̃Z�b�g����
//-----------------------------------------------------------------------------
void SetPlayer(Player Player)
{
	g_Player = Player;
}

//-----------------------------------------------------------------------------
//���[�V�����Đ�����
//-----------------------------------------------------------------------------
void MotionPlayer(void)
{
	int nMotion = g_PlayerMotionIdx.nMotionIdx;
	int nKey = g_PlayerMotionIdx.nKeySetIdx;
	int nFrame = g_Player.aMotion[nMotion].nFrame[nKey];

	if (nFrame <= 0)
	{//�e�L�X�g�Őݒ肳�ꂽ�t���[������0�ȉ��������ꍇ
		nFrame = 1;
	}

	//���[�V�����Đ�����
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_Player.aParts[nCnt].bUse)
		{
			//�ʒu�X�V�i���[�J�����W�j
			g_Player.aParts[nCnt].pos += g_Player.aMotion[nMotion].aKey[nKey].pos[nCnt] / (float)g_Player.aMotion[nMotion].nFrame[nKey];

			//�p�x�X�V
			g_Player.aParts[nCnt].rot += (g_Player.aParts[nCnt].baseRot + g_Player.aMotion[nMotion].aKey[nKey].rot[nCnt] - g_Player.aParts[nCnt].rot) / (float)g_Player.aMotion[nMotion].nFrame[nKey];
		}
	}

	//�t���[���̉��Z
	g_PlayerMotionIdx.nFrame++;

	if (g_Player.aMotion[nMotion].nFrame[nKey] <= g_PlayerMotionIdx.nFrame)
	{//�t���[�������ݒ�̒l�𒴂�����

		//�Đ����̃L�[���̉��Z
		g_PlayerMotionIdx.nKeySetIdx++;

		//�t���[���̏�����
		g_PlayerMotionIdx.nFrame = 0;

		if (g_Player.aMotion[nMotion].nNumKey <= g_PlayerMotionIdx.nKeySetIdx)
		{//�Đ����̃L�[�����ݒ�̒l�𒴂�����
			if (g_Player.aMotion[nMotion].nLoop == 1)
			{
				g_PlayerMotionIdx.nKeySetIdx = 0;
			}
			else
			{//���ݍĐ����̃��[�V��������j���[�g�������[�V�����ɕύX
				g_PlayerMotionIdx.nKeySetIdx = 0;
				g_PlayerMotionIdx.nMotionIdx = 0;
			}
		}
	}
}

////======================================================
////	���f���̓����蔻�菈��
////======================================================
//bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Width, float Height)
//{
//	bool bIsMoving = false;		//�����Ă��邩�ǂ���
//
//	//�ォ��u���b�N�ɒ��n�����ꍇ
//	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPosOld->y - (Height / 2) <= g_Player.pos.y
//		&& pPos->y - (Height / 2) > g_Player.pos.y)
//	{//�v���C���[�����~������
//		g_Player.pos.y = pPos->y - (Height / 2);
//	}
//
//	//�ォ��u���b�N�ɒ��n�����ꍇ
//	if (pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPos->y - (Height / 2) < g_Player.pos.y
//		&& pPosOld->y - (Height / 2) >= g_Player.pos.y)
//	{//�v���C���[���㏸������
//		g_Player.pos.y = pPos->y - (Height / 2);
//	}
//
//	//�E����u���b�N�ɂԂ��������ꍇ
//	if (pPosOld->x + (Width / 2) <= g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->x + (Width / 2) > g_Player.pos.x - (PLAYER_WIDTH / 2)
//		&& pPos->y - (Height / 2) < g_Player.pos.y
//		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_HEIGHT)
//	{//�u���b�N�̉E�ɉ����o��
//		g_Player.pos.x = pPos->x + (Width / 2) + (PLAYER_WIDTH / 2);
//	}
//
//	//������u���b�N�ɂԂ��������ꍇ
//	if (pPosOld->x - (Width / 2) >= g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPos->x - (Width / 2) < g_Player.pos.x + (PLAYER_WIDTH / 2)
//		&& pPos->y - (Height / 2) < g_Player.pos.y
//		&& pPos->y + (Height / 2) > g_Player.pos.y - PLAYER_HEIGHT)
//	{//�u���b�N�̍��ɉ����o��
//		g_Player.pos.x = pPos->x - (Width / 2) - (PLAYER_WIDTH / 2);
//	}
//	return bIsMoving;
//}

//======================================================
//	�v���C���[�̏����擾
//======================================================
Player *GetPlayer(void)
{
	return &g_Player;
}