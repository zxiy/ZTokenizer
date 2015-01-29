
#ifndef __ISEGMENTER_H__
#define __ISEGMENTER_H__
#include "AnalyzeContext.h"
//子分词器基类
class ISegmenter
{
public:
	virtual ~ISegmenter()
	{

	}
	virtual void analyze(shared_ptr<AnalyzeContext>context)
	{

	}
	virtual void reset()
	{

	}
};

#endif