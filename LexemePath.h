#ifndef __LEXEMEPATH_H_
#define __LEXEMEPATH_H_
#include "quicksortset.h"
//各种分词结果集
class LexemePath:public QuickSortSet
{
private:
	int pathBegin;//起始位置
	int pathEnd;//结束位置
	int payloadLength;//词元链的有效字符长度
public:
	LexemePath();
	bool addCrossLexeme(shared_ptr<Lexeme> lexeme);//向LexemePath追加相交的Lexeme
	bool addNotCrossLexeme(shared_ptr<Lexeme>lexeme);//向LexemePath追加不相交的Lexeme
	bool checkCross(shared_ptr<Lexeme>lexeme);//检测词元位置交叉（有歧义的切分）
	shared_ptr<Lexeme> removeTail();//移除尾部的Lexeme
	int getPathBegin();
	int getPathEnd();
	int getPayloadLength();//获取该path的有效词长
	int getPathLength();//获取LexemePath的路径长度
	int getXWeight();//用于歧义裁决的权重X（词元长度积）
	int getPWeight();//用于歧义裁决的权重P（位置权重）
	shared_ptr<LexemePath> copy();
	int compareTo(LexemePath &o);//用于歧义裁决，比较两个词元链
};

#endif