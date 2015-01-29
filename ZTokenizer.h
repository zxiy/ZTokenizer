#ifndef __ITOKENIZER_H__
#define __ITOKENIZER_H__
#include "AnalyzeContext.h"
#include "Arbitrator.h"
#include "CJKSegmenter.h"
#include "LetterSegmenter.h"
#include "CN_QuantifierSegmenter.h"
//Ö÷·Ö´ÊÆ÷
class ZTokenizer
{
private:
	shared_ptr<AnalyzeContext> context;
	shared_ptr<Arbitrator> arbitrator;
	vector<ISegmenter*>segmenters;
	wstringstream reader;
	bool useSmart;
	void init();
	void destory();
public:
	ZTokenizer(wstring wstr,bool useSmart);
	wstring next();
	void reset(wstring wstr,bool useSmart);
	~ZTokenizer();
};


#endif