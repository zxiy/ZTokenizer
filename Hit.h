#ifndef __HIT_H_
#define __HIT_H_
#include <memory>
using namespace std;
const int UNMATCH = 0x00;
//Hit完全匹配
const int MATCH = 0x01;
//Hit前缀匹配
const int PREFIX = 0x10;
class DictSegment;
//用来描述匹配结果
class Hit
{
private:
	//该HIT当前状态，默认未匹配
	int hitState;
	//记录词典匹配过程中，当前匹配到的词典分支节点
	shared_ptr<DictSegment> matchedDictSegment; 
	/*
	 * 词段开始位置
	 */
	int begin;
	/*
	 * 词段的结束位置
	 */
	int end;
public:
	Hit();
	/**
	 * 判断是否完全匹配
	 */
	bool isMatch();
	/**
	 * 
	 */
	void setMatch();
	/**
	 * 判断是否是词的前缀
	 */
	bool isPrefix();
	/**
	 * 
	 */
	void setPrefix();
	/**
	 * 判断是否是不匹配
	 */
	bool isUnmatch();
	void setUnmatch();
	shared_ptr<DictSegment> getMatchedDictSegment();
	void setMatchedDictSegment(shared_ptr<DictSegment> matchedDictSegment);
	int getBegin();
	void setBegin(int begin);
	int getEnd();
	void setEnd(int end);	
	
};
#endif