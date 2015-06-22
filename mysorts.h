#ifndef _MYSORTS_H
#define _MYSORTS_H

#include "mytool.h"

//插入排序 InsertSort
//时间复杂度 平均：O(n^2) 最差(反序)：O(n^2) 最好(已排好序)：O(n)
//Example:
//      0: 2,4,3,1,5
//      1: 2,4,3,1,5  temp(4)
//      2: 2,3,4,1,5  ==> 2,4,4,1,5  ==> 2,3,4,1,5  temp(3)
//      3: 1,2,3,4,5  ==> 2,3,4,4,5  ==> 2,3,3,4,5  ==> 2,2,3,4,5  ==> 1,2,3,4,5  temp(1)
//      4: 1,2,3,4,5  temp(5)
template<typename T>
void myinsertsort(T* pArray, int nSize)
{
	int i, j;
	for(i = 1; i < nSize; ++i)
	{
		T temp = pArray[i];
		for(j = i; j > 0; --j)
		{
			if(pArray[j-1] > temp)
				pArray[j] = pArray[j-1]; 
			else{
				pArray[j] = temp;
				break;
			}
		}
	}
}	

//希尔排序 ShellSort
template<typename T>
void myshellsort(T* pArray, int nSize)
{

}

//快速排序 QuickSort
template<typename T>
void myquicksort(T* pArray, int nBegin, int nEnd)
{
	int diff = nEnd - nBegin;
	if(diff < 2)
		return;
	if(diff < 15)
	{
		myinsertsort(pArray + nBegin, nEnd - nBegin);
		return;	
	}
	T key = pArray[nBegin];
	int i = nBegin + 1;
	int j = nEnd - 1;
	while(i <= j)
	{
		while(i <= j && pArray[i] <= key) ++i;
		while(i <= j && pArray[j] >= key) --j;
		if(i < j)
			myswap<T>(pArray[i++], pArray[j--]);
		else
			break;
	}
	myswap<T>(pArray[nBegin], pArray[i-1]);
	myquicksort<T>(pArray, nBegin, j);
	myquicksort<T>(pArray, i, nEnd);
}

template<typename T>
void myquicksort2(T* pArray, int nBegin, int nEnd)
{
	int diff = nEnd - nBegin;
	if(diff < 2)
		return;
	if(diff < 15)
	{
		myinsertsort(pArray + nBegin, nEnd - nBegin);
		return;	
	}
	int mystack[128];
	int ssize = 0;
	mystack[ssize++] = nBegin;
	mystack[ssize++] = nEnd;
	while(ssize)
	{
		nEnd = mystack[--ssize];
		nBegin = mystack[--ssize];
		T key = pArray[nBegin];
		int i = nBegin + 1;
		int j = nEnd - 1;
		while(i <= j)
		{
			while(i <= j && pArray[i] <= key) ++i;
			while(i <= j && pArray[j] >= key) --j;
			if(i < j)
				myswap<T>(pArray[i++], pArray[j--]);
			else
				break;
		}	
		myswap<T>(pArray[nBegin], pArray[i-1]);
		if(nEnd - i > 1)
		{
			mystack[ssize++] = i;
			mystack[ssize++] = nEnd;
		}
		if(j - nBegin > 1)
		{
			mystack[ssize++] = nBegin;
			mystack[ssize++] = j;
		}
	}
}

template<typename T>
bool checksorts(T* pArray, int n)
{
	for(int i = 0; i < n - 1; ++i)
	{
		if(pArray[i] > pArray[i+1])
			return false;
	}
	return true;
}









#endif
