//===================================================================
//
//	�G����[enemy.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "bullet.h"
#include "model.h"
#include "sound.h"
#include "function.h"
#include "particle.h"
#include "life.h"
#include "score.h"
#include "game.h"

//======================================================
//	�}�N����`
//======================================================
#define ENEMY_WIDTH			(50.0f)		//�G�̕�
#define ENEMY_HEIGHT		(2.0f)		//�G�̍���
#define ENEMY_MOVE			(0.7f)		//�G�̈ړ���
#define ENEMY_MOVEDOWN		(0.3f)		//�G�ւ̏d��
#define ENEMY_MOVEUP		(8.0f)		//�G�̃W�����v��

//-----------------------------------------------------------------------------
//	�\���̂̒�`
//-----------------------------------------------------------------------------
//���[�V�����ԍ����̍\����
typedef struct
{
	int nFrame;					//���݂̃t���[����
	int nKeySetIdx;				//�Đ����̃L�[�Z�b�g�ԍ�
	int nMotionIdx;				//�Đ����̃��[�V�����ԍ�
}EnemyMotionIdx;

//���f�����̍\����
typedef struct
{
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;				//�}�e���A�����̐�
	bool bUse;						//�g�p���Ă��邩�ǂ���
}EnemyPartsInfo;

//======================================================
//	�O���[�o���錾
//======================================================
D3DXMATRIX g_EnemyPartsmtxWorld[MAX_ENEMY_PARTS];	//���[���h�}�g���b�N�X(�G�̃p�[�c)
EnemyPartsInfo g_aEnemyPartsInfo[MAX_ENEMY_PARTS];	//���f��(�p�[�c)�̏��
EnemyMotionIdx g_EnemyMotionIdx;					//�G�̃��[�V�����ԍ����
Enemy g_aEnemy[MAX_ENEMY];							//�G�̏��
EnemyInfo g_EnemyInfo;								//�G�̃��f�����

//======================================================
//	�v���C���[�̏���������
//======================================================
void InitEnemy(void)
{
	//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
	ZeroMemory(&g_EnemyPartsmtxWorld, sizeof(g_EnemyPartsmtxWorld));
	//�v���C���[�\���̂̏�����
	ZeroMemory(&g_aEnemyPartsInfo, sizeof(g_aEnemyPartsInfo));
	ZeroMemory(&g_aEnemy, sizeof(g_aEnemy));
	//�v���C���[�̃��[�V�����ԍ����̏�����
	ZeroMemory(&g_EnemyMotionIdx, sizeof(EnemyMotionIdx));

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//�e�̐ݒ�
		g_aEnemy[nCnt].nIdxShadow = SetShadow(D3DXVECTOR3(g_aEnemy[nCnt].pos.x, 1.0f, g_aEnemy[nCnt].pos.z), g_aEnemy[nCnt].rot, 30.0f, 30.0f);
	}

	//�G�̏����z�u
	SetEnemy();
}

//======================================================
//	�v���C���[�̏I������
//======================================================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_PARTS; nCnt++)
	{
		//���b�V���̔j��
		if (g_aEnemyPartsInfo[nCnt].pMesh != NULL)
		{
			g_aEnemyPartsInfo[nCnt].pMesh->Release();
			g_aEnemyPartsInfo[nCnt].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aEnemyPartsInfo[nCnt].pBuffMat != NULL)
		{
			g_aEnemyPartsInfo[nCnt].pBuffMat->Release();
			g_aEnemyPartsInfo[nCnt].pBuffMat = NULL;
		}
	}
}

