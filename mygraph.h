#ifndef _MYGRAPH_H
#define _MYGRAPH_H


//几个问题：
//1.判断一个图是否连通(每个结点到其他结点是否存在路径)
//  深度优先和广度优先搜索可以知道一个结点到其他结点是否存在路径，这样只要进行n次，就可以判断图是否连通。
//2.判断加入一条边之后是否存在圈
//  从边的起点开始搜索，看能否回到起点。
//3.最小生成树(Prim和Kruskal)
//	首先必须保证图是连通的
//  对于无向图，Prim每次选取一个与现有树权值最短的新结点连接进来 问题：如何取得权值最短的新结点 效率考虑
//              Kruskal每次选取一条最短边(不构成圈)连接进来 问题：判断是否构成圈 效率考虑
//  对于有向图，每次选取最长的边，如果删除该边，2个结点仍能连通，则删除，直到所有边都遍历完
//4.最短路径(Dijkstra)
//  求一个结点到其他所有结点的最短路径，在Dijkstra基础上能否改良
//  求每个结点间的最短路径，对每个结点分别求，是否存在其他方法
//5.具有负边值的图如何处理
//6.网络流问题
//7.拓扑排序的现实意义









#include <vector>
#include <string>
#include <map>
#include <bitset>
#include "myheap.h"
//#include <unordered_map>
using namespace std;

typedef map<string, int> container;

struct gLine{
	public:
		gLine(int in, int out, int value):m_nIn(in),m_nOut(out),m_nValue(value),m_pInNext(NULL),m_pInPre(NULL),m_pOutNext(NULL),m_pOutPre(NULL){}
		int m_nIn;
		int m_nOut;
		int m_nValue;
		gLine* m_pInNext; //以in为起点的下一条边
		gLine* m_pInPre;  //以in为起点的上一条边
		gLine* m_pOutNext; //以out为终点的下一条边
		gLine* m_pOutPre; //以out为终点的上一条边
		void* m_pTemp;    //保留扩展使用
};

struct cmpgLine{ 
	public:
		bool operator ==(const cmpgLine& right)
		{
			return m_pgLine->m_nValue == right.m_pgLine->m_nValue;
		}
		bool operator <(const cmpgLine& right)
		{
			return m_pgLine->m_nValue < right.m_pgLine->m_nValue;
		}
		bool operator <=(const cmpgLine& right)
		{
			return m_pgLine->m_nValue <= right.m_pgLine->m_nValue;
		}
		bool operator >(const cmpgLine& right)
		{
			return m_pgLine->m_nValue > right.m_pgLine->m_nValue;
		}
		bool operator >=(const cmpgLine& right)
		{
			return m_pgLine->m_nValue >= right.m_pgLine->m_nValue;
		}
		gLine* m_pgLine;	
};

struct gNode{
	public:
		gNode(const string& name):m_sName(name),m_pInList(NULL),m_pOutList(NULL){}
		string m_sName;
		gLine* m_pInList; //入边集
		gLine* m_pOutList; //出边集
};

//有向图 十字邻接表存储
class mygraph{ 
	private:
		const string& GetPointName(int index);
		bool isLinked(int nIn, int nOut, bitset<8>* pBitsets);
		bool isLinked(int nIn, int nOut, bitset<8>* pBitsets, bool(*fptrget)(void* arg), void(*fptrset)(void* arg));
		bool Erase(int nIn, int nOut);
	public:
		mygraph();
		mygraph(const mygraph& rhs);
		mygraph& operator =(const mygraph& rhs);
		~mygraph();
		void FindPathOneToAll(const string& point); // 最短路径--迪克斯特拉算法 Dijkstra
		void FindPathAllToAll(const string& point);
		mygraph* MinSpanTree(); // 最小生成树--普利姆算法 Prim
		void MinSpanTree2(); //最小生成树--克鲁斯卡尔算法 Kruskal
		void Insert(const string& inPoint, const string& outPoint, int value);
		void Erase(const string& point);
		void Erase(const string& inPoint, const string& outPoint);
		void Clear();
		void PrintList(const string& point);
		bool isLinked(int nIn, int nOut, bool(*fptrget)(void* arg), void(*fptrset)(void* arg));
		bool isLinked(int nIn, int nOut);
		int Size();
	private:
		container m_indexMap;
		vector<gNode*> m_vgNodes;
};

