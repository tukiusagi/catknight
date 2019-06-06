//=============================================================================
//
// ファイルロード処理 [fileload.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "fileload.h"

//=============================================================================
// 読み込む言葉一覧
//=============================================================================
typedef enum
{
	LOAD_SCRIPT,
	LOAD_END_SCRIPT,
	LOAD_NUM_MODEL,
	LOAD_MODEL_FILENAME,
	LOAD_CHARACTERSET,
	LOAD_MOVE,
	LOAD_ATTACK,
	LOAD_JUMP,
	LOAD_RADIUS,
	LOAD_LIFE,
	LOAD_NUM_PARTS,
	LOAD_END_CHARACTERSET,
	LOAD_PARTSSET,
	LOAD_INDEX,
	LOAD_PARENT,
	LOAD_POS,
	LOAD_ROT,
	LOAD_END_PARTSSET,
	LOAD_MOTIONSET,
	LOAD_LOOP,
	LOAD_NUM_KEY,
	LOAD_COLLISION,
	LOAD_ORBIT,
	LOAD_END_MOTIONSET,
	LOAD_KEYSET,
	LOAD_FRAME,
	LOAD_KEY,
	LOAD_END_KEY,
	LOAD_END_KEYSET,
	MAX_WORD
}LOAD_WORD;

//=============================================================================
// マクロ定義
//=============================================================================
#define SCRIPT				"SCRIPT"			//読み込み開始
#define END_SCRIPT			"END_SCRIPT"		//読み込み終了
#define NUM_MODEL			"NUM_MODEL = "		//モデル数
#define MODEL_FILENAME		"MODEL_FILENAME = "	//モデルファイル名

#define CHARACTERSET		"CHARACTERSET"		//キャラクター情報
#define END_CHARACTERSET	"END_CHARACTERSET"	//キャラ情報終わり
#define MOVE				"MOVE = "			//移動速度
#define ATTACK				"ATTACK = "			//攻撃力
#define JUMP				"JUMP = "			//ジャンプ力
#define RADIUS				"RADIUS = "			//半径
#define LIFE				"LIFE = "			//体力
#define NUM_PARTS			"NUM_PARTS = "		//パーツ数
	
#define PARTSSET			"PARTSSET"			//パーツ情報
#define END_PARTSSET		"END_PARTSSET"		//パーツ情報終わり
#define INDEX				"INDEX = "			//パーツ番号
#define PARENT				"PARENT = "			//親の番号
#define POS					"POS = "			//位置
#define ROT					"ROT = "			//角度

#define MOTIONSET			"MOTIONSET"			//モーション情報
#define END_MOTIONSET		"END_MOTIONSET"		//モーション情報終わり
#define LOOP				"LOOP = "			//ループの有無
#define NUM_KEY				"NUM_KEY = "		//キー数
#define COLLISION			"COLLISION = "		//当たり判定
#define ORBIT				"ORBIT = "			//軌跡
#define KEYSET				"KEYSET"			//キー情報
#define END_KEYSET			"END_KEYSET"		//キー情報終わり
#define FRAME				"FRAME = "			//フレーム数

#define KEY					"KEY"				//パーツ毎キー情報
#define END_KEY				"END_KEY"			//パーツ毎キー情報終わり

//=============================================================================
// グローバル変数
//=============================================================================
static LOAD g_Load[CHARA_MAX];
char g_LoadWord[MAX_WORD][MAX_CHARA] = { SCRIPT ,END_SCRIPT ,NUM_MODEL ,MODEL_FILENAME ,
										CHARACTERSET ,MOVE ,ATTACK,JUMP ,RADIUS ,LIFE ,NUM_PARTS ,END_CHARACTERSET ,
										PARTSSET ,INDEX ,PARENT ,POS ,ROT,END_PARTSSET ,
										MOTIONSET ,LOOP ,NUM_KEY ,COLLISION ,ORBIT,END_MOTIONSET ,
										KEYSET ,FRAME  ,KEY ,END_KEY,END_KEYSET };
char *pStrCur = NULL;	//文字列の先頭へのポインタ

//=============================================================================
//	ロードデータの取得
//=============================================================================
LOAD *GetLoad(int nCntType)
{
	return &g_Load[nCntType];
}

