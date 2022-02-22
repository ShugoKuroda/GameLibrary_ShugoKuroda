#ifndef _RANK_H_			//このマクロ定義がされなかったら
#define _RANK_H_			//2重インクルード防止のマクロ定義

//マクロ定義
#define MAX_RANK_IMAGE	(4)		//リザルト画面で必要なテクスチャ数
#define MAX_RANK		(6)		//表示するランキング数
#define MAX_NUM			(8)		//スコアの桁数

//ランキングスコア構造体
typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
}RankScore;

// プロトタイプ宣言
void InitRank(void);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);
void SetRank(int nScore, int nCntScore);
void LoadScore(void);
void SaveScore(void);

#endif