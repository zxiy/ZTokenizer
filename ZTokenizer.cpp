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
		* 从reader中读取数据，填充buffer
		* 如果reader是分次读入buffer的，那么buffer要进行移位处理
		* 移位处理上次读入的但未处理的数据
		*/
		int available=context->fillBuffer(reader);
		if(available<=0)
		{
			context->reset();
			return L"";
		}
		else
		{
			//初始化指针
			context->initCursor();
			do{
				//遍历子分词器
				for(ISegmenter *segmenter:segmenters)
				{
					segmenter->analyze(context);
				}
				//字符缓冲区接近读完，需要读入新的字符
				if(context->needRefillBuffer())
				{
					break;
				}
				//向前移动指针
			}
			while(context->moveCursor());
			//重置子分词器，为下轮循环进行初始化
			for(ISegmenter *segmenter:segmenters)
			{
				segmenter->reset();
			}
		}
		//对分词进行歧义处理
		this->arbitrator->process(context,useSmart);
		//处理未切分CJK字符
		context->outputToResult();
		//记录本次分词的缓冲区位移
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
