//=============================================================================
//
// ���[�h���� [load.cpp]
// Author : SHUGO KURODA
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
#include "enemy.h"

//======================================================
// �}�N����`
//======================================================
#define LINE_MAX_READING_LENGTH (256)			//��s�̍ő�ǂݎ�蕶����

//-----------------------------------------------------------------------------
//���f���̃Z�b�g���[�h
//-----------------------------------------------------------------------------
HRESULT LoadSetModel(HWND hWnd)
{
	FILE *pFile = NULL;			//�t�@�C���|�C���^�[�錾

	char cBff[LINE_MAX_READING_LENGTH];			//��s���ǂݎ��ϐ�
	char cBffHead[LINE_MAX_READING_LENGTH];		//���̕�����ǂݎ��ϐ�
	
	//�t�@�C�����J��
	pFile = fopen("model.txt", "r");

	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ������ꍇ
		MessageBox(hWnd, "���f���e�L�X�g���J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}
	
	//������̓ǂݎ�胋�[�v����
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//������̕���
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//X�t�@�C���̑��΃p�X�p

			char sPath[LINE_MAX_READING_LENGTH];		//���΃p�X�ۑ��p

			//��s�̕����񂩂瑊�΃p�X�̓ǂݎ��
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//X�t�@�C���̓ǂݍ���
			LoadModelType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "MODELSET") == 0)
		{//���f���̔z�u�p
			Model Model;				//���f���\���̏��̊i�[�ϐ�
			float fShadowX, fShadowZ;	//�e�̑傫�����̊i�[�ϐ�
			int nCheck;					//�e���g�p���邩�ǂ���

			//���f���z�u�ɕK�v�ȏ��ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "TYPE") == 0)
				{//���f���̎�ޗp
					sscanf(cBff, "%s = %d", &cBffHead, &Model.nType);
				}
				else if (strcmp(&cBffHead[0], "POS") == 0)
				{//POS�p
					sscanf(cBff, "%s = %f%f%f", &cBffHead, &Model.pos.x, &Model.pos.y, &Model.pos.z);
				}
				else if (strcmp(&cBffHead[0], "ROT") == 0)
				{//ROT�p
					sscanf(cBff, "%s = %f%f%f", &cBffHead, &Model.rot.x, &Model.rot.y, &Model.rot.z);
				}
				else if (strcmp(&cBffHead[0], "SHADOW") == 0)
				{//�e�̃Z�b�g
					sscanf(cBff, "%s = %d%f%f", &cBffHead, &nCheck, &fShadowX, &fShadowZ);
				}
				else if (strcmp(&cBffHead[0], "END_MODELSET") == 0)
				{//���f���̃Z�b�g�ɕK�v�ȏ���ǂݐ؂�����
					break;
				}
			}

			if (nCheck >= 1)
			{//nCheck��1�ȏ�̏ꍇ�A�e��ݒ肷��
				Model.nIdxShadow = SetShadow(D3DXVECTOR3(Model.pos.x, 0.1f, Model.pos.z), Model.rot, fShadowX, fShadowZ);
			}

			//���f���z�u
			SetModel(hWnd,Model);
		}
		else if (strcmp(&cBffHead[0], "TEXT_END") == 0)
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
	FILE *pFile = NULL;			//�t�@�C���|�C���^�[�錾

	char cBff[LINE_MAX_READING_LENGTH];		//��s���ǂݎ�邽�߂̕ϐ�
	char cBffHead[LINE_MAX_READING_LENGTH];	//���̕�����ǂݎ�邽�߂̕ϐ�
	Player *pPlayer = GetPlayer();			//�v���C���[���̎擾
	int nMotionIdx = 0;			//���[�V�����ԍ�

	//�t�@�C�����J��
	pFile = fopen("motion.txt", "r");

	if (pFile == NULL)
	{//�J���Ȃ��������p
		MessageBox(hWnd, "���[�V�����e�L�X�g���J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//�����ǂݍ��ݗp�ϐ��̏�����
		memset(&cBffHead, 0, sizeof(cBffHead));
		//������̕���
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//X�t�@�C���̑��΃p�X�p

			char sPath[LINE_MAX_READING_LENGTH];		//���΃p�X�ۑ��p

			//��s�̕����񂩂瑊�΃p�X�̓ǂݎ��
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//X�t�@�C���̓ǂݍ���
			LoadPlayerType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//�v���C���[�̔z�u�p

			//�v���C���[���̓ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//�p�[�c��
					//������̕���
					sscanf(cBff, "%s = %d", &cBffHead,&pPlayer->nNumParts);
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//�ړ���
					//������̕���
					sscanf(cBff, "%s = %f", &cBffHead, &pPlayer->fMove);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//�W�����v��
					//������̕���
					sscanf(cBff, "%s = %f", &cBffHead, &pPlayer->fJump);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{
					//�C���f�b�N�X�ۑ��p
					int nIndex = 0;

					//�v���C���[�p�[�c���̓ǂݎ�胋�[�v����
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//������̕���
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//�p�[�c�ԍ�
							//������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &nIndex);
							pPlayer->aParts[nIndex].nIndex = nIndex;
							pPlayer->aParts[nIndex].bUse = true;
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//���݂̃p�[�c�̐e
							//������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aParts[nIndex].nParent);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//�ʒu
							//������̕���
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pPlayer->aParts[nIndex].pos.x, &pPlayer->aParts[nIndex].pos.y, &pPlayer->aParts[nIndex].pos.z);
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//��](�p�x)
							//������̕���
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pPlayer->aParts[nIndex].rot.x, &pPlayer->aParts[nIndex].rot.y, &pPlayer->aParts[nIndex].rot.z);
							pPlayer->aParts[nIndex].baseRot = pPlayer->aParts[nIndex].rot;
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//�p�[�c�ǂݍ��ݏI��
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//�v���C���[�ǂݍ��ݏI��
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//���[�V�����ݒ�p

			//�L�[�Z�b�g�ԍ��̕ۑ��p�ϐ�
			int nKeySetIdx = 0;
			 //���[�V�������̓ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//���[�v�ݒ�
					//������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aMotion[nMotionIdx].nLoop);
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//�L�[�̐�
					//������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aMotion[nMotionIdx].nNumKey);
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{
					//���f���p�[�c�̃Z�b�g�ԍ�
					int nPartsIdx = 0;

					//�L�[�Z�b�g���̓ǂݎ�胋�[�v����
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//������̕���
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//���[�v�ݒ�
							//������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &pPlayer->aMotion[nMotionIdx].nFrame[nKeySetIdx]);
							if (pPlayer->aMotion[nMotionIdx].nFrame[nKeySetIdx] == 0)
							{//�Đ��t���[������0�̏ꍇ1�ɂ���
								pPlayer->aMotion[nMotionIdx].nFrame[nKeySetIdx] = 1;
							}
						}
						if (strcmp(&cBffHead[0], "KEY") == 0)
						{//���[�v�ݒ�
							//�L�[���̓ǂݎ�胋�[�v����
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//������̕���
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//���[�V�����Đ����̈ʒu���ǂݍ���
									//������̕���
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].x,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].y,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//���[�V�����Đ����̉�]���ǂݍ���
									//������̕���
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].x,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].y,
										&pPlayer->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "END_KEY") == 0)
								{
									nPartsIdx++;
									break;
								}
							}
						}
						else if (strcmp(&cBffHead[0], "END_KEYSET") == 0)
						{
							nKeySetIdx++;
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{
					nMotionIdx++;
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//�e�L�X�g�t�@�C����ǂݐ؂�����
			break;
		}
	}

	//�v���C���[���g�p���Ă����Ԃɂ���
	pPlayer->bUse = true;

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}

