#pragma warning(disable:4244)
#pragma warning(disable:4267)
#include "AnalyzeContext.h"

/*
const int AnalyzeContext::BUFF_SIZE=4096;
const int AnalyzeContext::BUFF_EXHAUST_CRITICAL=100;*/

const int BUFF_SIZE=4096;//缓冲区大小
const int BUFF_EXHAUST_CRITICAL=256;//缓冲区快要耗尽的临界值
int AnalyzeContext::fillBuffer(wstringstream &reader)
{
	size_t readCount=0;
	if(buffOffset==0)//首次读取
	{
		reader.read(&segmentBuff[0],BUFF_SIZE);
		readCount=reader.gcount();
	}

	else if(reader.eof())
	{
		return 0;
	}
	else
	{
		int offset=this->available-this->cursor;
		if(offset>0)
		{
			//最近一次读取的>最近一次处理的，将未处理的字串拷贝到segmentBuff头部
			for(int i=0;i<offset;i++)
			{
				segmentBuff[i]=segmentBuff[i+this->cursor];
			}
			readCount=offset;
		}
		else offset=0;
		reader.read(&segmentBuff[offset],BUFF_SIZE-offset);//继续读取reader，
		readCount+=reader.gcount();
	}
	this->available=readCount;//记录最后一次从Reader中读入的可用字符长度
	this->cursor=0;//重置当前指针
	return readCount;
}

AnalyzeContext::AnalyzeContext(bool useSmart)
{
	this->useSmart=useSmart;
	this->available=0;
	this->buffOffset=0;
	this->cursor=0;
	segmentBuff.resize(BUFF_SIZE+1);
	charTypes.resize(BUFF_SIZE+1);
	orgLexemes.reset(new QuickSortSet());
}

int AnalyzeContext::getCursor()
{
	return this->cursor;
}

wchar_t AnalyzeContext::getCurrentChar()
{
	return this->segmentBuff[this->cursor];
}

wchar_t* AnalyzeContext::getSegmentBuff()
{
	return &segmentBuff[0];
}

int AnalyzeContext::getCurrentCharType()
{
	return this->charTypes[this->cursor];
}

int AnalyzeContext::getBufferOffset()
{
	return this->buffOffset;
}

void AnalyzeContext::initCursor()
{
	this->cursor=0;
	this->segmentBuff[this->cursor]=charutil.regularize(this->segmentBuff[this->cursor]);
	this->charTypes[this->cursor]=charutil.identifyCharType(this->segmentBuff[this->cursor]);
}

bool AnalyzeContext::moveCursor()
{
	if(this->cursor<this->available-1)
	{
		this->cursor++;
		this->segmentBuff[this->cursor]=charutil.regularize(this->segmentBuff[this->cursor]);
		this->charTypes[this->cursor]=charutil.identifyCharType(this->segmentBuff[this->cursor]);
		return true;
	}
	else return false;
}

void AnalyzeContext::lockBuffer( string segmenterName )
{
	buffLocker.insert(segmenterName);
}

void AnalyzeContext::unlockBuffer( string segmenterName )
{
	buffLocker.erase(segmenterName);
}

bool AnalyzeContext::isBufferLocked()
{
	return this->buffLocker.size()>0;
}

bool AnalyzeContext::isBufferConsumed()
{
	return this->cursor==this->available-1;
}
 /*
	* 满足一下条件时，
	* 1.available == BUFF_SIZE 表示buffer满载
	* 2.buffIndex < available - 1 && buffIndex > available - BUFF_EXHAUST_CRITICAL表示当前指针处于临界区内
	* 3.!context.isBufferLocked()表示没有segmenter在占用buffer
	* 要中断当前循环（buffer要进行移位，并再读取数据的操作*/
bool AnalyzeContext::needRefillBuffer()
{
	return this->available == BUFF_SIZE && this->cursor < this->available - 1 && this->cursor  > this->available - BUFF_EXHAUST_CRITICAL && !this->isBufferLocked();
}

void AnalyzeContext::markBufferOffset()
{
	this->buffOffset += this->cursor;
}

void AnalyzeContext::addLexeme(shared_ptr<Lexeme>lexeme )
{
	this->orgLexemes->addLexeme(lexeme);
}

void AnalyzeContext::addLexemePath(shared_ptr<LexemePath>path)
{
	this->pathMap[path->getPathBegin()]=path;
}

