//===================================================================
//
//		�e�̐ݒ菈���̃w�b�_�[[shadow.h]
//		Author:���c ����
//
//===================================================================
#ifndef _SHADOW_H_			//���̃}�N����`������Ȃ�������
#define _SHADOW_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�e�̍\���̒�`
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	float fRadiusX;			//X��
	float fRadiusZ;			//Z��
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Shadow;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadiusX, float fRadiusZ);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif