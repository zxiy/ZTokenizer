
#ifndef __ISEGMENTER_H__
#define __ISEGMENTER_H__
#include "AnalyzeContext.h"
//�ӷִ�������
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