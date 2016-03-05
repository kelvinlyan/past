///////////////////////////////////////////////////////////
//  WhereTrigger.cpp
//  Implementation of the Class WhereTrigger
//  Created on:      17-ÆßÔÂ-2012 11:21:05
//  Original author: XiaoLei
///////////////////////////////////////////////////////////

#include "base/WhereTrigger.h"
#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <string>
#include "Log4cxxLogger.h"

using namespace std;
using namespace kpitrigger;

vector<string> UnAllowedWords = {" not ", " in ", " in(", ">", "<", "=", "!", "(", ")"};

bool WhereTrigger::IsTriggered(const Data* data){
	bool result = true;
	Where_Node* pNode = m_Where_Node;
	while(pNode != NULL)
	{
		if(pNode->isTrue(data))
		{
			result = true;
			pNode = pNode->m_True_Node;
		}else{
			result = false;
			pNode = pNode->m_False_Node;
		}
	}
	return result;
}

bool WhereTrigger::TransformationValue(const string& str, int type, StatValue& sv)
{
	guint64 u64src = 0;
	gint64 i64src = 0;
	double flosrc = 0;
	string s;
	WHERE_VALUE temp;
	temp = IsNum(str);
	switch(temp)
	{
		case WHERE_VALUE::POSITIVE_VALUE:
			if(type == SVT_STRING)
			{
				Log4cxxLogger::GetObj().Error("WhereTrigger", "StatValue's ValueType is 10(SVT_STRING), you have to add a pair of (\'), like(SGSN = \'SGSN206\')");
				return false;
			}
			u64src = strtoul(str.c_str(), NULL, 10);
			if(IsOutofRange<guint64>(u64src, type))
				return false;
			break;
		case WHERE_VALUE::NEGATIVE_VALUE:
			if(type == SVT_STRING)
			{
				Log4cxxLogger::GetObj().Error("WhereTrigger", "StatValue's ValueType is 10(SVT_STRING), you have to add a pair of (\'), like(SGSN = \'SGSN206\')");
				return false;
			}
			i64src = strtol(str.c_str(), NULL, 10);
			if(IsOutofRange<gint64>(i64src, type))
				return false;
			break;
		case WHERE_VALUE::DECIMAL_VALUE:
			if(type == SVT_STRING)
			{
				Log4cxxLogger::GetObj().Error("WhereTrigger", "StatValue's ValueType is 10(SVT_STRING), you have to add a pair of (\'), like(SGSN = \'SGSN206\')");
				return false;
			}
			flosrc = strtod(str.c_str(), NULL);
			break;
		case WHERE_VALUE::NONE_VALUE:
			if(type != SVT_STRING)
			{
				ostringstream os;
				os << "StatValue's ValueType is " << type << " , But \"" << str << "\" is not a Number!";
				Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
				return false;
			}
			s = str;
			if(!IsString(s)){
				Log4cxxLogger::GetObj().Error("WhereTrigger", "StatValue's ValueType is 10(SVT_STRING), you have to add a pair of (\'), like(SGSN = \'SGSN206\')");
				return false;
			}
			break;
		default:
			return false;
	}
	switch(type) 
	{
		case SVT_UNKNOWN:
			return false;
		case SVT_UINT8:
			sv.SetStatValueU8((guint8)u64src);
			return true;
		case SVT_UINT16:
			sv.SetStatValueU16((guint16)u64src);
			return true;
		case SVT_UINT32:
			sv.SetStatValueU32((guint32)u64src);
			return true;
		case SVT_UINT64:
			sv.SetStatValueU64(u64src);
			return true;
		case SVT_INT8:
			sv.SetStatValueI8((gint8)i64src);
			return true;
		case SVT_INT16:
			sv.SetStatValueI16((gint16)i64src);
			return true;
		case SVT_INT32:
			sv.SetStatValueI32((gint32)i64src);
			return true;
		case SVT_INT64:
			sv.SetStatValueI64(i64src);
			return true;
		case SVT_FLOAT:
			sv.SetStatValueFloat((gfloat)flosrc);
			return true;
		case SVT_STRING:
			sv.SetStatValueString(s.c_str());
			return true;
		default:
			return false;
	}
}

