//-----------------------------------------------------------------------------------------------
//
// �G�{�X�̏���[enemy_boss.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------------
#include "enemy_boss.h"
#include "manager.h"
#include "renderer.h"

#include "game.h"
#include "library.h"
#include "effect.h"
#include "logo.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// �萔�錾
//-----------------------------------------------------------------------------------------------
// �̗�
const int CEnemyBoss::LIFE = 1000;
// ��
const float CEnemyBoss::SIZE_WIDTH = 300.0f;
// ����
const float CEnemyBoss::SIZE_HEIGHT = 270.0f;
// U���W�̍ő啪����
const int CEnemyBoss::DIVISION_U = 4;
// V���W�̍ő啪����
const int CEnemyBoss::DIVISION_V = 1;
// �U���O�̗\������Ԋu
const int CEnemyBoss::RUSH_OPERATION = 60;

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CEnemyBoss::CEnemyBoss() :m_pattern(PATTERN_ENTRY), m_nCounter(0)
{
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CEnemyBoss::~CEnemyBoss()
{
}

//-----------------------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------------------
CEnemyBoss *CEnemyBoss::Create(const D3DXVECTOR3& pos, CEnemy::TYPE type)
{
	// �|�C���^�N���X��錾
	CEnemyBoss* pEnemy = new CEnemyBoss;

	if (pEnemy != nullptr)
	{
		//�ʒu�ݒ�
		pEnemy->SetPosition(pos);

		//��ނ̐ݒ�
		pEnemy->SetType(type);

		// ������
		pEnemy->Init();

		// �e�N�X�`���̐ݒ�
		pEnemy->BindTexture(CEnemy::GetTexture(type));
	}

	return pEnemy;
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CEnemyBoss::Init()
{
	// ��ނ�ݒ�
	SetObjType(EObject::OBJ_ENEMYBOSS);
	// �����̐ݒ�
	CEnemy::SetLife(LIFE);
	// �T�C�Y�ݒ�
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// �I�u�W�F�N�g���̏�����
	CObject2D::Init();
	// ���_�J���[�̐ݒ�
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	//�e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimation(0, 0, DIVISION_U, DIVISION_V);

	// �U���O�̗\������Ԋu��ݒ�
	m_nCountOperation = RUSH_OPERATION;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Update()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	// �ʒu�̎擾
	D3DXVECTOR3 move = CEnemy::GetMove();

	switch (m_pattern)
	{
		//=================================
		// �o��
		//=================================
	case CEnemyBoss::PATTERN_ENTRY:
		pos += D3DXVECTOR3(-0.5f, -2.0f, 0.0f);

		if (pos.y <= 250.0f)
		{
			m_pattern = PATTERN_NORMAL;
		}
		break;
	
		//=================================
		// �ʏ�
		//=================================
	case CEnemyBoss::PATTERN_NORMAL:

		//�ړ��ʂ̉��Z
		move += D3DXVECTOR3(1.0f, 1.0f, 0.0f);

		// �ړ��ʂ̍X�V
		pos += D3DXVECTOR3(sinf(move.x / 5) * 2, sinf(move.y / 25) * 5, 0.0f);

		//���̍s���Ɉڂ��܂ł̃J�E���^�[���Z
		m_nCounter++;

		if (m_nCounter >= 300)
		{
			//�J�E���^�[���Z�b�g
			m_nCounter = 0;
			//���̍s���������_���Ō��߂�
			m_pattern = (PATTERN)LibrarySpace::GetRandNum(PATTERN_BARRAGE, PATTERN_RUSH);
			m_pattern = PATTERN_RUSH;
		}
		break;

		//=================================
		// �A���ːi�U��
		//=================================
	case CEnemyBoss::PATTERN_RUSH:

		if (m_nCountOperation <= 0)
		{
			CEffect::Create(pos, D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT), CEffect::TYPE_AFTERIMAGE, CEffect::TEX_BOSS);
			pos.x += -30.0f;

			m_nCountOperation--;

			if (m_nCountOperation == -120)
			{
				// �G�̗\���O���͈̓��S
				CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH * 1.5f, SIZE_HEIGHT),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f), 0.0f, CLogo::TYPE_NONE, CLogo::ANIM_NONE, 60);

				// ���ӊ��N�W���̃��S
				CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(200.0f, 200.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_REMINDER, CLogo::ANIM_FLASHING, 60);
			}
		}
		else
		{
			m_nCountOperation--;
			pos.x += m_nCountOperation * 0.1f;
		}

		break;
	case CEnemyBoss::PATTERN_CHARGE:
		break;
	case CEnemyBoss::PATTERN_BARRAGE:
		break;
	case CEnemyBoss::PATTERN_DIE:
		break;
	case CEnemyBoss::PATTERN_MAX:
		break;
	default:
		break;
	}

	if (GetLife() <= 0)
	{// ���C�t��0
		// �����̐���
		CExplosion::Create(pos, GetSize());
		// �j��
		Uninit();
		return;
	}

	//�ړ��ʍX�V
	CEnemy::SetMove(move);
	//�A�j���[�V��������
	SetAnim();
	//��ԊǗ�
	State();
	// �ʒu�̍X�V
	CObject2D::SetPosition(pos);
	//���_���W�̐ݒ�
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Draw()
{
	//�`�揈��
	CEnemy::Draw();
}

//-----------------------------------------------------------------------------------------------
// �v���C���[�Ƃ̓����蔻��
//-----------------------------------------------------------------------------------------------
bool CEnemyBoss::Collision(D3DXVECTOR3 posStart)
{
	return CEnemy::Collision(posStart);
}

//-----------------------------------------------------------------------------------------------
// �_���[�W����
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Damage(int nDamage)
{
	CEnemy::Damage(nDamage);
}

//-----------------------------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::State()
{
	CEnemy::State();
}

//-----------------------------------------------------------------------------------------------
// �G���ƂɃA�j���[�V����,������ݒ�
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::SetAnim()
{
	switch (GetType())
	{
	case CEnemy::TYPE_RING_BOSS:

		break;

	case CEnemy::TYPE_DARK_BOSS:

		break;

	default:
		break;
	}
}
