#ifndef __CN_QUANTIFIERSEGMENTER_H__
#define __CN_QUANTIFIERSEGMENTER_H__
#include "isegmenter.h"
const wstring Chn_Num =L"һ�������������߰˾�ʮ��Ҽ��������½��ƾ�ʰ��ǧ����ʰ��Ǫ�f�|��ئإ";
const string SEGMENTER_NAME_QS = "QUAN_SEGMENTER";
//���������ʷִ���
class CN_QuantifierSegmenter:public ISegmenter
{
private:
	set<wchar_t>ChnNumberChars;//��������
	int nStart;//��Ԫ�Ŀ�ʼλ�ã�ͬʱ��Ϊ�ӷִ���״̬��ʶ ��start > -1 ʱ����ʶ��ǰ�ķִ������ڴ����ַ�
	int nEnd;//�����������hit����
	list<shared_ptr<Hit> >countHits;
	void outputNumLexeme(shared_ptr<AnalyzeContext> context);//������ʴ�Ԫ�������
	void processCNumber(shared_ptr<AnalyzeContext> context);//������������
	bool needCountScan(shared_ptr<AnalyzeContext> context);//�ж��Ƿ���Ҫɨ������
	void processCount(shared_ptr<AnalyzeContext> context);//������������

public:
	CN_QuantifierSegmenter();
	void analyze(shared_ptr<AnalyzeContext> context);
	void reset();
};
#endif
