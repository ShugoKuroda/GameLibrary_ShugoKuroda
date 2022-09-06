#ifndef _PLAYER_H_					//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_					//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define PLAYER_SIZE_X		(60.0f)		//�v���C���[�̃T�C�Y(X)
#define PLAYER_SIZE_LOOK_X	(15.0f)		//�v���C���[�̌�����X�T�C�Y(�]�����𖳎����������̑傫��)
#define PLAYER_SIZE_Y		(80.0f)		//�v���C���[�̃T�C�Y(Y)
#define PLAYER_MOVE			(0.7f)		//�v���C���[�̈ړ���
#define PLAYER_MOVEDOWN		(1.0f)		//�v���C���[�ւ̏d��
#define PLAYER_MOVEUP		(-23.0f)	//�v���C���[�̃W�����v��

//�v���C���[�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�O��̈ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 BlockMove;			//����Ă���u���b�N�̈ړ��ʏ��
	bool bUse;						//�g�p���Ă��邩�ǂ���
	int nDirectionMove;				//�v���C���[�̌���(0:�E����,1:������)
	bool bIsJumping;				//�W�����v�����ǂ���
	int nCounterAnim;				//�A�j���[�V�����J�E���^�[
	int nPatternAnim;				//�A�j���[ �V�����̃p�^�[��No.
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
void GameScroll(int nType);
bool DiePlayer();

#endif