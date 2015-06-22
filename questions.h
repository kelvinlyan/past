#ifndef _QUESTIONS_H
#define _QUESTIONS_H

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "mytool.h"
using namespace std;

//Question 3:
//在一个二维数组中，每一行都按照从左到右递增的顺序排序，
//每一列都按照从上到下递增的顺序排序。请完成一个函数，
//输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

bool Q3(int* pArray, int n, int m, int value)
{
	int n1 = 0, n2 = n, m1 = 0, m2 = m;
	int _n1 = n1, _m1 = m1, _n2, _m2;

	while(true)
	{
		if(pArray[n1*m+m1] > value || pArray[(n2-1)*m+m2-1] < value)
			return false;
		while(_n1 != n2)
		{
			if(pArray[_n1*m+m1] > value || pArray[_n1*m+m2-1] < value)
				++_n1;
			else
				break;
		}
		if(_n1 == n2)
			return false;
		_n2 = _n1;
		while(_n2 != n2)
		{
			if(pArray[_n2*m+m1] > value || pArray[_n2*m+m2-1] < value)
				break;
			else
				++_n2;
		}

		while(_m1 != m2)
		{
			if(pArray[n1*m+_m1] > value || pArray[(n2-1)*m+_m1] < value)
				++_m1;
			else
				break;
		}
		if(_m1 == m2)
			return false;
		_m2 = _m1;
		while(_m2 != m2)
		{
			if(pArray[n1*m+_m2] > value || pArray[(n2-1)*m+_m2] < value)
				break;
			else
				++_m2;
		}
		if(n1 == _n1 && n2 == _n2 && m1 == _m1 && m2 == _m2)
			return true;
		n1 = _n1;
		n2 = _n2;
		m1 = _m1;
		m2 = _m2;
	}
}
bool Q3_2(int* pArray, int n, int m, int value)
{
	if(pArray == NULL)
		return false;
	int _n = n - 1, _m = 0;
	while(_n != -1 && _m != m){
		if(pArray[_n*m+_m] == value)
			return true;
		else if(pArray[_n*m+_m] > value)
			--_n;
		else
			++_m;
	}
	return false;
}

//Question 4:
//请实现一个函数，把字符串中的每个空格替换成"%20"。
//例如输入"we are happy."，则输出"we%20are%20happy."。
void Q4(char* const str1, const char c, const char* str2)
{
	if(str1 == NULL || str2 == NULL)
		return;
	int count = 0, len1 = 0;
	int len2 = strlen(str2);
	char* pTemp = str1;
	while(*pTemp != '\0')
	{
		if(*(pTemp++) == c)
			++count;
		++len1;
	}
	int i = len1 + (len2 - 1) * count + 1;
	str1[i--] = '\0';
	while(i >= 0)
	{
		if(str1[len1] != c)
			str1[i--] = str1[len1--];
		else{
			memcpy(str1 + i - len2 + 1, str2, len2);
			i -= len2;
			--len1;
		}
	}
}

//Question 5:
//输入一个链表的头结点，从尾到头反过来打印出每个结点的值
struct node{
	public:
		node():m_pNext(NULL){}
		node* m_pNext;
		int m_nValue;
};
void Q5(node* ptr)
{
	if(ptr == NULL)
		return;
	else{
		Q5(ptr->m_pNext);
		cout << ptr->m_nValue << " ";
	}
}
void Q5_2(node* ptr)
{
	vector<node*> vNodes;
	while(ptr != NULL){
		vNodes.push_back(ptr);
		ptr = ptr->m_pNext;
	}
	while(!vNodes.empty())
	{
		cout << vNodes.back()->m_nValue << " ";
		vNodes.pop_back();
	}
	cout << endl;
}

//Question 6:
//输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。
//假设输入的前序遍历和中序遍历的结果中都不含重复的数字。
struct bnode{
	public:
		bnode(int value):m_nValue(value),m_pLeft(NULL),m_pRight(NULL){}
		int m_nValue;
		bnode* m_pLeft;
		bnode* m_pRight;
};
bnode* Q6(int* pArray1, int* pArray2, int begin1, int end1, int begin2, int end2)
{
	if(begin1 == end1)
		return NULL;
	bnode* pbnode = new bnode(pArray1[begin1]);
	int offset = begin2;
	while(offset != end2)
	{
		if(pArray2[offset] != pArray1[begin1])
			++offset;
		else
			break;
	}
	pbnode->m_pLeft = Q6(pArray1, pArray2, begin1+1, begin1+1+offset-begin2, begin2, offset);
	pbnode->m_pRight = Q6(pArray1, pArray2, begin1+1+offset-begin2, end1, offset+1, end2);
	return pbnode;
}
void PrintQ6(bnode* ptr)
{
	if(ptr != NULL){
		PrintQ6(ptr->m_pLeft);
		cout << ptr->m_nValue << " ";
		PrintQ6(ptr->m_pRight);
	}
}