bool WhereTrigger::IsString(string& str)
{
	int offset = 0, begin = 0, end = 0;
	while(offset < str.size())
	{
		if(str[offset] == ' ')
			++offset;
		else
			break;
	}
	if(offset == str.size())
		return false;
	if(str[offset++] != '\'')
		return false;
	begin = offset;
	while(offset < str.size())
	{
		if(str[offset] == '\''){
			end = offset;
			break;
		}
		else
			++offset;
	}
	if(offset == str.size())
		return false;
	string s = str.substr(begin, end - begin);
	if(IsNullString(s))
		return false;
	++offset;
	while(offset < str.size())
	{
		if(str[offset] != ' ')
			return false;
		++offset;
	}
	str = s;
	return true;
}

WHERE_VALUE WhereTrigger::IsNum(const string& str){
	int offset = 0, begin = 0, end = 0, pointOffset = 0;
	bool pointFlag = false, signFlag = false;
	while(offset < str.size() && str[offset] == ' ')
		++offset;
	if(offset == str.size())
		return WHERE_VALUE::NONE_VALUE;
	begin = offset;
	while(offset < str.size() && str[offset] != ' ')
		++offset;
	end = offset;
	if(offset != str.size())
	{
		while(offset < str.size())
		{
			if(str[offset] == ' ')
				++offset;
			else
				return WHERE_VALUE::NONE_VALUE;
		}
	}
	offset = 0;
	string s = str.substr(begin, end - begin);
	if(s[0] == '-')
		signFlag = true;
	while(offset < s.size())
	{
		if(s[offset] == '.'){
			if(pointFlag == false){
				pointFlag = true;
				pointOffset = offset;
			}else
				return WHERE_VALUE::NONE_VALUE;
		}
		++offset;
	}
	if(pointFlag == true && pointOffset + 1 >= s.size())
		return WHERE_VALUE::NONE_VALUE;
	if(signFlag == true){
		if(pointFlag == true){
			if(pointOffset < 2)
				return WHERE_VALUE::NONE_VALUE;
			if(s[1] == '0'){
				if(s[2] >= '0' && s[2] <= '9')
					return WHERE_VALUE::NONE_VALUE;
			}
		}
		else{
			if(s[1] == '0')
				return WHERE_VALUE::NONE_VALUE;
		}
	}else{
		if(s[0] < '0' || s[0] > '9')
			return WHERE_VALUE::NONE_VALUE;
		if(pointFlag == true){
			if(pointOffset < 1)
				return WHERE_VALUE::NONE_VALUE;
			if(s[0] == '0'){
				if(s[1] >= '0' && s[1] <= '9')
					return WHERE_VALUE::NONE_VALUE;
			}
		}else{
			if(s.size() > 1)
			{
				if(s[0] == '0'){
					if(s[1] >= '0' && s[1] <= '9')
						return WHERE_VALUE::NONE_VALUE;
				}
			}
		}
	}
	offset = 1;
	while(offset < s.size())
	{
		if(pointFlag == true){
			if(offset != pointOffset){
				if(s[offset] < '0' || s[offset] > '9')
					return WHERE_VALUE::NONE_VALUE;
			}
		}else{
			if(s[offset] < '0' || s[offset] > '9')
				return WHERE_VALUE::NONE_VALUE;
		}
		++offset;
	}
	if(pointFlag == true)
		return WHERE_VALUE::DECIMAL_VALUE;
	if(signFlag == true)
		return WHERE_VALUE::NEGATIVE_VALUE;
	return WHERE_VALUE::POSITIVE_VALUE;
}

