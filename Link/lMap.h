#ifndef LMAP_H
#define LMAP_H

#define assert(a) ;
#include <iostream>
#include <vector>
using namespace std;

typedef int vType;

struct lPoint{
	public:
		lPoint():m_bExist(false){}
		lPoint(int abs, int ord):m_nAbs(abs),m_nOrd(ord),m_bExist(false){}
		void PrintPosition()
		{
			cout << "(" << m_nAbs << ", " << m_nOrd << ")";
		}
		int m_nAbs;
		int m_nOrd;
		vType m_Value;
		bool m_bExist;
};

struct lLine{
	public:
		lLine():m_pNext(NULL){}
		lLine(int begin, int end):m_nBegin(begin),m_nEnd(end),m_pNext(NULL){}
		int m_nBegin;
		int m_nEnd;
		lLine* m_pNext;
};

const int MAX_ABS_DFL = 10;
const int MAX_ORD_DFL = 10;

class lMap{
	public:
		void Init(int max_abs = MAX_ABS_DFL, int max_ord = MAX_ORD_DFL);
		bool Insert(const int abs, const int ord, vType value);
		bool Erase(const int abs, const int ord);
		bool IsLinked(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord);
		bool IsEmpty();
		bool IsDead();
		void PrintLineMap();
		void PrintMap();
		void PrintPath();

	private:
		void _Insert(const int abs, const int ord, vType value);
		void _Erase(const int abs, const int ord);
		bool DoCheckLinked(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord);
		bool IsLinkedInOneAbs(const int pointA_ord, const int pointB_ord, const int point_abs);
		bool IsLinkedInOneOrd(const int pointA_abs, const int pointB_abs, const int point_ord);
		bool IsLinkedInSomeAbs(const int pointA_ord, const int pointB_ord, const int point_abs_begin, const int point_abs_end);
		bool IsLinkedInSomeOrd(const int pointA_abs, const int pointB_abs, const int point_ord_begin, const int point_ord_end);
		bool _CheckLinkedInAbs(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord);
		bool CheckLinkedInAbs(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord);
		bool _CheckLinkedInOrd(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord);
		bool CheckLinkedInOrd(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord);
		void GetCommonRange(int rangeA_begin, int rangeA_end, int rangeB_begin, int rangeB_end, int& range_begin, int& range_end);


	private:
		lPoint* m_pMap;
		lLine** m_LineMap_abs;
		lLine** m_LineMap_ord;
		vector<lPoint> m_Path_map;
		int m_nUsed;	
		int m_max_abs;
		int m_max_ord;
};

void lMap::PrintPath()
{
	if(m_Path_map.empty())
	{
		fprintf(stderr, "PrintPath: Path_map is empty!\n");
		return;
	}else{
		for(vector<lPoint>::iterator iter = m_Path_map.begin(); iter != m_Path_map.end(); ++iter){
			if(iter != m_Path_map.begin())
				cout << " --> ";
			(*iter).PrintPosition();
		}
		cout << endl;
	}
}

void lMap::PrintMap()
{
	cout << "map count:" << m_nUsed << endl;
	for(int i = 1; i < m_max_abs - 1; ++i)
	{
		for(int j = 1; j < m_max_ord - 1; ++j)
		{
			if(m_pMap[i * m_max_ord + j].m_bExist)
				cout << "| " << m_pMap[i * m_max_ord + j].m_Value << " ";
			else
				cout << "|   ";
			if(j == m_max_ord - 2)
				cout << "|";
		}
		cout << endl;
	}
}

