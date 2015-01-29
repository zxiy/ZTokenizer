
#ifndef __LETTERSEGMENTER_H_
#define __LETTERSEGMENTER_H_
#include "isegmenter.h"
#include <string>
const wstring Letter_Connector=L"#&+-.@_";
const wstring Num_Connector=L",./";
const string SEGMENTER_NAME_LS = "LETTER_SEGMENTER";
//Ӣ�ġ���ַ������ִ���
class LetterSegmenter:public ISegmenter
{
private:
	int start;//��Ԫ�Ŀ�ʼλ�ã�ͬʱ��Ϊ�ӷִ���״̬��ʶ����start > -1 ʱ����ʶ��ǰ�ķִ������ڴ����ַ�
	int end;//��¼��Ԫ����λ��,end��¼�����ڴ�Ԫ�����һ�����ֵ�Letter����Sign_Connector���ַ���λ��
	int englishStart;//��ĸ��ʼλ��
	int englishEnd;///��ĸ����λ��
	int arabicStart;//��ĸ��ʼλ��
	int arabicEnd;///��ĸ����λ��
	bool isLetterConnector(wchar_t input);//�ж��Ƿ�����ĸ���ӷ���
	bool isNumConnector(wchar_t input);//�ж��Ƿ����������ӷ���
	bool processMixLetter(shared_ptr<AnalyzeContext>context);//����������ĸ����������windows7����zhangxiuyu008@tinysoft.com
	bool processEnglishLetter(shared_ptr<AnalyzeContext>context);//����Ӣ����ĸ���
	bool processArabicLetter(shared_ptr<AnalyzeContext>context);//���������������
public:
	LetterSegmenter();
	void analyze(shared_ptr<AnalyzeContext> context);
	void reset();
};
#endif

