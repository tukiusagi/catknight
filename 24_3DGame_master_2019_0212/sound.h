//=============================================================================
//
// サウンド処理 [sound.h]
// Author : KANAZAWA TUKIHO
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// サウンドファイル
//=============================================================================
typedef enum
{
	SOUND_LABEL_BGM_OP = 0,		// OP
	SOUND_LABEL_BGM_GAME,		// GAME
	SOUND_LABEL_BGM_CLEAR,		//ゲームクリア
	SOUND_LABEL_BGM_GAMEOVER,	//ゲームオーバー
	SOUND_LABEL_SE_SWORD,		//剣
	SOUND_LABEL_SE_NAIL,		//ひっかき
	SOUND_LABEL_SE_DAMAGE,		//被ダメ
	SOUND_LABEL_SE_CHARGE,		//チャージ
	SOUND_LABEL_SE_CHARGEMAX,	//チャージ終了
	SOUND_LABEL_SE_MAGIC,		//魔法
	SOUND_LABEL_SE_BREAK,		//砕ける音
	SOUND_LABEL_SE_CAT,			//にゃーん
	SOUND_LABEL_SE_PAUSE,		// ポーズ音
	SOUND_LABEL_SE_ENTER,		// 決定音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);	//どの音を鳴らす？
void StopSound(SOUND_LABEL label);		//どの音を止める？
void StopSound(void);					//全部の音を止める

#endif
