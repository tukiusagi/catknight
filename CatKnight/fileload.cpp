//=============================================================================
//
// �t�@�C�����[�h���� [fileload.cpp]
// Author : KANAZAWA TSUKIHO
//
//=============================================================================
#include "fileload.h"

//=============================================================================
// �ǂݍ��ތ��t�ꗗ
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
// �}�N����`
//=============================================================================
#define SCRIPT				"SCRIPT"			//�ǂݍ��݊J�n
#define END_SCRIPT			"END_SCRIPT"		//�ǂݍ��ݏI��
#define NUM_MODEL			"NUM_MODEL = "		//���f����
#define MODEL_FILENAME		"MODEL_FILENAME = "	//���f���t�@�C����

#define CHARACTERSET		"CHARACTERSET"		//�L�����N�^�[���
#define END_CHARACTERSET	"END_CHARACTERSET"	//�L�������I���
#define MOVE				"MOVE = "			//�ړ����x
#define ATTACK				"ATTACK = "			//�U����
#define JUMP				"JUMP = "			//�W�����v��
#define RADIUS				"RADIUS = "			//���a
#define LIFE				"LIFE = "			//�̗�
#define NUM_PARTS			"NUM_PARTS = "		//�p�[�c��
	
#define PARTSSET			"PARTSSET"			//�p�[�c���
#define END_PARTSSET		"END_PARTSSET"		//�p�[�c���I���
#define INDEX				"INDEX = "			//�p�[�c�ԍ�
#define PARENT				"PARENT = "			//�e�̔ԍ�
#define POS					"POS = "			//�ʒu
#define ROT					"ROT = "			//�p�x

#define MOTIONSET			"MOTIONSET"			//���[�V�������
#define END_MOTIONSET		"END_MOTIONSET"		//���[�V�������I���
#define LOOP				"LOOP = "			//���[�v�̗L��
#define NUM_KEY				"NUM_KEY = "		//�L�[��
#define COLLISION			"COLLISION = "		//�����蔻��
#define ORBIT				"ORBIT = "			//�O��
#define KEYSET				"KEYSET"			//�L�[���
#define END_KEYSET			"END_KEYSET"		//�L�[���I���
#define FRAME				"FRAME = "			//�t���[����

#define KEY					"KEY"				//�p�[�c���L�[���
#define END_KEY				"END_KEY"			//�p�[�c���L�[���I���

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
static LOAD g_Load[CHARA_MAX];
char g_LoadWord[MAX_WORD][MAX_CHARA] = { SCRIPT ,END_SCRIPT ,NUM_MODEL ,MODEL_FILENAME ,
										CHARACTERSET ,MOVE ,ATTACK,JUMP ,RADIUS ,LIFE ,NUM_PARTS ,END_CHARACTERSET ,
										PARTSSET ,INDEX ,PARENT ,POS ,ROT,END_PARTSSET ,
										MOTIONSET ,LOOP ,NUM_KEY ,COLLISION ,ORBIT,END_MOTIONSET ,
										KEYSET ,FRAME  ,KEY ,END_KEY,END_KEYSET };
char *pStrCur = NULL;	//������̐擪�ւ̃|�C���^

//=============================================================================
//	���[�h�f�[�^�̎擾
//=============================================================================
LOAD *GetLoad(int nCntType)
{
	return &g_Load[nCntType];
}

//=============================================================================
//	��s�ǂݍ���
//=============================================================================
void LoadLine(FILE *pFile, char *cLine, char *cStr)
{
	int nCnt = 0;	//�󔒃J�E���^�[

	fgets(cLine, MAX_CHARA, pFile);	//1�s256�����ǂݍ���
	while (1)
	{	//�����̑O�ɑ��݂���󔒂�^�u���J�b�g����
		if (cLine[nCnt] == ' ' || cLine[nCnt] == '\t')
		{
			nCnt++;
		}
		else
		{
			break;
		}
	}
	pStrCur = &cLine[nCnt];	//����������
	strcpy(cStr, pStrCur);	//pStrCur�œǂݎ����������cStr�Ɋi�[
}

//=============================================================================
//	TAB�ȉ����J�b�g����
//=============================================================================
void CutTub( char *cStr)
{
	for (int nCntStr = 0; nCntStr < MAX_CHARA; nCntStr++)
	{
		if (cStr[nCntStr] == '\t')	//TAB���������炻���ŕ������؂�
		{
			cStr[nCntStr] = '\0';
			strcpy(pStrCur, cStr);
			break;
		}
	}

}