shared_ptr<QuickSortSet> AnalyzeContext::getOrgLexemes()
{
	return this->orgLexemes;
}

void AnalyzeContext::outputSingleCJK( int index )
{
	if(CHAR_CHINESE==this->charTypes[index])
	{			
		shared_ptr<Lexeme>singleCharLexeme(new Lexeme(this->buffOffset,index,1,TYPE_CNCHAR));
		this->results.push_back(singleCharLexeme);
	}
	else if(CHAR_OTHERCJK==this->charTypes[index])
	{
		shared_ptr<Lexeme>singleCharLexeme(new Lexeme(this->buffOffset,index,1,TYPE_OTHERWCHAR));
		this->results.push_back(singleCharLexeme);
	}
}

void AnalyzeContext::outputToResult()
{
	for(int index=0;index<=this->cursor;)
	{
		//跳过标点符号等字符
		if(CHAR_UNKNOWN==this->charTypes[index]){
			index++;
			continue;
		}
		//从pathMap找出对应index位置的LexemePath
		shared_ptr<LexemePath>path =(this->pathMap[index]);
		if(path!=NULL)
		{
			//输出LexemePath中的lexeme到results集合
			shared_ptr<Lexeme>l = path->popHead();
			while(l != NULL)
			{
				this->results.push_back(l);
				//将index移至lexeme后
				index=l->getBegin()+l->getLength();
				l=NULL;
				l=path->popHead();
				if(l != NULL)
				{
					//输出path内部，词元间遗漏的单字
					for(;index < l->getBegin();index++){
						this->outputSingleCJK(index);
					}
				}
			}
		}
		else{//pathMap中找不到index对应的LexemePath
			//单字输出
			this->outputSingleCJK(index);
			index++;
		}
	}
	//清空当前的Map
	this->pathMap.clear();
}
static int k=0;
shared_ptr<Lexeme> AnalyzeContext::getNextLexeme()
{
	//从结果集取出，并移除第一个Lexme
	if (results.size()==0)
	{
		return NULL;
	}
	shared_ptr<Lexeme>result=this->results.front();
	this->results.pop_front();
	while(result!=NULL)
	{
		//数量词合并
		this->compound(result);
		if(Dictionary::getSingleton()->isStopWord(&(this->segmentBuff[0]),result->getBegin(),result->getLength()))
		{
			//是停止词继续取列表的下一个
//			result=this->results.front();
			if (results.size())
			{
				result=this->results.front();
				results.pop_front();
			}
			else result=NULL;
			
		}
		else
		{
			//不是停止词, 生成lexeme的词元文本,输出
			wstring res(&segmentBuff[result->getBegin()],&segmentBuff[result->getBegin()+result->getLength()]);
			result->setLexemeText(res);
			break;
		}
	}
	return result;
}

void AnalyzeContext::compound(shared_ptr<Lexeme>result )
{
	if(!useSmart) return ;
	//数量词合并处理
	if(this->results.size())
	{

		if(TYPE_ARABIC == result->getLexemeType())
		{
			shared_ptr<Lexeme>nextLexeme = this->results.front();
			bool appendOk = false;
			if(TYPE_CNUM == nextLexeme->getLexemeType())
			{
				//合并英文数词+中文数词
				appendOk=result->append(*nextLexeme,TYPE_CNUM);
			}
			else if(TYPE_COUNT==nextLexeme->getLexemeType())
			{
				//合并英文数词+中文量词
				appendOk=result->append(*nextLexeme,TYPE_CQUAN);
			}
			if(appendOk)
			{
				//弹出
				this->results.pop_front(); 
			}
		}

		//可能存在第二轮合并
		if(TYPE_CNUM==result->getLexemeType()&&this->results.size())
		{
			shared_ptr<Lexeme>nextLexeme = this->results.front();
			bool appendOk = false;
			if(TYPE_COUNT == nextLexeme->getLexemeType())
			{
				//合并中文数词+中文量词
				appendOk = result->append(*nextLexeme,TYPE_CQUAN);
			}  
			if(appendOk)
			{
				//弹出
				this->results.pop_front();
			}
		}

	}
}

void AnalyzeContext::reset()
{
	segmentBuff.clear();
	segmentBuff.resize(BUFF_SIZE);
	charTypes.clear();
	charTypes.resize(BUFF_SIZE);
	available=0;
	buffOffset=0;
	cursor=0;
	results.clear();
	pathMap.clear();
	orgLexemes.reset(new QuickSortSet());
}
