#ifndef _FLAG_H_		//���̃}�N����`������ĂȂ�������
#define _FLAG_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define FLAG_SIZE_X		(50.0f)		//�Q�[�����̃��S��X�T�C�Y
#define FLAG_SIZE_Y		(50.0f)		//�Q�[�����̃��S��Y�T�C�Y
#define MAX_FLAG		(2)			//�Q�[�����̃��S�e�N�X�`����

//���Ԓn�_�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}FLAG;

// �v���g�^�C�v�錾
void InitGameFlag(void);
void UninitGameFlag(void);
void UpdateGameFlag(void);
void DrawGameFlag(void);
void SetGameFlag(D3DXVECTOR3 pos, int nType);
void CollisionFlag(D3DXVECTOR3 *pPos, float Width, float Height);
int FlagStage(void);
FLAG *GetFlag(void);

#endif