void lMap::PrintLineMap()
{
	cout << "LineMap_abs:" << endl;
	for(int i = 0; i < m_max_abs; ++i)
	{
		cout << " < " << i << " > :";
		lLine* pLine = m_LineMap_abs[i];
		while(pLine != NULL)
		{
			cout << " (" << pLine->m_nBegin << ", " << pLine->m_nEnd << ") ";
			pLine = pLine->m_pNext;
		}
		cout << endl;
	}
	cout << endl << "LineMap_ord:" << endl;
	for(int i = 0; i < m_max_ord; ++i)
	{
		cout << " < " << i << " > :";
		lLine* pLine = m_LineMap_ord[i];
		while(pLine != NULL)
		{
			cout << " (" << pLine->m_nBegin << ", " << pLine->m_nEnd << ") ";
			pLine = pLine->m_pNext;
		}
		cout << endl;
	}
}

void lMap::Init(int max_abs, int max_ord)
{
	m_nUsed = 0;
	m_max_abs = max_abs + 2;
	m_max_ord = max_ord + 2;
	m_pMap = new lPoint[(m_max_abs) * (m_max_ord)];
	m_LineMap_abs = new lLine*[m_max_abs];
	for(int i = 0; i < m_max_abs; ++i)
		m_LineMap_abs[i] = new lLine(0, m_max_ord);
	m_LineMap_ord = new lLine*[m_max_ord];
	for(int i = 0; i < m_max_ord; ++i)
		m_LineMap_ord[i] = new lLine(0, m_max_abs);
}

bool lMap::IsEmpty()
{
	return m_nUsed == 0;
}

bool lMap::IsDead()
{
	return false;
}

bool lMap::IsLinked(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord)
{
	if(pointA_abs == 0 || pointA_abs == m_max_abs - 1 || pointA_ord == 0 || pointA_ord == m_max_ord - 1 
			|| pointB_abs == 0 || pointB_abs == m_max_abs - 1 || pointB_ord == 0 || pointB_ord == m_max_ord - 1)
	{
		fprintf(stderr, "IsLinked: Abs or Ord Out of Range(Abs Range[1, %d], Ord Range[1, %d])!\n", m_max_abs - 2, m_max_ord - 2);
		return false;
	}
	m_Path_map.clear();
	m_Path_map.push_back(lPoint(pointA_abs, pointA_ord));
	if(!(m_pMap)[pointA_abs * m_max_ord + pointA_ord].m_bExist || !(m_pMap)[pointB_abs * m_max_ord + pointB_ord].m_bExist)
	{
		fprintf(stderr, "IsLinked: Point<%d, %d> or Point<%d, %d> not Exist!\n", pointA_abs, pointA_ord, pointB_abs, pointB_ord);
		return false;
	}
	if(pointA_abs == pointB_abs && pointA_ord == pointB_ord)
	{
		fprintf(stderr, "IsLinked: Point<%d, %d> and Point<%d, %d> are the same Point!\n", pointA_abs, pointA_ord, pointB_abs, pointB_ord);
		return false;
	}
	if((m_pMap)[pointA_abs * m_max_ord + pointA_ord].m_Value != (m_pMap)[pointB_abs * m_max_ord + pointB_ord].m_Value)
	{
		fprintf(stderr, "IsLinked: Point<%d, %d> and Point<%d, %d> are not the same Kind!\n", pointA_abs, pointA_ord, pointB_abs, pointB_ord);
		return false;
	}
	if(DoCheckLinked(pointA_abs, pointA_ord, pointB_abs, pointB_ord))
	{
		if(m_Path_map.back().m_nAbs != pointB_abs || m_Path_map.back().m_nOrd != pointB_ord)
			m_Path_map.push_back(lPoint(pointB_abs, pointB_ord));
		return true;
	}
	return false;
}

bool lMap::Insert(const int abs, const int ord, vType value)
{
	if(abs == 0 || abs == m_max_abs - 1 || ord == 0 || ord == m_max_ord - 1)
	{
		fprintf(stderr, "Insert: Abs or Ord Out of Range(Abs Range[1, %d], Ord Range[1, %d])!\n", m_max_abs - 1, m_max_ord - 1);
		return false;
	}

	if(m_pMap[abs * m_max_ord + ord].m_bExist)
	{
		fprintf(stderr, "Insert: Point<%d, %d> Exist!\n", abs, ord);
		return false;
	}
	_Insert(abs, ord, value);
	m_nUsed++;
	return true;
}

