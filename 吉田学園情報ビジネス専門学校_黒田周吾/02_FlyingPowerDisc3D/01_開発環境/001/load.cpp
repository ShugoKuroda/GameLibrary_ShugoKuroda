//=============================================================================
//
//	���[�h���� [load.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================

//======================================================
// �C���N���[�h
//======================================================
#include "main.h"
#include <stdio.h>
#include "player.h"
#include "model.h"
#include "load.h"
#include "shadow.h"

//======================================================
// �}�N����`
//======================================================
#define MAX_CHAR (256)			//��s�̍ő�ǂݎ�蕶����

//-----------------------------------------------------------------------------
//���f���̃Z�b�g���[�h
//-----------------------------------------------------------------------------
HRESULT LoadSetModel(HWND hWnd)
{
	FILE *pFile = NULL;				//�t�@�C���|�C���^�[�錾
	char cScanf[MAX_CHAR];			//��s���ǂݎ��ϐ�
	char cScanfHead[MAX_CHAR];		//���̕�����ǂݎ��ϐ�

	//�t�@�C�����J��
	pFile = fopen("model.txt", "r");

	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ������ꍇ
		MessageBox(hWnd, "���f���e�L�X�g���J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
	{
		//������̕���
		sscanf(cScanf, "%s", &cScanfHead);

		if (strcmp(&cScanfHead[0], "MODEL_FILENAME") == 0)
		{//X�t�@�C���̑��΃p�X�p

			char sPath[MAX_CHAR];		//���΃p�X�ۑ��p

			//��s�̕����񂩂瑊�΃p�X�̓ǂݎ��
			sscanf(cScanf, "%s = %s", &cScanfHead, &sPath[0]);

			//X�t�@�C���̓ǂݍ���
			LoadModelType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cScanfHead[0], "MODELSET") == 0)
		{//���f���̔z�u�p
			Model model;				//���f���\���̏��̊i�[�ϐ�
			memset(&model, 0, (sizeof(Model)));		//������
			float fShadowX, fShadowZ;	//�e�̑傫�����̊i�[�ϐ�
			int nCheck;					//�e���g�p���邩�ǂ���

			//���f���z�u�ɕK�v�ȏ��ǂݎ�胋�[�v����
			while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
			{
				//������̕���
				sscanf(cScanf, "%s", &cScanfHead);

				if (strcmp(&cScanfHead[0], "TYPE") == 0)
				{//���f���̎�ޗp
					sscanf(cScanf, "%s = %d", &cScanfHead, &model.nType);
				}
				else if (strcmp(&cScanfHead[0], "PLAYER") == 0)
				{//�v���C���[�̃S�[���ԍ��p(0�̓S�[������Ȃ�)
					sscanf(cScanf, "%s = %d", &cScanfHead, &model.nPlayerGoal);
				}
				else if (strcmp(&cScanfHead[0], "POS") == 0)
				{//POS�p
					sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &model.pos.x, &model.pos.y, &model.pos.z);
				}
				else if (strcmp(&cScanfHead[0], "ROT") == 0)
				{//ROT�p
					sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &model.rot.x, &model.rot.y, &model.rot.z);
				}
				else if (strcmp(&cScanfHead[0], "SHADOW") == 0)
				{//�e�̃Z�b�g
					sscanf(cScanf, "%s = %d%f%f", &cScanfHead, &nCheck, &fShadowX, &fShadowZ);
				}
				else if (strcmp(&cScanfHead[0], "END_MODELSET") == 0)
				{//���f���̃Z�b�g�ɕK�v�ȏ���ǂݐ؂�����
					break;
				}
			}

			if (nCheck >= 1)
			{//nCheck��1�ȏ�̏ꍇ�A�e��ݒ肷��
				model.nIdxShadow = SetShadow(D3DXVECTOR3(model.pos.x, 0.1f, model.pos.z), model.rot, fShadowX, fShadowZ);
			}

			//���f���z�u
			SetModel(hWnd, model);
		}
		else if (strcmp(&cScanfHead[0], "TEXT_END") == 0)
		{//�e�L�X�g�t�@�C����ǂݐ؂�����
			break;
		}
	}

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}

