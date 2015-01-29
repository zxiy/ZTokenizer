#ifndef __HIT_H_
#define __HIT_H_
#include <memory>
using namespace std;
const int UNMATCH = 0x00;
//Hit��ȫƥ��
const int MATCH = 0x01;
//Hitǰ׺ƥ��
const int PREFIX = 0x10;
class DictSegment;
//��������ƥ����
class Hit
{
private:
	//��HIT��ǰ״̬��Ĭ��δƥ��
	int hitState;
	//��¼�ʵ�ƥ������У���ǰƥ�䵽�Ĵʵ��֧�ڵ�
	shared_ptr<DictSegment> matchedDictSegment; 
	/*
	 * �ʶο�ʼλ��
	 */
	int begin;
	/*
	 * �ʶεĽ���λ��
	 */
	int end;
public:
	Hit();
	/**
	 * �ж��Ƿ���ȫƥ��
	 */
	bool isMatch();
	/**
	 * 
	 */
	void setMatch();
	/**
	 * �ж��Ƿ��Ǵʵ�ǰ׺
	 */
	bool isPrefix();
	/**
	 * 
	 */
	void setPrefix();
	/**
	 * �ж��Ƿ��ǲ�ƥ��
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