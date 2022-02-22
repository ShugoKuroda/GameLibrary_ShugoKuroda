#ifndef _EXPLOSION_H_		 //���̃}�N����`������Ȃ�������
#define _EXPLOSION_H_		 //2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define MAX_EXPLOSION		(128)		//�����̍ő吔
#define EXPLOSION_SIZE		(100.0f)	//�����̃T�C�Y

//�G�l�~�[�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 posExplosion;				//�ʒu
	float nSizeExplosion;					//�T�C�Y
	bool bUse;								//�g�p���Ă邩�ǂ���
	int nCounterAnimExplosion;				//�A�j���[�V�����J�E���^�[
	int nPatternAnimExplosion;				//�A�j���[�V�����p�^�[��No.
}Explosion;

//�v���g�^�C�v�錾
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, float Size);

#endif

