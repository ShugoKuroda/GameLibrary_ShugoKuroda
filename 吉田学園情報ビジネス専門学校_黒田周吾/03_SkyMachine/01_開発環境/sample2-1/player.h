#ifndef _PLAYER_H_					//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_					//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define PLAYER_SIZE_X	(40.0f)		//�v���C���[�̃T�C�Y(X)
#define PLAYER_SIZE_Y	(20.0f)		//�v���C���[�̃T�C�Y(Y)
#define PLAYER_MOVE		(10.0f)		//�v���C���[�̈ړ���

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//�ʏ�
	PLAYERSTATE_RESPAWN,		//���G(���X�|�[��)���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 posPlayer;			//�ʒu
	D3DXVECTOR3 movePlayer;			//�ړ���
	PLAYERSTATE State;				//���
	int nCounterState;				//��ԃJ�E���^�[
	int nCounterAnimPlayer;			//�A�j���[�V�����J�E���^�[
	int nPatternAnimPlayer;			//�A�j���[�V�����̃p�^�[��No.
	int nCntAttack = 0;				//�U���Ԋu�̃J�E���^�[
	int nHitCnt;					//�q�b�g�J�E���g
	bool bUse;						//�v���C���[�����邩�ǂ����̔���
	bool bcontrol;					//�v���C���[�𑀍�\���ǂ����̔���
	bool bcontrol2;					//�v���C���[�𑀍�\���ǂ����̔���2
	int nCntRespawn;				//�v���C���[�̕����J�E���^�[
	int nCntRespawn2;				//�v���C���[�̕����J�E���^�[2
	int nCntEndGame;				//�Q�[���I���J�ڂ܂ł̃J�E���^�[
	bool bEndGame;					//�Q�[���I������
	bool bInvincible;				//�v���C���[�̓_�ŏ�Ԑ؂�ւ�����
	bool bVictory;					//�v���C���[�̏�������
	bool bDie;						//���S����
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(void);
void DiePlayer(void);

#endif