bool WhereTrigger::AnalyticWhereCondition(const string& str, DataFormat* dataformat)
{
	WHERE_SIGN temp, lastSign = WHERE_SIGN::SIGN_NONE;
	int offset = 0, start = 0, end = 0, lastOffset = 0;
	vector<Where_LinkNode> LinkNodeVec;
	vector<WHERE_SIGN> SignVec;
	while((temp = FindSign(str, offset)) != WHERE_SIGN::SIGN_NONE)
	{
		lastSign = temp;
		lastOffset = offset;
		if(temp == WHERE_SIGN::SIGN_ERROR)
			return false;
		if(offset == start)
			start = GetNewStart(temp, offset);
		else{
			end = offset;
			string s(str.begin() + start, str.begin() + end);
			start = GetNewStart(temp, offset);
			if(!IsNullString(s))
			{
				Where_Node* oneNode = new Where_Node();
				if(!FillOptTableStruct(s, oneNode->m_OptTable, dataformat))
					return false;
				Where_LinkNode oneLinkNode;
				oneLinkNode.m_Enter_Node = oneNode;
				oneLinkNode.m_True_Nodes.push_back(&oneNode->m_True_Node);
				oneLinkNode.m_False_Nodes.push_back(&oneNode->m_False_Node);
				LinkNodeVec.push_back(oneLinkNode);
			}
		}
		switch(temp)
		{
			case WHERE_SIGN::SIGN_LEFT:
				SignVec.push_back(temp);
				break;
			case WHERE_SIGN::SIGN_AND:
				while(!SignVec.empty() && SignVec.back() == WHERE_SIGN::SIGN_AND)
				{
					SignVec.pop_back();
					if(LinkNodeVec.size() < 2)
						return false;
					LinkAndOrSign(WHERE_SIGN::SIGN_AND, LinkNodeVec[LinkNodeVec.size() - 2], LinkNodeVec[LinkNodeVec.size() - 1]);
					LinkNodeVec.pop_back();
				}
				SignVec.push_back(temp);
				break;
			case WHERE_SIGN::SIGN_OR:
				while(!SignVec.empty() && SignVec.back() != WHERE_SIGN::SIGN_LEFT)
				{
					WHERE_SIGN sign = SignVec.back();
					SignVec.pop_back();
					if(LinkNodeVec.size() < 2)
						return false;
					LinkAndOrSign(sign, LinkNodeVec[LinkNodeVec.size() - 2], LinkNodeVec[LinkNodeVec.size() - 1]);
					LinkNodeVec.pop_back();
				}
				SignVec.push_back(temp);
				break;
			case WHERE_SIGN::SIGN_RIGHT:
				while(!SignVec.empty() && SignVec.back() != WHERE_SIGN::SIGN_LEFT)
				{
					WHERE_SIGN sign = SignVec.back();
					SignVec.pop_back();
					if(LinkNodeVec.size() < 2)
						return false;
					LinkAndOrSign(sign, LinkNodeVec[LinkNodeVec.size() - 2], LinkNodeVec[LinkNodeVec.size() - 1]);
					LinkNodeVec.pop_back();
				}
				if(SignVec.empty())
					return false;
				SignVec.pop_back();
				break;
			default:
				return false;
				break;
		}
		++offset;
	}
	if(lastSign == WHERE_SIGN::SIGN_NONE){
		if(offset == 0 || IsNullString(str))
			return true;
	}else{
		start = GetNewStart(lastSign, lastOffset);
	}
	if(start != str.size())
	{
		string s(str.begin() + start, str.end());

		if(!IsNullString(s))
		{
			Where_Node* oneNode = new Where_Node();
			if(!FillOptTableStruct(s, oneNode->m_OptTable, dataformat)){
				return false;
			}
			Where_LinkNode oneLinkNode;
			oneLinkNode.m_Enter_Node = oneNode;
			oneLinkNode.m_True_Nodes.push_back(&oneNode->m_True_Node);
			oneLinkNode.m_False_Nodes.push_back(&oneNode->m_False_Node);
			LinkNodeVec.push_back(oneLinkNode);
			while(!SignVec.empty())
			{
				WHERE_SIGN sign = SignVec.back();
				SignVec.pop_back();
				if(LinkNodeVec.size() < 2)
					return false;
				LinkAndOrSign(sign, LinkNodeVec[LinkNodeVec.size() - 2], LinkNodeVec[LinkNodeVec.size() - 1]);
				LinkNodeVec.pop_back();
			}
		}
	}
	while(LinkNodeVec.size() != 1)
		return false;
	m_Where_Node = LinkNodeVec[0].m_Enter_Node;
    //cout << "----------------------------------------------------------------------------------" << endl;
    //PrintNode(m_Where_Node);
	return true;
} 

