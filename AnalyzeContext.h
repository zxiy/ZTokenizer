
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
//文本分析器 负责总体上处理文本，分析并调用各个其他功能进行处理
class AnalyzeContext
{
private:
	bool useSmart;//是否使用歧义裁决
	vector<wchar_t>segmentBuff;//字符串读取缓冲
	vector<int>charTypes;//字符类型
	int buffOffset;//记录Reader内已分析的字串总长度,在分多段分析词元时,该变量累计当前的segmentBuff相对于reader起始位置的位移
	int cursor;//当前缓冲区位置指针
	int available;//可处理的字符串长度
	set<string>buffLocker;//该集合非空，说明有子分词器在占用segmentBuff
	shared_ptr<QuickSortSet> orgLexemes;//未经歧义处理的原始分词结果集合
	map<int,shared_ptr<LexemePath> >pathMap;//Lexeme索引位置表
	list<shared_ptr<Lexeme> >results;//最终分词结果集
	CharacterUtil charutil;//字符处理器
	void outputSingleCJK(int index);//对CJK字符进行单字输出
	void compound(shared_ptr<Lexeme>result);//组合数量词相关词元
public:
	AnalyzeContext(bool useSmart);
	int getCursor();
	wchar_t* getSegmentBuff();
	wchar_t getCurrentChar();
	int getCurrentCharType();
	int getBufferOffset();
	int fillBuffer(wstringstream &reader);
	void initCursor();//初始化buff指针，处理第一个字符
	bool moveCursor();//指针+1并处理当前字符,成功返回 true； 指针已经到了buff尾部，不能前进，返回false
	void lockBuffer(string segmenterName);//设置当前segmentBuff为锁定状态,加入占用segmentBuff的子分词器名称，表示占用segmentBuff
	void unlockBuffer(string segmenterName);//移除指定的子分词器名，释放对segmentBuff的占用
	bool isBufferLocked();//只要buffLocker中存在segmenterName,则buffer被锁定
	bool isBufferConsumed();//判断当前segmentBuff是否已经用完
	bool needRefillBuffer();//判断segmentBuff是否需要读取新数据
	void markBufferOffset();//累计当前的segmentBuff相对于reader起始位置的位移
	void addLexeme(shared_ptr<Lexeme>lexeme);//向分词结果集添加词元
	void addLexemePath(shared_ptr<LexemePath>path);//添加分词结果路径 路径起始位置 ---> 路径 映射表
	shared_ptr<QuickSortSet> getOrgLexemes();//返回原始分词结果
	void outputToResult();//处理未知类型的CJK字符
	shared_ptr<Lexeme> getNextLexeme();//返回lexeme 同时处理合并
	void reset();//重置上下文
};
#endif