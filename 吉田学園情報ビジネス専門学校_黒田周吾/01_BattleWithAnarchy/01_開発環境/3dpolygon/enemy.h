//===================================================================
//
//	�G�����̃w�b�_�[[enemy.h]
//	Author:���c ����
//
//===================================================================
#ifndef _ENEMY_H_			//���̃}�N����`������Ȃ�������
#define _ENEMY_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�}�N����`
//======================================================
#define MAX_ENEMY			(32)		//�G�̍ő吔
#define MAX_ENEMY_PARTS		(10)		//�v���C���[�̃��f���p�[�c�̍ő吔
#define MAX_ENEMY_MOTION	(20)		//���[�V�����̍ő吔
#define MAX_ENEMY_KEY		(20)		//���[�V�����L�[�̍ő吔
#define MAX_ENEMY_FRAME		(120)		//���[�V�����̍ő�t���[����
#define ENEMY_COLLISION		(50.0f)		//�G�̓����蔻��(��)�̔��a

//======================================================
//	�e�v���C���[�\���̂̒�`
//======================================================

//�v���C���[�p�[�c�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//��]
	int nIndex;					//���̃p�[�c�̔ԍ�
	int nParent;				//�e�p�[�c�̔ԍ�
	bool bUse;					//�g�p���Ă��邩�ǂ���
}EnemyParts;

//���[�V�������̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_ENEMY_KEY];		//���[�V�����Đ����̈ʒu
	D3DXVECTOR3 rot[MAX_ENEMY_KEY];		//���[�V�����Đ����̌���
}EnemyKeySet;

//���[�V�������̍\����
typedef struct
{
	int nLoop;
	int nNumKey;
	int nFrame[MAX_ENEMY_FRAME];
	EnemyKeySet aKey[MAX_ENEMY_KEY];
	bool bUse;
}EnemyMotionSet;

//���[�V�������̍\����
typedef struct
{
	EnemyParts aParts[MAX_ENEMY_PARTS];				//�p�[�c�̏��
	EnemyMotionSet aMotion[MAX_ENEMY_MOTION];		//�G�̃��[�V�������
	int nNumParts;			//�p�[�c��
	float fMove;			//�ړ���(�s��)
	float fJump;			//�W�����v��
	int nCntSpawn;			//�G�̏o���J�E���^�[
}EnemyInfo;

//�G�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 rot;			//��]
	D3DXVECTOR3	rotDest;		//�ړI�̊p�x
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 moveKnockBack;	//��_�����̃m�b�N�o�b�N�ړ���
	int nIdxShadow;				//�e�̔ԍ�
	int nCntAttack;				//�ˌ��Ԋu�̃J�E���^�[
	int nLife;					//�̗�
	int nCntDamage;				//��_����Ԃ̐؂�ւ��J�E���^�[
	int nCntDie;				//�G�����ʂ܂ł̃J�E���^�[
	int nCntPlayerAttack;		//�v���C���[�̑��i���U���h�~�J�E���^�[
	bool bIsJumping;			//�W�����v�����ǂ���
	bool bIsSwordHit;			//�v���C���[�̌��U��Hit��̑��iHit�h�~
	bool bIsAttacking;			//�U�������ǂ���
	bool bIsDamage;				//��_�������ǂ���
	bool bUse;					//�g�p���Ă��邩�ǂ���
}Enemy;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(void);
void HomingEnemy(int nCnt);
EnemyInfo *GetEnemyInfo(void);
HRESULT LoadEnemyType(HWND hWnd, char *sXFilePath);
void MotionEnemy(void);

#endif