//-----------------------------------------------------------------------------
//�v���C���[�̃Z�b�g���[�h
//-----------------------------------------------------------------------------
HRESULT LoadSetEnemy(HWND hWnd)
{
	FILE *pFile = NULL;			//�t�@�C���|�C���^�[�錾

	char cBff[LINE_MAX_READING_LENGTH];			//��s���ǂݎ�邽�߂̕ϐ�
	char cBffHead[LINE_MAX_READING_LENGTH];		//���̕�����ǂݎ�邽�߂̕ϐ�
	EnemyInfo *pEnemy = GetEnemyInfo();			//�G���̎擾
	int nMotionIdx = 0;							//���[�V�����ԍ�

	//�t�@�C�����J��
	pFile = fopen("motion_wolf.txt", "r");

	if (pFile == NULL)
	{//�J���Ȃ��������p
		MessageBox(hWnd, "���[�V�����e�L�X�g���J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//������̕���
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//X�t�@�C���̑��΃p�X�p

			char sPath[LINE_MAX_READING_LENGTH];		//���΃p�X�ۑ��p

			//��s�̕����񂩂瑊�΃p�X�̓ǂݎ��
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//X�t�@�C���̓ǂݍ���
			LoadEnemyType(hWnd, &sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//�v���C���[�̔z�u�p

			//�v���C���[���̓ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//�p�[�c��
				 //������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->nNumParts);
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//�ړ���
				 //������̕���
					sscanf(cBff, "%s = %f", &cBffHead, &pEnemy->fMove);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//�W�����v��
				 //������̕���
					sscanf(cBff, "%s = %f", &cBffHead, &pEnemy->fJump);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{
					//�C���f�b�N�X�ۑ��p
					int nIndex = 0;

					//�v���C���[�p�[�c���̓ǂݎ�胋�[�v����
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//������̕���
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//�p�[�c�ԍ�
						 //������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &nIndex);
							pEnemy->aParts[nIndex].nIndex = nIndex;
							pEnemy->aParts[nIndex].bUse = true;
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//���݂̃p�[�c�̐e
						 //������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aParts[nIndex].nParent);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//�ʒu
						 //������̕���
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pEnemy->aParts[nIndex].pos.x, &pEnemy->aParts[nIndex].pos.y, &pEnemy->aParts[nIndex].pos.z);
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//��](�p�x)
						 //������̕���
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &pEnemy->aParts[nIndex].rot.x, &pEnemy->aParts[nIndex].rot.y, &pEnemy->aParts[nIndex].rot.z);
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//�p�[�c�ǂݍ��ݏI��
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//�v���C���[�ǂݍ��ݏI��
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//���[�V�����ݒ�p

		 //�L�[�Z�b�g�ԍ��̕ۑ��p�ϐ�
			int nKeySetIdx = 0;
			//���[�V�������̓ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//���[�v�ݒ�
				 //������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aMotion[nMotionIdx].nLoop);
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//�L�[�̐�
				 //������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aMotion[nMotionIdx].nNumKey);
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{
					//���f���p�[�c�̃Z�b�g�ԍ�
					int nPartsIdx = 0;

					//�L�[�Z�b�g���̓ǂݎ�胋�[�v����
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//������̕���
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//���[�v�ݒ�
						 //������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &pEnemy->aMotion[nMotionIdx].nFrame[nKeySetIdx]);
							if (pEnemy->aMotion[nMotionIdx].nFrame[nKeySetIdx] == 0)
							{//�Đ��t���[������0�̏ꍇ1�ɂ���
								pEnemy->aMotion[nMotionIdx].nFrame[nKeySetIdx] = 1;
							}
						}
						if (strcmp(&cBffHead[0], "KEY") == 0)
						{//���[�v�ݒ�
						 //�L�[���̓ǂݎ�胋�[�v����
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//������̕���
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//���[�V�����Đ����̈ʒu���ǂݍ���
								 //������̕���
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].x,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].y,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].pos[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//���[�V�����Đ����̉�]���ǂݍ���
									//������̕���
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].x,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].y,
										&pEnemy->aMotion[nMotionIdx].aKey[nKeySetIdx].rot[nPartsIdx].z);
								}
								else if (strcmp(&cBffHead[0], "END_KEY") == 0)
								{
									nPartsIdx++;
									break;
								}
							}
						}
						else if (strcmp(&cBffHead[0], "END_KEYSET") == 0)
						{
							nKeySetIdx++;
							break;
						}
					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{
					nMotionIdx++;
					break;
				}
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//�e�L�X�g�t�@�C����ǂݐ؂�����
			break;
		}
	}

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}