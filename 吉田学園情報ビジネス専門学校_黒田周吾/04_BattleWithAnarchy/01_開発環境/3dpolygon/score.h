//=============================================================================
//
//	�X�R�A�����̃w�b�_�[[score.h]
//	Author : ���c ����
//
//=============================================================================
#ifndef _SCORE_H_		//���̃}�N����`������Ȃ�������
#define _SCORE_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//======================================================
//	�v���g�^�C�v�錾
//======================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nType);
int GetScore(void);

#endif