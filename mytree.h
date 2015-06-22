#ifndef _MYTREE_H
#define _MYTREE_H

#include <iostream>
using namespace std;

struct node{
	public:
		node():m_pLeft(NULL),m_pRight(NULL){}
		int m_nValue;
		node* m_pLeft;
		node* m_pRight;
};

int Depth(node* phead)
{
	if(phead == NULL)
		return 0;
	else{
		int left = Depth(phead->m_pLeft) + 1;
		int right = Depth(phead->m_pRight) + 1;
		if(left > right)
			return left;
		else
			return right;
	}
}

void Print(node* phead)
{
	if(phead == NULL)
		return;
	node* nQueue[30];
	nQueue[0] = phead;
	int pQhead = 0, pQtail = 1;
	while(pQhead != pQtail)
	{
		cout << nQueue[pQhead]->m_nValue << " ";
		if(nQueue[pQhead]->m_pLeft != NULL){
			nQueue[pQtail] = nQueue[pQhead]->m_pLeft; 
			pQtail++;
			pQtail %= 30;
		}
		if(nQueue[pQhead]->m_pRight != NULL){
			nQueue[pQtail] = nQueue[pQhead]->m_pRight; 
			pQtail++;
			pQtail %= 30;
		}
		pQhead++;
		pQhead %= 30;
	}
	cout << endl;
}


#endif
