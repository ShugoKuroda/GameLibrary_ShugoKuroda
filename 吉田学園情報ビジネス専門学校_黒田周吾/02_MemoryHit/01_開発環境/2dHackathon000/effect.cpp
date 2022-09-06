//-------------------------------------------
//
//エフェクト処理[effect.cpp]
//Author:平野舞
//
//-------------------------------------------
#include "effect.h"

//マクロ定義
#define MAX_EFFECT		(4096)			//エフェクトの最大数
#define MAX_TEXEFFECT	(20)			//エフェクト画像の最大数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_TEXEFFECT] = {};	//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];								//エフェクトの情報

//外部ファイル読み込み用のグローバル変数宣言
char g_cReadEffect[256];									//文字読み込み用
char g_cTexName[30][256];									//画像名読み込み用
int g_nNumEffect;											//画像の数獲得用

//-------------------------------------------
//エフェクトの初期化処理
//-------------------------------------------
void InitEffect(void)
{
	//外部ファイル読み込み
	LoadEffectFile();

	//エフェクト構造体の初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//カラー
		g_aEffect[nCntEffect].fSize = 0.0f;								//サイズ
		g_aEffect[nCntEffect].nLife = 0;								//ライフ
		g_aEffect[nCntEffect].nSelect = 0;								//処理の選択
		g_aEffect[nCntEffect].nType = EFFECT_EFE1;						//種類
		g_aEffect[nCntEffect].bUse = false;								//使用しない
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < g_nNumEffect; nCount++)
	{//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			&g_cTexName[nCount][0],
			&g_pTextureEffect[nCount]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y - (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y - (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y + (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y + (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r,g_aEffect[nCntEffect].col.g,g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
		pVtx[1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r,g_aEffect[nCntEffect].col.g,g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
		pVtx[2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r,g_aEffect[nCntEffect].col.g,g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
		pVtx[3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r,g_aEffect[nCntEffect].col.g,g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//エフェクトの終了処理
//-------------------------------------------
void UninitEffect(void)
{
	for (int nCount = 0; nCount < g_nNumEffect; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureEffect[nCount] != NULL)
		{
			g_pTextureEffect[nCount]->Release();
			g_pTextureEffect[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//-------------------------------------------
//エフェクトの更新処理
//-------------------------------------------
void UpdateEffect(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//処理の選択処理
			SelectEffect(nCntEffect, g_aEffect[nCntEffect].nSelect);

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y - (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y - (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y + (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y + (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);

			//頂点カラーの更新
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fSize <= 0 || g_aEffect[nCntEffect].col.a <= 0.0f)
			{//寿命、サイズ、a値が0になったら
				g_aEffect[nCntEffect].bUse = false;		//使用していない状態にする
			}
		}
		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//エフェクトの描画処理
//-------------------------------------------
void DrawEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている場合

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//aブレインディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//aブレインディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-------------------------------------------
//エフェクトの設定処理
//
//D3DXVECTOR3 pos → 位置を指定
//D3DXCOLOR col　 → 色を指定
//int nLife		  → 表示時間(寿命)
//float fSize	  → サイズを指定
//int nType	　	　→ 画像の種類	
//int nSelect	  → 処理の選択
//
//-------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fSize, int nLife, EFFECT nType, int nSelect)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//使用していなかったら
			g_aEffect[nCntEffect].pos = pos;			//位置
			g_aEffect[nCntEffect].col = col;			//カラー
			g_aEffect[nCntEffect].nLife = nLife;		//ライフ
			g_aEffect[nCntEffect].fSize = fSize;		//サイズ
			g_aEffect[nCntEffect].nType = nType;		//画像の種類
			g_aEffect[nCntEffect].nSelect = nSelect;	//処理の種類
			g_aEffect[nCntEffect].bUse = true;			//使用する

			//位置とサイズの更新
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y - (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y - (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y + (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (g_aEffect[nCntEffect].fSize / 2.0f), g_aEffect[nCntEffect].pos.y + (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);

			//頂点カラーの更新
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}
		pVtx += 4;		//データ情報を進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}

void SelectEffect(int nCount,int nSelect)
{
	switch (nSelect)
	{
	case 0:		//寿命だけが減る
		g_aEffect[nCount].nLife -= 1;		//寿命を減らしていく
		break;

	case 1:		//サイズだけが減る
		g_aEffect[nCount].fSize -= 1;		//サイズを減らしていく
		break;

	case 2:		//a値だけが減る
		g_aEffect[nCount].col.a -= 0.05f;	//a値を減らしていく
		break;

	case 3:		//サイズと寿命が減る
		g_aEffect[nCount].fSize -= 1;		//サイズを減らしていく
		g_aEffect[nCount].nLife -= 1;		//寿命を減らしていく
		break;
	
	case 4:		//サイズとa値が減る
		g_aEffect[nCount].fSize -= 1;		//サイズを減らしていく
		g_aEffect[nCount].col.a -= 0.05f;	//a値を減らしていく
		break;
	
	case 5:		//寿命とa値が減る
		g_aEffect[nCount].nLife -= 1;		//寿命を減らしていく
		g_aEffect[nCount].col.a -= 0.05f;	//a値を減らしていく
		break;
	
	case 6:		//寿命とサイズとa値が減る
		g_aEffect[nCount].fSize -= 1;		//サイズを減らしていく
		g_aEffect[nCount].nLife -= 1;		//寿命を減らしていく
		g_aEffect[nCount].col.a -= 0.05f;	//a値を減らしていく
		break;
	
	default:
		assert(false);
		break;
	}
}

//-------------------------------------------
//外部ファイル読み込み処理
//-------------------------------------------
void LoadEffectFile(void)
{
	FILE * pFile;		//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data/effect.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		while (1)
		{
			fscanf(pFile, "%s", &g_cReadEffect[0]);		//文字列の読み込み

			if (strcmp(&g_cReadEffect[0], "NUM_EFFECT") == 0)
			{//モデルの数読み込み
				fscanf(pFile, "%s", &g_cReadEffect[0]);
				fscanf(pFile, "%d", &g_nNumEffect);		//文字列の読み込み
			}

			if (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") == 0)
			{//モデルファイル読み込み
				for (int nCount = 0; nCount < g_nNumEffect; nCount++)
				{
					while (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") != 0)
					{//モデルファイルネームではない場合
						fscanf(pFile, "%s", &g_cReadEffect[0]);
					}

					if (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") == 0)
					{//モデルファイルネームだったら
						fscanf(pFile, "%s", &g_cReadEffect[0]);
						fscanf(pFile, "%s", &g_cTexName[nCount][0]);		//文字列の読み込み
					}
				}
			}
			if (strcmp(&g_cReadEffect[0], "END_SCRIPT") == 0)
			{//この文があったら抜ける
				break;
			}
		}
		fclose(pFile);									//ファイルを閉じる
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした");
	}
}