bool WhereTrigger::FillOptTableStruct(const string& str, Where_OptTable& OptTable, DataFormat* dataformat)
{
	int offset = 0 ,end = 0, temp = 0, last = 0;
	int index = 0, type = 0;
	StatValue sv;
	string s, st;
	WHERE_OPERATOR opt = FindOperator(str, offset, end);
	cout << "OneString:" << str << ".Opt:" << opt << endl;
	switch(opt)
	{
		case WHERE_OPERATOR::OP_ERROR:
		case WHERE_OPERATOR::OP_NONE:
			return false;
		case WHERE_OPERATOR::OP_EQUAL:
		case WHERE_OPERATOR::OP_NOT_EQUAL:
		case WHERE_OPERATOR::OP_GREATER:
		case WHERE_OPERATOR::OP_GREATER_OR_EQUAL:
		case WHERE_OPERATOR::OP_LESS:
		case WHERE_OPERATOR::OP_LESS_OR_EQUAL:
			s = str.substr(0, offset);
			if(!RemoveSpaceChar(s))
				return false;
			cout << "source string:" << s << "." << endl;
			index = dataformat->GetFieldIndex(s);
			if(index == -1){
				ostringstream os;
				os << "\"" << s << "\"" << " can't be Found in DataFormat.xml";
				Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
				return false;
			}
			OptTable.m_SrcIndex = index;
			type = dataformat->GetFieldType(s);
			if(type == -1)
				return false;
 			OptTable.m_SrcType = type;
			s = str.substr(end, str.size() - end);
			if(!RemoveSpaceChar(s))
				return false;
			cout << "destine string:" << s << "." << endl;
			if(!TransformationValue(s.c_str(), OptTable.m_SrcType, sv)){
				return false;
			}
			cout << "Transform Value Success!" << endl;
			OptTable.m_DstValues.insert(sv);
			OptTable.m_Opt = opt;
			return true;
		case WHERE_OPERATOR::OP_IN:
		case WHERE_OPERATOR::OP_NOT_IN:
			s = str.substr(0, offset);
			if(!RemoveSpaceChar(s))
				return false;
			cout << "source string:" << s << "." << endl;
			index = dataformat->GetFieldIndex(s);
			if(index == -1){
				ostringstream os;
				os << "\"" << s << "\"" << " can't be Found in DataFormat.xml";
				Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
				return false;
			}
			OptTable.m_SrcIndex = index;
			type = dataformat->GetFieldType(s);
			if(type == -1)
				return false;
			OptTable.m_SrcType = type;
			temp = offset;
			while(str[temp++] != '(');
			s = str.substr(temp, end - temp - 1);
			cout << "destine string:" << s << "." << endl;
			temp = 0;
			last = 0;
			while(temp < s.size())
			{
				if(s[temp] == ',')
				{
					st = s.substr(last, temp - last);
					if(!RemoveSpaceChar(st))
						return false;
					cout << "split string:" << st << "." << endl;
					if(!TransformationValue(st.c_str(), OptTable.m_SrcType, sv))
						return false;
					cout << "Transform Split Value Success!" << endl;
					OptTable.m_DstValues.insert(sv);
					last = temp + 1;
				}
				++temp;
			}
			st = s.substr(last, temp - last);
			if(!RemoveSpaceChar(st))
				return false;
			cout << "split string:" << st << "." << endl;
			if(!TransformationValue(st.c_str(), OptTable.m_SrcType, sv))
				return false;
			cout << "Transform Value Success!" << endl;
			OptTable.m_DstValues.insert(sv);
			OptTable.m_Opt = opt;
			return true;
		default:
			return false;
	}
}

WHERE_SIGN WhereTrigger::FindSign(const string& str, int& offset)
{
	if(offset < 0)
		return WHERE_SIGN::SIGN_ERROR;
	bool in_Flag = false;
	int temp = 0;
	while(offset < str.size())
	{
		switch(str[offset])
		{
			case '(':
				temp = offset - 1;
				if(temp > 2)
				{
					while(str[temp] == ' ' && temp > 2)
						--temp;
					if(str[temp - 2] == ' ' && str[temp - 1] == 'i' && str[temp] == 'n')
						in_Flag = true;
					else
						in_Flag = false;
				}
				if(in_Flag == true)
					break;
				else
					return WHERE_SIGN::SIGN_LEFT;
			case ')':
				if(in_Flag == true)
				{
					in_Flag = false;
					break;
				}
				else
					return WHERE_SIGN::SIGN_RIGHT;

			case 'a':
				if(offset < 2 || offset + 5 > str.size())
					break;
				if(str[offset - 1] == ' ' && str[offset + 1] == 'n' && str[offset + 2] == 'd' && str[offset + 3] == ' ')
					return WHERE_SIGN::SIGN_AND;
			case 'o':
				if(offset < 2 || offset + 4 > str.size())
					break;
				if(str[offset - 1] == ' ' && str[offset + 1] == 'r' && str[offset + 2] == ' ')
					return WHERE_SIGN::SIGN_OR;
			default:
				break;
		}
		++offset;
	}
	return WHERE_SIGN::SIGN_NONE;
}

