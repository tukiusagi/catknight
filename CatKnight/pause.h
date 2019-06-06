//=============================================================================
//
// ポーズ処理[pause.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// ポーズの状態 
//=============================================================================
typedef enum
{
	PAUSESTATE_NONE = 0,
	PAUSESTATE_CONTINUE,	//続きから
	PAUSESTATE_RETRY,		//最初から
	PAUSESTATE_QUIT,		//タイトルに戻る
	PAUSESTATE_MAX
}PAUSESTATE;

typedef enum
{
	PAUSE_COLORSTATE_NONE = 0,
	PAUSE_COLORSTATE_CONTINUE,	//続きから
	PAUSE_COLORSTATE_RETRY,		//最初から
	PAUSE_COLORSTATE_QUIT,		//タイトルに戻る
	PAUSE_COLORSTATE_MAX
}PAUSE_COLORSTATE;

//=============================================================================
// プロトタイプ宣言(関数の型式を確認)
//=============================================================================
void InitPause(void);		//初期化
void UninitPause(void);		//終了処理
void UpdatePause(void);		//更新処理
void DrawPause(void);		//描画処理

#endif // !_Pause_H_