#include "CN_QuantifierSegmenter.h"


void CN_QuantifierSegmenter::outputNumLexeme(shared_ptr<AnalyzeContext> context )
{
	if(nStart > -1 && nEnd > -1)
	{
		//输出数词
		shared_ptr<Lexeme>newLexeme (new Lexeme(context->getBufferOffset() , nStart , nEnd - nStart + 1,TYPE_CNUM));
		context->addLexeme(newLexeme);
	}
}

void CN_QuantifierSegmenter::processCNumber(shared_ptr<AnalyzeContext> context )
{
	if(nStart == -1 && nEnd == -1)
	{//初始状态
		if(CHAR_CHINESE == context->getCurrentCharType() 
			&& ChnNumberChars.count(context->getCurrentChar()))
		{
			//记录数词的起始、结束位置
			nStart = context->getCursor();
			nEnd = context->getCursor();
		}
	}
	else
	{//正在处理状态
		if(CHAR_CHINESE == context->getCurrentCharType() 
			&& ChnNumberChars.count(context->getCurrentChar()))
		{
			//记录数词的结束位置
			nEnd = context->getCursor();
		}
		else
		{
			//输出数词
			this->outputNumLexeme(context);
			//重置头尾指针
			nStart = -1;
			nEnd = -1;
		}
	}

	//缓冲区已经用完，还有尚未输出的数词
	if(context->isBufferConsumed())
	{
		if(nStart != -1 && nEnd != -1)
		{
			//输出数词
			outputNumLexeme(context);
			//重置头尾指针
			nStart = -1;
			nEnd = -1;
		}
	}
}

bool CN_QuantifierSegmenter::needCountScan( shared_ptr<AnalyzeContext> context )
{
	if((nStart!=-1&&nEnd!=-1)||!countHits.empty())
	{
		//正在处理中文数词,或者正在处理量词
		return true;
	}
	else
	{
		//找到一个相邻的数词
		if(!context->getOrgLexemes()->isEmpty())
		{
			shared_ptr<Lexeme> l = context->getOrgLexemes()->peekLast();
			if(TYPE_CNUM == l->getLexemeType()||TYPE_ARABIC == l->getLexemeType())
			{
				if(l->getBegin() + l->getLength() == context->getCursor())
				{
					return true;
				}
			}
		}
	}
	return false;
}

void CN_QuantifierSegmenter::processCount(shared_ptr<AnalyzeContext> context )
{
	// 判断是否需要启动量词扫描
	if(!this->needCountScan(context))
	{
		return;
	}
	if(CHAR_CHINESE == context->getCurrentCharType())
	{
		//优先处理countHits中的hit
		if(!this->countHits.empty())
		{
			//处理词段队列
			vector<shared_ptr<Hit> >tmpArray(this->countHits.begin(),this->countHits.end());
			for(shared_ptr<Hit>hit:tmpArray)
			{
				hit = Dictionary::getSingleton()->matchWithHit(context->getSegmentBuff(),context->getCursor(),hit);
				if(hit->isMatch())
				{
					//输出当前的词
					shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),hit->getBegin(),context->getCursor() - hit->getBegin() + 1 ,TYPE_COUNT));
					context->addLexeme(newLexeme);
					if(!hit->isPrefix())
					{//不是词前缀，hit不需要继续匹配，移除
						this->countHits.remove(hit);
					}
				}
				else if(hit->isUnmatch())
				{
					//hit不是词，移除
					this->countHits.remove(hit);
				}					
			}
		}				
		//*********************************
		//对当前指针位置的字符进行单字匹配
		shared_ptr<Hit> singleCharHit = Dictionary::getSingleton()->matchInQuantifierDict(context->getSegmentBuff(), context->getCursor(),1);
		if(singleCharHit->isMatch())
		{//首字成量词词
			//输出当前的词
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),context->getCursor(),1,TYPE_COUNT));
			context->addLexeme(newLexeme);
			//同时也是词前缀
			if(singleCharHit->isPrefix())
			{
				//前缀匹配则放入hit列表
				this->countHits.push_back(singleCharHit);
			}
		}
		else if(singleCharHit->isPrefix())
		{//首字为量词前缀
			//前缀匹配则放入hit列表
			this->countHits.push_back(singleCharHit);
		}
	}
	else
	{
		//输入的不是中文字符
		//清空未成形的量词
		this->countHits.clear();
	}
	//缓冲区数据已经读完，还有尚未输出的量词
	if(context->isBufferConsumed())
	{
		//清空未成形的量词
		this->countHits.clear();
	}
}

CN_QuantifierSegmenter::CN_QuantifierSegmenter()
{
	nStart=-1;
	nEnd=-1;
}

void CN_QuantifierSegmenter::analyze( shared_ptr<AnalyzeContext> context )
{
	//处理中文数词
	this->processCNumber(context);
	//处理中文量词
	this->processCount(context);
	//判断是否锁定缓冲区
	if(this->nStart == -1 && this->nEnd == -1&& countHits.empty())
	{
		//对缓冲区解锁
		context->unlockBuffer(SEGMENTER_NAME_QS);
	}
	else
	{
		context->lockBuffer(SEGMENTER_NAME_QS);
	}
}

void CN_QuantifierSegmenter::reset()
{
	nStart = -1;
	nEnd = -1;
	countHits.clear();
}
