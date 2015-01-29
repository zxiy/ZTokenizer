#include "ZTokenizer.h"


void ZTokenizer::init()
{
	context.reset(new AnalyzeContext(useSmart));
	arbitrator.reset(new Arbitrator());
	Dictionary::getSingleton();
	segmenters.push_back(new CJKSegmenter());
	segmenters.push_back(new CN_QuantifierSegmenter());
	segmenters.push_back(new LetterSegmenter());
}

void ZTokenizer::destory()
{
	for (ISegmenter* segmenter:segmenters)
	{
		delete segmenter;
	}
	segmenters.clear();
}

ZTokenizer::ZTokenizer(wstring wstr,bool useSmart)
{
	
	reader.str(wstr);
	this->useSmart=useSmart;
	init();
}
 
std::wstring ZTokenizer::next()
{
	shared_ptr<Lexeme> l;
	while ((l=context->getNextLexeme())==NULL)
	{
		/*
		* ��reader�ж�ȡ���ݣ����buffer
		* ���reader�Ƿִζ���buffer�ģ���ôbufferҪ������λ����
		* ��λ�����ϴζ���ĵ�δ���������
		*/
		int available=context->fillBuffer(reader);
		if(available<=0)
		{
			context->reset();
			return L"";
		}
		else
		{
			//��ʼ��ָ��
			context->initCursor();
			do{
				//�����ӷִ���
				for(ISegmenter *segmenter:segmenters)
				{
					segmenter->analyze(context);
				}
				//�ַ��������ӽ����꣬��Ҫ�����µ��ַ�
				if(context->needRefillBuffer())
				{
					break;
				}
				//��ǰ�ƶ�ָ��
			}
			while(context->moveCursor());
			//�����ӷִ�����Ϊ����ѭ�����г�ʼ��
			for(ISegmenter *segmenter:segmenters)
			{
				segmenter->reset();
			}
		}
		//�Էִʽ������崦��
		this->arbitrator->process(context,useSmart);
		//����δ�з�CJK�ַ�
		context->outputToResult();
		//��¼���ηִʵĻ�����λ��
		context->markBufferOffset();		
	}
	wstring resstr=l->getLexemeText();
	return resstr;
}

void ZTokenizer::reset( wstring wstr,bool useSmart )
{
	destory();
	reader.clear();
	reader.str(wstr);
	this->useSmart=useSmart;
	init();
}

ZTokenizer::~ZTokenizer()
{
	destory();
}
