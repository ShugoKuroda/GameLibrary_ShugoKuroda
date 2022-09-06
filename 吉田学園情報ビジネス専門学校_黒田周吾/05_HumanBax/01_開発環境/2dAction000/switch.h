#ifndef _SWITCH_H_			//このマクロ定義がされなかったら
#define _SWITCH_H_			//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_SWITCH_TEX	(1)			//タイトル画面で必要な画像数
#define MAX_SWITCH		(16)		//スイッチの最大数

//スイッチ構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	bool bUse;				//使用しているかどうか
	int nPushCnt;			//スイッチの押下判定
	float fWidth;			//幅
	float fHeight;			//高さ
}Switch;

// プロトタイプ宣言
void InitSwitch(void);
void UninitSwitch(void);
void UpdateSwitch(void);
void DrawSwitch(void);
void SetSwitch(D3DXVECTOR3 pos, float Width, float Height);
void CollisionSwitch(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float Height);
Switch *GetSwitch(void);

#endif