//======================================================
//	�v���C���[�̍X�V����
//======================================================
void UpdateEnemy(void)
{
	//�v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	//���S�o�����̎擾
	LogoCount *pLogoCount = GetLogoCount();

	g_EnemyInfo.nCntSpawn++;		//�G�o���J�E���^�[�̉��Z
	if (g_EnemyInfo.nCntSpawn >= 600)
	{//�J�E���^�[����萔(10�b)
		SetEnemy();			//�G���o��
		g_EnemyInfo.nCntSpawn = 0;			//�J�E���^�[���Z�b�g
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{//�G���g�p����Ă���

			//�O��̈ʒu��ۑ�
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (pLogoCount->bNot)
			{
				if (g_aEnemy[nCntEnemy].nLife > 0)
				{//�G�̗̑͂�����ꍇ
					g_aEnemy[nCntEnemy].nCntAttack++;		//�U���J�E���^�[�̉��Z

					if (g_aEnemy[nCntEnemy].bIsDamage)
					{//�G���_���[�W��Ԃ̏ꍇ
						g_aEnemy[nCntEnemy].nCntDamage++;			//�_���[�W��Ԑ؂�ւ��J�E���^�[�̉��Z

						g_aEnemy[nCntEnemy].move.x += g_aEnemy[nCntEnemy].moveKnockBack.x * ENEMY_MOVE;
						g_aEnemy[nCntEnemy].move.z += g_aEnemy[nCntEnemy].moveKnockBack.z * ENEMY_MOVE;

						if (g_EnemyMotionIdx.nMotionIdx != 2)
						{//������у��[�V�����̍Đ�
							g_EnemyMotionIdx.nMotionIdx = 2;
							g_EnemyMotionIdx.nKeySetIdx = 0;
							g_EnemyMotionIdx.nFrame = 0;
						}
						if (g_aEnemy[nCntEnemy].nCntDamage == 1)
						{
							g_aEnemy[nCntEnemy].move.y = (g_EnemyInfo.fJump / 3);
						}
						if (g_aEnemy[nCntEnemy].nCntDamage >= 30)
						{//�J�E���^�[�����ȏ�	
							g_aEnemy[nCntEnemy].bIsDamage = false;		//�v���C���[�̃_���[�W��Ԃ�����
							g_aEnemy[nCntEnemy].nCntDamage = 0;			//�_���[�W�J�E���^�[�̃��Z�b�g
						}
					}

					//�G�̍U������
					else if (SphereCollision(pPlayer->pos, g_aEnemy[nCntEnemy].pos, ENEMY_COLLISION - 20.0f, PLAYER_COLLISION))
					{//�v���C���[�ɋ߂Â�����
						if (g_aEnemy[nCntEnemy].nCntAttack >= 180 && !g_aEnemy[nCntEnemy].bIsAttacking)
						{//���Ԋu�ōU��
							g_aEnemy[nCntEnemy].bIsAttacking = true;		//�U���t���O�𗧂Ă�

							g_aEnemy[nCntEnemy].move.y = g_EnemyInfo.fJump;			//�W�����v
							g_aEnemy[nCntEnemy].bIsJumping = true;		//�W�����v�����L���ɂ���

							if (g_EnemyMotionIdx.nMotionIdx != 2)
							{//�U�����[�V�����̍Đ�
								g_EnemyMotionIdx.nMotionIdx = 2;
								g_EnemyMotionIdx.nKeySetIdx = 0;
								g_EnemyMotionIdx.nFrame = 0;
							}
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = 0.0f;
							g_aEnemy[nCntEnemy].move.z = 0.0f;
						}
						if (g_aEnemy[nCntEnemy].nCntAttack >= 185 && g_aEnemy[nCntEnemy].bIsAttacking)
						{//�U�����[�V�������I������
							if (!pPlayer->bIsDamage)
							{//�v���C���[���_���[�W��Ԃł͂Ȃ���
								//�G�̍U������
								if (SphereCollision(pPlayer->pos, g_aEnemy[nCntEnemy].pos, ENEMY_COLLISION - 20.0f, PLAYER_COLLISION))
								{//�v���C���[���U���͈͓���������
									pPlayer->bIsDamage = true;

									//�v���C���[�̃��C�t����
									AddLife(-20);

									//�v���C���[�ƓG�̋���������ۑ�(�ړI�̈ʒu - ���݂̈ʒu)
									D3DXVECTOR3 vecToEnemy = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

									//�G����v���C���[�ւ̃x�N�g��(�ړ���)�ɕϊ�����
									D3DXVec3Normalize(&vecToEnemy, &vecToEnemy);

									//�ړ��ʂ̍X�V
									pPlayer->moveKnockBack.x = vecToEnemy.x * (pPlayer->fMove);
									pPlayer->moveKnockBack.z = vecToEnemy.z * (pPlayer->fMove);

									//�v���C���[�̌����X�V
									pPlayer->rotDest.y = atan2f(vecToEnemy.x, vecToEnemy.z);

									//�T�E���h�Đ�
									PlaySound(SOUND_LABEL_SE_DAMAGE);

									//�p�[�e�B�N������
									for (int nCnt = 0; nCnt < 20; nCnt++)
									{
										SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z),		//�ʒu
											D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//�ړ���
												tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
												cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
											D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),		//�F
											D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//�傫��
											0);			//���
									}
								}
							}
						}
						if (g_aEnemy[nCntEnemy].nCntAttack >= 210 && g_aEnemy[nCntEnemy].bIsAttacking)
						{//�U�����[�V�������I������
							g_aEnemy[nCntEnemy].nCntAttack = 0;			//�U���J�E���^�[�����Z�b�g
							g_aEnemy[nCntEnemy].bIsAttacking = false;	//�U���t���O�𖳌���
						}
					}
					else
					{//�v���C���[���߂��ɂ��Ȃ�������
						//�v���C���[��Ǐ]���鏈��
						HomingEnemy(nCntEnemy);
						g_aEnemy[nCntEnemy].bIsAttacking = false;		//�U���t���O�̖�����
					}

					g_aEnemy[nCntEnemy].nCntPlayerAttack++;			//�v���C���[�̑��i���U���h�~�J�E���^�[�̉��Z
					if (g_aEnemy[nCntEnemy].nCntPlayerAttack >= 20)
					{//�J�E���^�[����萔
						g_aEnemy[nCntEnemy].bIsSwordHit = false;		//�G�̌��U��Hit��Ԃ�����
						g_aEnemy[nCntEnemy].nCntPlayerAttack = 0;		//�J�E���^�[���Z�b�g
					}
				}
			}

			g_aEnemy[nCntEnemy].move.y -= ENEMY_MOVEDOWN;		//�d�͕��ׂ�������
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;		//�ʒu�X�V

			//�ړ��ʂ̌���(�ړI�̒l - ���݂̒l) * �����W��
			g_aEnemy[nCntEnemy].move.x += -g_aEnemy[nCntEnemy].move.x * 0.2f;
			g_aEnemy[nCntEnemy].move.z += -g_aEnemy[nCntEnemy].move.z * 0.2f;

			if (g_aEnemy[nCntEnemy].nLife <= 0)
			{//�G�̗̑͂�0�ȉ�
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̃��Z�b�g
				g_aEnemy[nCntEnemy].nCntDie++;		//���S�J�E���^�[�̉��Z
				if (g_EnemyMotionIdx.nMotionIdx != 3)
				{//���ꃂ�[�V�����̍Đ�
					g_EnemyMotionIdx.nMotionIdx = 3;
					g_EnemyMotionIdx.nKeySetIdx = 0;
					g_EnemyMotionIdx.nFrame = 0;
				}
				if (g_aEnemy[nCntEnemy].nCntDie >= 60)
				{//���S�J�E���^�[����萔�ȏ�
					//�p�[�e�B�N������
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z),		//�ʒu
							D3DXVECTOR3(sinf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,		//�ړ���
								tanf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f,
								cosf((float)(rand() % 30) / 300 + 1) * (float)(rand() % 629 - 314) / 100.0f),
							D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f),		//�F
							D3DXVECTOR3(3.0f, 3.0f, 3.0f),			//�傫��
							0);				//�傫��
					}
					AddScore(3);			//�X�R�A���Z
					g_aEnemy[nCntEnemy].bUse = false;		//�G������
					g_aEnemy[nCntEnemy].nCntDie = 0;			//�J�E���^�[���Z�b�g

					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
			}
			else if (!g_aEnemy[nCntEnemy].bIsDamage)
			{//�G���_���[�W��ԂłȂ��ꍇ
				if (!g_aEnemy[nCntEnemy].bIsAttacking)
				{//�G���U�����łȂ��ꍇ
					if (g_aEnemy[nCntEnemy].move.x > 0.1f
						|| g_aEnemy[nCntEnemy].move.x < -0.1f
						|| g_aEnemy[nCntEnemy].move.z > 0.1f
						|| g_aEnemy[nCntEnemy].move.z < -0.1f)
					{//�G���ړ���
						if (g_EnemyMotionIdx.nMotionIdx != 1)
						{//�ړ����[�V�����̍Đ�
							g_EnemyMotionIdx.nMotionIdx = 1;
							g_EnemyMotionIdx.nKeySetIdx = 0;
							g_EnemyMotionIdx.nFrame = 0;
						}
					}
					else
					{//�~�܂��Ă���ꍇ
						if (g_EnemyMotionIdx.nMotionIdx != 0)
						{//�j���[�g�������[�V�����̍Đ�
							g_EnemyMotionIdx.nMotionIdx = 0;
							g_EnemyMotionIdx.nKeySetIdx = 0;
							g_EnemyMotionIdx.nFrame = 0;
						}
					}
				}
			}

			//�p�x�̍X�V
			if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
			}
			if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
			}
			g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * 0.1f;

			//�p�x�̐��K��
			if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}
			if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}

			//�u���b�N�̓����蔻��
			g_aEnemy[nCntEnemy].bIsJumping = !CollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, ENEMY_WIDTH, ENEMY_HEIGHT, 1);

			//�ǁE���̓����蔻�菈��
			if (g_aEnemy[nCntEnemy].pos.x - (ENEMY_WIDTH / 2) <= -2000.0f / 2)
			{//����
				g_aEnemy[nCntEnemy].pos.x = (-2000.0f / 2) + (ENEMY_WIDTH / 2);
			}
			else if (g_aEnemy[nCntEnemy].pos.x + (ENEMY_WIDTH / 2) >= 2000.0f / 2)
			{//�E��
				g_aEnemy[nCntEnemy].pos.x = (2000.0f / 2) - (ENEMY_WIDTH / 2);
			}
			if (g_aEnemy[nCntEnemy].pos.z + (ENEMY_WIDTH / 2) >= 2000.0f / 2)
			{//����
				g_aEnemy[nCntEnemy].pos.z = (2000.0f / 2) - (ENEMY_WIDTH / 2);
			}
			else if (g_aEnemy[nCntEnemy].pos.z - (ENEMY_WIDTH / 2) <= -2000.0f / 2)
			{//��O��
				g_aEnemy[nCntEnemy].pos.z = (-2000.0f / 2) + (ENEMY_WIDTH / 2);
			}
			if (g_aEnemy[nCntEnemy].pos.y - (ENEMY_HEIGHT / 2) <= 0.0f)
			{//��
				g_aEnemy[nCntEnemy].pos.y = (ENEMY_HEIGHT / 2);
				g_aEnemy[nCntEnemy].move.y = 0.0f;		//�ړ���Y�̏�����
				g_aEnemy[nCntEnemy].bIsJumping = false;
			}

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 1.0f, g_aEnemy[nCntEnemy].pos.z), g_aEnemy[nCntEnemy].rot);

			//���[�V�����Đ�
			MotionEnemy();
		}
	}
}

