#include "Pot.h"
#include <fstream>
#include <iostream>
using namespace std;
Transport::Transport(const char *a_FileName)
{
	int i, j;
	ifstream l_Input;
	l_Input.open(a_FileName);

	l_Input >> m_SourceCount;
	l_Input >> m_ReceiverCount;
	m_SourceSize   = new double[m_SourceCount];
	m_ReceiverSize = new double[m_ReceiverCount];

	for (i = 0; i < m_ReceiverCount; i++)
	{
		l_Input >> m_ReceiverSize[i];
	}
	m_Cost  = new double*[m_SourceCount];
	m_Goods = new double*[m_SourceCount];
	for (i = 0; i < m_SourceCount; i++)
	{
		l_Input >> m_SourceSize[i];

		m_Cost [i] = new double[m_ReceiverCount];
		m_Goods[i] = new double[m_ReceiverCount];

		for (j = 0; j < m_ReceiverCount; j++)
		{
			l_Input >> m_Cost[i][j];
			m_Goods[i][j] = -1.0;
		}
	}
}

Transport::~Transport()
{
	int i;
	delete[] m_SourceSize;
	delete[] m_ReceiverSize;

	for (i = 0; i < m_SourceCount; i++)
	{
		delete[] m_Cost[i];
		delete[] m_Goods[i];
	}
	delete[] m_Cost;
	delete[] m_Goods;
}

void Transport::Print(const char *a_FileName)
{
	int i, j;
	ofstream l_Output;
	l_Output.open(a_FileName);

	double l_Result = 0.0;
	for (i = 0; i < m_SourceCount; i++)
		for (j = 0; j < m_ReceiverCount; j++)
			if (m_Goods[i][j] > -1.0)
				l_Result += m_Goods[i][j] * m_Cost[i][j];
	l_Output << "Answer: " << l_Result << endl;
	for (i = 0; i < m_SourceCount; i++)
	{
		for (j = 0; j < m_ReceiverCount; j++)
			if (m_Goods[i][j] < 0.0)
				l_Output <<" "<< 0.0;
			else
				l_Output << " " << m_Goods[i][j];
		l_Output << endl;
	}
}

void Transport::NWCorner()
{
	int i = 0, j = 0;
	double l_SourceRest = m_SourceSize[0], l_ReceiverRest = m_ReceiverSize[0];
	while ((i < m_SourceCount) && (j < m_ReceiverCount))
	{
		if (l_SourceRest > l_ReceiverRest)
		{
			m_Goods[i][j] = l_ReceiverRest;
			l_SourceRest -= l_ReceiverRest;
			j++;
			l_ReceiverRest = m_ReceiverSize[j];
		}
		else
		{
			m_Goods[i][j] = l_SourceRest;
			l_ReceiverRest -= l_SourceRest;
			i++;
			l_SourceRest = m_SourceSize[i];
		}
	}
}

