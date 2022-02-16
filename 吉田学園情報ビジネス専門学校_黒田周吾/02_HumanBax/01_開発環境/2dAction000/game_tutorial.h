#ifndef _GAME_TUTORIAL_H_		//���̃}�N����`������ĂȂ�������
#define _GAME_TUTORIAL_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define TUTORIAL_SIZE_X		(200.0f)		//�Q�[�����̃��S��X�T�C�Y
#define TUTORIAL_SIZE_Y		(50.0f)			//�Q�[�����̃��S��Y�T�C�Y
#define MAX_GAME_TUTORIAL		(3)				//�Q�[�����̃��S�e�N�X�`����

//�Q�[�����̃`���[�g���A���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}GAME_TUTORIAL;

// �v���g�^�C�v�錾
void InitGameTutorial(void);
void UninitGameTutorial(void);
void UpdateGameTutorial(void);
void DrawGameTutorial(void);
void SetGameTutorial(D3DXVECTOR3 pos, int nType);

#endif