//=============================================================================
//	一行読み込み
//=============================================================================
void LoadLine(FILE *pFile, char *cLine, char *cStr)
{
	int nCnt = 0;	//空白カウンター

	fgets(cLine, MAX_CHARA, pFile);	//1行256文字読み込み
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
	pStrCur = &cLine[nCnt];	//文字数整理
	strcpy(cStr, pStrCur);	//pStrCurで読み取った文字をcStrに格納
}

//=============================================================================
//	TAB以下をカットする
//=============================================================================
void CutTub( char *cStr)
{
	for (int nCntStr = 0; nCntStr < MAX_CHARA; nCntStr++)
	{
		if (cStr[nCntStr] == '\t')	//TABを見つけたらそこで文字列を切る
		{
			cStr[nCntStr] = '\0';
			strcpy(pStrCur, cStr);
			break;
		}
	}

}

//=============================================================================
//	ファイル読み込み
//=============================================================================
void LoadFile(char *FileName,int nCharaType)
{
	//ファイルポイント
	FILE *pFile;

	//変数宣言
	char cLine[MAX_CHARA];	//文字列読み込み用
	char cStr[MAX_CHARA];	//文字列抜き出し用
	int nCntMotion = 0;		//モーション番号
	int nCntParts = 0;	//パーツ番号

	pFile = fopen(FileName, "r");

	//開けた
	if (pFile != NULL)
	{
		LoadLine(pFile, cLine, cStr);

		while (strncmp(cStr, g_LoadWord[LOAD_SCRIPT], strlen(g_LoadWord[LOAD_SCRIPT])) != 0)	//指定の文字列を見つけるまで読み込む
		{
			LoadLine(pFile, cLine, cStr);
		}

		if (strncmp(cStr, g_LoadWord[LOAD_SCRIPT], strlen(g_LoadWord[LOAD_SCRIPT])) == 0)
		{
			while (strncmp(cStr, g_LoadWord[LOAD_END_SCRIPT], strlen(g_LoadWord[LOAD_END_SCRIPT])) != 0)
			{
				LoadLine(pFile, cLine, cStr);
				//読み込むモデル数
				if (strncmp(cStr, g_LoadWord[LOAD_NUM_MODEL], strlen(g_LoadWord[LOAD_NUM_MODEL])) == 0)
				{
					pStrCur += strlen(g_LoadWord[LOAD_NUM_MODEL]);	//文字列の自動計算
					strcpy(cStr, pStrCur);
					g_Load[nCharaType].nNumModel = atoi(cStr);	//atoi　char型をint型に変換する
				}
				//モデルファイル名の読み込み
				if (strncmp(cStr, g_LoadWord[LOAD_MODEL_FILENAME], strlen(g_LoadWord[LOAD_MODEL_FILENAME])) == 0)
				{
					CutTub(cStr);
					pStrCur += strlen(g_LoadWord[LOAD_MODEL_FILENAME]);
					strcpy(g_Load[nCharaType].cFileNeme[nCntParts], pStrCur);
					nCntParts++;
				}
				//キャラクター情報
				if (strncmp(cStr, g_LoadWord[LOAD_CHARACTERSET], strlen(g_LoadWord[LOAD_CHARACTERSET])) == 0)
				{
					LoadChara(pFile, cLine, cStr, nCharaType);
				}
				//モーション情報
				if (strncmp(cStr, g_LoadWord[LOAD_MOTIONSET], strlen(g_LoadWord[LOAD_MOTIONSET])) == 0)
				{
					LoadMotion(pFile, cLine, cStr, nCntMotion, nCharaType);
					nCntMotion++;	//次のモーションへ
				}//MOTIONSET
			}//END_SCRIPT
		}//SCRIPT
	}//ファイルが開いたら
}

