#ifndef _MYHEAP_H
#define _MYHEAP_H

#include "mytool.h"
#include <vector>
using namespace std;

//小堆
template<typename T>
class mysmallheap{
	public:
		mysmallheap();
		void Insert(T ele);
		T Min();
		void Pop();
		int Size();
		//void Print();
	private:
		vector<T> m_vElements;	
};

template<typename T>
mysmallheap<T>::mysmallheap()
{
	m_vElements.push_back(T());
}

template<typename T>
inline int mysmallheap<T>::Size()
{
	return m_vElements.size() - 1;
}

template<typename T>
void mysmallheap<T>::Insert(T ele)
{
	m_vElements.push_back(ele);
	int eleIndex;
	int i = m_vElements.size() - 1;
	while(i != 1)
	{
		eleIndex = i;
		i /= 2;
		if(m_vElements[eleIndex] < m_vElements[i])
			myswap<T>(m_vElements[eleIndex], m_vElements[i]);	
		else
			break;
	}
}

template<typename T>
T mysmallheap<T>::Min()
{
	return m_vElements[1];
}

template<typename T>
void mysmallheap<T>::Pop()
{
	if(m_vElements.size() < 2)
		return;
	m_vElements[1] = m_vElements.back();
	m_vElements.pop_back();
	int eleIndex;
	int cmpIndex = 1;
	int cmpIndex2;
	int total = m_vElements.size() - 1;
	int final = total / 2;
	while(cmpIndex <= final)
	{
		eleIndex = cmpIndex;
		cmpIndex *= 2;
		if(m_vElements[eleIndex] < m_vElements[cmpIndex])
		{
			cmpIndex += 1;
			if(cmpIndex <= total)
			{
				if(m_vElements[eleIndex] < m_vElements[cmpIndex])
					break;
				else
					myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex]);
			}
			else
				break;
		}
		else{
			cmpIndex2 = cmpIndex + 1;
			if(cmpIndex2 <= total)
			{
				if(m_vElements[cmpIndex] < m_vElements[cmpIndex2])
					myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex]);
				else{
					myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex2]);
					cmpIndex = cmpIndex2;
				}
			}
			else
				myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex]);
		}
	}
}

/*template<typename T>
void mysmallheap<T>::Print()
{
	if(m_vElements.size() < 2)
		return;
	for(int i = 1; i < m_vElements.size(); ++i)
	{
		cout << m_vElements[i] << " ";	
	}
	cout << endl;
}*/


//大堆
template<typename T>
class mybigheap{
	public:
		mybigheap();
		void Insert(T ele);
		T Max();
		void Pop();
		int Size();
		//void Print();
	private:
		vector<T> m_vElements;	
};

template<typename T>
mybigheap<T>::mybigheap()
{
	m_vElements.push_back(T());
}

template<typename T>
inline int mybigheap<T>::Size()
{
	return m_vElements.size() - 1;
}

template<typename T>
void mybigheap<T>::Insert(T ele)
{
	m_vElements.push_back(ele);
	int eleIndex;
	int i = m_vElements.size() - 1;
	while(i != 1)
	{
		eleIndex = i;
		i /= 2;
		if(m_vElements[eleIndex] > m_vElements[i])
			myswap<T>(m_vElements[eleIndex], m_vElements[i]);	
		else
			break;
	}
}

template<typename T>
T mybigheap<T>::Max()
{
	return m_vElements[1];
}

template<typename T>
void mybigheap<T>::Pop()
{
	if(m_vElements.size() < 2)
		return;
	m_vElements[1] = m_vElements.back();
	m_vElements.pop_back();
	int eleIndex;
	int cmpIndex = 1;
	int cmpIndex2;
	int total = m_vElements.size() - 1;
	int final = total / 2;
	while(cmpIndex <= final)
	{
		eleIndex = cmpIndex;
		cmpIndex *= 2;
		if(m_vElements[eleIndex] > m_vElements[cmpIndex])
		{
			cmpIndex += 1;
			if(cmpIndex <= total)
			{
				if(m_vElements[eleIndex] > m_vElements[cmpIndex])
					break;
				else
					myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex]);
			}
			else
				break;
		}
		else{
			cmpIndex2 = cmpIndex + 1;
			if(cmpIndex2 <= total)
			{
				if(m_vElements[cmpIndex] > m_vElements[cmpIndex2])
					myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex]);
				else{
					myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex2]);
					cmpIndex = cmpIndex2;
				}
			}
			else
				myswap<T>(m_vElements[eleIndex], m_vElements[cmpIndex]);
		}
	}
}

#endif
