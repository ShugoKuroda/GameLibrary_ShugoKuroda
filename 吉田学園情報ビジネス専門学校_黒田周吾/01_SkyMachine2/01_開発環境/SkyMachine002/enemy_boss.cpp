//-----------------------------------------------------------------------------------------------
//
// 敵ボスの処理[enemy_boss.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
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
// 定数宣言
//-----------------------------------------------------------------------------------------------
// 体力
const int CEnemyBoss::LIFE = 1000;
// 幅
const float CEnemyBoss::SIZE_WIDTH = 300.0f;
// 高さ
const float CEnemyBoss::SIZE_HEIGHT = 270.0f;
// U座標の最大分割数
const int CEnemyBoss::DIVISION_U = 4;
// V座標の最大分割数
const int CEnemyBoss::DIVISION_V = 1;
// 攻撃前の予備動作間隔
const int CEnemyBoss::RUSH_OPERATION = 60;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CEnemyBoss::CEnemyBoss() :m_pattern(PATTERN_ENTRY), m_nCounter(0)
{
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CEnemyBoss::~CEnemyBoss()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CEnemyBoss *CEnemyBoss::Create(const D3DXVECTOR3& pos, CEnemy::TYPE type)
{
	// ポインタクラスを宣言
	CEnemyBoss* pEnemy = new CEnemyBoss;

	if (pEnemy != nullptr)
	{
		//位置設定
		pEnemy->SetPosition(pos);

		//種類の設定
		pEnemy->SetType(type);

		// 初期化
		pEnemy->Init();

		// テクスチャの設定
		pEnemy->BindTexture(CEnemy::GetTexture(type));
	}

	return pEnemy;
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CEnemyBoss::Init()
{
	// 種類を設定
	SetObjType(EObject::OBJ_ENEMYBOSS);
	// 寿命の設定
	CEnemy::SetLife(LIFE);
	// サイズ設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// オブジェクト情報の初期化
	CObject2D::Init();
	// 頂点カラーの設定
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	//テクスチャ座標の設定
	CObject2D::SetAnimation(0, 0, DIVISION_U, DIVISION_V);

	// 攻撃前の予備動作間隔を設定
	m_nCountOperation = RUSH_OPERATION;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	// 位置の取得
	D3DXVECTOR3 move = CEnemy::GetMove();

	switch (m_pattern)
	{
		//=================================
		// 登場
		//=================================
	case CEnemyBoss::PATTERN_ENTRY:
		pos += D3DXVECTOR3(-0.5f, -2.0f, 0.0f);

		if (pos.y <= 250.0f)
		{
			m_pattern = PATTERN_NORMAL;
		}
		break;
	
		//=================================
		// 通常
		//=================================
	case CEnemyBoss::PATTERN_NORMAL:

		//移動量の加算
		move += D3DXVECTOR3(1.0f, 1.0f, 0.0f);

		// 移動量の更新
		pos += D3DXVECTOR3(sinf(move.x / 5) * 2, sinf(move.y / 25) * 5, 0.0f);

		//次の行動に移すまでのカウンター加算
		m_nCounter++;

		if (m_nCounter >= 300)
		{
			//カウンターリセット
			m_nCounter = 0;
			//次の行動をランダムで決める
			m_pattern = (PATTERN)LibrarySpace::GetRandNum(PATTERN_BARRAGE, PATTERN_RUSH);
			m_pattern = PATTERN_RUSH;
		}
		break;

		//=================================
		// 連続突進攻撃
		//=================================
	case CEnemyBoss::PATTERN_RUSH:

		if (m_nCountOperation <= 0)
		{
			CEffect::Create(pos, D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT), CEffect::TYPE_AFTERIMAGE, CEffect::TEX_BOSS);
			pos.x += -30.0f;

			m_nCountOperation--;

			if (m_nCountOperation == -120)
			{
				// 敵の予測軌道範囲ロゴ
				CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH * 1.5f, SIZE_HEIGHT),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f), 0.0f, CLogo::TYPE_NONE, CLogo::ANIM_NONE, 60);

				// 注意喚起標識のロゴ
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
	{// ライフが0
		// 爆発の生成
		CExplosion::Create(pos, GetSize());
		// 破棄
		Uninit();
		return;
	}

	//移動量更新
	CEnemy::SetMove(move);
	//アニメーション処理
	SetAnim();
	//状態管理
	State();
	// 位置の更新
	CObject2D::SetPosition(pos);
	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Draw()
{
	//描画処理
	CEnemy::Draw();
}

//-----------------------------------------------------------------------------------------------
// プレイヤーとの当たり判定
//-----------------------------------------------------------------------------------------------
bool CEnemyBoss::Collision(D3DXVECTOR3 posStart)
{
	return CEnemy::Collision(posStart);
}

//-----------------------------------------------------------------------------------------------
// ダメージ処理
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Damage(int nDamage)
{
	CEnemy::Damage(nDamage);
}

//-----------------------------------------------------------------------------------------------
// 状態
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::State()
{
	CEnemy::State();
}

//-----------------------------------------------------------------------------------------------
// 敵ごとにアニメーション,挙動を設定
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
