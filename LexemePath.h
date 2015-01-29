#ifndef __LEXEMEPATH_H_
#define __LEXEMEPATH_H_
#include "quicksortset.h"
//���ִַʽ����
class LexemePath:public QuickSortSet
{
private:
	int pathBegin;//��ʼλ��
	int pathEnd;//����λ��
	int payloadLength;//��Ԫ������Ч�ַ�����
public:
	LexemePath();
	bool addCrossLexeme(shared_ptr<Lexeme> lexeme);//��LexemePath׷���ཻ��Lexeme
	bool addNotCrossLexeme(shared_ptr<Lexeme>lexeme);//��LexemePath׷�Ӳ��ཻ��Lexeme
	bool checkCross(shared_ptr<Lexeme>lexeme);//����Ԫλ�ý��棨��������з֣�
	shared_ptr<Lexeme> removeTail();//�Ƴ�β����Lexeme
	int getPathBegin();
	int getPathEnd();
	int getPayloadLength();//��ȡ��path����Ч�ʳ�
	int getPathLength();//��ȡLexemePath��·������
	int getXWeight();//��������þ���Ȩ��X����Ԫ���Ȼ���
	int getPWeight();//��������þ���Ȩ��P��λ��Ȩ�أ�
	shared_ptr<LexemePath> copy();
	int compareTo(LexemePath &o);//��������þ����Ƚ�������Ԫ��
};

#endif