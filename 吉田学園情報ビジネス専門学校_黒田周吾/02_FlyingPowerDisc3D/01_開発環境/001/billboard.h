//========================================================
//
//						�r���{�[�h
//						AUTHOR:�z�{���V
//
//========================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//�e�N�X�`���^�C�v�̍\����
typedef enum
{
	TEXTURETYPE_1 = 0,
	TEXTURETYPE_2,
	TEXTURETYPE_3,
	TEXTURETYPE_4,
	TEXTURETYPE_5,
	TEXTURETYPE_MAX
}BILLBOARDTEXTURE;

//�v���g�^�C�v�錾
void InitBillboard(void);										//�r���{�[�h����������
void UninitBillboard(void);										//�r���{�[�h�I������
void UpdateBillboard(void);										//�r���{�[�h�X�V����
void DrawBillboard(void);										//�r���{�[�h�`�揈��
void SetBillboard(D3DXVECTOR3 pos, float fSize,int nCntTime, char *pFilename);	//�r���{�[�h�̐ݒ菈��(�ʒu�A�T�C�Y�A�摜�t�@�C���̖��O�A�e�N�X�`���̎��)

#endif 