//=============================================================================
//	キャラ情報
//=============================================================================
void LoadChara(FILE *pFile, char *cLine, char *cStr,int nCharaType)
{
	int nLoadCount = LOAD_MOVE;

	while (strncmp(cStr, g_LoadWord[LOAD_END_CHARACTERSET], strlen(g_LoadWord[LOAD_END_CHARACTERSET])) != 0)
	{
		LoadLine(pFile, cLine, cStr);

		if (nLoadCount < LOAD_END_CHARACTERSET)
		{
			if (strncmp(pStrCur, g_LoadWord[nLoadCount], strlen(g_LoadWord[nLoadCount])) == 0)
			{
				pStrCur += strlen(g_LoadWord[nLoadCount]);
				strcpy(cStr, pStrCur);
				switch (nLoadCount)
				{
				case LOAD_MOVE:
					//移動量
					g_Load[nCharaType].fSpeed = (float)atof(cStr);	//atof　char型をfloat型に変換する
					break;
				case LOAD_ATTACK:
					g_Load[nCharaType].nAttack = atoi(cStr);
					break;
				case LOAD_RADIUS:
					//半径
					g_Load[nCharaType].fRadius = (float)atof(cStr);
					break;
				case LOAD_LIFE:
					//体力
					g_Load[nCharaType].nMaxLife = atoi(cStr);
					break;
				case LOAD_NUM_PARTS:
					//パーツ数
					g_Load[nCharaType].nNumParts = atoi(cStr);
					break;
				default:
					break;
				}
				nLoadCount++;
			}
		}
		//キャラパーツ情報
		if (strncmp(pStrCur, g_LoadWord[LOAD_PARTSSET], strlen(g_LoadWord[LOAD_PARTSSET])) == 0)
		{
			LoadParts(pFile,cLine,cStr, nCharaType);
		}// PARTSSET
	}// END_CHARACTERSET
}

//=============================================================================
//	パーツ情報
//=============================================================================
void LoadParts(FILE *pFile, char *cLine, char *cStr, int nCharaType)
{
	int nCntParts;
	char *cPunctuation = NULL;		//文字区切り用
	int nLoadCount = LOAD_INDEX;

	while (strncmp(pStrCur, g_LoadWord[LOAD_END_PARTSSET], strlen(g_LoadWord[LOAD_END_PARTSSET])) != 0)
	{
		LoadLine(pFile,  cLine, cStr);

		if (nLoadCount < LOAD_END_PARTSSET)
		{
			if (strncmp(pStrCur, g_LoadWord[nLoadCount], strlen(g_LoadWord[nLoadCount])) == 0)
			{
				pStrCur += strlen(g_LoadWord[nLoadCount]);
				strcpy(cStr, pStrCur);
				switch (nLoadCount)
				{
				case LOAD_INDEX:
					//パーツ番号
					nCntParts = atoi(cStr);
					break;
				case LOAD_PARENT:
					//親の番号
					g_Load[nCharaType].nParent[nCntParts] = atoi(cStr);
					break;
				case LOAD_POS:
					//位置
					LoadXYZ(&g_Load[nCharaType].pos[nCntParts], cStr, cPunctuation);
					break;
				case LOAD_ROT:
					//角度
					LoadXYZ(&g_Load[nCharaType].rot[nCntParts], cStr, cPunctuation);
					break;
				default:
					break;
				}
				nLoadCount++;
			}
		}
	}// END_PARTSSET
}

