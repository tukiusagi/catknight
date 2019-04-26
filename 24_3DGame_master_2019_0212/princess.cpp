//=============================================================================
//
// 姫の処理 [princess.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "princess.h"
#include "input.h"
#include "joystick.h"
#include "camera.h"
#include "shadow.h"
#include "meshfield.h"
#include "line.h"
#include"game.h"
#include "enemy.h"
#include "wave.h"
#include "magic.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PRINCESS_TEXTURE	"data/TEXTURE/mizuiro.jpg"	//読み込むテクスチャ名
#define PRINCESS_TEXTURE2	"data/TEXTURE/white2.jpg"	//読み込むテクスチャ名
#define PRINCESS_SHADOW		(15.0f)			//影の大きさ
#define PRINCESS_ROTSPEAD	(0.1f)			//回転速度
#define PRINCESS_KEY_MAX	(4)				//キーの最大数
#define PRINCESS_TEX_MAX	(6)				//テクスチャの最大数
#define MAX_CHARA			(256)			//読み込む文字の最大数

//=============================================================================
// モーション管理
//=============================================================================
//キー要素
typedef struct
{
	D3DXVECTOR3	pos;	//位置
	D3DXVECTOR3	rot;	//向き
} KEY;

//キー情報
typedef struct
{
	int nFrame;				//フレーム数
	KEY	Key[PRINCESS_MAX];	//キー要素[パーツ数]
} KEY_INFO;

//モーション情報
typedef struct
{
	bool		bLoop;		//ループの有無
	int			nNumKey;	//キー数
	KEY_INFO	KeyInfo[PRINCESS_KEY_MAX];	//キー情報[キーの数]
} MOTION_INFO;

//=============================================================================
// グローバル変数
//=============================================================================
PRINCESS			g_Princess;					//姫の情報
LPDIRECT3DTEXTURE9	g_pTexturePrincess[PRINCESS_TEX_MAX] = {};	//テクスチャのポインタ
int	g_nIdxShadowPrincess;		//影のID
int g_NumPrincess = 0;			//読み込むモデル数
int g_NumPrincessParts = 0;			//読み込むモデル数
char g_PrincessFileNeme[PRINCESS_MAX][MAX_CHARA];

