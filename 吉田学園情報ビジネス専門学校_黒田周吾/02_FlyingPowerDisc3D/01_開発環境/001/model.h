//===================================================================
//
//		���f�������̃w�b�_�[[model.h]
//		Author:���c ����
//
//===================================================================
#ifndef _MODEL_H_			//���̃}�N����`������Ȃ�������
#define _MODEL_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	���f���̍\���̒�`
//======================================================
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�O��̈ʒu
	D3DXVECTOR3 rot;				//��]
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 vtxMax,vtxMin;		//���f���̍ő�l,�ŏ��l
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	int nIdxShadow;					//�e�̔ԍ�
	int nType;						//���
	int nPlayerGoal;				//�v���C���[�̃S�[���ԍ�
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Model;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(int nCntCamera);
HRESULT SetModel(HWND hWnd, Model Model);
HRESULT LoadModelType(HWND hWnd, char *sXFilePath);
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Width, float Height);
Model *GetModel(void);

#endif