int WhereTrigger::GetNewStart(WHERE_SIGN sign, int offset)
{
	switch(sign)
	{
		case WHERE_SIGN::SIGN_LEFT:
		case WHERE_SIGN::SIGN_RIGHT:
			return offset + 1;
		case WHERE_SIGN::SIGN_AND:
			return offset + 3;
		case WHERE_SIGN::SIGN_OR:
			return offset + 2;
		default:
			return -1;
	}
}

bool WhereTrigger::IsNullString(const string& s)
{
	if(s.size() == 0)
		return true;
	int i = 0;
	while(i < s.size())
		if(s[i++] != ' ')
			return false;
	return true;
}

void WhereTrigger::LinkAndOrSign(WHERE_SIGN sign, Where_LinkNode& left, Where_LinkNode& right)
{
	if(sign == WHERE_SIGN::SIGN_AND)
	{
		vector<Where_Node**>::iterator iter = left.m_True_Nodes.begin();
		while(iter != left.m_True_Nodes.end())
		{
			**iter = right.m_Enter_Node;
			++iter;
		}
		left.m_True_Nodes = right.m_True_Nodes;
		iter = right.m_False_Nodes.begin();
		while(iter != right.m_False_Nodes.end())
		{
			left.m_False_Nodes.push_back(*iter);
			++iter;
		}
	}
	if(sign == WHERE_SIGN::SIGN_OR)
	{
		vector<Where_Node**>::iterator iter = left.m_False_Nodes.begin();
		while(iter != left.m_False_Nodes.end())
		{
			**iter = right.m_Enter_Node;
			++iter;
		}
		left.m_False_Nodes = right.m_False_Nodes;
		iter = right.m_True_Nodes.begin();
		while(iter != right.m_True_Nodes.end())
		{
			left.m_True_Nodes.push_back(*iter);
			++iter;
		}
	}
}