void lMap::_Insert(const int abs, const int ord, vType value)
{
	m_pMap[abs * m_max_ord + ord].m_Value = value;
	m_pMap[abs * m_max_ord + ord].m_bExist = true;
	lLine* pLine = m_LineMap_abs[abs];
	lLine* pTemp = pLine;
	while(pLine != NULL)
	{
		if(ord < pLine->m_nEnd)
		{
			if(pLine->m_nEnd - pLine->m_nBegin == 1)
			{
				pTemp->m_pNext = pLine->m_pNext;
				delete pLine;
			}
			else if(ord == pLine->m_nBegin)
				pLine->m_nBegin++;
			else if(ord == pLine->m_nEnd - 1)
				pLine->m_nEnd--;
			else
			{
				lLine* pNewLine = new lLine(ord + 1, pLine->m_nEnd);
				pLine->m_nEnd = ord;
				pNewLine->m_pNext = pLine->m_pNext;
				pLine->m_pNext = pNewLine;
			}
			break;
		}
		pTemp = pLine;
		pLine = pLine->m_pNext;
	}
	pLine = m_LineMap_ord[ord];
	pTemp = pLine;
	while(pLine != NULL)
	{
		if(abs < pLine->m_nEnd)
		{
			if(pLine->m_nEnd - pLine->m_nBegin == 1)
			{
				pTemp->m_pNext = pLine->m_pNext;
				delete pLine;
			}
			else if(abs == pLine->m_nBegin)
				pLine->m_nBegin++;
			else if(abs == pLine->m_nEnd - 1)
				pLine->m_nEnd--;
			else
			{
				lLine* pNewLine = new lLine(abs + 1, pLine->m_nEnd);
				pLine->m_nEnd = abs;
				pNewLine->m_pNext = pLine->m_pNext;
				pLine->m_pNext = pNewLine;
			}
			break;
		}
		pTemp = pLine;
		pLine = pLine->m_pNext;
	}
}

bool lMap::Erase(const int abs, const int ord)
{
	if(!(m_pMap)[abs * m_max_ord + ord].m_bExist)
	{
		fprintf(stderr, "Erase: Point<%d, %d> not Exist!\n", abs, ord);
		return false;
	}
	_Erase(abs, ord);
	m_nUsed--;
	return true;
}

void lMap::_Erase(const int abs, const int ord)
{
	m_pMap[abs * m_max_ord + ord].m_bExist = false;
	lLine* pLine = m_LineMap_abs[abs];
	lLine* pTemp = pLine;
	while(pLine != NULL)
	{
		if(ord < pLine->m_nBegin)
		{
			if(pLine->m_nBegin - pTemp->m_nEnd == 1)
			{
				pTemp->m_nEnd = pLine->m_nEnd;
				pTemp->m_pNext = pLine->m_pNext;
				delete pLine;
			}
			else if(ord == pLine->m_nBegin - 1)
				pLine->m_nBegin--;
			else if(ord == pTemp->m_nEnd)
				pTemp->m_nEnd++;
			else
			{
				lLine* pNewLine = new lLine(ord, ord + 1);
				pTemp->m_pNext = pNewLine;
				pNewLine->m_pNext = pLine;
			}
			break;
		}
		pTemp = pLine;
		pLine = pLine->m_pNext;
	}
	pLine = m_LineMap_ord[ord];
	pTemp = pLine;
	while(pLine)
	{
		if(abs < pLine->m_nBegin)
		{
			if(pLine->m_nBegin - pTemp->m_nEnd == 1)
			{
				pTemp->m_nEnd = pLine->m_nEnd;
				pTemp->m_pNext = pLine->m_pNext;
				delete pLine;
			}
			else if(abs == pLine->m_nBegin - 1)
				pLine->m_nBegin--;
			else if(abs == pTemp->m_nEnd)
				pTemp->m_nEnd++;
			else
			{
				lLine* pNewLine = new lLine(abs, abs + 1);
				pTemp->m_pNext = pNewLine;
				pNewLine->m_pNext = pLine;
			}
			break;
		}
		pTemp = pLine;
		pLine = pLine->m_pNext;
	}
}

