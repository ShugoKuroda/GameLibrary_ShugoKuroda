//========================================================
//
//					���_����Set�J�E���g����
//					AUTHOR:���c����
//
//========================================================
#ifndef _SET_H_
#define _SET_H_

#include "main.h"

//���_���̓��_�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
} Set;

//�v���g�^�C�v�錾
void InitSet(void);
void UninitSet(void);
void UpdateSet(void);
void DrawSet(void);
void SetCounter(int nIdxPlayer);

#endif
