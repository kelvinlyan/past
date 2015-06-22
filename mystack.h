#ifndef _MYSTACK_H
#define _MYSTACK_H
#include <vector>
using namespace std;

template<typename T>
class mystack{
	public:
		mystack(){}
		mystack(const mystack& rhs):m_vElements(rhs.m_vElements){}
		mystack& operator =(const mystack& rhs){
			m_vElements = rhs.m_vElements;
			return *this;
		}
		~mystack(){}
		void push(T ele){
			m_vElements.push_back(ele);
		}
		void pop(){
			m_vElements.pop_back();
		}
		T top(){
			return m_vElements.back();
		}
		int size(){
			return m_vElements.size();
		}
		bool empty(){
			return m_vElements.empty();
		}
	private:
		vector<T> m_vElements;
};






#endif
