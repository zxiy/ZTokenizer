#ifndef __CJKSEGMENTER_H__
#define __CJKSEGMENTER_H__
#include "isegmenter.h"

const string SEGMENTER_NAME_CJK="CJK_SEGMENTER";
//中文分词器
class CJKSegmenter:public ISegmenter
{
private:
	list<shared_ptr<Hit> >tmpHits;//待处理的分词hit队列
public:
	CJKSegmenter();
	void analyze(shared_ptr<AnalyzeContext>context);
	void reset();
};

#endif