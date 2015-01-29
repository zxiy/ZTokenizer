#pragma warning(disable:4267)
#include "LetterSegmenter.h"



LetterSegmenter::LetterSegmenter()
{
	this->start = -1;
	this->end = -1;
	this->englishStart = -1;
	this->englishEnd = -1;
	this->arabicStart = -1;
	this->arabicEnd = -1;
}


bool LetterSegmenter::isLetterConnector( wchar_t input )
{
	int index = Letter_Connector.find(input);
	return index >= 0;
}

bool LetterSegmenter::isNumConnector( wchar_t input )
{
	int index=Num_Connector.find(input);
	return index >= 0;
}

bool LetterSegmenter::processMixLetter(shared_ptr<AnalyzeContext>context)
{
	bool needLock = false;

	if(this->start==-1)//当前的分词器尚未开始处理字符
	{
		if(CHAR_ARABIC==context->getCurrentCharType()
			||CHAR_ENGLISH==context->getCurrentCharType())
		{
			//记录起始指针的位置,标明分词器进入处理状态
			this->start=context->getCursor();
			this->end=start;
		}
	}
	else//当前的分词器正在处理字符
	{			
		if(CHAR_ARABIC == context->getCurrentCharType()
			||CHAR_ENGLISH == context->getCurrentCharType())
		{
			//记录下可能的结束位置
			this->end = context->getCursor();
		}
		else if(CHAR_UNKNOWN==context->getCurrentCharType()
			&&this->isLetterConnector(context->getCurrentChar()))
		{
			//记录下可能的结束位置
			this->end = context->getCursor();
		}
		else
		{
			//遇到非Letter字符，输出词元
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),this->start,this->end-this->start+1,TYPE_LETTER));
			context->addLexeme(newLexeme);
			this->start=-1;
			this->end=-1;
		}			
	}

	//判断缓冲区是否已经读完
	if(context->isBufferConsumed())
	{
		if(this->start != -1 && this->end != -1)
		{
			//缓冲以读完，输出词元
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),this->start,this->end-this->start+1,TYPE_LETTER));
			context->addLexeme(newLexeme);
			this->start = -1;
			this->end = -1;
		}
	}

	//判断是否锁定缓冲区
	if(this->start == -1 && this->end == -1)
	{
		//对缓冲区解锁
		needLock = false;
	}
	else
	{
		needLock = true;
	}
	return needLock;
}

bool LetterSegmenter::processEnglishLetter(shared_ptr<AnalyzeContext> context)
{
	bool needLock = false;

	if(this->englishStart == -1)//当前的分词器尚未开始处理英文字符
	{
		if(CHAR_ENGLISH == context->getCurrentCharType())
		{
			//记录起始指针的位置,标明分词器进入处理状态
			this->englishStart = context->getCursor();
			this->englishEnd = this->englishStart;
		}
	}
	else 
	{//当前的分词器正在处理英文字符	
		if(CHAR_ENGLISH == context->getCurrentCharType())
		{
			//记录当前指针位置为结束位置
			this->englishEnd =  context->getCursor();
		}
		else
		{
			//遇到非English字符,输出词元
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),this->englishStart,this->englishEnd-this->englishStart+1 ,TYPE_ENGLISH));
			context->addLexeme(newLexeme);
			this->englishStart = -1;
			this->englishEnd= -1;
		}
	}

	//判断缓冲区是否已经读完
	if(context->isBufferConsumed())
	{
		if(this->englishStart != -1 && this->englishEnd != -1){
			//缓冲以读完，输出词元
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),this->englishStart,this->englishEnd-this->englishStart + 1,TYPE_ENGLISH));
			context->addLexeme(newLexeme);
			this->englishStart = -1;
			this->englishEnd= -1;
		}
	}	

	//判断是否锁定缓冲区
	if(this->englishStart == -1 && this->englishEnd == -1)
	{
		//对缓冲区解锁
		needLock = false;
	}
	else
	{
		needLock = true;
	}
	return needLock;
}

bool LetterSegmenter::processArabicLetter(shared_ptr<AnalyzeContext> context)
{
	bool needLock = false;
	if(this->arabicStart == -1)
	{//当前的分词器尚未开始处理数字字符	
		if(CHAR_ARABIC == context->getCurrentCharType())
		{
			//记录起始指针的位置,标明分词器进入处理状态
			this->arabicStart = context->getCursor();
			this->arabicEnd = this->arabicStart;
		}
	}
	else
	{//当前的分词器正在处理数字字符	
		if(CHAR_ARABIC == context->getCurrentCharType())
		{
			//记录当前指针位置为结束位置
			this->arabicEnd = context->getCursor();
		}
		else if(CHAR_UNKNOWN == context->getCurrentCharType()
			&& this->isNumConnector(context->getCurrentChar()))
		{
			//不输出数字，但不标记结束
		}
		else
		{
			////遇到非Arabic字符,输出词元
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset() ,this->arabicStart,this->arabicEnd-this->arabicStart + 1,TYPE_ARABIC));
			context->addLexeme(newLexeme);
			this->arabicStart = -1;
			this->arabicEnd = -1;
		}
	}
	//判断缓冲区是否已经读完
	if(context->isBufferConsumed())
	{
		if(this->arabicStart != -1 && this->arabicEnd != -1)
		{
			//生成已切分的词元
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),this->arabicStart,this->arabicEnd - this->arabicStart + 1,TYPE_ARABIC));
			context->addLexeme(newLexeme);
			this->arabicStart = -1;
			this->arabicEnd = -1;
		}
	}

	//判断是否锁定缓冲区
	if(this->arabicStart == -1 && this->arabicEnd == -1)
	{
		//对缓冲区解锁
		needLock = false;
	}
	else
	{
		needLock = true;
	}
	return needLock;
}

void LetterSegmenter::analyze(shared_ptr<AnalyzeContext> context)
{
	bool bufferLockFlag = false;
	//处理英文字母
	bufferLockFlag = this->processEnglishLetter(context) || bufferLockFlag;
	//处理阿拉伯字母
	bufferLockFlag = this->processArabicLetter(context) || bufferLockFlag;
	//处理混合字母
	bufferLockFlag = this->processMixLetter(context) || bufferLockFlag;

	//判断是否锁定缓冲区
	if(bufferLockFlag)
	{
		context->lockBuffer(SEGMENTER_NAME_LS);
	}
	else
	{
		//对缓冲区解锁
		context->unlockBuffer(SEGMENTER_NAME_LS);
	}
}

void LetterSegmenter::reset()
{
	this->start = -1;
	this->end = -1;
	this->englishStart = -1;
	this->englishEnd = -1;
	this->arabicStart = -1;
	this->arabicEnd = -1;
}