//======================================================
//	�v���C���[�̕`�揈��
//======================================================
void DrawEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			D3DXMATRIX mtxRotParent;		//�e�̃��[���h�}�g���b�N�X
			D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

			//���[���h�}�g���b�N�X�̏������i�e�j
			D3DXMatrixIdentity(&mtxRotParent);

			//�v���C���[�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxRot);

			//�v���C���[�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);

			D3DXMatrixMultiply(&mtxRotParent, &mtxRotParent, &mtxTrans);

			for (int nCnt = 0; nCnt < MAX_ENEMY_PARTS; nCnt++)
			{
				if (g_EnemyInfo.aParts[nCnt].bUse)
				{
					D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
					D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

					//���[���h�}�g���b�N�X�̏������i�q�j
					D3DXMatrixIdentity(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex]);

					//�����𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRot,
						g_EnemyInfo.aParts[nCnt].rot.y, g_EnemyInfo.aParts[nCnt].rot.x, g_EnemyInfo.aParts[nCnt].rot.z);

					D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &mtxRot);

					//�ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans,
						g_EnemyInfo.aParts[nCnt].pos.x, g_EnemyInfo.aParts[nCnt].pos.y, g_EnemyInfo.aParts[nCnt].pos.z);

					D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &mtxTrans);

					//�����̐e���f���̃}�g���b�N�X�Ƃ̊|���Z
					if (g_EnemyInfo.aParts[nCnt].nParent < 0)
					{
						//���f���̃}�g���b�N�X �� �e�̃��[���h�}�g���b�N�X
						D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &mtxRotParent);
					}
					else
					{
						//���f���̃}�g���b�N�X �� �e�̃��[���h�}�g���b�N�X
						D3DXMatrixMultiply(&g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex], &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nParent]);
					}

					//���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_EnemyPartsmtxWorld[g_EnemyInfo.aParts[nCnt].nIndex]);

					//���݂̃}�e���A����ێ�
					pDevice->GetMaterial(&matDef);

					//�}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL*)g_aEnemyPartsInfo[g_EnemyInfo.aParts[nCnt].nIndex].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aEnemyPartsInfo[g_EnemyInfo.aParts[nCnt].nIndex].nNumMat; nCntMat++)
					{
						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//���f���p�[�c�̕`��
						g_aEnemyPartsInfo[g_EnemyInfo.aParts[nCnt].nIndex].pMesh->DrawSubset(nCntMat);
					}

					//�ێ����Ă����}�e���A����߂�
					pDevice->SetMaterial(&matDef);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	���f���^�C�v�ǂݍ��ݏ���
