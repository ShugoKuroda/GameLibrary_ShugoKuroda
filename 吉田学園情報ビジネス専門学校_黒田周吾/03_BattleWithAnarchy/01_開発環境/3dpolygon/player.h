//===================================================================
//
//		�v���C���[�����̃w�b�_�[[player.h]
//		Author:���c ����
//
//===================================================================
#ifndef _PLAYER_H_			//���̃}�N����`������Ȃ�������
#define _PLAYER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�}�N����`
//======================================================
#define MAX_PARTS			(20)		//�v���C���[�̃��f���p�[�c�̍ő吔
#define MAX_MOTION			(20)		//���[�V�����̍ő吔
#define MAX_KEY				(20)		//���[�V�����L�[�̍ő吔
#define MAX_FRAME			(120)		//���[�V�����̍ő�t���[����
#define PLAYER_COLLISION	(40.0f)		//�v���C���[�̓����蔻��
#define PLAYER_LIFE			(100)		//�v���C���[�̗̑�

//======================================================
//	�e�v���C���[�\���̂̒�`
//======================================================

//�v���C���[�p�[�c�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//��]
	D3DXVECTOR3 baseRot;		//��{�̊p�x
	int nIndex;					//���̃p�[�c�̔ԍ�
	int nParent;				//�e�p�[�c�̔ԍ�
	bool bUse;					//�g�p���Ă��邩�ǂ���
}PlayerParts;

//���[�V�������̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_KEY];		//���[�V�����Đ����̈ʒu
	D3DXVECTOR3 rot[MAX_KEY];		//���[�V�����Đ����̌���
}KeySet;

//���[�V�������̍\����
typedef struct
{
	int nLoop;
	int nNumKey;
	int nFrame[MAX_FRAME];
	KeySet aKey[MAX_KEY];
	bool bUse;
}MotionSet;

//�v���C���[�̕���
typedef enum
{
	PLAYERWEAPON_GUN = 0,		//�e
	PLAYERWEAPON_SWORD,			//��
	PLAYERWEAPON_MAX
}PLAYERWEAPON;

//�v���C���[�̌��U�����̃R���{���
typedef enum
{
	PLAYERATTACK_1COMBO = 0,	//�ꌂ��
	PLAYERATTACK_2COMBO,		//�񌂖�
	PLAYERATTACK_3COMBO,		//�O����
	PLAYERATTACK_LOOP,			//�J��Ԃ�
	PLAYERATTACK_MAX
}PLAYERATTACK;

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 rot;			//��]
	D3DXVECTOR3	rotDest;		//�ړI�̊p�x
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 moveKnockBack;	//��_�����̃m�b�N�o�b�N�ړ���
	PLAYERWEAPON Weapon;		//����
	PLAYERATTACK AttackCombo;	//���U�����̃R���{���
	PlayerParts aParts[MAX_PARTS];		//�p�[�c�̏��
	MotionSet aMotion[MAX_MOTION];		//�e���[�V�����̏��
	int nIdxShadow;				//�e�̔ԍ�
	int nNumParts;				//�p�[�c��
	int nCntShot;				//�ˌ��Ԋu�̃J�E���^�[
	int nCntAttack;				//����؂�ւ��J�E���^�[
	int nCntDamage;				//��_����Ԃ̐؂�ւ��J�E���^�[
	int nCntDie;				//���S��̋����p�J�E���^�[
	float fMove;				//�ړ���(�s��)
	float fMoveDash;			//�_�b�V�����̒ǉ��ړ���
	float fJump;				//�W�����v��
	bool bIsJumping;			//�W�����v�����ǂ���
	bool bIsAttacking;			//�v���C���[���U�����̃t���O(���̃��[�V������Đ��h�~�p)
	bool bIsAttackingSword;		//�v���C���[���ߐڍU�����̃t���O(�U�����̃L�[���͖����p)
	bool bIsDashing;			//�_�b�V�������ǂ���(�ړ��ʕω��p)
	bool bIsDamage;				//�_���[�W���󂯂Ă��邩�ǂ���
	bool bIsDie;				//���S�������ǂ���
	bool bIsVictory;			//�Q�[���ɏ����������ǂ���
	bool bUse;					//�g�p���Ă��邩�ǂ���
}Player;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
//bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath);
void SetPlayer(Player Player);
void MotionPlayer(void);

#endif