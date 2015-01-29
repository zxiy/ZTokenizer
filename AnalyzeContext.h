
#ifndef __ANALYZECONTEXT_H__
#define __ANALYZECONTEXT_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <memory>
#include <sstream>
#include "LexemePath.h"
#include "Dictionary.h"
#include "CharacterUtil.h"
using namespace std;
//�ı������� ���������ϴ����ı������������ø����������ܽ��д���
class AnalyzeContext
{
private:
	bool useSmart;//�Ƿ�ʹ������þ�
	vector<wchar_t>segmentBuff;//�ַ�����ȡ����
	vector<int>charTypes;//�ַ�����
	int buffOffset;//��¼Reader���ѷ������ִ��ܳ���,�ڷֶ�η�����Ԫʱ,�ñ����ۼƵ�ǰ��segmentBuff�����reader��ʼλ�õ�λ��
	int cursor;//��ǰ������λ��ָ��
	int available;//�ɴ�����ַ�������
	set<string>buffLocker;//�ü��Ϸǿգ�˵�����ӷִ�����ռ��segmentBuff
	shared_ptr<QuickSortSet> orgLexemes;//δ�����崦���ԭʼ�ִʽ������
	map<int,shared_ptr<LexemePath> >pathMap;//Lexeme����λ�ñ�
	list<shared_ptr<Lexeme> >results;//���շִʽ����
	CharacterUtil charutil;//�ַ�������
	void outputSingleCJK(int index);//��CJK�ַ����е������
	void compound(shared_ptr<Lexeme>result);//�����������ش�Ԫ
public:
	AnalyzeContext(bool useSmart);
	int getCursor();
	wchar_t* getSegmentBuff();
	wchar_t getCurrentChar();
	int getCurrentCharType();
	int getBufferOffset();
	int fillBuffer(wstringstream &reader);
	void initCursor();//��ʼ��buffָ�룬�����һ���ַ�
	bool moveCursor();//ָ��+1������ǰ�ַ�,�ɹ����� true�� ָ���Ѿ�����buffβ��������ǰ��������false
	void lockBuffer(string segmenterName);//���õ�ǰsegmentBuffΪ����״̬,����ռ��segmentBuff���ӷִ������ƣ���ʾռ��segmentBuff
	void unlockBuffer(string segmenterName);//�Ƴ�ָ�����ӷִ��������ͷŶ�segmentBuff��ռ��
	bool isBufferLocked();//ֻҪbuffLocker�д���segmenterName,��buffer������
	bool isBufferConsumed();//�жϵ�ǰsegmentBuff�Ƿ��Ѿ�����
	bool needRefillBuffer();//�ж�segmentBuff�Ƿ���Ҫ��ȡ������
	void markBufferOffset();//�ۼƵ�ǰ��segmentBuff�����reader��ʼλ�õ�λ��
	void addLexeme(shared_ptr<Lexeme>lexeme);//��ִʽ������Ӵ�Ԫ
	void addLexemePath(shared_ptr<LexemePath>path);//��ӷִʽ��·�� ·����ʼλ�� ---> ·�� ӳ���
	shared_ptr<QuickSortSet> getOrgLexemes();//����ԭʼ�ִʽ��
	void outputToResult();//����δ֪���͵�CJK�ַ�
	shared_ptr<Lexeme> getNextLexeme();//����lexeme ͬʱ����ϲ�
	void reset();//����������
};
#endif