///========================================================
//
//					�v���C���[
//					AUTHOR:�z�{���V,���c����
//
//========================================================
#ifndef _PLAYER_H_			//���̃}�N����`������Ȃ�������
#define _PLAYER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"

//======================================================
//	�}�N����`
//======================================================
#define MAX_PARTS			(20)		//�v���C���[�̃��f���p�[�c�̍ő吔
#define MAX_MOTION			(20)		//���[�V�����̍ő吔aa
#define MAX_KEY				(20)		//���[�V�����L�[�̍ő吔
#define MAX_FRAME			(120)		//���[�V�����̍ő�t���[����
#define PLAYER_WIDTH		(50.0f)		//�v���C���[�̕�
#define PLAYER_COLLISION	(10.0f)		//�v���C���[�̓����蔻��͈�
#define MAX_PLAYER			(2)			//�v���C���[�̍ő吔
#define FIELD_SIZE_X		(240.0f)	//�X�e�[�W�̃T�C�Y(X)
#define FIELD_SIZE_Z		(480.0f)	//�X�e�[�W�̃T�C�Y(Z)

//======================================================
//	�񋓌^��`
//======================================================
//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//�ʏ�̏��
	PLAYERSTATE_HAVEDISC,		//�f�B�X�N�������Ă�����
	PLAYERSTATE_THROW,			//�f�B�X�N�𓊂�����
	PLAYERSTATE_CATCH,			//�f�B�X�N���L���b�`�������
	PLAYERSTATE_WIN,			//�Q�[���ɏ����������
	PLAYERSTATE_LOSE,			//�Q�[���ɔs�k�������
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[���f�B�X�N�𓊂������
typedef enum
{
	PLAYERTHROW_STRAIGHT = 0,		//����
	PLAYERTHROW_STRAIGHT_LEFT,		//���O
	PLAYERTHROW_LEFT,				//��
	PLAYERTHROW_STRAIGHT_RIGHT,		//�E�O
	PLAYERTHROW_RIGHT,				//�E
	PLAYERTHROW_MAX
}PLAYERTHROW;

//�v���C���[
typedef enum
{
	PLAYER_1 = 0,		//1P
	PLAYER_2,			//2P
	PLAYER_MAX
} PLAYER;

//======================================================
//	�\���̒�`
//======================================================
//�v���C���[�p�[�c�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//��]
	D3DXVECTOR3 baseRot;		//��{�̊p�x
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
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

//���[�V�����ԍ����̍\����
typedef struct
{
	int nFrame;					//���݂̃t���[����
	int nKeySetIdx;				//�Đ����̃L�[�Z�b�g�ԍ�
	int nMotionIdx;				//�Đ����̃��[�V�����ԍ�
}MotionIdx;

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 rot;			//��]
	D3DXVECTOR3	rotDest;		//�ړI�̊p�x
	D3DXVECTOR3 move;			//�ړ���
	PLAYERSTATE State;			//���
	PLAYERTHROW Throw;			//���������
	PlayerParts aParts[MAX_PARTS];			//�p�[�c�̏��
	MotionIdx MotionIdx;					//���[�V�����ԍ����
	MotionSet aMotion[MAX_MOTION];			//�e���[�V�����̏��
	int nIdxShadow;				//�e�̔ԍ�
	int nNumParts;				//�p�[�c��
	int nSlidingTime;			//�X���C�f�B���O�̎�������
	int nSlidingCoolTime;		//�X���C�f�B���O�̃N�[���^�C��
	float fMove;				//�ړ���(�s��)
	float fMoveDash;			//�_�b�V�����̒ǉ��ړ���
	float fJump;				//�W�����v��
	bool bUse;					//�g�p���Ă��邩�ǂ���
	int nCntThrow;				//�����铮�����߂�܂ł̃J�E���^�[
}Player;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
HRESULT LoadPlayerType(HWND hWnd, char *sXFilePath);
void SetPlayer(Player player, D3DXVECTOR3 InitPos, int nCnt);
void MotionPlayer(int nCntPlayer);
void CollisionDisc(int nIdxPlayer);
void DiscChange(int nIdxPlayer);
void GameSet(int nWinPlayer, int nLosePlayer);
Player *GetPlayer(void);

#endif