#ifndef _BLOCK_H_
#define _BLOCK_H_

//�}�N����`
#define MAX_BLOCK			(128)		//�u���b�N�̍ő吔
#define MAX_BLOCK_TEX		(7)			//�u���b�N�̍ő�e�N�X�`����

//�u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�O��̈ʒu
	D3DXVECTOR3	move;				//�ړ���
	D3DXVECTOR3 Airmove;			//��������̈ړ���
	D3DXVECTOR3 BlockMove;			//���X�N���[���̈ړ���
	int nMoveCnt;					//�ړ��J�E���^�[
	int nMoveCntMax;				//�ړ��J�E���^�[(���)
	int nTypeSwitch;				//�X�C�b�`�ƘA������M�~�b�N�ԍ�
	int nMoveCntSwitch;				//�Q�[�g���J������܂ł̃J�E���^�[
	int nType;						//���
	float fWidth;					//��
	float fHeight;					//����
	bool bUse;						//�g�p���Ă��邩�ǂ���
	float fTexX;					//�e�N�X�`�����W(X)
	float fTexY;					//�e�N�X�`�����W(Y)
}Block;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nMoveCntMax, int nType, int nTypeSwitch, float width, float height, float TexSizeX, float TexSizeY);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pMovePlayer, float Width, float Height, int nType);
void CollisionMoveBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
int OpenGate(int nOpenGate);
Block*GetBlock(void);

#endif