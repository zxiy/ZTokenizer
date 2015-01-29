
#ifndef __LETTERSEGMENTER_H_
#define __LETTERSEGMENTER_H_
#include "isegmenter.h"
#include <string>
const wstring Letter_Connector=L"#&+-.@_";
const wstring Num_Connector=L",./";
const string SEGMENTER_NAME_LS = "LETTER_SEGMENTER";
//英文、网址、邮箱分词器
class LetterSegmenter:public ISegmenter
{
private:
	int start;//词元的开始位置，同时作为子分词器状态标识，当start > -1 时，标识当前的分词器正在处理字符
	int end;//记录词元结束位置,end记录的是在词元中最后一个出现的Letter但非Sign_Connector的字符的位置
	int englishStart;//字母起始位置
	int englishEnd;///字母结束位置
	int arabicStart;//字母起始位置
	int arabicEnd;///字母结束位置
	bool isLetterConnector(wchar_t input);//判断是否是字母连接符号
	bool isNumConnector(wchar_t input);//判断是否是数字连接符号
	bool processMixLetter(shared_ptr<AnalyzeContext>context);//处理数字字母混合输出，如windows7或者zhangxiuyu008@tinysoft.com
	bool processEnglishLetter(shared_ptr<AnalyzeContext>context);//处理纯英文字母输出
	bool processArabicLetter(shared_ptr<AnalyzeContext>context);//处理阿拉伯数字输出
public:
	LetterSegmenter();
	void analyze(shared_ptr<AnalyzeContext> context);
	void reset();
};
#endif