//=============================================================================
//	�t�@�C���ǂݍ���
//=============================================================================
void LoadFile(char *FileName,int nCharaType)
{
	//�t�@�C���|�C���g
	FILE *pFile;

	//�ϐ��錾
	char cLine[MAX_CHARA];	//������ǂݍ��ݗp
	char cStr[MAX_CHARA];	//�����񔲂��o���p
	int nCntMotion = 0;		//���[�V�����ԍ�
	int nCntParts = 0;	//�p�[�c�ԍ�

	pFile = fopen(FileName, "r");

	//�J����
	if (pFile != NULL)
	{
		LoadLine(pFile, cLine, cStr);

		while (strncmp(cStr, g_LoadWord[LOAD_SCRIPT], strlen(g_LoadWord[LOAD_SCRIPT])) != 0)	//�w��̕������������܂œǂݍ���
		{
			LoadLine(pFile, cLine, cStr);
		}

		if (strncmp(cStr, g_LoadWord[LOAD_SCRIPT], strlen(g_LoadWord[LOAD_SCRIPT])) == 0)
		{
			while (strncmp(cStr, g_LoadWord[LOAD_END_SCRIPT], strlen(g_LoadWord[LOAD_END_SCRIPT])) != 0)
			{
				LoadLine(pFile, cLine, cStr);
				//�ǂݍ��ރ��f����
				if (strncmp(cStr, g_LoadWord[LOAD_NUM_MODEL], strlen(g_LoadWord[LOAD_NUM_MODEL])) == 0)
				{
					pStrCur += strlen(g_LoadWord[LOAD_NUM_MODEL]);	//������̎����v�Z
					strcpy(cStr, pStrCur);
					g_Load[nCharaType].nNumModel = atoi(cStr);	//atoi�@char�^��int�^�ɕϊ�����
				}
				//���f���t�@�C�����̓ǂݍ���
				if (strncmp(cStr, g_LoadWord[LOAD_MODEL_FILENAME], strlen(g_LoadWord[LOAD_MODEL_FILENAME])) == 0)
				{
					CutTub(cStr);
					pStrCur += strlen(g_LoadWord[LOAD_MODEL_FILENAME]);
					strcpy(g_Load[nCharaType].cFileNeme[nCntParts], pStrCur);
					nCntParts++;
				}
				//�L�����N�^�[���
				if (strncmp(cStr, g_LoadWord[LOAD_CHARACTERSET], strlen(g_LoadWord[LOAD_CHARACTERSET])) == 0)
				{
					LoadChara(pFile, cLine, cStr, nCharaType);
				}
				//���[�V�������
				if (strncmp(cStr, g_LoadWord[LOAD_MOTIONSET], strlen(g_LoadWord[LOAD_MOTIONSET])) == 0)
				{
					LoadMotion(pFile, cLine, cStr, nCntMotion, nCharaType);
					nCntMotion++;	//���̃��[�V������
				}//MOTIONSET
			}//END_SCRIPT
		}//SCRIPT
	}//�t�@�C�����J������
}

//=============================================================================
//	�L�������
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
					//�ړ���
					g_Load[nCharaType].fSpeed = (float)atof(cStr);	//atof�@char�^��float�^�ɕϊ�����
					break;
				case LOAD_ATTACK:
					g_Load[nCharaType].nAttack = atoi(cStr);
					break;
				case LOAD_RADIUS:
					//���a
					g_Load[nCharaType].fRadius = (float)atof(cStr);
					break;
				case LOAD_LIFE:
					//�̗�
					g_Load[nCharaType].nMaxLife = atoi(cStr);
					break;
				case LOAD_NUM_PARTS:
					//�p�[�c��
					g_Load[nCharaType].nNumParts = atoi(cStr);
					break;
				default:
					break;
				}
				nLoadCount++;
			}
		}
		//�L�����p�[�c���
		if (strncmp(pStrCur, g_LoadWord[LOAD_PARTSSET], strlen(g_LoadWord[LOAD_PARTSSET])) == 0)
		{
			LoadParts(pFile,cLine,cStr, nCharaType);
		}// PARTSSET
	}// END_CHARACTERSET
}

