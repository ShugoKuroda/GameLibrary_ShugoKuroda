//===================================================================
//
//	敵の処理[enemy.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

#include "enemy.h"

//*******************************************************************
//	敵クラスの定義
//*******************************************************************
class CEnemyBoss : public CEnemy
{
public:		//定数
	// 幅
	static const float SIZE_WIDTH;
	// 高さ
	static const float SIZE_HEIGHT;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;
private:
	// 寿命
	static const int LIFE;
	// 攻撃前の予備動作間隔
	static const int RUSH_OPERATION;

public:
	//状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_DAMAGE,		//ダメージ状態
		STATE_MAX
	};

	//行動パターン
	enum PATTERN
	{
		PATTERN_ENTRY = 0,	//登場
		PATTERN_NORMAL,		//通常
		PATTERN_RUSH,		//連続突進攻撃
		PATTERN_CHARGE,		//チャージ弾攻撃
		PATTERN_BARRAGE,	//弾幕攻撃
		PATTERN_DIE,		//死亡
		PATTERN_MAX
	};

	CEnemyBoss();
	~CEnemyBoss() override;

	//メンバ関数
	static CEnemyBoss *Create(const D3DXVECTOR3& pos, CEnemy::TYPE type);	//インスタンス生成処理

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	bool Collision(D3DXVECTOR3 posStart) override;
	void Damage(int nDamage) override;
	void State() override;
	void SetAnim() override;

private:
	PATTERN m_pattern;		//行動パターン
	int m_nCounter;			//行動変化までのカウンター
	int m_nCountOperation;	//突進前の予備動作の間隔
};

#endif	// _ENEMY_BOSS_H_
