//===================================================================
//
//		�J���������̃w�b�_�[[camera.h]
//		Author:���c ����
//
//===================================================================
#ifndef _CAMERA_H_			//���̃}�N����`������Ȃ�������
#define _CAMERA_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�}�N����`
//======================================================
#define CAMERA_POS_MOVE		(1.7f)		//���_�̈ړ���
#define CAMERA_ROT_MOVE		(0.03f)		//��]�̈ړ���
#define MAX_CAMERA			(2)			//�J�����̍ő吔

//======================================================
//	�J�����ԍ��̗񋓌^�錾
//======================================================
typedef enum
{
	CAMERA_1 = 0,		//1P�̃J����(����)
	CAMERA_2,			//2P�̃J����(�E��)
	CAMERA_MAX
}CAMERANUM;

//======================================================
//	�J�����̍\����
//======================================================
typedef struct
{
	D3DXVECTOR3 posV;		//���_
	D3DXVECTOR3 posR;		//�����_
	D3DXVECTOR3 vecU;		//������x�N�g��
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 posVDest;	//�ړI�̎��_
	D3DXVECTOR3 posRDest;	//�ړI�̒����_
	float fDistance;		//���_���璍���_�܂ł̋���
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X
	D3DVIEWPORT9 viewport;		//�r���[�|�[�g
}Camera;

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nCntCamera);
Camera *GetCamera(void);

#endif