MOTION_INFO	g_aPrincessMotionInfo[PRINCESSSTATE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
void InitPrincess(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//データのロード
	LoadPrincess();

	// 位置・向きの初期設定
	g_Princess.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Princess.state = PRINCESSSTATE_NEUTRAL;
	g_Princess.bDamege = false;
	g_Princess.bPinch = false;
	g_Princess.nCntState = 0;
	g_Princess.nCntFrame = 0;
	g_Princess.nCntKeyPrincess = 0;

	//影の作成
	g_nIdxShadowPrincess = SetSadow(g_Princess.pos, g_Princess.rot, PRINCESS_SHADOW, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	if (*GetMode() != MODE_TITLE)
	{
		SetMagic(g_Princess.pos);
	}


	// Xファイルの読み込み
	for (int nCntModel = 0; nCntModel < g_NumPrincess; nCntModel++)
	{
		D3DXLoadMeshFromX(g_Princess.aModel[nCntModel].pFilename,	//読み込むファイル名
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Princess.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Princess.aModel[nCntModel].nNumMat,
			&g_Princess.aModel[nCntModel].pMesh);

	}

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < PRINCESS_TEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			PRINCESS_TEXTURE2,
			&g_pTexturePrincess[nCntTex]);
	}

	D3DXCreateTextureFromFile(pDevice,
		PRINCESS_TEXTURE,
		&g_pTexturePrincess[2]);

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPrincess(void)
{
	for (int nCntPrincess = 0; nCntPrincess < g_NumPrincess; nCntPrincess++)
	{
		// メッシュの開放
		if (g_Princess.aModel[nCntPrincess].pMesh != NULL)
		{
			g_Princess.aModel[nCntPrincess].pMesh->Release();
			g_Princess.aModel[nCntPrincess].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_Princess.aModel[nCntPrincess].pBuffMat != NULL)
		{
			g_Princess.aModel[nCntPrincess].pBuffMat->Release();
			g_Princess.aModel[nCntPrincess].pBuffMat = NULL;
		}
	}

	//テクスチャの開放
	for (int nCntTex = 0; nCntTex < PRINCESS_TEX_MAX; nCntTex++)
	{
		if (g_pTexturePrincess[nCntTex] != NULL)
		{
			g_pTexturePrincess[nCntTex]->Release();
			g_pTexturePrincess[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePrincess(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();
	WAVE *pWave = GetWave();
	g_Princess.bPinch = false;
	g_Princess.posOld = g_Princess.pos;

	if (g_Princess.bDamege)
	{	//ダメージ状態
		g_Princess.nCntState++;
		if (g_Princess.nCntState % 10 == 0)
		{
			g_Princess.bDamege = false;	//状態解除
		}
	}

	//フレーム増加
	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{	//死亡時には増加しない
	}
	else if (pWave->state == WAVE_STATE_ATTACK && g_Princess.nCntKeyPrincess >= 1 && *GetGameState() == GAMESTATE_NORMAL)
	{
		//魔法使用時も止める
	}
	else
	{
		g_Princess.nCntFrame++;
		if (g_Princess.nCntFrame > g_aPrincessMotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame)
		{
			g_Princess.nCntFrame = 0;
			g_Princess.nCntKeyPrincess++;
			if (g_Princess.nCntKeyPrincess >= g_aPrincessMotionInfo[g_Princess.state].nNumKey)
			{
				g_Princess.nCntKeyPrincess = 0;
			}
		}
	}
	//モーションの切り替え
	if (*GetMode() == MODE_GAME)
	{
		switch (*GetGameState())
		{
		case GAMESTATE_CLEAR:
			g_Princess.state = PRINCESSSTATE_WIN;
			break;
		case GAMESTATE_GAMEOVER:
			if (g_Princess.state != PRINCESSSTATE_DETH)
			{
				g_Princess.state = PRINCESSSTATE_DETH;
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess = 0;
			}
			break;
		default:
			if (g_Princess.state != PRINCESSSTATE_ATTACK && g_Princess.state != PRINCESSSTATE_WIN)
			{
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
			}
			break;
		}

		//回転
		if (g_Princess.rotmove.y = g_Princess.rotDest.y - g_Princess.rot.y)
		{
			if (g_Princess.rotmove.y < -D3DX_PI)
			{
				g_Princess.rotmove.y += D3DX_PI * 2;
			}
			else if (g_Princess.rotmove.y > D3DX_PI)
			{
				g_Princess.rotmove.y -= D3DX_PI * 2;
			}
		}
		if (g_Princess.rot.y += g_Princess.rotmove.y * PRINCESS_ROTSPEAD)
		{
			if (g_Princess.rot.y > D3DX_PI)
			{
				g_Princess.rot.y -= D3DX_PI * 2;
			}
			else if (g_Princess.rot.y < -D3DX_PI)
			{
				g_Princess.rot.y += D3DX_PI * 2;
			}
		}

		//重力
		g_Princess.move.y -= 0.8f;

		//慣性
		g_Princess.move.x += (0.0f - g_Princess.move.x)*0.25f;
		g_Princess.move.z += (0.0f - g_Princess.move.z)*0.25f;

		//移動
		g_Princess.pos.x += g_Princess.move.x;
		g_Princess.pos.z += g_Princess.move.z;
		g_Princess.pos.y += g_Princess.move.y;

	}

	if (g_Princess.pos.y < 0)
	{	//地面にめり込まないように
		g_Princess.pos.y = 0;
		g_Princess.move.y = 0;
	}
	//左右壁判定
	if (g_Princess.pos.x > MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Princess.fRadius)
	{
		g_Princess.pos.x = MESHFIELD_SIZE * MESHFIELD_W * 0.5f - g_Princess.fRadius;
	}
	if (g_Princess.pos.x < -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Princess.fRadius)
	{
		g_Princess.pos.x = -MESHFIELD_SIZE * MESHFIELD_W * 0.5f + g_Princess.fRadius;
	}
	//奥手前壁判定
	if (g_Princess.pos.z > MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Princess.fRadius)
	{
		g_Princess.pos.z = MESHFIELD_SIZE * MESHFIELD_D * 0.5f - g_Princess.fRadius;
	}
	if (g_Princess.pos.z < -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Princess.fRadius)
	{
		g_Princess.pos.z = -MESHFIELD_SIZE * MESHFIELD_D * 0.5f + g_Princess.fRadius;
	}

	if (g_Princess.state == PRINCESSSTATE_DETH && g_Princess.nCntKeyPrincess >= 1)
	{
	}
	else
	{
		//モーション再生
		PrincessMotion();
	}


	//影の位置を設定
	SetPositionSadow(g_nIdxShadowPrincess, g_Princess.pos, PRINCESS_SHADOW + ((0.0f - g_Princess.pos.y)*-0.1f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f - ((0.0f - g_Princess.pos.y)*-0.008f)));
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPrincess(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス
	D3DXMATERIAL	*pMat;	//現在のマテリアル保存用
	D3DMATERIAL9	matDef;	//マテリアルデータへのポインタ
	D3DMATERIAL9		matKeep;	//保存用

//姫の設定
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Princess.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Princess.rot.y, g_Princess.rot.x, g_Princess.rot.z);

	D3DXMatrixMultiply(&g_Princess.mtxWorld, &g_Princess.mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Princess.pos.x, g_Princess.pos.y, g_Princess.pos.z);

	D3DXMatrixMultiply(&g_Princess.mtxWorld, &g_Princess.mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Princess.mtxWorld);

//モデルの設定
	for (int nCntPrincess = 0; nCntPrincess < g_NumPrincessParts; nCntPrincess++)
	{
		//親の識別
		if (g_Princess.aModel[nCntPrincess].nIdxModelParent == -1)
		{
			mtxParent = g_Princess.mtxWorld;
		}
		else
		{
			mtxParent = g_Princess.aModel[g_Princess.aModel[nCntPrincess].nIdxModelParent].mtxWorld;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Princess.aModel[nCntPrincess].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Princess.aModel[nCntPrincess].rot.y, g_Princess.aModel[nCntPrincess].rot.x, g_Princess.aModel[nCntPrincess].rot.z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxRot);	//ワールド行列＝ワールド行列＊回転行列

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Princess.aModel[nCntPrincess].pos.x, g_Princess.aModel[nCntPrincess].pos.y, g_Princess.aModel[nCntPrincess].pos.z);

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxTrans);	//ワールド行列＝ワールド行列＊移動行列

		D3DXMatrixMultiply(&g_Princess.aModel[nCntPrincess].mtxWorld, &g_Princess.aModel[nCntPrincess].mtxWorld, &mtxParent);	//ワールド行列＝ワールド行列＊親のワールド行列
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Princess.aModel[nCntPrincess].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_Princess.aModel[nCntPrincess].pBuffMat->GetBufferPointer();
		// テクスチャの設定をするならここ(1枚の時)

		for (int nCntMat = 0; nCntMat < (int)g_Princess.aModel[nCntPrincess].nNumMat; nCntMat++)
		{
			// テクスチャの設定をするならここ(複数枚の時)
			pDevice->SetTexture(0, g_pTexturePrincess[nCntMat]);

			pMat[nCntMat].MatD3D.Ambient.r = pMat[nCntMat].MatD3D.Diffuse.r/12;
			pMat[nCntMat].MatD3D.Ambient.g = pMat[nCntMat].MatD3D.Diffuse.g/12;
			pMat[nCntMat].MatD3D.Ambient.b = pMat[nCntMat].MatD3D.Diffuse.b/12;

			matKeep = pMat[nCntMat].MatD3D;	//保存

			if (g_Princess.bDamege)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = 255;	//赤色変更
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D = matKeep;	//色を戻す

			// 描画
			g_Princess.aModel[nCntPrincess].pMesh->DrawSubset(nCntMat);	//格子状態
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// 姫情報の取得
//=============================================================================
PRINCESS *GetPrincess(void)
{
	return &g_Princess;
}

//=============================================================================
//	モーションの再生
//=============================================================================
void PrincessMotion(void)
{
	//パーツ数分
	for (int nCntMotion = 0; nCntMotion < g_NumPrincessParts; nCntMotion++)
	{
		KEY *pKey, *pNextKey;
		int nNextKey = g_Princess.nCntKeyPrincess + 1;


		if (nNextKey >= g_aPrincessMotionInfo[g_Princess.state].nNumKey)
		{
			if (g_aPrincessMotionInfo[g_Princess.state].bLoop)
			{
				nNextKey = 0;
			}
			else
			{
				g_Princess.state = PRINCESSSTATE_NEUTRAL;
				g_Princess.nCntFrame = 0;
				g_Princess.nCntKeyPrincess = 0;
			}
		}
		pKey = &g_aPrincessMotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].Key[nCntMotion];		//現在のキー	
		pNextKey = &g_aPrincessMotionInfo[g_Princess.state].KeyInfo[nNextKey].Key[nCntMotion];	//次のキー		

		//回転
		g_Princess.aModel[nCntMotion].rotMove = (pNextKey->rot - pKey->rot)*((float)g_Princess.nCntFrame / (float)g_aPrincessMotionInfo[g_Princess.state].KeyInfo[g_Princess.nCntKeyPrincess].nFrame);

		g_Princess.aModel[nCntMotion].rot = pKey->rot + g_Princess.aModel[nCntMotion].rotMove;
	}
}

//=============================================================================
// 連れ歩き
//=============================================================================
void MovePrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove)
{
	float fDistX, fDistZ, fDistans;
	WAVE *pWave = GetWave();
	MAGIC *pMagic = GetMagic();
	pMagic->bUse = false;
	pWave->bUse = false;

	fDistX = pPos->x - g_Princess.pos.x;
	fDistZ = pPos->z - g_Princess.pos.z;
	fDistans = (float)sqrt(fDistX * fDistX + fDistZ * fDistZ);

	//移動方向を向く
	g_Princess.rotDest.y = atan2f(fDistX, fDistZ) + D3DX_PI;

	//回転
	if (g_Princess.rotmove.y = g_Princess.rotDest.y - g_Princess.rot.y)
	{
		if (g_Princess.rotmove.y < -D3DX_PI)
		{
			g_Princess.rotmove.y += D3DX_PI * 2;
		}
		else if (g_Princess.rotmove.y > D3DX_PI)
		{
			g_Princess.rotmove.y -= D3DX_PI * 2;
		}
	}
	if (g_Princess.rot.y += g_Princess.rotmove.y * PRINCESS_ROTSPEAD)
	{
		if (g_Princess.rot.y > D3DX_PI)
		{
			g_Princess.rot.y -= D3DX_PI * 2;
		}
		else if (g_Princess.rot.y < -D3DX_PI)
		{
			g_Princess.rot.y += D3DX_PI * 2;
		}
	}

	g_Princess.move.x = pMove->x;
	g_Princess.move.z = pMove->z;

	g_Princess.pos.x += g_Princess.move.x;
	g_Princess.pos.z += g_Princess.move.z;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CollisionPrincess(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float Radius, int nType)
{
	bool bLand = false;
	float fDistance = (pPos->x - g_Princess.pos.x)*(pPos->x - g_Princess.pos.x) + (pPos->z - g_Princess.pos.z)*(pPos->z - g_Princess.pos.z);	//2点間の距離
	float fLength = (Radius + g_Princess.fRadius)*(Radius + g_Princess.fRadius);	//半径
	float fDiff = (Radius + g_Princess.fRadius) - (float)sqrt(fDistance);	//差
	D3DXVECTOR3 vec = D3DXVECTOR3((pPos->x - g_Princess.pos.x), 0.0f, (pPos->z - g_Princess.pos.z));	//ベクトル
	D3DXVec3Normalize(&vec, &vec);

	if (fDistance <= fLength)
	{
		if (nType == 1)
		{	//敵が近くにいたらピンチ
			g_Princess.bPinch = true;
		}
		bLand = true;
		pPos->x = pPos->x + vec.x * fDiff;
		pPos->z = pPos->z + vec.z * fDiff;
	}

	return bLand;
}

//=============================================================================
//	ファイルの読み込み
//=============================================================================
void LoadPrincess(void)
{
	//ファイルポイント
	FILE *pFile;

	//変数宣言
	char *pStrCur = NULL;	//文字列の先頭へのポインタ
	char cLine[MAX_CHARA];	//文字列読み込み用
	char cStr[MAX_CHARA];	//文字列抜き出し用
	char *cPos;				//文字区切り用
	int nCnt = 0;			//空白カウンター
	int nCntModel = 0;		//モデル番号
	int nCntMotion = 0;		//モーション番号
	int nCntKey = 0;		//現在キー
	int nCntParts;			//パーツ番号

	pFile = fopen("data/motion_Princess.txt", "r");

	//開けた
	if (pFile != NULL)
	{
		while (memcmp(cStr, "SCRIPT", strlen("SCRIPT")) != 0)	//指定の文字列を見つけるまで読み込む
		{
			//テキスト1行読み込むのに↓3行1セットが必要
			fgets(cLine, MAX_CHARA, pFile);	//1行256文字読み込み
			pStrCur = &cLine[0];			//文字数整理
			strcpy(cStr, pStrCur);			//pStrCurで読み取った文字をcStrに格納
		}
		while (memcmp(cStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0)
		{
			fgets(cLine, MAX_CHARA, pFile);
			pStrCur = &cLine[0];
			strcpy(cStr, pStrCur);
			//読み込むモデル数
			if (memcmp(cStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				pStrCur += strlen("NUM_MODEL = ");	//文字列の自動計算
				strcpy(cStr, pStrCur);
				g_NumPrincess = atoi(cStr);	//atoi　char型をint型に変換する
			}

			//モデルファイル名の読み込み
			if (memcmp(cStr, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
			{
				for (int nCntStr = 0; nCntStr < MAX_CHARA; nCntStr++)
				{
					if (cStr[nCntStr] == '\t')	//TABを見つけたらそこで文字列を切る
					{
						cStr[nCntStr] = '\0';
						strcpy(pStrCur, cStr);
					}
				}
				pStrCur += strlen("MODEL_FILENAME = ");
				strcpy(g_PrincessFileNeme[nCntModel], pStrCur);
				g_Princess.aModel[nCntModel].pFilename = g_PrincessFileNeme[nCntModel];
				nCntModel++;
			}
			//キャラクター情報
			if (memcmp(cStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{
				while (memcmp(cStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0)
				{
					fgets(cLine, MAX_CHARA, pFile);
					nCnt = 0;
					while (1)
					{	//文字の前に存在する空白やタブをカットする
						if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
						{
							nCnt++;
						}
						else
						{
							break;
						}
					}
					pStrCur = &cLine[nCnt];
					strcpy(cStr, pStrCur);
					if (memcmp(&cLine[nCnt], "RADIUS = ", strlen("RADIUS = ")) == 0)
					{	//半径
						pStrCur += strlen("RADIUS = ");
						strcpy(cStr, pStrCur);
						g_Princess.fRadius = (float)atof(cStr);
					}
					if (memcmp(&cLine[nCnt], "LIFE = ", strlen("LIFE = ")) == 0)
					{	//体力
						pStrCur += strlen("LIFE = ");
						strcpy(cStr, pStrCur);
						g_Princess.nLife = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{	//読み込むパーツ数
						pStrCur += strlen("NUM_PARTS = ");
						strcpy(cStr, pStrCur);
						g_NumPrincessParts = atoi(cStr);
					}
					//キャラパーツ情報
					if (memcmp(&cLine[nCnt], "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (memcmp(&cLine[nCnt], "END_PARTSSET", strlen("END_PARTSSET")) != 0)
						{
							fgets(cLine, MAX_CHARA, pFile);
							nCnt = 0;
							while (1)
							{
								if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
								{
									nCnt++;
								}
								else
								{
									break;
								}
							}
							pStrCur = &cLine[nCnt];
							strcpy(cStr, pStrCur);

							if (memcmp(&cLine[nCnt], "INDEX = ", strlen("INDEX = ")) == 0)
							{	//これは何番のパーツか
								pStrCur += strlen("INDEX = ");
								strcpy(cStr, pStrCur);
								nCntParts = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "PARENT = ", strlen("PARENT = ")) == 0)
							{	//親の番号
								pStrCur += strlen("PARENT = ");
								strcpy(cStr, pStrCur);
								g_Princess.aModel[nCntParts].nIdxModelParent = atoi(cStr);
							}
							if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
							{	//位置
								pStrCur += strlen("POS = ");
								strcpy(cStr, pStrCur);

								cPos = strtok(cStr, " ");	//文字を空白ごとに区切って分解
								g_Princess.aModel[nCntParts].pos.x = (float)atof(cPos);

								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].pos.y = (float)atof(cPos);

								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].pos.z = (float)atof(cPos);
							}
							if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
							{	//角度
								pStrCur += strlen("ROT = ");
								strcpy(cStr, pStrCur);
								cPos = strtok(cStr, " ");
								g_Princess.aModel[nCntParts].rot.x = (float)atof(cPos);
								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].rot.y = (float)atof(cPos);
								cPos = strtok(NULL, " ");
								g_Princess.aModel[nCntParts].rot.z = (float)atof(cPos);
							}
						}
					}
				}
			}
			//モーション情報
			if (memcmp(cStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				nCntKey = 0;	//キーカウンターリセット
				while (memcmp(&cLine[nCnt], "END_MOTIONSET", strlen("END_MOTIONSET")) != 0)
				{
					fgets(cLine, MAX_CHARA, pFile);
					nCnt = 0;
					while (1)
					{
						if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
						{
							nCnt++;
						}
						else
						{
							break;
						}
					}
					pStrCur = &cLine[nCnt];
					strcpy(cStr, pStrCur);

					if (memcmp(&cLine[nCnt], "LOOP = ", strlen("LOOP = ")) == 0)
					{	//ループの有無
						pStrCur += strlen("LOOP = ");
						strcpy(cStr, pStrCur);
						g_aPrincessMotionInfo[nCntMotion].bLoop = atoi(cStr);
					}
					if (memcmp(&cLine[nCnt], "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{	//キー数
						pStrCur += strlen("NUM_KEY = ");
						strcpy(cStr, pStrCur);
						g_aPrincessMotionInfo[nCntMotion].nNumKey = atoi(cStr);
					}
					//キー情報
					if (memcmp(&cLine[nCnt], "KEYSET", strlen("KEYSET")) == 0)
					{
						nCntParts = 0;	//パーツカウンターリセット
						while (memcmp(&cLine[nCnt], "END_KEYSET", strlen("END_KEYSET")) != 0)
						{
							fgets(cLine, MAX_CHARA, pFile);
							nCnt = 0;
							while (1)
							{
								if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
								{
									nCnt++;
								}
								else
								{
									break;
								}
							}
							pStrCur = &cLine[nCnt];
							strcpy(cStr, pStrCur);

							if (memcmp(&cLine[nCnt], "FRAME = ", strlen("FRAME = ")) == 0)
							{	//フレーム数
								pStrCur += strlen("FRAME = ");
								strcpy(cStr, pStrCur);
								g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
							}
							//キー要素
							if (memcmp(&cLine[nCnt], "KEY", strlen("KEY")) == 0)
							{
								while (memcmp(&cLine[nCnt], "END_KEY", strlen("END_KEY")) != 0)
								{
									fgets(cLine, MAX_CHARA, pFile);
									nCnt = 0;
									while (1)
									{
										if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
										{
											nCnt++;
										}
										else
										{
											break;
										}
									}
									pStrCur = &cLine[nCnt];
									strcpy(cStr, pStrCur);

									if (memcmp(&cLine[nCnt], "POS = ", strlen("POS = ")) == 0)
									{	//位置
										pStrCur += strlen("POS = ");
										strcpy(cStr, pStrCur);
										cPos = strtok(cStr, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.x = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.y = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos.z = (float)atof(cPos);
									}
									if (memcmp(&cLine[nCnt], "ROT = ", strlen("ROT = ")) == 0)
									{	//角度
										pStrCur += strlen("ROT = ");
										strcpy(cStr, pStrCur);
										cPos = strtok(cStr, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.x = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.y = (float)atof(cPos);
										cPos = strtok(NULL, " ");
										g_aPrincessMotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot.z = (float)atof(cPos);
									}
								}
								nCntParts++;	//次のパーツへ
							}
						}
						nCntKey++;	//次のキーへ
					}
				}
				nCntMotion++;	//次のモーションへ
			}
		}
	}
}
