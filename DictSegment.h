#ifndef __DICTSEGMENT_H_
#define __DICTSEGMENT_H_


#include <map>
#include <vector>
#include <algorithm>
#include "Hit.h"
using namespace std;
//词典的Trie树节点
class DictSegment
{
private:
	static map<wchar_t,wchar_t> charMap;/*储存汉字的公用字典表*/
	static const int ArrayLengthLimit=3;/*储存字典书的结构的限制，设定子树数量在超过某一个限制后，使用map，否则使用数组*/
	map<wchar_t,shared_ptr<DictSegment> > childMap;/** hashmap的储存结构*/
	vector<shared_ptr<DictSegment> > childArray;/** 数组的储存结构*/
	wchar_t NodeChar;	/*当前节点储存的字*/
	int storeSize;/*当前节点的子节点（子树）的数量子树数量在超过限制(DictSegment.ArrayLengthLimit)后，使用map，否则使用数组*/
	int NodeState;/*当前节点（DictSegment）是否成词的状态，1表示从父节点到这个节点的路径表示一个词，0则相反*/	
	void fillSegment(const wchar_t *charArray,int begin,int length,int disableFlag);/*填充或屏蔽词典的具体实现*/
	shared_ptr<DictSegment> findSegment(wchar_t keyChar,int create);
public:
public:
	DictSegment();/*构造一个空的节点*/
	DictSegment(wchar_t NodeChar);/*构造函数*传入当前节点的字符*/
	wchar_t getNodeChar();/*获得该节点的字符*/
	bool hasChildNode();/*判断是否有子节点*/
	shared_ptr<Hit> match(const wchar_t *charArray,int begin,int length,shared_ptr<Hit> HitResult);/* 对词段进行匹配,返回命中结果。*/
	shared_ptr<Hit> match(const wchar_t *charArray,int begin,int length);
	shared_ptr<Hit> match(const wchar_t *charArray);
	void fillSegment(const wchar_t *charArray);//加载填充词典片段
	void disableSegment(const wchar_t *charArray);//屏蔽词典中的某一个词
	bool operator < (const DictSegment &other) const;/*为字典序排序重载小于号*/
};

#endif
