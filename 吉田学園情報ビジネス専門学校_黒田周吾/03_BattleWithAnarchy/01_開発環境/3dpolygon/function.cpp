//===================================================================
//
//	共通関数[function.cpp]
//	Author:黒田 周吾
//
//===================================================================

//======================================================
//	インクルード
//======================================================
#include "main.h"
#include "function.h"

//======================================================
//	球体の当たり判定
//======================================================
bool SphereCollision(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, int StartLength, int EndLength)
{
	//返り値の初期化
	bool bSphereCollision = false;

	//2つの座標差分を求める
	D3DXVECTOR3 Length = posStart - posEnd;

	//座標差分から距離を求める
	float fLength = D3DXVec3Length(&Length);

	if (StartLength + EndLength >= fLength)
	{//お互いの球体サイズの合計より距離が小さかったら
		bSphereCollision = true;		//返り値を真にする
	}

	return bSphereCollision;
}