mygraph::mygraph()
{
}

mygraph::mygraph(const mygraph& rhs)
{
	gNode* pgNode;
	gLine* pgLine;
	for(int i = 0; i < rhs.m_vgNodes.size(); ++i)
	{
		pgNode = rhs.m_vgNodes[i];
		pgLine = pgNode->m_pOutList;
		while(pgLine != NULL)
		{
			Insert(rhs.m_vgNodes[pgLine->m_nIn]->m_sName, rhs.m_vgNodes[pgLine->m_nOut]->m_sName, pgLine->m_nValue);
			pgLine = pgLine->m_pInNext;
		}
	}
}

mygraph& mygraph::operator =(const mygraph& rhs)
{
	gNode* pgNode;
	gLine* pgLine;
	for(int i = 0; i < rhs.m_vgNodes.size(); ++i)
	{
		pgNode = rhs.m_vgNodes[i];
		pgLine = pgNode->m_pOutList;
		while(pgLine != NULL)
		{
			Insert(rhs.m_vgNodes[pgLine->m_nIn]->m_sName, rhs.m_vgNodes[pgLine->m_nOut]->m_sName, pgLine->m_nValue);
			pgLine = pgLine->m_pInNext;
		}
	}
	return *this;
}

mygraph::~mygraph()
{
	Clear();
}

//由结点下标返回结点名字
inline const string& mygraph::GetPointName(int index)
{
	return m_vgNodes[index]->m_sName;
}

//返回结点个数
inline int mygraph::Size()
{
	return m_vgNodes.size();
}

//打印出该结点的边集合
void mygraph::PrintList(const string& point)
{
	container::iterator iter;
	if((iter = m_indexMap.find(point)) == m_indexMap.end())
	{
		cout << "Point <" << point << "> not Exist!" << endl; 
		return;
	}
	else{
		int index = iter->second;
		gNode* pgNode = m_vgNodes[index];
		gLine* pgLine = pgNode->m_pInList;
		cout << "Point <" << point << "> Index <" << index << ">" << endl; 
		cout << "In: ";
		while(pgLine != NULL)
		{
			cout << m_vgNodes[pgLine->m_nIn]->m_sName << " ";
			pgLine = pgLine->m_pOutNext;
		}
		cout << endl;
		cout << "Out: ";
		pgLine = pgNode->m_pOutList;
		while(pgLine != NULL)
		{
			cout << m_vgNodes[pgLine->m_nOut]->m_sName << " ";
			pgLine = pgLine->m_pInNext;
		}
		cout << endl;
	}
}

//插入边
void mygraph::Insert(const string& inPoint, const string& outPoint, int value)
{
	gNode* pIngNode;
    gNode* pOutgNode;
	container::iterator iter;
	int inIndex, outIndex;

	//查找边的两个结点，如果结点不存在，则新增结点。
	if((iter = m_indexMap.find(inPoint)) == m_indexMap.end())
	{
		inIndex = m_vgNodes.size();
		pIngNode = new gNode(inPoint);
		m_vgNodes.push_back(pIngNode);
		m_indexMap.insert(make_pair(inPoint, inIndex));
	}
	else{
		inIndex = iter->second;
		pIngNode = m_vgNodes[inIndex];
	}

	if((iter = m_indexMap.find(outPoint)) == m_indexMap.end())
	{
		outIndex = m_vgNodes.size();
		pOutgNode = new gNode(outPoint);
		m_vgNodes.push_back(pOutgNode);
		m_indexMap.insert(make_pair(outPoint, outIndex));
	}
	else{
		outIndex = iter->second;
		pOutgNode = m_vgNodes[outIndex];
	}

	gLine* pNewgLine = new gLine(inIndex, outIndex, value);	
	gLine* pIngLine = pIngNode->m_pOutList;     
	gLine* pOutgLine = pOutgNode->m_pInList;    

	//更新inPoint结点的出边集合
	if(pIngLine == NULL){
		pIngNode->m_pOutList = pNewgLine;
	}
	else{
		gLine* ptemp = pIngLine;
		while(pIngLine != NULL)
		{
			ptemp = pIngLine;
			pIngLine = pIngLine->m_pInNext;
		}
		ptemp->m_pInNext = pNewgLine;
		pNewgLine->m_pInPre = ptemp;
	}

	//更新outPoint结点的入边集合
	if(pOutgLine == NULL){
		pOutgNode->m_pInList = pNewgLine;
	}
	else{
		gLine* ptemp = pOutgLine;
		while(pOutgLine != NULL)
		{
			ptemp = pOutgLine;
			pOutgLine = pOutgLine->m_pOutNext;
		}
		ptemp->m_pOutNext = pNewgLine;
		pNewgLine->m_pOutPre = ptemp;
	}
	cout << "Insert Line < " << inPoint << ", " << outPoint << ", " << value << " > Success!" << endl;
}

