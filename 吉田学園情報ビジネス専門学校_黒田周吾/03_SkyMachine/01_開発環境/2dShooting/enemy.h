#ifndef _ENEMY_H_			//���̃}�N����`������Ȃ�������
#define _ENEMY_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define MAX_ENEMY			(128)		//�G�̍ő吔
#define MAX_ENEMY_TYPE		(5)			//�G�̎�ސ�
#define ENEMY_SIZE			(50.0f)		//�G�̃T�C�Y

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//�ʏ�
	ENEMYSTATE_DAMAGE,			//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 posEnemy;		//�ʒu
	D3DXVECTOR3 moveEnemy;		//�ړ���
	ENEMYSTATE state;			//���
	float nSize;				//�T�C�Y
	int nType;					//���
	int nMoveType;				//�������̎��
	bool bMoveCnt;				//�������؂�ւ��J�E���^�[
	int nCntAttack;				//�U���Ԋu�J�E���^�[
	int nCounterState;			//��ԃJ�E���^�[
	int nLife;					//�̗�
	bool bUse;					//�g�p���Ă邩�ǂ���
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType,int nMoveType,int nLife,float fSize);		//SetEnemy(�ʒu,���,�������̃p�^�[��,���C�t,�T�C�Y)
void HitEnemy(int nCntEnemy, int nDamage);
Enemy*GetEnemy(void);

#endif