void Transport::GeneratePotentials()
{
	int i, j;
	int l_Dir;
	for (i = 0; i < m_SourceCount; i++)
		for (j = 0; j < m_ReceiverCount; j++)
			m_SearchInfo[i][j] = DIR_NONE;
	i = 0;
	j = 0;
	l_Dir = DIR_HOR;
	m_SourcePotential[0] = 0.0;
	while (1)
	{
		if (l_Dir == DIR_HOR)
		{
			while (j < m_ReceiverCount)
			{
				if ((m_SearchInfo[i][j] == DIR_NONE) && (m_Goods[i][j] > -1.0))
					break;
				j++;
			}
			if (j < m_ReceiverCount)
			{
				m_SearchInfo[i][j] = DIR_HOR;
				m_ReceiverPotential[j] = m_Cost[i][j] - m_SourcePotential[i];

				l_Dir = DIR_VERT;
				i = 0;
				continue;
			}
			else
			{
				j = 0;
				while (j < m_ReceiverCount)
				{
					if (m_SearchInfo[i][j] == DIR_VERT)
						break;
					j++;
				}
				if (j < m_ReceiverCount)
				{
					l_Dir = DIR_VERT;
					continue;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			while (i < m_SourceCount)
			{
				if ((m_SearchInfo[i][j] == DIR_NONE) && (m_Goods[i][j] > -1.0))
					break;
				i++;
			}
			if (i < m_SourceCount)
			{
				m_SearchInfo[i][j] = DIR_VERT;
				m_SourcePotential[i] = m_Cost[i][j] - m_ReceiverPotential[j];

				l_Dir = DIR_HOR;
				j = 0;
				continue;
			}
			else
			{
				i = 0;
				while (i < m_SourceCount)
				{
					if (m_SearchInfo[i][j] == DIR_HOR)
						break;
					i++;
				}
				if (i < m_SourceCount)
				{
					l_Dir = DIR_HOR;
					continue;
				}
				else
				{
					return;
				}
			}
		}
	}
}

void Transport::ModifyGoods(int a_X, int a_Y)
{
	int i, j;
	int l_OptX, l_OptY;
	double l_Min;
	int l_Dir;
	for (i = 0; i < m_SourceCount; i++)
		for (j = 0; j < m_ReceiverCount; j++)
			m_SearchInfo[i][j] = DIR_NONE;
	i = a_Y;
	j = 0;
	l_Dir = DIR_HOR;
	while (1)
	{
		if (l_Dir == DIR_HOR)
		{
			while (j < m_ReceiverCount)
			{
				if ((m_SearchInfo[i][j] == DIR_NONE) && (m_Goods[i][j] > -1.0))
					break;
				j++;
			}
			if (j < m_ReceiverCount)
			{
				m_SearchInfo[i][j] = DIR_HOR;

				if (j == a_X)
					break;

				l_Dir = DIR_VERT;
				i = 0;
				continue;
			}
			else
			{
				j = 0;
				while (j < m_ReceiverCount)
				{
					if (m_SearchInfo[i][j] == DIR_VERT)
						break;
					j++;
				}
				if (j < m_ReceiverCount)
				{
					l_Dir = DIR_VERT;
					continue;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			while (i < m_SourceCount)
			{
				if ((m_SearchInfo[i][j] == DIR_NONE) && (m_Goods[i][j] > -1.0))
					break;
				i++;
			}
			if (i < m_SourceCount)
			{
				m_SearchInfo[i][j] = DIR_VERT;

				l_Dir = DIR_HOR;
				j = 0;
				continue;
			}
			else
			{
				i = 0;
				while (i < m_SourceCount)
				{
					if (m_SearchInfo[i][j] == DIR_HOR)
						break;
					i++;
				}
				if (i < m_SourceCount)
				{
					l_Dir = DIR_HOR;
					continue;
				}
				else
				{
					return;
				}
			}
		}
	}

	m_Goods[a_Y][a_X] = 0.0;
	m_SearchInfo[a_Y][a_X] = DIR_VERT;

	l_Min = -1.0;

	i = 0;
	j = a_X;

	do 
	{
		for (i = 0; i < m_SourceCount; i++)
			if (m_SearchInfo[i][j] == DIR_HOR)
				break;
		if ((m_Goods[i][j] <= l_Min) || (l_Min < 0.0))
		{
			l_Min = m_Goods[i][j];
			l_OptX = j;
			l_OptY = i;
		}
		for (j = 0; j < m_ReceiverCount; j++)
			if (m_SearchInfo[i][j] == DIR_VERT)
				break;
	} while(i != a_Y);

	do 
	{
		for (i = 0; i < m_SourceCount; i++)
			if (m_SearchInfo[i][j] == DIR_HOR)
				break;
		m_Goods[i][j] -= l_Min;
		for (j = 0; j < m_ReceiverCount; j++)
			if (m_SearchInfo[i][j] == DIR_VERT)
				break;
		m_Goods[i][j] += l_Min;
	} while(i != a_Y);

	m_Goods[l_OptY][l_OptX] = -1.0;
}

void Transport::PotentialSolution()
{
	int i, j;
	int l_Flag = 1;

	m_SourcePotential   = new double[m_SourceCount  ];
	m_ReceiverPotential = new double[m_ReceiverCount];

	m_SearchInfo = new int*[m_SourceCount];
	for (i = 0; i < m_SourceCount; i++)
		m_SearchInfo[i] = new int[m_ReceiverCount];

	while (l_Flag == 1)
	{
		l_Flag = 0;

		GeneratePotentials();

		for (i = 0; i < m_SourceCount; i++)
		{
			for (j = 0; j < m_ReceiverCount; j++)
			{
				if (m_Cost[i][j] < m_SourcePotential[i] + m_ReceiverPotential[j])
				{
					ModifyGoods(j, i);
					l_Flag = 1;
				}
				if (l_Flag == 1)
					break;
			}
			if (l_Flag == 1)
				break;
		}
	}

	delete[] m_SourcePotential;
	delete[] m_ReceiverPotential;
	for (i = 0; i < m_SourceCount; i++)
		delete[] m_SearchInfo[i];
	delete[] m_SearchInfo;
}