//删除结点
void mygraph::Erase(const string& point)
{
	container::iterator iter;
	//查找结点
	if((iter = m_indexMap.find(point)) == m_indexMap.end())
	{
		cout << "Point <" << point << "> not Exist!" << endl; 
		return;
	}
	else{
		int index = iter->second;
		m_indexMap.erase(iter);
		gNode* pgNode = m_vgNodes[index];

		//删除结点的入边集合
		gLine* pgLine = pgNode->m_pInList;
		gLine* pPre;
		gLine* pNext;
		gLine* pDelgLine;
		while(pgLine != NULL)
		{
			pPre = pgLine->m_pInPre;
			pNext = pgLine->m_pInNext;
			if(pPre == NULL)
				m_vgNodes[pgLine->m_nIn]->m_pOutList = pNext;
			else
				pPre->m_pInNext = pNext;
			if(pNext != NULL)
				pNext->m_pInPre = pPre;
			pDelgLine = pgLine;
			pgLine = pgLine->m_pOutNext;
			delete pDelgLine;
		}

		//删除结点的出边集合
		pgLine = pgNode->m_pOutList;
		while(pgLine != NULL)
		{
			pPre = pgLine->m_pOutPre;
			pNext = pgLine->m_pOutNext;
			if(pPre == NULL)
				m_vgNodes[pgLine->m_nOut]->m_pInList = pNext;
			else
				pPre->m_pOutNext = pNext;
			if(pNext != NULL)
				pNext->m_pOutPre = pPre;
			pDelgLine = pgLine;
			pgLine = pgLine->m_pInNext;
			delete pDelgLine;
		}

		//删除结点
		delete m_vgNodes[index];

		//如果被删除结点不是m_vgNodes的最后一个结点，则把最后一个结点移到被删除结点的位置，并更新边集合和m_indexMap的值
		if(index != m_vgNodes.size() - 1){
			m_vgNodes[index] = m_vgNodes.back();	
			gLine* ptemp = m_vgNodes[index]->m_pInList;
			while(ptemp != NULL)
			{
				ptemp->m_nOut = index;
				ptemp = ptemp->m_pOutNext;
			}
			ptemp = m_vgNodes[index]->m_pOutList;
			while(ptemp != NULL)
			{
				ptemp->m_nIn = index;
				ptemp = ptemp->m_pInNext;
			}
			m_indexMap[m_vgNodes.back()->m_sName] = index;
		}
		m_vgNodes.pop_back();
	}
	cout << "Erase Point <" << point << "> success!" << endl;
} 

//删除边
void mygraph::Erase(const string& inPoint, const string& outPoint)
{
	container::iterator iter;
	//查找结点是否存在，获取结点下标
	if((iter = m_indexMap.find(inPoint)) == m_indexMap.end())
	{
		cout << "Point <" << inPoint << "> not Exist!" << endl;
		return;
	}
	int nIn = iter->second;
	if((iter = m_indexMap.find(outPoint)) == m_indexMap.end())
	{
		cout << "Point <" << outPoint << "> not Exist!" << endl;
		return;
	}
	int nOut = iter->second;
	
	//执行删除边的操作
	if(Erase(nIn, nOut))
		cout << "Erase Line <" << inPoint << "," << outPoint << "> success!" << endl;
	else
		cout << "Line <" << inPoint << "," << outPoint << "> not Exist!" << endl;
}

