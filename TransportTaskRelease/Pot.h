#pragma once

#include <stdio.h>

enum DIRECTION
{
	DIR_NONE  = -1,
	DIR_HOR   = -2,
	DIR_VERT  = -3,
	DIR_LEFT  = -4,
	DIR_RIGHT = -5,
	DIR_UP    = -6,
	DIR_DOWN  = -7
};

class Transport
{
private:
	int m_SourceCount, m_ReceiverCount;

	double *m_SourceSize, *m_ReceiverSize;
	double *m_SourcePotential, *m_ReceiverPotential;

	double **m_Cost;
	double **m_Goods;

	int **m_SearchInfo;

	void GeneratePotentials();

	void ModifyGoods(int a_X, int a_Y);

public:
	Transport(const char *a_FileName);
	~Transport();
	void NWCorner();
	void PotentialSolution();
	void Print(const char *a_FileName);
};