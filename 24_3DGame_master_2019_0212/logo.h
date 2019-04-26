//=============================================================================
//
// ロゴ処理[logo.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// ロゴの種類
//=============================================================================
typedef enum
{
	LOGO_TITLE,		//タイトル
	LOGO_CLEAR,		//クリア
	LOGO_GAMEOVER,	//ゲームオーバー
	LOGO_RESULT,	//リザルト
	LOGO_RANKING,	//ランキング
	LOGO_MAX
} LOGOTYPE;

//=============================================================================
// プロトタイプ宣言(関数の型式を確認)
//=============================================================================
void InitLogo(void);		//初期化
void UninitLogo(void);		//終了処理
void UpdateLogo(void);		//更新処理
void DrawLogo(void);		//描画処理

void SetLogo(LOGOTYPE nType);
void DeleteLogo(void);
#endif // !_LOGO_H_