//Question 7:
//用两个栈实现一个队列，实现尾部插入和头部弹出的操作。
template<typename T>
class Q7{
	public:
		void push_back(const T& value);
		T pop();

	private:
		vector<T> m_stack1;
		vector<T> m_stack2;
};
template<typename T>
void Q7<T>::push_back(const T& value)
{
	m_stack1.push_back(value);
}
template<typename T>
T Q7<T>::pop()
{
	if(m_stack2.empty())
	{
		while(!m_stack1.empty())
		{
			m_stack2.push_back(m_stack1.back());
			m_stack1.pop_back();
		}
		if(m_stack2.empty())
			return T();
	}
	T temp = m_stack2.back();
	m_stack2.pop_back();
	return temp;
}

//Question 8:
//把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
//输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。
//例如 3,4,5,1,2是1,2,3,4,5的一个旋转，则该数组最小元素为1。

int Q8(int* pArray, int nBegin, int nEnd)
{
	if(nEnd - nBegin == 1)
		return pArray[nBegin];
	int nMid = (nEnd + nBegin) / 2;
	if(pArray[nMid] >= pArray[nBegin])
		return Q8(pArray, nMid+1, nEnd);
	else
		return Q8(pArray, nBegin+1, nMid);
}

//Question 9:
//写一个函数，输入n，求Fibonacci数列的第n项
//f(n)=f(n-1)+f(n-2),f(0)=0,f(1)=1
int Q9(int n)
{
	if(n == 0 || n == 1)
		 return n;
	int a1 = 0, a2 = 1;
	int i = 2;
	int result = 0;
	while(i++ != n)
	{
		result = a1 + a2;
		a2 = a1;
		a1 = result;
	}
	return result;
}

//Question 10:
//实现一个函数，输入一个整数，输出该数二进制表示中1的个数，例如输入9(1001)，则输出2。
int Q10(int n)
{
	int count = 0;
	for(int i = 0; i < 31; ++i)
	{
		if(n & 1 == 1)
			++count;
		n >>= 1;
	}
	return count;
}

//Question 11:
//实现函数double Power(double base, int exponent),求base的exponent次方。
bool Q11_g_bInvalidInput = false;
bool Q11_Equal(double a, double b)
{
	if((a - b > -0.0000001) && (a - b < 0.0000001))
		return true;
	else
		return false;
}
double Q11(double base, int exponent)
{
	Q11_g_bInvalidInput = false;
	if(Q11_Equal(base, 0.0) && exponent < 0){
		Q11_g_bInvalidInput = true;
		return 0.0;
	}
	bool flag = false;
	if(exponent < 0){
		exponent *= -1.0;
		flag = true;
	}
	double result = 1.0;
	while(exponent--)
		result *= base;
	if(flag == false)
		result = 1.0 / result;
	return result;
}
double Q11_power_with_unsigned_exponent(double base, int exponent)
{
	if(exponent == 0)
		return 1.0;
	if(exponent == 1)
		return base;
	double result = Q11_power_with_unsigned_exponent(base, exponent >> 1);
	result *= result;
	if(exponent & 0x1 == 1)
		result *= base;
	return result;
}
double Q11_2(double base, int exponent)
{
	Q11_g_bInvalidInput = false;
	if(Q11_Equal(base, 0.0) && exponent < 0){
		Q11_g_bInvalidInput = true;
		return 0.0;
	}
	bool flag = false;
	if(exponent < 0){
		exponent *= -1.0;
		flag = true;
	}
	double result = Q11_power_with_unsigned_exponent(base, exponent);
	if(flag == false)
		result = 1.0 / result;
	return result;
}

//Question 12:
//输入数字n，按顺序打印出从1到最大的n位十进制数。
void Q12_Increment(char* pstr, int n)
{
	while(1)
	{
		*(pstr + n - 1) += 1;
		if(*(pstr + n - 1) > '9')
		{
			*(pstr + n - 1) = '0';
			--n;
		}
		else{
			break;
		}
	}
}
void Q12_Print(char* pstr)
{
	while(*pstr == '0')
		++pstr;
	cout << pstr << " ";
}
void Q12(int n)
{
	char* pstr = new char[n+2];
	memset(pstr, '0', n+1);
	pstr[n+1] = '\0';
	do{
		Q12_Increment(pstr+1, n);
		Q12_Print(pstr+1);
	}while(pstr[0] == '0');
}
void Q12_Do(char* pstr, int nBegin, int nEnd)
{
	if(nBegin == nEnd)
	{
		Q12_Print(pstr);
		return;
	}
	for(int i = 0; i < 10; ++i)
	{
		pstr[nBegin] = '0' + i;
		Q12_Do(pstr, nBegin+1, nEnd);
	}
}
void Q12_2(int n)
{
	char* pstr = new char[n+1];
	memset(pstr, '0', n);
	pstr[n] = '\0';
	Q12_Do(pstr, 0, n);
}

