#ifndef __CN_QUANTIFIERSEGMENTER_H__
#define __CN_QUANTIFIERSEGMENTER_H__
#include "isegmenter.h"
const wstring Chn_Num =L"一二两三四五六七八九十零壹贰叁肆伍陆柒捌玖拾百千万亿拾佰仟萬億兆卅廿";
const string SEGMENTER_NAME_QS = "QUAN_SEGMENTER";
//中文数量词分词器
class CN_QuantifierSegmenter:public ISegmenter
{
private:
	set<wchar_t>ChnNumberChars;//中文数词
	int nStart;//词元的开始位置，同时作为子分词器状态标识 当start > -1 时，标识当前的分词器正在处理字符
	int nEnd;//待处理的量词hit队列
	list<shared_ptr<Hit> >countHits;
	void outputNumLexeme(shared_ptr<AnalyzeContext> context);//添加数词词元到结果集
	void processCNumber(shared_ptr<AnalyzeContext> context);//处理中文数词
	bool needCountScan(shared_ptr<AnalyzeContext> context);//判断是否需要扫描量词
	void processCount(shared_ptr<AnalyzeContext> context);//处理中文量词

public:
	CN_QuantifierSegmenter();
	void analyze(shared_ptr<AnalyzeContext> context);
	void reset();
};
#endif
