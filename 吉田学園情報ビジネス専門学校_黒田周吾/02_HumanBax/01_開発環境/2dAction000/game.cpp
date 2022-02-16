#include "main.h"
#include "game.h"
#include "player.h"
#include "bg.h"
#include "time.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "block.h"
#include "switch.h"
#include "particle.h"
#include "game_tutorial.h"
#include "flag.h"

//マクロ定義
#define	PAUSE_IMAGE_MAX			(5)			//ポーズ画面に必要な画像数
#define	PAUSE_SELECT_MAX		(4)			//ポーズ画面に必要な選択数
#define PAUSE_UI_SIZE_X			(1000.0f)	//ポーズ画面UIのXサイズ
#define PAUSE_UI_SIZE_Y			(400.0f)	//ポーズ画面UIのYサイズ
#define PAUSE_SELECT_SIZE_X		(160.0f)	//ポーズ画面の選択項目のXサイズ
#define PAUSE_SELECT_SIZE_Y		(200.0f)	//ポーズ画面の選択項目のYサイズ

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_IMAGE_MAX] = { NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause[3] = { NULL };			//頂点バッファへのポインタ
D3DXVECTOR3 g_PausePos[2];		//ポーズ画面の位置
bool g_bPause;					//ゲームのポーズ(一時停止)判定
bool g_bDiePlayer;				//プレイヤーの死亡判定
float g_fColPauseA[PAUSE_IMAGE_MAX + 1];	//ポーズアルファ値
int g_nPauseSelecting;						//ポーズ画面中に選択した項目の変数
int g_nCntCol;			//ポーズ画面のフェードの出す順用
int g_nColPoseCnt;		//ポーズ画面中に選択している項目をチカチカさせる用変数
int g_nFade;			//フェードまでの余韻
int g_nCntSet;			//ブロックの配置タイミングずらし用変数

//======================================================
//		ゲーム全体の初期化処理
//======================================================
void InitGame(void)
{
	//各ポインターを宣言
	Block *pBlock;
	Switch *pSwitch;
	FLAG *pFlag;

	//各情報の取得
	pBlock = GetBlock();
	pSwitch = GetSwitch();
	pFlag = GetFlag();

	//背景の初期化処理
	InitBg();

	//ゲーム中のチュートリアル画像の初期化
	InitGameTutorial();

	//スイッチの初期化処理
	InitSwitch();

	//プレイヤーの初期化処理
	InitPlayer();

	//ブロックの初期化処理
	InitBlock();

	//中間地点の初期化処理
	InitGameFlag();

	//ブロック配置(	位置 / 移動量 / 移動往復カウンター / 種類 / ギミック番号 / 幅 / 高さ / テクスチャの大きさ(X) / テクスチャの大きさ(Y))
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH / 1.2f, SCREEN_HEIGHT - 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),	 0,	 0, 0, SCREEN_WIDTH + 500.0f,	  100.0f,			 1.0f, 1.0f);				//地面
	SetBlock(D3DXVECTOR3(-200.0f, SCREEN_HEIGHT / 2, 0.0f),				D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  0, 0, SCREEN_WIDTH / 6,  SCREEN_HEIGHT * 2, 1.0f, 1.0f);						//左壁
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH -100.0f, 500.0f, 0.0f),				D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  2, 0, 70.0f,			  70.0f,			 1.0f, 1.0f);			//可動ブロック(ゴール)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 400.0f, 0.0f),				D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  0, 0, 100.0f,			  160.0f,			 1.0f, 1.0f);				//空中に浮いてる
	SetBlock(D3DXVECTOR3((SCREEN_WIDTH * 1.2f) + 75.0f, 400.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0,  0, 0, 50.0f,			  50.0f,			 1.0f, 1.0f);			//空中に浮いてる
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.6f + 10.0f, 520.0f, 0.0f),				 D3DXVECTOR3(0.0f, 1.0f, 0.0f), 140,  0, 0, 200.0f,			  50.0f,			 1.0f, 1.0f);	//リフト(縦)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.9f - 20.0f, SCREEN_HEIGHT - 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 500.0f, 220.0f, 1.0f, 1.0f);				//地面2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.2f - 100.0f, SCREEN_HEIGHT - 60.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 110.0f, 100.0f, 1.0f, 1.0f);				//地面2小
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.3f - 20.0f, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 200.0f, 40.0f, 1.0f, 1.0f);						//空中に浮いてる2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.3f - 20.0f, SCREEN_HEIGHT / 3, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 0, 100.0f, 100.0f, 1.0f, 1.0f);					//可動ブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.4f - 50.0f, SCREEN_HEIGHT - 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 500.0f, 220.0f, 1.0f, 1.0f);				//地面2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.7f - 80.0f, 250.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 160, 0, 0, 200.0f, 50.0f, 1.0f, 1.0f);								//リフト(縦)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 80.0f, SCREEN_HEIGHT - 130.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 300.0f, 50.0f, 1.0f, 1.0f);				//地面2コ下
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.8f - 80.0f, SCREEN_HEIGHT - 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 40.0f, 300.0f, 1.0f, 1.0f);				//地面2(壁)コ
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 70.0f, SCREEN_HEIGHT - 425.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 300.0f, 50.0f, 1.0f, 1.0f);				//地面2コ上
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.0f - 0.0f, SCREEN_HEIGHT - 35.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 110.0f, 40.0f, 1.0f, 1.0f);					//地面2小
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 40.0f, SCREEN_HEIGHT - 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 0, 100.0f, 100.0f, 1.0f, 1.0f);				//可動ブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.1f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 160, 0, 0, 100.0f, 50.0f, 1.0f, 1.0f);								//リフト(縦)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2.9f - 60.0f, SCREEN_HEIGHT - 600.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 4, 0, 100.0f, 100.0f, 1.0f, 1.0f);				//可動ブロック(とげ付き)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.1f - 70.0f, SCREEN_HEIGHT - 75.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH * 2, 160.0f, 1.0f, 1.0f);		//地面3
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.5f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 120, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.7f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), 90, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3.9f - 0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 11.0f, 0.0f), 40, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)
	//SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.7f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 2, 0, 70.0f, 70.0f, 1.0f, 1.0f);											//可動ブロック(ゴール)仮
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.8f - 10.0f, SCREEN_HEIGHT - 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH * 1.5, 50.0f, 1.0f, 1.0f);		//地面4下
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.2f - 120.0f, 220.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 190, 0, 0, 145.0f, 50.0f, 1.0f, 1.0f);								//リフト(縦)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.0f - 100.0f, SCREEN_HEIGHT / 3, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH * 1.5, 50.0f, 1.0f, 1.0f);			//地面4上
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.3f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 100.0f, 100.0f, 1.0f, 1.0f);				//とげブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.5f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 145.0f, 100.0f, 1.0f, 1.0f);				//とげブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.8f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 250.0f, 100.0f, 1.0f, 1.0f);				//とげブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 5.8f - 0.0f, SCREEN_HEIGHT - 170.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 40.0f, 40.0f, 1.0f, 1.0f);				//空中に浮いてる2
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.1f - 0.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 300.0f, 100.0f, 1.0f, 1.0f);				//とげブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.0f + 10.0f, 525.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 60, 0, 0, 50.0f, 50.0f, 1.0f, 1.0f);							//リフト(横)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.4f - 80.0f, SCREEN_HEIGHT - 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 5, 0, 300.0f, 100.0f, 1.0f, 1.0f);			//とげブロック
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.4f + 10.0f, 525.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 60, 0, 0, 50.0f, 50.0f, 1.0f, 1.0f);							//リフト(横)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.6f + 20.0f, SCREEN_HEIGHT / 1.5f + 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 100.0f, 550.0f, 1.0f, 1.0f);				//壁(最後)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.7f + 20.0f, SCREEN_HEIGHT / 3, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, 160.0f, 50.0f, 1.0f, 1.0f);						//地面(最後上)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.9f + 20.0f, SCREEN_HEIGHT - 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 0, 0, SCREEN_WIDTH / 1.5f, 200.0f, 1.0f, 1.0f);		//地面(最後)
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 7.1f, SCREEN_HEIGHT - 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 6, 0, 100.0f, 100.0f, 1.0f, 1.0f);			//出口

	//スイッチ配置( 位置 / 幅 / 高さ)
	SetSwitch(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 310.0f, 0.0f), 50.0f, 30.0f);
	SetSwitch(D3DXVECTOR3(SCREEN_WIDTH * 6.0f - 90.0f, SCREEN_HEIGHT - 55.0f, 0.0f), 50.0f, 30.0f);
	SetSwitch(D3DXVECTOR3(SCREEN_WIDTH * 6.6f - 120.0f, SCREEN_HEIGHT - 55.0f, 0.0f), 50.0f, 30.0f);

	//ゲート配置( 位置 / 移動量 / 移動往復カウンター / 種類 / ギミック番号 / 幅 / 高さ / テクスチャの大きさ(X) / テクスチャの大きさ(Y))
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 550.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3, 1, 50.0f, 150.0f, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.3f - 0.0f, SCREEN_HEIGHT / 4 - 45.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3, 2, 50.0f, 150.0f, 1.0f, 1.0f);
	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 6.5f - 0.0f, SCREEN_HEIGHT / 4 - 45.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 3, 3, 50.0f, 150.0f, 1.0f, 1.0f);

	//中間地点配置
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 1.2f, 310.0f, 0.0f), 0);
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 1.4f, 310.0f, 0.0f), 0);
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 5.2f, 310.0f, 0.0f), 0);
	SetGameFlag(D3DXVECTOR3(SCREEN_WIDTH * 7.2f, 310.0f, 0.0f), 1);

	//パーティクルの初期化処理
	InitParticle();

	//タイムの初期化処理
	InitTime();

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (FlagStage() == 1)
	{

	}

	//各項目の初期化
	g_PausePos[0] = D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, 0.0f);				//ポーズ画面の選択項目位置の初期化
	g_PausePos[1] = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / 2.0f) - 20.0f, 0.0f);		//ポーズ画面UIの位置の初期化
	g_nCntCol = 0;			//ポーズ画面のフェードの出す順用
	g_nFade = 0;			//フェードまでの余韻
	g_nPauseSelecting = 2;	//ゲームを続けるを選択に初期化
	g_nColPoseCnt = 0;		//ポーズ画面中に選択している項目をチカチカさせる用の初期化
	g_bPause = false;		//ポーズ判定
	g_bDiePlayer = false;	//プレイヤーの死亡判定
	g_nCntSet = 0;			//ブロックの配置タイミングずらし用変数の初期化

	for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
	{
		g_fColPauseA[nCnt] = 0.0f;	//ポーズアルファ値初期化
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause000.png",
		&g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause002.png",
		&g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause001.png",
		&g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause003.png",
		&g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/PauseUI000.png",
		&g_pTexturePause[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//画面全体のバッファ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[0],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//ポーズUIのバッファ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[1],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_SELECT_MAX,		//ポーズ選択項目のバッファ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause[2],
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffPause[0]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_PausePos[1].x - (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y - (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PausePos[1].x + (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y - (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PausePos[1].x - (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y + (PAUSE_UI_SIZE_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PausePos[1].x + (PAUSE_UI_SIZE_X / 2), g_PausePos[1].y + (PAUSE_UI_SIZE_Y / 2), 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffPause[1]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) - (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y - (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) + (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y - (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) - (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y + (PAUSE_SELECT_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_PausePos[0].x + 200.0f * nCnt) + (PAUSE_SELECT_SIZE_X / 2), g_PausePos[0].y + (PAUSE_SELECT_SIZE_Y / 2), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[2 + nCnt]);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffPause[2]->Unlock();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM001);
}

//======================================================
//		ゲーム全体の終了処理
//======================================================
void UninitGame(void)
{
	//サウンド停止
	StopSound();

	//背景の終了処理
	UninitBg();

	//ゲーム中のチュートリアル画像の終了処理
	UninitGameTutorial();

	//スイッチの終了処理
	UninitSwitch();

	//プレイヤーの終了処理
	UninitPlayer();

	//ブロックの終了処理
	UninitBlock();

	//中間地点の終了処理
	UninitGameFlag();

	//パーティクルの終了処理
	UninitParticle();

	//スコアの終了処理
	UninitTime();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX; nCnt++)
	{
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffPause[nCnt] != NULL)
		{
			g_pVtxBuffPause[nCnt]->Release();
			g_pVtxBuffPause[nCnt] = NULL;
		}
	}
}

//======================================================
//		ゲーム全体の更新処理
//======================================================
void UpdateGame(void)
{
	Player *pPlayer;	//プレイヤーのポインタ

	//プレイヤー情報を取得
	pPlayer = GetPlayer();

	//リフトブロックをずらして配置
	if (g_nCntSet < 65)
	{
		g_nCntSet++;
		if (g_nCntSet == 5)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.2f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)連続
		}
		if (g_nCntSet == 10)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.3f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)連続
		}
		if (g_nCntSet == 15)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.4f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)連続
		}
		if (g_nCntSet == 20)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.5f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)連続
		}
		if (g_nCntSet == 25)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.6f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)連続
		}
		if (g_nCntSet == 30)
		{
			SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4.7f - 0.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 150, 0, 0, 100.0f, 100.0f, 1.0f, 1.0f);								//リフト(縦)連続
		}
	}

	//Pキーが押された場合
	if (GetKeyboardTrigger(DIK_P) == true && g_bPause == false)
	{	
		//ゲームを止める
		g_nCntCol = 0;
		g_bPause = true;
	}

	//ポーズ中の場合
	if (g_bPause == true)
	{
		if (g_nCntCol == 0)
		{//バックのゲーム画面を暗くする
			g_fColPauseA[0] += 0.1f;
			if (g_fColPauseA[0] > 0.5f)
			{
				g_fColPauseA[0] = 0.5f;
				g_nCntCol = 1;
			}
		}
		else if (g_nCntCol == 1)
		{//ポーズ画面を出す
			for (int nCnt = 1; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
			{
				g_fColPauseA[nCnt] += 0.1f;
				if (g_fColPauseA[nCnt] > 1.0f)
				{
					g_fColPauseA[nCnt] = 1.0f;
					g_nCntCol = 2;
				}
			}
		}
		else if (g_nCntCol == 2)
		{//ポーズ選択画面
			if (GetKeyboardTrigger(DIK_A)== true)
			{
				g_nPauseSelecting--;
				//選択していない項目のリセット
				g_fColPauseA[2] = 1.0f;
				g_fColPauseA[3] = 1.0f;
				g_fColPauseA[4] = 1.0f;
				g_fColPauseA[5] = 1.0f;
				if (g_nPauseSelecting < 2)
				{
					g_nPauseSelecting = 5;
				}
			}
			else if (GetKeyboardTrigger(DIK_D) == true)
			{
				g_nPauseSelecting++;
				//選択していない項目のリセット
				g_fColPauseA[2] = 1.0f;
				g_fColPauseA[3] = 1.0f;
				g_fColPauseA[4] = 1.0f;
				g_fColPauseA[5] = 1.0f;
				if (g_nPauseSelecting > 5)
				{
					g_nPauseSelecting = 2;
				}
			}
			else if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) == true)
			{
				if (g_nPauseSelecting == 2)
				{//ゲームを続ける
					g_bPause = false;
				}
				else if (g_nPauseSelecting == 3)
				{//リトライ
					//DiedPlayer();
					SetMode(MODE_GAME);
				}
				else if (g_nPauseSelecting == 4)
				{//リタイア
					g_bPause = false;
				}
				else if (g_nPauseSelecting == 5)
				{//タイトルに戻る
					//DiedPlayer();
					SetFade(MODE_TITLE);
				}
			}

			if (g_nColPoseCnt == 0)
			{//選択中の項目をチカチカさせる
				g_fColPauseA[g_nPauseSelecting] -= 0.05f;
				if (g_fColPauseA[g_nPauseSelecting] < 0.0f)
				{
					g_fColPauseA[g_nPauseSelecting] = 0.0f;
					g_nColPoseCnt = 1;
				}
			}
			else if (g_nColPoseCnt == 1)
			{
				g_fColPauseA[g_nPauseSelecting] += 0.05f;
				if (g_fColPauseA[g_nPauseSelecting] > 1.0f)
				{
					g_fColPauseA[g_nPauseSelecting] = 1.0f;
					g_nColPoseCnt = 0;
				}
			}
		}
	}
	else
	{//ポーズ画面を出していないとき
		for (int nCnt = 0; nCnt < PAUSE_IMAGE_MAX + 1; nCnt++)
		{
			g_fColPauseA[nCnt] -= 0.1f;
			if (g_fColPauseA[nCnt] < 0.0f)
			{
				g_fColPauseA[nCnt] = 0.0f;
			}
		}
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fColPauseA[0]);

	//頂点バッファをアンロック
	g_pVtxBuffPause[0]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[1]);

	//頂点バッファをアンロック
	g_pVtxBuffPause[1]->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColPauseA[nCnt + 2]);

		pVtx += 4;  //頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffPause[2]->Unlock();

	//プレイヤーが死亡した場合
	g_bDiePlayer = DiePlayer();

	if (g_bDiePlayer == false)
	{
		if (g_nFade > 100)
		{
			SetMode(MODE_GAME);
		}
		else
		{
			g_nFade++;
		}
	}

	if (g_bPause == false)
	{//ポーズ中でなければ

		//背景の更新処理
		UpdateBg();

		//ゲーム中のチュートリアル画像の更新処理
		UpdateGameTutorial();

		//スイッチの更新処理
		UpdateSwitch();

		//プレイヤーの更新処理
		UpdatePlayer();

		//ブロックの更新処理
		UpdateBlock();

		//中間地点の更新処理
		UpdateGameFlag();

		//パーティクルの更新処理
		UpdateParticle();

		//スコアの更新処理
		UpdateTime();
	}
}

//======================================================
//		ゲーム全体の描画処理
//======================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBg();

	//ゲーム中のチュートリアル画像の描画処理
	DrawGameTutorial();

	//スイッチの描画処理
	DrawSwitch();

	//プレイヤーの描画処理
	DrawPlayer();

	//ブロックの描画処理
	DrawBlock();

	//中間地点の描画処理
	DrawGameFlag();

	//パーティクルの描画処理
	DrawParticle();

	//スコアの描画処理
	//DrawTime();

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[0], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,		//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[1], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause[4]);
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,		//描画する最初の頂点インデックス
		2);		//プリミティブ（ポリゴン）数


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause[2], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < PAUSE_SELECT_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			4 * nCnt,	//描画する最初の頂点インデックス
			2);			//プリミティブ（ポリゴン）数
	}
}