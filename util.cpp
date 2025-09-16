//=====================================================================
// 
// �ėp�֐� [util.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** �C���N���[�h�t�@�C�� *****
// 
//*********************************************************************
#include "util.h"

//=====================================================================
// ������͈͓��Ɏ��߂鏈��
//=====================================================================
int Clamp(int nNum, int nMin, int nMax)
{
	if (nNum < nMin)
	{
		return nMin;
	}
	else if (nNum > nMax)
	{
		return nMax;
	}

	return nNum;
}

//=====================================================================
// ������͈͓��Ɏ��߂鏈���i�|�C���^���璼�ڕҏW�j
//=====================================================================
void Clamp(int* pNum, int nMin, int nMax)
{
	if (*pNum < nMin)
	{
		*pNum = nMin;
	}
	else if (*pNum > nMax)
	{
		*pNum = nMax;
	}
}

//=====================================================================
// ������͈͓��Ɏ��߂鏈��
//=====================================================================
int Clampf(float fNum, float fMin, float fMax)
{
	if (fNum < fMin)
	{
		return fMin;
	}
	else if (fNum > fMax)
	{
		return fMax;
	}

	return fNum;
}

//=====================================================================
// ������͈͓��Ɏ��߂鏈���i�|�C���^���璼�ڕҏW�j
//=====================================================================
void Clampf(float* pNum, float fMin, float fMax)
{
	if (*pNum < fMin)
	{
		*pNum = fMin;
	}
	else if (*pNum > fMax)
	{
		*pNum = fMax;
	}
}

//=====================================================================
// �x�N�g���̓���
//=====================================================================
float Dot(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

//=====================================================================
// �x�N�g���̐��K������
//=====================================================================
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector)
{
	float fMagnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);

	return D3DXVECTOR3(vector.x / fMagnitude, vector.y / fMagnitude, vector.z / fMagnitude);
}

//=====================================================================
// �����x�N�g���i��̃x�N�g������j
//=====================================================================
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return Normalize(to - from);
}

//=====================================================================
// �����x�N�g���i�p�x����j
//=====================================================================
D3DXVECTOR3 Direction(float fAngle)
{
	return D3DXVECTOR3(sin(fAngle), cos(fAngle), 0.0f);
}

//=====================================================================
// ��̃x�N�g���̑傫���i�����j�����߂鏈��
//=====================================================================
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	D3DXVECTOR3 fDistance = to - from;

	return sqrtf(fDistance.x * fDistance.x + fDistance.y * fDistance.y);
}

//=====================================================================
// �x�N�g������ʂ̃x�N�g���܂ł̊p�x�����߂鏈��
//=====================================================================
float Angle(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return atan2f(to.x - from.x, to.y - from.y);
}

//=====================================================================
// 3�����x�N�g������2�����x�N�g���ւ̕ϊ�����
//=====================================================================
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source)
{
	return D3DXVECTOR2(source.x, source.y);
}

//=====================================================================
// 2�����x�N�g������3�����x�N�g���ւ̕ϊ�����
//=====================================================================
D3DXVECTOR3 Vector2To3(D3DXVECTOR3 source, float fValueZ)
{
	return D3DXVECTOR3(source.x, source.y, fValueZ);
}

//=====================================================================
// �w�肵���͈̗͂����i�����j���擾���鏈��
//=====================================================================
int RandRange(int nMin, int nMax)
{
	return (rand() % (nMax - nMin)) + nMin;
}

//=====================================================================
// �o�C�i���t�@�C���ǂݍ��ݏ���
//=====================================================================
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "rb");

	if (pFile != NULL)
	{
		fread(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================================
// �o�C�i���t�@�C�������o������
//=====================================================================
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "wb");

	if (pFile != NULL)
	{
		fwrite(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}
