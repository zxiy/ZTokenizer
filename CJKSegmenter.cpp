#include "CJKSegmenter.h"


void CJKSegmenter::analyze(shared_ptr<AnalyzeContext>context )
{
	if(CHAR_UNKNOWN!= context->getCurrentCharType())
	{
		//优先处理tmpHits中的hit
		if(!this->tmpHits.empty())
		{
			//处理词段队列
			vector<shared_ptr<Hit> >tmpArray(this->tmpHits.begin(),this->tmpHits.end());
			for(shared_ptr<Hit> hit : tmpArray)
			{
				hit = Dictionary::getSingleton()->matchWithHit(context->getSegmentBuff(),context->getCursor(),hit);
				if(hit->isMatch())
				{
					//输出当前的词
					shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),hit->getBegin(),context->getCursor()-hit->getBegin()+1,TYPE_CNWORD));
					context->addLexeme(newLexeme);
					if(!hit->isPrefix())
					{//不是词前缀，hit不需要继续匹配，移除
						this->tmpHits.remove(hit);
					}

				}
				else if(hit->isUnmatch())
				{
					//hit不是词，移除
					this->tmpHits.remove(hit);
				}
			}
		}			
		//*********************************
		//再对当前指针位置的字符进行单字匹配
		shared_ptr<Hit>singleCharHit = Dictionary::getSingleton()->matchInMainDict(context->getSegmentBuff(),context->getCursor(), 1);
		if(singleCharHit->isMatch())
		{//首字成词
			//输出当前的词
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),context->getCursor(),1,TYPE_CNWORD));
			context->addLexeme(newLexeme);
			//同时也是词前缀
			if(singleCharHit->isPrefix())
			{
				//前缀匹配则放入hit列表
				this->tmpHits.push_back(singleCharHit);
			}
			else
			{
				//不是前缀，删除释放空间
				singleCharHit.reset();
			}
		}
		else if(singleCharHit->isPrefix())
		{//首字为词前缀
			//前缀匹配则放入hit列表
			this->tmpHits.push_back(singleCharHit);
		}
		else
		{
			//不匹配，删除释放空间
			singleCharHit.reset();
		}
	}
	else
	{
		//遇到CHAR_USELESS字符
		//清空队列
		//for_each(tmpHits.begin(),tmpHits.end(),[](Hit* a){delete a;});
		this->tmpHits.clear();
	}
	//判断缓冲区是否已经读完
	if(context->isBufferConsumed())
	{
		//清空队列
		//for_each(tmpHits.begin(),tmpHits.end(),[](Hit* a){delete a;});
		this->tmpHits.clear();
	}

	//判断是否锁定缓冲区
	if(this->tmpHits.size() == 0)
	{
		context->unlockBuffer(SEGMENTER_NAME_CJK);
	}
	else
	{
		context->lockBuffer(SEGMENTER_NAME_CJK);
	}
}

CJKSegmenter::CJKSegmenter()
{
	this->tmpHits.clear();
}

void CJKSegmenter::reset()
{
	//清空队列
	this->tmpHits.clear();
}