WHERE_OPERATOR WhereTrigger::FindOperator(const string& str, int& offset, int& end)
{
	if(offset < 0)
		return WHERE_OPERATOR::OP_ERROR;
	bool Left_Flag = false, Right_Flag = false, Not_Flag = false;
	int temp_left_right = 0, temp_right = 0, temp_not = 0;
	while(offset < str.size())
	{
		switch(str[offset])
		{
			case '=':
				end = offset + 1;
				return WHERE_OPERATOR::OP_EQUAL;
			case '!':
				if(offset + 1 < str.size() && str[offset + 1] == '='){
					end = offset + 2;
					return WHERE_OPERATOR::OP_NOT_EQUAL;
				}
				else{
					ostringstream os;
					os << "Something Wrong Near " << "\'" << str << "\'";
					Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
					return WHERE_OPERATOR::OP_ERROR;
				}
			case '>':
				if(offset + 1 < str.size() && str[offset + 1] == '='){
					end = offset + 2;
					return WHERE_OPERATOR::OP_GREATER_OR_EQUAL;
				}
				else{
					end = offset + 1;
					return WHERE_OPERATOR::OP_GREATER;
				}
			case '<':
				if(offset + 1 < str.size() && str[offset + 1] == '='){
					end = offset + 2;
					return WHERE_OPERATOR::OP_LESS_OR_EQUAL;
				}
				else{
					end = offset + 1; 
					return WHERE_OPERATOR::OP_LESS;
				}
			case 'i':
				if(offset > 1 && offset + 4 < str.size() &&
					str[offset - 1] == ' ' && str[offset + 1] == 'n' && (str[offset + 2] == ' ' || str[offset + 2] == '('))
				{
					if(str[offset + 2] == '(')
					{
						temp_right = offset + 4;
						while(temp_right < str.size())
						{
							if(str[temp_right] == ')')
							{
								Right_Flag = true;
								end = temp_right + 1;
								break;
							}
							else
								++temp_right;							
						}
						if(Right_Flag == false){
							ostringstream os;
							os << "Something Wrong Near " << "\'" << str << "\'";
							Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
							return WHERE_OPERATOR::OP_ERROR;
						}
					}
					else
					{
						temp_left_right = offset + 3;
						while(temp_left_right < str.size())
						{
							if(str[temp_left_right] == ' ')
								++temp_left_right;
							else if(str[temp_left_right] == '('){
								Left_Flag = true;
								break;
							}else{
								Left_Flag = false;
								break;
							}
						}
						if(Left_Flag == false){
							ostringstream os;
							os << "Something Wrong Near " << "\'" << str << "\'";
							Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
							return WHERE_OPERATOR::OP_ERROR;
						}
						while(temp_left_right < str.size())
						{
							if(str[temp_left_right] == ')')
							{
								Right_Flag = true;
								end = temp_left_right + 1;
								break;
							}
							++temp_left_right;
						}
						if(Right_Flag == false){
							ostringstream os;
							os << "Something Wrong Near " << "\'" << str << "\'";
							Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
							return WHERE_OPERATOR::OP_ERROR;
						}
					}
					Not_Flag = false;
					temp_not = offset - 2;
					while(temp_not > 3)
					{
						if(str[temp_not] == 't' && str[temp_not - 1] == 'o' && str[temp_not - 2] == 'n' && str[temp_not - 3] == ' '){
							Not_Flag = true;
							offset = temp_not - 2;
							break;
						}
						else
							--temp_not;
					}
					if(Not_Flag == true)
						return WHERE_OPERATOR::OP_NOT_IN;
					else
						return WHERE_OPERATOR::OP_IN;
				}
		}
		++offset;
	}
	return WHERE_OPERATOR::OP_NONE;
}

bool WhereTrigger::RemoveSpaceChar(string& str)
{
	int offset = 0, begin = 0, end = 0;
	for(vector<string>::iterator iter = UnAllowedWords.begin(); iter != UnAllowedWords.end(); ++iter)
	{
		if(str.find(*iter) != string::npos){
			ostringstream os;
			os << "Something Wrong Near " << "\'" << str << "\'";
			Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
			return false;
		}
	}
	while(offset < str.size() && str[offset] == ' ')
		++offset;
	if(offset == str.size())
		return false;
	begin = offset;
	while(offset < str.size() && str[offset] != ' ')
		++offset;
	end = offset;
	while(offset < str.size() && str[offset++] == ' ');
	if(offset != str.size()){
		ostringstream os;
		os << "Something Wrong Near " << "\'" << str << "\'";
		Log4cxxLogger::GetObj().Error("WhereTrigger", os.str());
		return false;
	}
	str = str.substr(begin, end - begin);
	return true;
}

bool WhereTrigger::Init(string& str, DataFormat* data_format){
	if(!AnalyticWhereCondition(str, data_format))
		return false;
	return true;
}

/*void WhereTrigger::PrintNode(Where_Node* ptr)
{
    StatValue sv;
    if(ptr != NULL)
    {
		sv = *(ptr->m_OptTable.m_DstValues.begin());
        cout << sv.u64 << endl;
        cout << sv.u64 << " if true" << endl;
        PrintNode(ptr->m_True_Node);
        cout << sv.u64 << " if false" << endl;
        PrintNode(ptr->m_False_Node);
    }
    else{
        cout << "end" << endl;
    }
}*/



template<typename T>
bool WhereTrigger::IsOutofRange(T num, int type){
	switch(type)
	{
		case SVT_UINT8:
			if(num > max_uint8 || num < 0)
				return true;
			return false;
		case SVT_UINT16:
			if(num > max_uint16 || num < 0)
				return true;
			return false;
		case SVT_UINT32:
			if(num > max_uint32 || num < 0)
				return true;
			return false;
		case SVT_UINT64:
			if(num > max_uint64 || num < 0)
				return true;
			return false;
		case SVT_INT8:
			if(num > max_int8 || num < min_int8)
				return true;
			return false;
		case SVT_INT16:
			if(num > max_int16 || num < min_int16)
				return true;
			return false;
		case SVT_INT32:
			if(num > max_int32 || num < min_int32)
				return true;
			return false;
		case SVT_INT64:
			if(num > max_int64 || num < min_int64)
				return true;
			return false;
		default:
			return true;
	}
} 

