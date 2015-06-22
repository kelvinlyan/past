#ifndef _MYTOOL_H
#define _MYTOOL_H

#include <time.h>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
using namespace std;


/***************************************************************************************************************************************/


//对位赋值和取值
const char bytes[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const size_t bitsOfByte = 8;

inline int GetByte(char* c, int n)
{
	return (int)((*(c + n / bitsOfByte) >> n % bitsOfByte) & 0x01);
}

inline void SetByte(char* c, int n, bool value)
{
	if(!value)
		*(c + n / bitsOfByte) &= ~bytes[n % bitsOfByte];	
	else
		*(c + n / bitsOfByte) |= bytes[n % bitsOfByte];
}


/***************************************************************************************************************************************/


//功能：获得2次调用之间的时间差
class timer 
{
	public:
		timer(){
			m_Start = clock();
		}
		clock_t GetTime()
		{
			clock_t end = clock();
			clock_t diff = end - m_Start;
			m_Start = end;
			return diff;
		}
	private:
		clock_t m_Start;
};


/***************************************************************************************************************************************/


//功能：查找字符串中第一个出现keyword的位置
class finder
{
	public:
		string& GetKeyWord(int i);
		void SetKeyWords(vector<string>& key_words);
		int Find(const string& input, int& offset);
	private:
		vector<string> m_key_words;
};

string& finder::GetKeyWord(int i)
{
	return m_key_words[i];
}

void finder::SetKeyWords(vector<string>& key_words)
{
	m_key_words = key_words;
}

int finder::Find(const string& input, int& offset)
{
	if(offset < 0)
		return -1;
	for(int i = offset; i < input.size(); ++i)
	{
		for(int j = 0; j < m_key_words.size(); ++j)
		{
			if(m_key_words[j].size() == 0)
				continue;
			else{
				if(memcmp(input.c_str() + i, m_key_words[j].c_str(), m_key_words[j].size()) == 0){
					offset = i;
					return j;
				}	
			}
		}
	}
	offset = input.size();
	return -1;
}


/***************************************************************************************************************************************/


//功能：判断一个字符串是否全为空格
bool IsSpace(const string& str)
{
	if(str.size() == 0)
		return false;
	int i = 0;
	while(i < str.size())
	{
		if(str[i++] != ' ')
			return false;
	}
	return true;
}


/***************************************************************************************************************************************/


//功能：交换两个值
template<typename T>
inline void myswap(T& left, T& right)
{
	T temp = left;
  	left = right;
 	right = temp;
}


/***************************************************************************************************************************************/


//功能：生成一个n位的随机小写字符串
void GetRamStr(char * pstr, int n)
{
	int ram;
	//srand((int)time(0));
	ram = rand()%25;
	for(int i = 0; i < n; ++i)
	{
		ram = rand()%25;
		ram += 'a';
		memcpy(pstr+i, &ram, sizeof(char));
	}
}

/***************************************************************************************************************************************/


//未完
bool myatoi(const char* pstr, int len, int& value)
{
	int i = 0;
	while(i < len)
	{
		if(pstr[i] == ' ')
			++i;
		else
			break;
	}
	if(i == len)
		return false;
//	if()
}


/***************************************************************************************************************************************/


//功能：判断一个字符是否数字
bool myisdigit(char c)
{
	if(c >= '0' && c <= '9')
		return true;
	else
		return false;
}



template<typename T>
T GetMid(T a, T b, T c)
{
	if(a > b)
	{
		if(b > c)
			return b;
		if(a > c)
			return c;
		else
			return a;
	}
	else
	{
		if(a > c)
			return a;
		if(b > c)
			return c;
		else
			return b;	
	}
}



#endif
