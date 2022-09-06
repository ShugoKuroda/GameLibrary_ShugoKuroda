#ifndef _TIME_H_		//���̃}�N����`������Ȃ�������
#define _TIME_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define MAX_TIME		(8)			//�ő�X�R�A��
#define TIME_SIZE_X	(20.0f)		//�X�R�A�\�L�̃T�C�Y(x)
#define TIME_SIZE_Y	(40.0f)		//�X�R�A�\�L�̃T�C�Y(y)

//�^�C�}�[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int nData;				//�l
}Time;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nScore);
void AddTime(int nValue);
int GetTime(void);

#endif