//-----------------------------------------------------------------------------
HRESULT LoadEnemyType(HWND hWnd, char *sXFilePath)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < MAX_ENEMY_PARTS; nCntPatn++)
	{
		if (!g_aEnemyPartsInfo[nCntPatn].bUse)
		{
			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemyPartsInfo[nCntPatn].pBuffMat,
				NULL,
				&g_aEnemyPartsInfo[nCntPatn].nNumMat,
				&g_aEnemyPartsInfo[nCntPatn].pMesh);

			g_aEnemyPartsInfo[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	MessageBox(hWnd, "�ǂݍ��݉\���f�����𒴂��Ă��܂�", "�x���I", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//	�G�̐ݒ菈��
//-----------------------------------------------------------------------------
void SetEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)
		{//�g�p����Ă��Ȃ�
			g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
			g_aEnemy[nCntEnemy].bUse = true;		//�o��������
			g_aEnemy[nCntEnemy].nLife = 100;		//�̗͂̐ݒ�
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	���[�V�����Đ�����
//-----------------------------------------------------------------------------
void MotionEnemy(void)
{
	int nMotion = g_EnemyMotionIdx.nMotionIdx;
	int nKey = g_EnemyMotionIdx.nKeySetIdx;
	int nFrame = g_EnemyInfo.aMotion[nMotion].nFrame[nMotion];

	if (nFrame <= 0)
	{//�e�L�X�g�Őݒ肳�ꂽ�t���[������0�ȉ��������ꍇ
		nFrame = 1;
	}

	//���[�V�����Đ�����
	for (int nCnt = 0; nCnt < MAX_ENEMY_PARTS; nCnt++)
	{
		if (g_EnemyInfo.aParts[nCnt].bUse)
		{
			//�ʒu�X�V�i���[�J�����W�j
			g_EnemyInfo.aParts[nCnt].pos += g_EnemyInfo.aMotion[nMotion].aKey[nKey].pos[nCnt] / (float)g_EnemyInfo.aMotion[nMotion].nFrame[nKey];

			//�p�x�X�V
			g_EnemyInfo.aParts[nCnt].rot += (g_EnemyInfo.aMotion[nMotion].aKey[nKey].rot[nCnt] - g_EnemyInfo.aParts[nCnt].rot) / (float)g_EnemyInfo.aMotion[nMotion].nFrame[nKey];
		}
	}

	//�t���[���̉��Z
	g_EnemyMotionIdx.nFrame++;

	if (g_EnemyInfo.aMotion[nMotion].nFrame[nMotion] <= g_EnemyMotionIdx.nFrame)
	{//�t���[�������ݒ�̒l�𒴂�����

		//�Đ����̃L�[���̉��Z
		g_EnemyMotionIdx.nKeySetIdx++;

		//�t���[���̏�����
		g_EnemyMotionIdx.nFrame = 0;

		if (g_EnemyInfo.aMotion[nMotion].nNumKey <= g_EnemyMotionIdx.nKeySetIdx)
		{//�Đ����̃L�[�����ݒ�̒l�𒴂�����
			if (g_EnemyInfo.aMotion[nMotion].nLoop == 1)
			{
				g_EnemyMotionIdx.nKeySetIdx = 0;
			}
			else
			{//���ݍĐ����̃��[�V��������j���[�g�������[�V�����ɕύX
				g_EnemyMotionIdx.nKeySetIdx = 0;
				g_EnemyMotionIdx.nMotionIdx = 0;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	�G�̒Ǐ]����
//-----------------------------------------------------------------------------
void HomingEnemy(int nCnt)
{
	//�v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	//�v���C���[�ƓG�̋���������ۑ�(�ړI�̈ʒu - ���݂̈ʒu)
	D3DXVECTOR3 vecToPlayer = pPlayer->pos - g_aEnemy[nCnt].pos;

	//�G����v���C���[�ւ̃x�N�g��(�ړ���)�ɕϊ�����
	D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);

	//�ړ��ʂ̍X�V
	g_aEnemy[nCnt].move.x = vecToPlayer.x * g_EnemyInfo.fMove;
	g_aEnemy[nCnt].move.z = vecToPlayer.z * g_EnemyInfo.fMove;

	//���f���̌����X�V
	g_aEnemy[nCnt].rotDest.y = atan2f(vecToPlayer.x, vecToPlayer.z) + D3DX_PI;
}

//======================================================
//	�G�̏����擾
//======================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//======================================================
//	�G�̏����擾
//======================================================
EnemyInfo *GetEnemyInfo(void)
{
	return &g_EnemyInfo;
}