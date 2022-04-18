#ifndef _SWITCH_H_			//���̃}�N����`������Ȃ�������
#define _SWITCH_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define MAX_SWITCH_TEX	(1)			//�^�C�g����ʂŕK�v�ȉ摜��
#define MAX_SWITCH		(16)		//�X�C�b�`�̍ő吔

//�X�C�b�`�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	bool bUse;				//�g�p���Ă��邩�ǂ���
	int nPushCnt;			//�X�C�b�`�̉�������
	float fWidth;			//��
	float fHeight;			//����
}Switch;

// �v���g�^�C�v�錾
void InitSwitch(void);
void UninitSwitch(void);
void UpdateSwitch(void);
void DrawSwitch(void);
void SetSwitch(D3DXVECTOR3 pos, float Width, float Height);
void CollisionSwitch(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
Switch *GetSwitch(void);

#endif