bool lMap::DoCheckLinked(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord)
{
	if(pointA_abs == pointB_abs)
	{
		if(pointB_ord - pointA_ord == 1 || pointA_ord - pointB_ord == 1)
			return true;
		else
			return CheckLinkedInOrd(pointA_abs, pointA_ord, pointB_abs, pointB_ord);
	}
	else if(pointA_ord == pointB_ord)
	{
		if(pointA_abs - pointB_abs == 1 || pointB_abs - pointA_abs == 1)
			return true;
		else
			return CheckLinkedInAbs(pointA_abs, pointA_ord, pointB_abs, pointB_ord);
	}
	else if(CheckLinkedInAbs(pointA_abs, pointA_ord, pointB_abs, pointB_ord))
		return true;
	else
		return CheckLinkedInOrd(pointA_abs, pointA_ord, pointB_abs, pointB_ord);
}

bool lMap::IsLinkedInOneAbs(const int pointA_ord, const int pointB_ord, const int point_abs)
{
	lLine* pLine = m_LineMap_abs[point_abs];
	while(pLine != NULL)
	{
		if(pLine->m_nEnd > pointA_ord)
		{
			if(pLine->m_nBegin <= pointA_ord && pLine->m_nEnd > pointB_ord)
				return true;
			else
				return false;
		}
		pLine = pLine->m_pNext;
	}
}

bool lMap::IsLinkedInOneOrd(const int pointA_abs, const int pointB_abs, const int point_ord)
{
	lLine* pLine = m_LineMap_abs[point_ord];
	while(pLine != NULL)
	{
		if(pLine->m_nEnd > pointA_abs)
		{
			if(pLine->m_nBegin <= pointA_abs && pLine->m_nEnd > pointB_abs)
				return true;
			else
				return false;
		}
		pLine = pLine->m_pNext;
	}
}

bool lMap::IsLinkedInSomeAbs(const int pointA_ord, const int pointB_ord, const int point_abs_begin, const int point_abs_end) 
{
	assert(pointA_ord <= pointB_ord);
	for(int i = point_abs_begin; i < point_abs_end; ++i)
	{
		if(IsLinkedInOneAbs(pointA_ord, pointB_ord, i))
		{
			if(m_Path_map.back().m_nOrd == pointA_ord - 1)
			{
				if(m_Path_map.back().m_nAbs != i)
					m_Path_map.push_back(lPoint(i, pointA_ord - 1));
				m_Path_map.push_back(lPoint(i, pointB_ord + 1));
			}
			else if(m_Path_map.back().m_nOrd == pointB_ord + 1)
			{
				if(m_Path_map.back().m_nAbs != i)
					m_Path_map.push_back(lPoint(i, pointB_ord + 1));
				m_Path_map.push_back(lPoint(i, pointA_ord - 1));
			}
			return true;
		}
	}
	return false;	
}

bool lMap::IsLinkedInSomeOrd(const int pointA_abs, const int pointB_abs, const int point_ord_begin, const int point_ord_end)
{
	assert(pointA_abs <= pointB_abs);
	for(int i = point_ord_begin; i < point_ord_end; ++i)
	{
		if(IsLinkedInOneOrd(pointA_abs, pointB_abs, i))
		{
			if(m_Path_map.back().m_nAbs == pointA_abs - 1)
			{
				if(m_Path_map.back().m_nOrd != i)
					m_Path_map.push_back(lPoint(pointA_abs - 1, i));
				m_Path_map.push_back(lPoint(pointB_abs + 1, i));
			}
			if(m_Path_map.back().m_nAbs == pointB_abs + 1)
			{
				if(m_Path_map.back().m_nOrd != i)
					m_Path_map.push_back(lPoint(pointB_abs + 1, i));
				m_Path_map.push_back(lPoint(pointA_abs - 1, i));
			}
			return true;
		}
	}
	return false;	
}

