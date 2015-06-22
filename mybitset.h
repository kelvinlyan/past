#ifndef _MYBITSET_H
#define _MYBITSET_H

#include "mytool.h"
#include <bitset>
#include <stdlib.h>


class mybitset{
	public:
		mybitset();
		mybitset(const mybitset& rhs);
		mybitset& operator =(const mybitset& rhs);
		~mybitset();
		void push_back(int bit);
		void pop();
		void SetValue(int bit, int i);
		int GetValue(int i);
		int size();
	private:
		char* m_Ptr;
		size_t m_Alloc;
		size_t m_Len;
};

mybitset::mybitset()
{
	m_Ptr = (char*)malloc(1);
	m_Alloc = bitsOfByte * 1;
	m_Len = 0;	
}

mybitset::mybitset(const mybitset& rhs)
{
	m_Ptr = (char*)malloc(rhs.m_Alloc / bitsOfByte);
	memcpy(m_Ptr, rhs.m_Ptr, rhs.m_Len);
	m_Alloc = rhs.m_Alloc;
	m_Len = rhs.m_Len;
}

mybitset& mybitset::operator =(const mybitset& rhs)
{
	m_Ptr = (char*)malloc(rhs.m_Alloc / bitsOfByte);
	memcpy(m_Ptr, rhs.m_Ptr, rhs.m_Len);
	m_Alloc = rhs.m_Alloc;
	m_Len = rhs.m_Len;
	return *this;
}

mybitset::~mybitset()
{
	free(m_Ptr);
}

inline int mybitset::size()
{
	return m_Len;
}

inline void mybitset::pop()
{
	m_Len--;
}

inline void mybitset::push_back(int bit)
{
	if(m_Len == m_Alloc)
	{
		m_Ptr = (char*)realloc(m_Ptr, m_Alloc / bitsOfByte * 2);
		m_Alloc *= 2;
	}
	SetByte(m_Ptr[m_Len / bitsOfByte], m_Len % bitsOfByte, bit);
	m_Len++;	
}

inline int mybitset::GetValue(int i)
{
	return GetByte(m_Ptr[i / bitsOfByte], i % bitsOfByte);
}

inline void mybitset::SetValue(int bit, int i)
{
	SetByte(m_Ptr[i / bitsOfByte], i % bitsOfByte, bit);
}













#endif