//删除边
bool mygraph::Erase(int nIn, int nOut)
{
	gNode* pIngNode = m_vgNodes[nIn];
	gNode* pOutgNode = m_vgNodes[nOut];
	gLine* pgLine = pIngNode->m_pOutList;
	bool result = false;
	//查找边
	while(pgLine != NULL)
	{
		//链表操作，改变与边相关的边的pre指针和next指针
		if(pgLine->m_nOut == nOut)
		{
			if(pgLine->m_pInPre == NULL){
				pIngNode->m_pOutList = pgLine->m_pInNext;
				if(pgLine->m_pInNext != NULL)
					pgLine->m_pInNext->m_pInPre = NULL;
			}
			else{
				pgLine->m_pInPre->m_pInNext = pgLine->m_pInNext;
				if(pgLine->m_pInNext != NULL)
					pgLine->m_pInNext->m_pInPre = pgLine->m_pInPre;
			}
			if(pgLine->m_pOutPre == NULL){
				pOutgNode->m_pInList = pgLine->m_pOutNext;
				if(pgLine->m_pOutNext != NULL)
					pgLine->m_pOutNext->m_pOutPre = NULL;
			}
			else{
				pgLine->m_pOutPre->m_pOutNext = pgLine->m_pOutNext;
				if(pgLine->m_pOutNext != NULL)
					pgLine->m_pOutNext->m_pOutPre = pgLine->m_pOutPre;
			}
			delete pgLine;             //释放边资源
			result = true;
			break;
		}
		pgLine = pgLine->m_pInNext;
	}
	return result;
} 

//清空所有结点
void mygraph::Clear()
{
	//1.删除每个结点的出边集合(释放边资源)
	//2.删除该结点(释放结点资源)
	//3.清空m_vgNodes和m_indexMap
	gLine* pIngLine;
	gLine* pTemp;
	for(int i = 0; i < m_vgNodes.size(); ++i)
	{
		pIngLine = m_vgNodes[i]->m_pOutList;
		pTemp = pIngLine;
		while(pIngLine != NULL)
		{
			pTemp = pIngLine;
			pIngLine = pIngLine->m_pInNext;
			delete pTemp;
		}
		delete m_vgNodes[i];
	}
	m_vgNodes.clear();
	m_indexMap.clear();
	cout << "Clear Done!" << endl;
}

void mygraph::MinSpanTree2()
{
	vector<cmpgLine> vgLines;;
	for(int i = 0; i < m_vgNodes.size(); ++i)
	{
		gNode* pgNode = m_vgNodes[i];
		gLine* pgLine = pgNode->m_pOutList;
		while(pgLine != NULL)
		{
			cmpgLine temp;
			temp.m_pgLine = pgLine;
			vgLines.push_back(temp);
			pgLine = pgLine->m_pInNext;
		}
	}
	if(vgLines.empty())
		return;
	myquicksort<cmpgLine>(&vgLines.front(), 0, vgLines.size());
	mygraph* pMinSpanTree = new mygraph;
	int bytes = m_vgNodes.size() / 8 + 1;
	bitset<8>* pBitsets = new bitset<8>[bytes];
	int nCount = 0;
	for(int i = 0; i < vgLines.size(); ++i)
	{
		gLine* pgLine = vgLines[i].m_pgLine;	
		int nIn = pgLine->m_nIn;
		int nOut = pgLine->m_nOut;
		if(pBitsets[nIn / 8].test(nIn % 8))
		{
			if(pBitsets[nOut / 8].test(nOut % 8))
				continue;
			else{
				pBitsets[nOut / 8].set(nOut % 8);
				nCount++;
				pMinSpanTree->Insert(m_vgNodes[nIn]->m_sName, m_vgNodes[nOut]->m_sName, pgLine->m_nValue);
			}
		}
		else{
			pBitsets[nIn / 8].set(nIn % 8);
			nCount++;
			if(!pBitsets[nOut / 8].test(nOut % 8))
			{
				pBitsets[nOut / 8].set(nOut % 8);
				nCount++;
			}
			pMinSpanTree->Insert(m_vgNodes[nIn]->m_sName, m_vgNodes[nOut]->m_sName, pgLine->m_nValue);
		}
		if(nCount == m_vgNodes.size())
		{
			for(int i = 0; i < pMinSpanTree->Size(); ++i)
				pMinSpanTree->PrintList(GetPointName(i));
		}
	}
	delete pMinSpanTree;
	delete pBitsets;
} 

bool GetUsed(void* ptr)
{
	return *((char*)ptr) == 1;
}

void SetUsed(void* ptr)
{
}