//Question 13:
//给定单向链表的头指针和一个结点指针，定义一个函数在O(1)时间删除该结点。
void Q13(node** pHead, node* pNode)
{
	if(pHead == NULL || pNode == NULL)
		return;
	if(pNode->m_pNext != NULL)
	{
		node* pNext = pNode->m_pNext;
		pNode->m_pNext = pNext->m_pNext;
		pNode->m_nValue = pNext->m_nValue;
		delete pNext;
	}
	else if(*pHead == pNode)
	{
		delete pNode;
		pHead = NULL;
	}else{
		node* pPre = *pHead;
		while(pPre->m_pNext != pNode)
			pPre = pPre->m_pNext;
		pPre->m_pNext = NULL;
		delete pNode;
	}
}

//Question 14:
//输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有奇数位于数组的前半部分，偶数位于后半部分。
void Q14(int* pArray, int nSize)
{
	if(pArray == NULL || nSize == 0)
		return;
	int i = 0, j = nSize - 1;
	while(i < j)
	{
		while(i <= j && (pArray[i] & 0x1 == 1)) ++i;
		while(i <= j && (pArray[i] & 0x1 == 0)) --j;
		if(i < j)
			myswap<int>(pArray[i++], pArray[--j]);
	}
}

//Question 15:
//输入一个链表，输出该链表倒数第k个结点。
node* Q15(node* pHead, int k)
{
	if(pHead == NULL || k < 0)
		return NULL;
	int i = 0;
	node* pNow = pHead;
	while(i++ != k){
		if(pNow == NULL)
			return NULL;
		pNow = pNow->m_pNext;
	}
	while(pNow->m_pNext != NULL)
	{
		pNow = pNow->m_pNext;
		pHead = pHead->m_pNext;
	}
	return pHead;
}

//Question 16:
//反转链表
node* Q16(node* pHead)
{
	if(pHead == NULL)
		return NULL;
	node* p1 = pHead->m_pNext;
	node* p2;
	node* p3 = pHead;
	while(p1 != NULL)
	{
		p2 = p1->m_pNext;
		p1->m_pNext = p3;
		p3 = p1;
		p1 = p2;
	}
	pHead->m_pNext = NULL;
	return p3;
}

//Question 17:
//输入两个递增排序的链表，合并两个链表并使新链表中的结点仍然是递增排序的。
node* Q17(node* p1, node* p2) 
{
	if(p1 == NULL)
		return p2;
	if(p2 == NULL)
		return p1;
	node* pHead;
	node* pNode;
	if(p1->m_nValue < p2->m_nValue)
	{
		pHead = p1;
		pNode = p1;
		p1 = p1->m_pNext;
	}
	else{
		pHead = p2;
		pNode = p2;
		p2 = p2->m_pNext;
	}
	while(p1 != NULL && p2 != NULL)
	{
		if(p1->m_nValue < p2->m_nValue){
			pNode->m_pNext = p1;
			pNode = pNode->m_pNext;
			p1 = p1->m_pNext;
		}
		else{
			pNode->m_pNext = p2;
			pNode = pNode->m_pNext;
			p2 = p2->m_pNext;
		}
	}
	if(p1 == NULL)
		pNode->m_pNext = p2;
	else
		pNode->m_pNext = p1;
	return pHead;
}

//Code 1:
//链表排序，不使用数组和内存拷贝。
int ListLen(node* phead)
{
	int len = 0;
	while(phead != NULL)
	{
		++len;
		phead = phead->m_pNext;
	}
	return len;
}

node* C1(node* phead)
{
	int len = ListLen(phead);
	if(len < 2)
		return phead;
	else{
		int i = 0;
		node* ptemp = phead;
		node* pPre = ptemp;
		while(i++ < len / 2)
		{
			pPre = ptemp;
			ptemp = ptemp->m_pNext;
		}
		pPre->m_pNext = NULL;
		node* p1 = C1(phead);
		node* p2 = C1(ptemp);
		Q17(p1, p2);
	}
}










#endif