//=============================================================================
//	�p�[�c���
//=============================================================================
void LoadParts(FILE *pFile, char *cLine, char *cStr, int nCharaType)
{
	int nCntParts;
	char *cPunctuation = NULL;		//������؂�p
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
					//�p�[�c�ԍ�
					nCntParts = atoi(cStr);
					break;
				case LOAD_PARENT:
					//�e�̔ԍ�
					g_Load[nCharaType].nParent[nCntParts] = atoi(cStr);
					break;
				case LOAD_POS:
					//�ʒu
					LoadXYZ(&g_Load[nCharaType].pos[nCntParts], cStr, cPunctuation);
					break;
				case LOAD_ROT:
					//�p�x
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
//	���[�V�������
//=============================================================================
void LoadMotion(FILE *pFile, char *cLine, char *cStr,int nCntMotion, int nCharaType)
{
	int nCntParts;			//�p�[�c�ԍ�
	int nLoadMotion = LOAD_LOOP;
	int nCntKey = 0;		//�L�[�J�E���^�[���Z�b�g
	char *cPunctuation = NULL;		//������؂�p

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
					//���[�v�L��
					g_Load[nCharaType].MotionInfo[nCntMotion].bLoop = atoi(cStr);
					break;
				case LOAD_NUM_KEY:
					//�L�[��
					g_Load[nCharaType].MotionInfo[nCntMotion].nNumKey = atoi(cStr);
					break;
				case LOAD_COLLISION:
					//�����蔻��
					//�I�t�Z�b�g�l
					LoadXYZ(&g_Load[nCharaType].Collpos, cStr, cPunctuation);
					//�e�̔ԍ�
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nCollParent = atoi(cPunctuation);
					//���a
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].fCollRadius = (float)atof(cPunctuation);
					//�����蔻����J�n����t���[����
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nStart = atoi(cPunctuation);
					//�I������t���[����
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nEnd = atoi(cPunctuation);
					break;
				case LOAD_ORBIT:
					//�I�t�Z�b�g�l
					LoadXYZ(&g_Load[nCharaType].Meshpos[0], cStr, cPunctuation);
					//�e�̔ԍ�
					cPunctuation = strtok(NULL, " ");
					g_Load[nCharaType].nMeshParent = atoi(cPunctuation);
					break;
				default:
					break;
				}
				nLoadMotion++;
			}
		}
		//�L�[���
		if (strncmp(pStrCur, g_LoadWord[LOAD_KEYSET], strlen(g_LoadWord[LOAD_KEYSET])) == 0)
		{
			int CountKey = LOAD_FRAME;
			nCntParts = 0;	//�p�[�c�J�E���^�[���Z�b�g
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
							//�t���[����
							g_Load[nCharaType].MotionInfo[nCntMotion].KeyInfo[nCntKey].nFrame = atoi(cStr);
							break;
						case LOAD_KEY:
							//�L�[�v�f
							LoadKey(pFile, cLine, cStr, nCntMotion, nCntKey, nCntParts, nCharaType);
							CountKey = LOAD_FRAME;
							nCntParts++;	//���̃p�[�c��
						default:
							break;
						}
						CountKey++;
					}
				}
			}//END_KEYSET
			nCntKey++;	//���̃L�[��
		}//KEYSET
	}//END_MOTIONSET
}

//=============================================================================
//	�L�[�v�f
//=============================================================================
void LoadKey(FILE *pFile, char *cLine, char *cStr, int nCntMotion,int nCntKey,int nCntParts, int nCharaType)
{
	char *cPunctuation = NULL;		//������؂�p

	while (strncmp(pStrCur, g_LoadWord[LOAD_END_KEY], strlen(g_LoadWord[LOAD_END_KEY])) != 0)
	{
		LoadLine(pFile,  cLine, cStr);

		if (strncmp(pStrCur, g_LoadWord[LOAD_POS], strlen(g_LoadWord[LOAD_POS])) == 0)
		{	//�ʒu
			pStrCur += strlen(g_LoadWord[LOAD_POS]);
			strcpy(cStr, pStrCur);
			LoadXYZ(&g_Load[nCharaType].MotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].pos, cStr, cPunctuation);
		}
		if (strncmp(pStrCur, g_LoadWord[LOAD_ROT], strlen(g_LoadWord[LOAD_ROT])) == 0)
		{	//�p�x
			pStrCur += strlen(g_LoadWord[LOAD_ROT]);
			strcpy(cStr, pStrCur);
			LoadXYZ(&g_Load[nCharaType].MotionInfo[nCntMotion].KeyInfo[nCntKey].Key[nCntParts].rot, cStr, cPunctuation);
		}
	}//END_KEY

}

//=============================================================================
//	�ʒu�p�x�ǂݍ���
//=============================================================================
void LoadXYZ(D3DXVECTOR3 *xyz, char *cStr, char *cPunctuation)
{
	cPunctuation = strtok(cStr, " ");	//�������󔒂��Ƃɋ�؂��ĕ���
	xyz->x = (float)atof(cPunctuation);

	cPunctuation = strtok(NULL, " ");
	xyz->y = (float)atof(cPunctuation);

	cPunctuation = strtok(NULL, " ");
	xyz->z = (float)atof(cPunctuation);
}