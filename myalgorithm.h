#ifndef _MYALGORITHM_H
#define _MYALGORITHM_H

#include <string>
#include <vector>
#include <string.h>
#include "mytool.h"
using namespace std;


//中缀表达式到后缀表达式的转换
bool ToPostfix(const string& input)
{
	finder fd;
	string strs[] = {"+", "-", "*", "/", "(", ")"};
	vector<string> keywords = vector<string>(strs, strs + sizeof(strs)/sizeof(string));
	fd.SetKeyWords(keywords);
	vector<string> optStack;
	int result = 0;
	int last_offset = 0;
	int offset = 0;
	string s;
	string finalstr;
	while((result = fd.Find(input, offset)) != -1)
	{
		s = input.substr(last_offset, offset - last_offset);
		last_offset = ++offset;
		finalstr += s;
		switch(result){
			case 0: // "+"
			case 1: // "-"
				while(!optStack.empty() && optStack.back() != "(")
				{
					finalstr += optStack.back();
					optStack.pop_back();
				}
				optStack.push_back(fd.GetKeyWord(result));
				break;
			case 2: // "*"
			case 3: // "/"
				while(!optStack.empty() && (optStack.back() == "*" || optStack.back() == "/"))
				{
					finalstr += optStack.back();
					optStack.pop_back();
				}
				optStack.push_back(fd.GetKeyWord(result));
				break;
			case 4: // "("
				optStack.push_back(fd.GetKeyWord(result));
				break;
			case 5: // ")"
				while(!optStack.empty() && optStack.back() != "(")
				{
					finalstr += optStack.back();
					optStack.pop_back();
				}
				if(optStack.empty())
					return false;
				optStack.pop_back();
				break;
		}
	}
	s = input.substr(last_offset, offset - last_offset);
	finalstr += s;
	while(!optStack.empty())
	{
		finalstr += optStack.back();
		optStack.pop_back();
	}
	cout << finalstr << endl;
	return true;
}


/***************************************************************************************************************************************/


//遍历数组
template<typename T>
void PrintArray(T* pArray, int nSize)
{
	for(int i = 0; i < nSize; ++i)
		cout << pArray[i] << " ";
	cout << endl;
}


/***************************************************************************************************************************************/


//输出数组的所有排列情况
template<typename T>
void PrintPaiLie(T* pArray, int nBegin, int nEnd)
{
	if(nBegin == nEnd - 1)
	{
		PrintArray<T>(pArray, nEnd);
	}
	else
	{
		for(int i = nBegin; i < nEnd; ++i)
		{
			myswap<T>(pArray[nBegin], pArray[i]);
			test1(pArray, nBegin+1, nEnd);
			myswap<T>(pArray[nBegin], pArray[i]);
		}
	}
}


/***************************************************************************************************************************************/



//输出数组中取nNum个元素的所有组合情况
template<typename T>
void PrintZuHe(T* pArray, int nBegin, int nEnd, int nNum)
{
	static T* pResult = new T[nNum];
	static int now = 0;
	if(nNum == 0)
		PrintArray<T>(pResult, now);
	else{
		for(int i = nBegin; i < nEnd - nNum + 1; ++i)
		{
			pResult[now++] = pArray[i];
			PrintZuHe(pArray, i+1, nEnd, nNum-1);
			now--;
		}	
	}
}




#endif

