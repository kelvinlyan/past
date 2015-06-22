#ifndef _MYTERNARY_H
#define _MYTERNARY_H


struct ternarynode{
	public:
		ternarynode(char c):m_cChar(c),m_bExist(false),m_pLeft(NULL),m_pMid(NULL),m_pRight(NULL){}
		ternarynode* BuildNode(ternarynode* pNode);
		char m_cChar;
		bool m_bExist;
		ternarynode* m_pLeft;
		ternarynode* m_pMid;
		ternarynode* m_pRight;
};

ternarynode* ternarynode::BuildNode(ternarynode* pNode)
{
	if(pNode == NULL)
		return NULL;
	else{
		ternarynode* pNew = new ternarynode(pNode->m_cChar);
		pNew->m_bExist = pNode->m_bExist;
		pNew->m_pLeft = BuildNode(pNode->m_pLeft);
		pNew->m_pMid = BuildNode(pNode->m_pMid);
		pNew->m_pRight = BuildNode(pNode->m_pRight);
		return pNew;
	}
}



class myternaryset{
	private:
		void Insert(ternarynode*& pPre, ternarynode*& pNode, const char* pstr);
		void Destory(ternarynode* pNode);

	public:
		myternaryset();
		myternaryset(const myternaryset& rhs);
		myternaryset& operator =(const myternaryset& rhs);
		~myternaryset();
		void Insert(const char* pstr);
		bool Find(const char* pstr);
		void Erase(const char* pstr);
	private:
		ternarynode* m_pRoot;
};

myternaryset::myternaryset():m_pRoot(NULL){}

myternaryset::myternaryset(const myternaryset& rhs)
{
	m_pRoot = m_pRoot->BuildNode(rhs.m_pRoot);
}

myternaryset& myternaryset::operator =(const myternaryset& rhs)
{
	m_pRoot = m_pRoot->BuildNode(rhs.m_pRoot);
	return *this;
}

myternaryset::~myternaryset()
{
	Destory(m_pRoot);	
}

void myternaryset::Destory(ternarynode* pNode)
{
	if(pNode != NULL)
	{
		Destory(pNode->m_pLeft);
		Destory(pNode->m_pMid);
		Destory(pNode->m_pRight);
		delete pNode;
	}
}

void myternaryset::Insert(const char* pstr)
{
	if(pstr == NULL)
		return;
	if(m_pRoot == NULL){
		if(*pstr != '\0')
			m_pRoot = new ternarynode(*pstr);
		else
			return;
	}
	ternarynode* pNode = m_pRoot;
	if(pNode->m_cChar == *pstr)
		Insert(pNode, pNode->m_pMid, pstr+1);
	else if(pNode->m_cChar < *pstr)
		Insert(pNode, pNode->m_pLeft, pstr);
	else
		Insert(pNode, pNode->m_pRight, pstr);

}

void myternaryset::Insert(ternarynode*& pPre, ternarynode*& pNode, const char* pstr)
{
	if(*pstr == '\0'){
		pPre->m_bExist = true;
		return;
	}
	else{
		if(pNode == NULL){
			pNode = new ternarynode(*pstr);
			Insert(pNode, pNode->m_pMid, pstr+1);
		}else{
			if(pNode->m_cChar == *pstr)
				Insert(pNode, pNode->m_pMid, pstr+1);
			else if(pNode->m_cChar < *pstr)
				Insert(pNode, pNode->m_pLeft, pstr);
			else
				Insert(pNode, pNode->m_pRight, pstr);
		}
	}
}

bool myternaryset::Find(const char* pstr)
{
	ternarynode* pNode = m_pRoot;
	ternarynode* pPre;
	int i = 0;
	while(pstr[i] != '\0')
	{
		if(pNode == NULL)
			return false;
		else{
			if(pNode->m_cChar == pstr[i]){
				pPre = pNode;
				pNode = pNode->m_pMid;
				++i;
			}
			else if(pNode->m_cChar < pstr[i])
				pNode = pNode->m_pLeft;
			else
				pNode = pNode->m_pRight;
		}
	}
	if(pPre->m_bExist)
		return true;
	else
		return false;
}

#endif
