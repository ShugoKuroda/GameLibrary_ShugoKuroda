#ifndef _LIFE_H_
#define _LIFE_H_

//�}�N����`
#define LIFE_SIZE_X		(50.0f)			//�v���C���[�c�@�\����X�T�C�Y
#define LIFE_SIZE_Y		(30.0f)			//�v���C���[�c�@�\����Y�T�C�Y
#define MAX_LIFE		(2)				//�v���C���[�c�@�\����(���ۂ�3�@)

// �v���g�^�C�v�錾
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
void DownLifePlayer(void);

#endif