bool lMap::CheckLinkedInAbs(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord)
{
	if(pointA_abs < pointB_abs)
		return _CheckLinkedInAbs(pointA_abs, pointA_ord, pointB_abs, pointB_ord);
	else
		return _CheckLinkedInAbs(pointB_abs, pointB_ord, pointA_abs, pointA_ord);
}

bool lMap::_CheckLinkedInAbs(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord)
{
	int pointA_ord_begin = pointA_ord, pointA_ord_end = pointA_ord + 1;
	int	pointB_ord_begin = pointB_ord, pointB_ord_end = pointB_ord + 1;
	bool flag = false;
	lLine* pLine = m_LineMap_abs[pointA_abs];
	if(!m_pMap[pointA_abs * m_max_ord + pointA_ord - 1].m_bExist)
	{
		while(pLine != NULL)
		{
			if(pLine->m_nEnd == pointA_ord)
			{
				pointA_ord_begin = pLine->m_nBegin;
				flag = true;
				break;
			}
			pLine = pLine->m_pNext;
		}
		assert(pointA_ord_begin != pointA_ord);
	}
	if(!m_pMap[pointA_abs * m_max_ord + pointA_ord + 1].m_bExist)
	{
		if(flag)
		{
			pLine = pLine->m_pNext;
			assert(pLine != NULL && pointA_ord + 1 == pLine->m_nBegin);
			pointA_ord_end = pLine->m_nEnd;
			flag = false;
		}
		else
		{
			while(pLine != NULL)
			{
				if(pLine->m_nBegin == pointA_ord + 1){
					pointA_ord_end = pLine->m_nEnd;
					break;
				}
				pLine = pLine->m_pNext;
			}
			assert(pointA_ord_end != pointA_ord + 1);
		}
	}

	pLine = m_LineMap_abs[pointB_abs];
	if(!m_pMap[pointB_abs * m_max_ord + pointB_ord - 1].m_bExist)
	{
		while(pLine != NULL)
		{
			if(pLine->m_nEnd == pointB_ord)
			{
				pointB_ord_begin = pLine->m_nBegin;
				flag = true;
				break;
			}
			pLine = pLine->m_pNext;
		}
		assert(pointB_ord_begin != pointB_ord);
	}

	if(!m_pMap[pointB_abs * m_max_ord + pointB_ord + 1].m_bExist)
	{
		if(flag)
		{
			pLine = pLine->m_pNext;
			assert(pLine != NULL && pointB_ord + 1 == pLine->m_nBegin);
			pointB_ord_end = pLine->m_nEnd;
			flag = false;
		}
		else
		{
			while(pLine != NULL)
			{
				if(pLine->m_nBegin == pointB_ord + 1)
				{
					pointB_ord_end = pLine->m_nEnd;
					break;
				}
				pLine = pLine->m_pNext;
			}
			assert(pointB_ord_end != pointB_ord + 1);
		}
	}

	int point_ord_begin = 0, point_ord_end = 0;

	GetCommonRange(pointA_ord_begin, pointA_ord_end, pointB_ord_begin, pointB_ord_end, point_ord_begin, point_ord_end);

	if(point_ord_begin == point_ord_end)
		return false;

	return IsLinkedInSomeOrd(pointA_abs + 1, pointB_abs - 1, point_ord_begin , point_ord_end);
}

bool lMap::CheckLinkedInOrd(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord)
{
	if(pointA_ord < pointB_ord)
		return _CheckLinkedInOrd(pointA_abs, pointA_ord, pointB_abs, pointB_ord);
	else
		return _CheckLinkedInOrd(pointB_abs, pointB_ord, pointA_abs, pointA_ord);
}