//-----------------------------------------------------------------------------
//�v���C���[�̃Z�b�g���[�h
//-----------------------------------------------------------------------------
HRESULT LoadSetPlayer(HWND hWnd)
{
	FILE *pFile = NULL;					//�t�@�C���|�C���^�[�錾

	Player aPlayer;				//�v���C���[���̕ۑ��p
	char cScanf[MAX_CHAR];		//��s���ǂݎ�邽�߂̕ϐ�
	char cScanfHead[MAX_CHAR];	//���̕�����ǂݎ�邽�߂̕ϐ�
	int nMotionIdx = 0;			//���[�V�����ԍ�

	//�v���C���[���̏�����
	ZeroMemory(&aPlayer, sizeof(Player));

	//�t�@�C�����J��
	pFile = fopen("motion.txt", "r");

	if (pFile == NULL)
	{//�J���Ȃ��������p
		MessageBox(hWnd, "���[�V�����e�L�X�g���J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
	{
		//�����ǂݍ��ݗp�ϐ��̏�����
		memset(&cScanfHead, 0, sizeof(cScanfHead));
		//������̕���
		sscanf(cScanf, "%s", &cScanfHead);

		if (strcmp(&cScanfHead[0], "MODEL_FILENAME") == 0)
		{//X�t�@�C���̑��΃p�X�p

			char sPath[MAX_CHAR];		//���΃p�X�ۑ��p

			//��s�̕����񂩂瑊�΃p�X�̓ǂݎ��
			sscanf(cScanf, "%s = %s", &cScanfHead, &sPath[0]);

			//X�t�@�C���̓ǂݍ���
			LoadPlayerType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cScanfHead[0], "CHARACTERSET") == 0)
		{//�v���C���[�̔z�u�p

			//�v���C���[���̓ǂݎ�胋�[�v����
			while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
			{
				//������̕���
				sscanf(cScanf, "%s", &cScanfHead);

				if (strcmp(&cScanfHead[0], "NUM_PARTS") == 0)
				{//�p�[�c��
				 //������̕���
					sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.nNumParts);
				}
				else if (strcmp(&cScanfHead[0], "MOVE") == 0)
				{//�ړ���
				 //������̕���
					sscanf(cScanf, "%s = %f", &cScanfHead, &aPlayer.fMove);
				}
				else if (strcmp(&cScanfHead[0], "JUMP") == 0)
				{//�W�����v��
				 //������̕���
					sscanf(cScanf, "%s = %f", &cScanfHead, &aPlayer.fJump);
				}
				else if (strcmp(&cScanfHead[0], "PARTSSET") == 0)
				{
					//�C���f�b�N�X�ۑ��p
					int nIndex = 0;

					//�v���C���[�p�[�c���̓ǂݎ�胋�[�v����
					while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
					{
						//������̕���
						sscanf(cScanf, "%s", &cScanfHead);

						if (strcmp(&cScanfHead[0], "INDEX") == 0)
						{//�p�[�c�ԍ�
						 //������̕���
							sscanf(cScanf, "%s = %d", &cScanfHead, &nIndex);
							aPlayer.aParts[nIndex].nIndex = nIndex;
							aPlayer.aParts[nIndex].bUse = true;
						}
						else if (strcmp(&cScanfHead[0], "PARENT") == 0)
						{//���݂̃p�[�c�̐e
						 //������̕���
							sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aParts[nIndex].nParent);
						}
						else if (strcmp(&cScanfHead[0], "POS") == 0)
						{//�ʒu
						 //������̕���
							sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &aPlayer.aParts[nIndex].pos.x, &aPlayer.aParts[nIndex].pos.y, &aPlayer.aParts[nIndex].pos.z);
						}
						else if (strcmp(&cScanfHead[0], "ROT") == 0)
						{//��](�p�x)
						 //������̕���
							sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &aPlayer.aParts[nIndex].rot.x, &aPlayer.aParts[nIndex].rot.y, &aPlayer.aParts[nIndex].rot.z);
							aPlayer.aParts[nIndex].baseRot = aPlayer.aParts[nIndex].rot;
						}
						else if (strcmp(&cScanfHead[0], "END_PARTSSET") == 0)
						{//�p�[�c�ǂݍ��ݏI��
							break;
						}
					}
				}
				else if (strcmp(&cScanfHead[0], "END_CHARACTERSET") == 0)
				{//�v���C���[�ǂݍ��ݏI��
					break;
				}
			}
		}
		else if (strcmp(&cScanfHead[0], "MOTIONSET") == 0)
		{//���[�V�����ݒ�p

			//�L�[�Z�b�g�ԍ��̕ۑ��p�ϐ�
			int nKeySetIdx = 0;
			//���[�V�������̓ǂݎ�胋�[�v����
			while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
			{
				//������̕���
				sscanf(cScanf, "%s", &cScanfHead);

				if (strcmp(&cScanfHead[0], "LOOP") == 0)
				{//���[�v�ݒ�
				 //������̕���
					sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aMotion[nMotionIdx].nLoop);
				}
				else if (strcmp(&cScanfHead[0], "NUM_KEY") == 0)
				{//�L�[�̐�
				 //������̕���
					sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aMotion[nMotionIdx].nNumKey);
				}
				else if (strcmp(&cScanfHead[0], "KEYSET") == 0)
				{
					//���f���p�[�c�̃Z�b�g�ԍ�
					int nPartsIdx = 0;

					//�L�[�Z�b�g���̓ǂݎ�胋�[�v����
					while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
					{
						//������̕���
						sscanf(cScanf, "%s", &cScanfHead);

						if (strcmp(&cScanfHead[0], "FRAME") == 0)
						{//���[�v�ݒ�
							//������̕���
							sscanf(cScanf, "%s = %d", &cScanfHead, &aPlayer.aMotion[nMotionIdx].nFrame[nKeySetIdx]);
							if (aPlayer.aMotion[nMotionIdx].nFrame[nKeySetIdx] == 0)
							{//�Đ��t���[������0�̏ꍇ1�ɂ���
								aPlayer.aMotion[nMotionIdx].nFrame[nKeySetIdx] = 1;
							}
						}
						if (strcmp(&cScanfHead[0], "KEY") == 0)
						{//���[�v�ݒ�
						 //�L�[���̓ǂݎ�胋�[�v����
							while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
							{
								//������̕���
								sscanf(cScanf, "%s", &cScanfHead);

								if (strcmp(&cScanfHead[0], "POS") == 0)
								{//���[�V�����Đ����̈ʒu���ǂݍ���
									//������̕���
									sscanf(cScanf, "%s = %f%f%f", &cScanfHead,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].x,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].y,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].z);
								}
								else if (strcmp(&cScanfHead[0], "ROT") == 0)
								{//���[�V�����Đ����̉�]���ǂݍ���
									//������̕���
									sscanf(cScanf, "%s = %f%f%f", &cScanfHead,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].x,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].y,
										&aPlayer.aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].z);
								}
								else if (strcmp(&cScanfHead[0], "END_KEY") == 0)
								{
									nPartsIdx++;
									break;
								}
							}
						}
						else if (strcmp(&cScanfHead[0], "END_KEYSET") == 0)
						{
							nKeySetIdx++;
							break;
						}
					}
				}
				else if (strcmp(&cScanfHead[0], "END_MOTIONSET") == 0)
				{
					nMotionIdx++;
					break;
				}
			}
		}
		else if (strcmp(&cScanfHead[0], "END_SCRIPT") == 0)
		{//�e�L�X�g�t�@�C����ǂݐ؂�����
			break;
		}
	}

	//�v���C���[���g�p���Ă����Ԃɂ���
	aPlayer.bUse = true;

	//�v���C���[���̐ݒ�
	SetPlayer(aPlayer, D3DXVECTOR3(0.0f, 0.0f, -(FIELD_SIZE_Z - PLAYER_WIDTH)), 0);
	SetPlayer(aPlayer, D3DXVECTOR3(0.0f, 0.0f, FIELD_SIZE_Z - PLAYER_WIDTH), 1);

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}