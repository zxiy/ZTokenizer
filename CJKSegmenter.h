#ifndef __CJKSEGMENTER_H__
#define __CJKSEGMENTER_H__
#include "isegmenter.h"

const string SEGMENTER_NAME_CJK="CJK_SEGMENTER";
//���ķִ���
class CJKSegmenter:public ISegmenter
{
private:
	list<shared_ptr<Hit> >tmpHits;//������ķִ�hit����
public:
	CJKSegmenter();
	void analyze(shared_ptr<AnalyzeContext>context);
	void reset();
};

#endif