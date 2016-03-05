///////////////////////////////////////////////////////////
//  WhereTrigger.h
//  Implementation of the Class WhereTrigger
//  Created on:      17-ÆßÔÂ-2012 11:21:05
//  Original author: XiaoLei
///////////////////////////////////////////////////////////

#ifndef _KPITRIGGER_BASE_WHERETRIGGER_
#define _KPITRIGGER_BASE_WHERETRIGGER_

#include <string>
#include "Data.hpp"
#include "DataFormat.h"
#include <set>

using namespace std;
using namespace databusiness_common;

namespace kpitrigger
{

const guint64 max_uint64 = (guint64)0xFFFFFFFFFFFFFFFF;
const gint64  max_int64  = (gint64)0x7FFFFFFFFFFFFFFF;
const gint64  min_int64  = (gint64)0xFFFFFFFFFFFFFFFF;
const guint32 max_uint32 = (guint32)0xFFFFFFFF;
const gint32  max_int32  = (gint32)0x7FFFFFFF;
const gint32  min_int32  = (gint32)0xFFFFFFFF;
const guint16 max_uint16 = (guint16)0xFFFF;
const gint16  max_int16  = (gint16)0x7FFF;
const gint16  min_int16  = (gint16)0xFFFF;
const guint8  max_uint8  = (guint8)0xFF;
const gint8   max_int8   = (gint8)0x7F;
const gint8   min_int8   = (gint8)0xFF;


enum WHERE_OPERATOR{
	OP_ERROR,
	OP_NONE,
	OP_TRUE,
	OP_FALSE,
	OP_EQUAL,
	OP_NOT_EQUAL,
	OP_GREATER,
	OP_LESS,
	OP_GREATER_OR_EQUAL,
	OP_LESS_OR_EQUAL,
	OP_IN,
	OP_NOT_IN
};  

enum WHERE_SIGN{
	SIGN_ERROR,
	SIGN_NONE,
	SIGN_LEFT,
	SIGN_RIGHT,
	SIGN_AND,
	SIGN_OR
};

enum WHERE_VALUE{
	POSITIVE_VALUE,
	NEGATIVE_VALUE,
	DECIMAL_VALUE,
	STRING_VALUE,
	NONE_VALUE
};

class Where_OptTable{
	public:
		Where_OptTable(){}
		virtual ~Where_OptTable(){}
		bool isTrue(const Data* data);
	    bool CheckOptStr(const char* src, const char* dst);

		template<typename T>
		bool CheckOptNum(T src, T dst);

		int m_SrcIndex;
		int m_SrcType;
		WHERE_OPERATOR m_Opt;
		set<StatValue> m_DstValues;
}; 

class Where_Node{
	public:
		Where_Node():m_True_Node(NULL),m_False_Node(NULL){}
		bool isTrue(const Data* data);

		Where_Node* m_True_Node;
		Where_Node* m_False_Node;
		Where_OptTable m_OptTable;
};

struct Where_LinkNode{
	public:
		Where_LinkNode():m_Enter_Node(NULL){}
		Where_Node* m_Enter_Node;
		vector<Where_Node**> m_True_Nodes;
		vector<Where_Node**> m_False_Nodes;
};

class WhereTrigger
{

public:
	WhereTrigger():m_Where_Node(NULL){}
	virtual ~WhereTrigger(){}
	
	bool IsTriggered(const Data* data);
	bool Init(string& where, DataFormat* data_format);

private:
	bool FillOptTableStruct(const string& str, Where_OptTable& OptTable, DataFormat* dataformat);
	bool AnalyticWhereCondition(const string& where, DataFormat* data_format);
	bool TransformationValue(const string& str, int type, StatValue& sv);
	WHERE_SIGN FindSign(const string& str, int& offset);
	void LinkAndOrSign(WHERE_SIGN sign, Where_LinkNode& left, Where_LinkNode& right);
	WHERE_OPERATOR FindOperator(const string& str, int& offset, int& end);
	int GetNewStart(WHERE_SIGN sign, int offset);
	bool IsNullString(const string& s);
	bool RemoveSpaceChar(string& str);
	
	bool IsString(string& str);
	WHERE_VALUE IsNum(const string& str);
		
	template<typename T>
	bool IsOutofRange(T num, int type);

	Where_Node* m_Where_Node;
};  
}
#endif 