bool lMap::_CheckLinkedInOrd(const int pointA_abs, const int pointA_ord, const int pointB_abs, const int pointB_ord)
{
	int pointA_abs_begin = pointA_abs, pointA_abs_end = pointA_abs + 1;
	int	pointB_abs_begin = pointB_abs, pointB_abs_end = pointB_abs + 1;
	bool flag = false;
	lLine* pLine = m_LineMap_ord[pointA_ord];
	if(!m_pMap[(pointA_abs - 1) * m_max_ord + pointA_ord].m_bExist)
	{
		while(pLine != NULL)
		{
			if(pLine->m_nEnd == pointA_abs)
			{
				pointA_abs_begin = pLine->m_nBegin;
				flag = true;
				break;
			}
			pLine = pLine->m_pNext;
		}
		assert(pointA_abs_begin != pointA_abs);
	}
	if(!m_pMap[(pointA_abs + 1) * m_max_ord + pointA_ord].m_bExist)
	{
		if(flag)
		{
			pLine = pLine->m_pNext;
			assert(pLine != NULL && pointA_abs + 1 == pLine->m_nBegin);
			pointA_abs_end = pLine->m_nEnd;
			flag = false;
		}
		else
		{
			while(pLine != NULL)
			{
				if(pLine->m_nBegin == pointA_abs + 1){
					pointA_abs_end = pLine->m_nEnd;
					break;
				}
				pLine = pLine->m_pNext;
			}
			assert(pointA_abs_end != pointA_abs + 1);
		}
	}

	pLine = m_LineMap_ord[pointB_ord];
	if(!m_pMap[(pointB_abs - 1) * m_max_ord + pointB_ord].m_bExist)
	{
		while(pLine != NULL)
		{
			if(pLine->m_nEnd == pointB_abs)
			{
				pointB_abs_begin = pLine->m_nBegin;
				flag = true;
				break;
			}
			pLine = pLine->m_pNext;
		}
		assert(pointB_abs_begin != pointB_abs);
	}

	if(!m_pMap[(pointB_abs + 1) * m_max_ord + pointB_ord].m_bExist)
	{
		if(flag)
		{
			pLine = pLine->m_pNext;
			assert(pLine != NULL && pointB_abs + 1 == pLine->m_nBegin);
			pointB_abs_end = pLine->m_nEnd;
			flag = false;
		}
		else
		{
			while(pLine != NULL)
			{
				if(pLine->m_nBegin == pointB_abs + 1)
				{
					pointB_abs_end = pLine->m_nEnd;
					break;
				}
				pLine = pLine->m_pNext;
			}
			assert(pointB_abs_end != pointB_abs + 1);
		}
	}

	int point_abs_begin = 0, point_abs_end = 0;

	GetCommonRange(pointA_abs_begin, pointA_abs_end, pointB_abs_begin, pointB_abs_end, point_abs_begin, point_abs_end);

	if(point_abs_begin == point_abs_end)
		return false;

	return IsLinkedInSomeAbs(pointA_ord + 1, pointB_ord - 1, point_abs_begin , point_abs_end);
}

void lMap::GetCommonRange(int rangeA_begin, int rangeA_end, int rangeB_begin, int rangeB_end, int& range_begin, int& range_end)
{
	if(rangeA_begin < rangeB_begin)
	{
		if(rangeA_end <= rangeB_begin)
			return;
		else if(rangeB_end > rangeA_end)
		{
			range_begin = rangeB_begin;
			range_end = rangeA_end;
		}
		else
		{
			range_begin = rangeB_begin;
			range_end = rangeB_end;
		}
	}
	else
	{
		if(rangeB_end == rangeA_begin)
			return;
		else if(rangeB_end < rangeA_end)
		{
			range_begin = rangeA_begin;
			range_end = rangeB_end;
		}
		else
		{
			range_begin = rangeA_begin;
			range_end = rangeA_end;
		}
	}
}

#endif
