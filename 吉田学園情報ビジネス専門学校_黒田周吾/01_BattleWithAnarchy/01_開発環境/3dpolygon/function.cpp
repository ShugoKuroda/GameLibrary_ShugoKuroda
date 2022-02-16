//===================================================================
//
//	���ʊ֐�[function.cpp]
//	Author:���c ����
//
//===================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "main.h"
#include "function.h"

//======================================================
//	���̂̓����蔻��
//======================================================
bool SphereCollision(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, int StartLength, int EndLength)
{
	//�Ԃ�l�̏�����
	bool bSphereCollision = false;

	//2�̍��W���������߂�
	D3DXVECTOR3 Length = posStart - posEnd;

	//���W�������狗�������߂�
	float fLength = D3DXVec3Length(&Length);

	if (StartLength + EndLength >= fLength)
	{//���݂��̋��̃T�C�Y�̍��v��苗����������������
		bSphereCollision = true;		//�Ԃ�l��^�ɂ���
	}

	return bSphereCollision;
}