bool Where_OptTable::isTrue(const Data* data)
{
	StatValue* pSv = &(*data)[m_SrcIndex];
	if(!pSv->exist)
		return false;
	if(pSv->type != m_SrcType){
		Log4cxxLogger::GetObj().Error("WhereTrigger", "StatValue Type is different from Received!");
		return false;
	}
	
	if(m_Opt == WHERE_OPERATOR::OP_IN || m_Opt == WHERE_OPERATOR::OP_NOT_IN)
	{
		if(m_Opt == WHERE_OPERATOR::OP_IN)
		{
			if(m_DstValues.find(*pSv) == m_DstValues.end())
				return false;
			else
				return true;
		}
		else{
			if(m_DstValues.find(*pSv) == m_DstValues.end())
				return true;
			else
				return false;
		}	
	}else{
		StatValue sv = *m_DstValues.begin();
		switch(m_SrcType)
		{
			case SVT_UINT8:
				return CheckOptNum<guint8>(pSv->u8, sv.u8);
			case SVT_UINT16:
				return CheckOptNum<guint16>(pSv->u16, sv.u16);
			case SVT_UINT32:
				return CheckOptNum<guint32>(pSv->u32, sv.u32);
			case SVT_UINT64:
				return CheckOptNum<guint64>(pSv->u64, sv.u64);
			case SVT_STRING:
				return CheckOptStr(pSv->pchar, sv.pchar);
			case SVT_INT8:
				return CheckOptNum<gint8>(pSv->i8, sv.i8);
			case SVT_INT16:
				return CheckOptNum<gint16>(pSv->i16, sv.i16);
			case SVT_INT32:
				return CheckOptNum<gint32>(pSv->i32, sv.i32);
			case SVT_INT64:
				return CheckOptNum<gint64>(pSv->i64, sv.i64);
			case SVT_FLOAT:
				return CheckOptNum<float>(pSv->flo, sv.flo);
			default:
				return false;
		}
	}
}

bool Where_Node::isTrue(const Data* data)
{
	return m_OptTable.isTrue(data);
}

template<typename T>
bool Where_OptTable::CheckOptNum(T src, T dst)
{
	switch(m_Opt)
	{
		case WHERE_OPERATOR::OP_EQUAL:
			return src == dst;
		case WHERE_OPERATOR::OP_NOT_EQUAL:
			return src != dst;
		case WHERE_OPERATOR::OP_GREATER:
			return src > dst;
		case WHERE_OPERATOR::OP_GREATER_OR_EQUAL:
			return src >= dst;
		case WHERE_OPERATOR::OP_LESS:
			return src < dst;
		case WHERE_OPERATOR::OP_LESS_OR_EQUAL:
			return src <= dst;
		default:
			return false;
	}
}

bool Where_OptTable::CheckOptStr(const char* src, const char* dst)
{
	switch(m_Opt)
	{
		case WHERE_OPERATOR::OP_EQUAL:
			if(strcmp(src, dst) == 0)
				return true;
			else
				return false;
		case WHERE_OPERATOR::OP_NOT_EQUAL:
			if(strcmp(src, dst) == 0)
				return false;
			else
				return true;
		case WHERE_OPERATOR::OP_GREATER:
			if(strcmp(src, dst) > 0)
				return true;
			else
				return false;
		case WHERE_OPERATOR::OP_GREATER_OR_EQUAL:
			if(strcmp(src, dst) >= 0)
				return true;
			else
				return false;
		case WHERE_OPERATOR::OP_LESS:
			if(strcmp(src, dst) < 0)
				return true;
			else
				return false;
		case WHERE_OPERATOR::OP_LESS_OR_EQUAL:
			if(strcmp(src, dst) <= 0)
				return true;
			else
				return false;
		default:
			return false;
	}
}






