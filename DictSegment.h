#ifndef __DICTSEGMENT_H_
#define __DICTSEGMENT_H_


#include <map>
#include <vector>
#include <algorithm>
#include "Hit.h"
using namespace std;
//�ʵ��Trie���ڵ�
class DictSegment
{
private:
	static map<wchar_t,wchar_t> charMap;/*���溺�ֵĹ����ֵ��*/
	static const int ArrayLengthLimit=3;/*�����ֵ���Ľṹ�����ƣ��趨���������ڳ���ĳһ�����ƺ�ʹ��map������ʹ������*/
	map<wchar_t,shared_ptr<DictSegment> > childMap;/** hashmap�Ĵ���ṹ*/
	vector<shared_ptr<DictSegment> > childArray;/** ����Ĵ���ṹ*/
	wchar_t NodeChar;	/*��ǰ�ڵ㴢�����*/
	int storeSize;/*��ǰ�ڵ���ӽڵ㣨���������������������ڳ�������(DictSegment.ArrayLengthLimit)��ʹ��map������ʹ������*/
	int NodeState;/*��ǰ�ڵ㣨DictSegment���Ƿ�ɴʵ�״̬��1��ʾ�Ӹ��ڵ㵽����ڵ��·����ʾһ���ʣ�0���෴*/	
	void fillSegment(const wchar_t *charArray,int begin,int length,int disableFlag);/*�������δʵ�ľ���ʵ��*/
	shared_ptr<DictSegment> findSegment(wchar_t keyChar,int create);
public:
public:
	DictSegment();/*����һ���յĽڵ�*/
	DictSegment(wchar_t NodeChar);/*���캯��*���뵱ǰ�ڵ���ַ�*/
	wchar_t getNodeChar();/*��øýڵ���ַ�*/
	bool hasChildNode();/*�ж��Ƿ����ӽڵ�*/
	shared_ptr<Hit> match(const wchar_t *charArray,int begin,int length,shared_ptr<Hit> HitResult);/* �Դʶν���ƥ��,�������н����*/
	shared_ptr<Hit> match(const wchar_t *charArray,int begin,int length);
	shared_ptr<Hit> match(const wchar_t *charArray);
	void fillSegment(const wchar_t *charArray);//�������ʵ�Ƭ��
	void disableSegment(const wchar_t *charArray);//���δʵ��е�ĳһ����
	bool operator < (const DictSegment &other) const;/*Ϊ�ֵ�����������С�ں�*/
};

#endif
