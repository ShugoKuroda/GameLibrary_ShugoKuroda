#ifndef _RANK_H_			//���̃}�N����`������Ȃ�������
#define _RANK_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//�}�N����`
#define MAX_RANK_IMAGE	(4)		//���U���g��ʂŕK�v�ȃe�N�X�`����
#define MAX_RANK		(6)		//�\�����郉���L���O��
#define MAX_NUM			(8)		//�X�R�A�̌���

//�����L���O�X�R�A�\����
typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
}RankScore;

// �v���g�^�C�v�錾
void InitRank(void);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);
void SetRank(int nScore, int nCntScore);
void LoadScore(void);
void SaveScore(void);

#endif