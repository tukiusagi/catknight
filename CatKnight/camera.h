//=============================================================================
//
// カメラ処理 [camera.h]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// カメラの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 posV;			//現在の視点
	D3DXVECTOR3 posR;			//現在の注視点(参照点)
	D3DXVECTOR3 posVDest;		//目的の視点
	D3DXVECTOR3 posRDest;		//目的の注視点(参照点)
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXVECTOR3 moveV;			//視点移動
	D3DXVECTOR3 moveR;			//注視点移動
	D3DXVECTOR3 rot;			//回転
	D3DXVECTOR3 rotDest;		//目的の回転
	D3DXMATRIX  mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX  mtxView;		//ビューマトリックス
	float fLength;				//距離
}CAMERA;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA *GetCamera(void);
#endif