//=============================================================================
//	モーション情報
//=============================================================================
void LoadMotion(FILE *pFile, char *cLine, char *cStr,int nCntMotion, int nCharaType)
{
	int nCntParts;			//パーツ番号
	int nLoadMotion = LOAD_LOOP;
	int nCntKey = 0;		//キーカウンターリセット
	char *cPunctuation = NULL;		//文字区切り用

	while (strncmp(pStrCur, g_LoadWord[LOAD_END_MOTIONSET], strlen(g_LoadWord[LOAD_END_MOTIONSET])) != 0)
	{
		LoadLine(pFile, cLine, cStr);
		if (nLoadMotion < LOAD_END_MOTIONSET)
		{
			if (strncmp(cStr, g_LoadWord[nLoadMotion], strlen(g_LoadWord[nLoadMotion])) == 0)
			{
				pStrCur += strlen(g_LoadWord[nLoadMotion]);
				strcpy(cStr, pStrCur);
				switch (nLoadMotion)
				{
				case LOAD_LOOP:
					//ループ有無
					g_Load[nCharaType].MotionInfo[nCntMotion].bLoop = atoi(cStr);
					break;
				case LOAD_NUM_KEY:
					//キー数
					g_Load[nCharaType].MotionInfo[nCntMotion].nNumKey = atoi(cStr);
					break;
				case LOAD_COLLISION:
					//当たり判定
					//オフセット値
					LoadXYZ(&g_Load[nCharaType].Collpos, cStr, cPunctuation);
					//親の番号
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nCollParent = atoi(cPunctuation);
					//半径
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].fCollRadius = (float)atof(cPunctuation);
					//当たり判定を開始するフレーム数
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nStart = atoi(cPunctuation);
					//終了するフレーム数
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nEnd = atoi(cPunctuation);
					break;
				case LOAD_ORBIT:
					//オフセット値
					LoadXYZ(&g_Load[nCharaType].Meshpos[0], cStr, cPunctuation);
					//親の番号
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nMeshParent = atoi(cPunctuation);
					break;
				default:
					break;
				}
				nLoadMotion++;
			}
		}
		//キー情報
		if (strncmp(pStrCur, g_LoadWord[LOAD_KEYSET], strlen(g_LoadWord[LOAD_KEYSET])) == 0)
		{
			int CountKey = LOAD_FRAME;
			nCntParts = 0;	//パーツカウンターリセット
			while (strncmp(pStrCur, g_LoadWord[LOAD_END_KEYSET], strlen(g_LoadWord[LOAD_END_KEYSET])) != 0)
			{
				LoadLine(pFile,  cLine, cStr);

				if (CountKey < LOAD_END_KEYSET)
				{
					if (strncmp(cStr, g_LoadWord[CountKey], strlen(g_LoadWord[CountKey])) == 0)
					{
						pStrCur += strlen(g_LoadWord[LOAD_FRAME]);
						strcpy(cStr, pStrCur);
						switch (CountKey)
						{
						case LOAD_FRAME:
							//フレーム数
							g_Load[nCharaType].MotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
							break;
						case LOAD_KEY:
							//キー要素
							LoadKey(pFile, cLine, cStr, nCntMotion, nCntKey, nCntParts, nCharaType);
							CountKey = LOAD_FRAME;
							nCntParts++;	//次のパーツへ
						default:
							break;
						}
						CountKey++;
					}
				}
			}//END_KEYSET
			nCntKey++;	//次のキーへ
		}//KEYSET
	}//END_MOTIONSET
}

//=============================================================================
//	キー要素
//=============================================================================
void LoadKey(FILE *pFile, char *cLine, char *cStr, int nCntMotion,int nCntKey,int nCntParts, int nCharaType)
{
	char *cPunctuation = NULL;		//文字区切り用

	while (strncmp(pStrCur, g_LoadWord[LOAD_END_KEY], strlen(g_LoadWord[LOAD_END_KEY])) != 0)
	{
		LoadLine(pFile,  cLine, cStr);

		if (strncmp(pStrCur, g_LoadWord[LOAD_POS], strlen(g_LoadWord[LOAD_POS])) == 0)
		{	//位置
			pStrCur += strlen(g_LoadWord[LOAD_POS]);
			strcpy(cStr, pStrCur);
			LoadXYZ(&g_Load[nCharaType].MotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos, cStr, cPunctuation);
		}
		if (strncmp(pStrCur, g_LoadWord[LOAD_ROT], strlen(g_LoadWord[LOAD_ROT])) == 0)
		{	//角度
			pStrCur += strlen(g_LoadWord[LOAD_ROT]);
			strcpy(cStr, pStrCur);
			LoadXYZ(&g_Load[nCharaType].MotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot, cStr, cPunctuation);
		}
	}//END_KEY

}

//=============================================================================
//	位置角度読み込み
//=============================================================================
void LoadXYZ(D3DXVECTOR3 *xyz, char *cStr, char *cPunctuation)
{
	cPunctuation = strtok(cStr, " ");	//文字を空白ごとに区切って分解
	xyz->x = (float)atof(cPunctuation);

	cPunctuation = strtok(NULL, " ");
	xyz->y = (float)atof(cPunctuation);

	cPunctuation = strtok(NULL, " ");
	xyz->z = (float)atof(cPunctuation);
}