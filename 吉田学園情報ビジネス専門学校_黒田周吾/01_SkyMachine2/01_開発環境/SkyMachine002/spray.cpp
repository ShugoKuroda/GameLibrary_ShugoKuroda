//-----------------------------------------------------------------------------------------------
//
// 飛沫の情報[spray.cpp]
// Author : SHUGO KURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "spray.h"
#include "manager.h"		// アプリケーション
#include "renderer.h"		// レンダリング

//-----------------------------------------------------------------------------------------------
// 定数宣言
//-----------------------------------------------------------------------------------------------
// 幅
const float CSpray::SIZE_WIDTH = 100.0f;
// 高さ
const float CSpray::SIZE_HEIGHT = 50.0f;
// アニメーション間隔
const int CSpray::ANIM_INTERVAL = 5;
// アニメーション最大数
const int CSpray::MAX_ANIM = 6;
// U座標(X方向)の最大分割数
const int CSpray::DIVISION_U = 4;
// V座標(Y方向)の最大分割数
const int CSpray::DIVISION_V = 2;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CSpray::m_pTexture = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CSpray::CSpray()
	:m_nCntAnim(0), m_nPatternAnim(0), m_nPatterAnimV(0)
{
	SetObjType(CSpray::OBJ_SPRAY);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CSpray::~CSpray()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
// const D3DXVECTOR3& pos → 生成する位置
//-----------------------------------------------------------------------------------------------
CSpray* CSpray::Create(const D3DXVECTOR3& pos)
{
	// ポインタクラスを宣言
	CSpray* pSpray = new CSpray;

	if (pSpray != nullptr)
	{
		// 位置設定
		pSpray->SetPosition(pos);

		// 初期化
		pSpray->Init();

		// テクスチャの設定
		pSpray->BindTexture(m_pTexture);
	}

	return pSpray;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CSpray::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect001.png",
		&m_pTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CSpray::Unload()
{
	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CSpray::Init()
{
	// サイズ
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));

	//初期化
	CObject2D::Init();

	// テクスチャ更新
	CObject2D::SetAnimation(m_nPatternAnim, 2, DIVISION_U, DIVISION_V);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CSpray::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CSpray::Update()
{
	//常に右に流す
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	pos.x -= 1.0f;
	CObject2D::SetPosition(pos);

	// カウントを増やす
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_INTERVAL == 0)
	{
		// 今のアニメーションを1つ進める
		m_nPatternAnim++;
	}

	if (m_nPatternAnim == MAX_ANIM)
	{// アニメーションが終わったら
	 // 終了する
		Uninit();
		return;
	}

	if (m_nPatternAnim >= DIVISION_U && m_nPatterAnimV <= 0)
	{
		m_nPatterAnimV++;
	}

	//頂点座標の設定
	CObject2D::SetVertex();

	// テクスチャ更新
	CObject2D::SetAnimation(m_nPatternAnim, m_nPatterAnimV, DIVISION_U, DIVISION_V);
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CSpray::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// 移動処理
//-----------------------------------------------------------------------------------------------
void CSpray::Move(float fMove)
{
	D3DXVECTOR3 pos;	//位置情報格納用

	// 水しぶきオブジェクトの取得
	for (int nCntObject = 0; nCntObject < CObject::MAX_OBJECT; nCntObject++)
	{
		//オブジェクト情報の取得
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			//オブジェクトタイプの取得
			CObject::EObject objType = pObject->GetObjType();
			//オブジェクトタイプが水しぶきだったら
			if (objType == CObject::OBJ_SPRAY)
			{
				//上に移動する
				CObject2D *pSpray = (CObject2D*)pObject;
				pos = pSpray->GetPosition();
				pos.y += fMove;
				pSpray->SetPosition(pos);
				pSpray->SetVertex();
			}
		}
	}
}
