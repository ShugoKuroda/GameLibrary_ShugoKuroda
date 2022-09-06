//=============================================================================
//
//	���b�V��(��)���� [cylinder.h]
//	Author : SHUGO KURODA
//
//=============================================================================
#ifndef _CYLINDER_H_			//���̃}�N����`������Ȃ�������
#define _CYLINDER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//	�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
//	�񋓌^�̒�`
//-----------------------------------------------------------------------------
//�e�N�X�`���񋓌^
typedef enum
{
	CYLINDER_TEXTURE_0 = 0,		//�e�N�X�`�����Ȃ����
	CYLINDER_TEXTURE_1,			//���e�N�X�`��
	CYLINDER_TEXTURE_MAX		//�e�N�X�`���̍ő吔
}TexturetCylinder;

//-----------------------------------------------------------------------------
//	�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void InitMeshCylinder(void);		//������
void UninitMeshCylinder(void);		//�I������
void UpdateMeshCylinder(void);		//�X�V����
void DrawMeshCylinder(void);		//�`�揈��
void SetCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, int nMeshX, int nMeshZ, TexturetCylinder Texturetstate);

#endif