//最小生成树
mygraph* mygraph::MinSpanTree()
{
	//这里必须保证原来的图是连通的(前提)
	//对所有边进行排序，按权值从大到小开始选取，如果删除该边，边的两个结点仍然连通，则删除。
	mygraph* pGraph = new mygraph(*this);
	vector<cmpgLine> vgLines;
	for(int i = 0; i < pGraph->m_vgNodes.size(); ++i)
	{
		gNode* pgNode = pGraph->m_vgNodes[i];
		gLine* pgLine = pgNode->m_pOutList;
		while(pgLine != NULL)
		{
			cmpgLine temp;
			temp.m_pgLine = pgLine;
			pgLine->m_pTemp = (void*)new char(0);       //保留项，用作表示边是否已删除，如果已删除，则置为1，一开始都为0。
			vgLines.push_back(temp);
			pgLine = pgLine->m_pInNext;
		}
	}
	if(vgLines.empty())
		return NULL;
	myquicksort<cmpgLine>(&vgLines.front(), 0, vgLines.size());      //对边排序
	int nValue, nIn, nOut;
	for(int i = vgLines.size() - 1; i >= 0; --i)
	{
		nValue = vgLines[i].m_pgLine->m_nValue;
		nIn = vgLines[i].m_pgLine->m_nIn;
		nOut = vgLines[i].m_pgLine->m_nOut;
		*((char*)vgLines[i].m_pgLine->m_pTemp) = 1;           //保留项先置1表示边已删除，如果之后判断两结点仍连通，则删除该边，否则把保留项置回0
		if(pGraph->isLinked(nIn, nOut, GetUsed, SetUsed))
			pGraph->Erase(nIn, nOut);
		else
			*((char*)vgLines[i].m_pgLine->m_pTemp) = 0;
	}
	for(int i = 0; i < vgLines.size(); ++i)               //释放保留项的资源
		delete (char*)vgLines[i].m_pgLine->m_pTemp;
	return pGraph;
}

//判断两个结点是否连通
bool mygraph::isLinked(int nIn, int nOut)
{
	bitset<8>* pBitsets = new bitset<8>[m_vgNodes.size() / 8 + 1];
	bool result;
	result = isLinked(nIn, nOut, pBitsets);
	delete pBitsets;
	return result;
}

//判断两个结点是否连通
bool mygraph::isLinked(int nIn, int nOut, bool(*fptrget)(void* arg), void(*fptrset)(void* arg))
{
	bitset<8>* pBitsets = new bitset<8>[m_vgNodes.size() / 8 + 1];
	bool result;
	if(fptrget == NULL || fptrset == NULL)
		result = isLinked(nIn, nOut, pBitsets);
	else
		result = isLinked(nIn, nOut, pBitsets, fptrget, fptrset);
	delete pBitsets;
	return result;
}

//判断两个结点是否连通
bool mygraph::isLinked(int nIn, int nOut, bitset<8>* pBitsets)
{
	gNode* pgNode = m_vgNodes[nIn];
	gLine* pgLine = pgNode->m_pOutList;
	while(pgLine != NULL)
	{
		if(!pBitsets[pgLine->m_nOut / 8].test(pgLine->m_nOut % 8))
		{
			if(pgLine->m_nOut == nOut)
				return true;
			else{
				pBitsets[pgLine->m_nOut / 8].set(pgLine->m_nOut % 8);
				if(isLinked(pgLine->m_nOut, nOut, pBitsets))
					return true;
			}
		}
		pgLine = pgLine->m_pInNext;
	}
	return false;
}

//判断两个结点是否连通
bool mygraph::isLinked(int nIn, int nOut, bitset<8>* pBitsets, bool(*fptrget)(void* arg), void(*fptrset)(void* arg))
{
	gNode* pgNode = m_vgNodes[nIn];
	gLine* pgLine = pgNode->m_pOutList;
	while(pgLine != NULL)
	{
		if(!(*fptrget)(pgLine->m_pTemp))
		{
			if(!pBitsets[pgLine->m_nOut / 8].test(pgLine->m_nOut % 8))
			{
				if(pgLine->m_nOut == nOut)
					return true;
				else{
					pBitsets[pgLine->m_nOut / 8].set(pgLine->m_nOut % 8);
					if(isLinked(pgLine->m_nOut, nOut, pBitsets, fptrget, fptrset))
						return true;
				}
			}
			(*fptrset)(pgLine->m_pTemp);
		}
		pgLine = pgLine->m_pInNext;
	}
	return false;
}

void mygraph::FindPathOneToAll(const string& point)
{
	
}











#endif
