#pragma warning(disable:4244)
#pragma warning(disable:4267)
#include "AnalyzeContext.h"

/*
const int AnalyzeContext::BUFF_SIZE=4096;
const int AnalyzeContext::BUFF_EXHAUST_CRITICAL=100;*/

const int BUFF_SIZE=4096;//��������С
const int BUFF_EXHAUST_CRITICAL=256;//��������Ҫ�ľ����ٽ�ֵ
int AnalyzeContext::fillBuffer(wstringstream &reader)
{
	size_t readCount=0;
	if(buffOffset==0)//�״ζ�ȡ
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
			//���һ�ζ�ȡ��>���һ�δ���ģ���δ������ִ�������segmentBuffͷ��
			for(int i=0;i<offset;i++)
			{
				segmentBuff[i]=segmentBuff[i+this->cursor];
			}
			readCount=offset;
		}
		else offset=0;
		reader.read(&segmentBuff[offset],BUFF_SIZE-offset);//������ȡreader��
		readCount+=reader.gcount();
	}
	this->available=readCount;//��¼���һ�δ�Reader�ж���Ŀ����ַ�����
	this->cursor=0;//���õ�ǰָ��
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
	* ����һ������ʱ��
	* 1.available == BUFF_SIZE ��ʾbuffer����
	* 2.buffIndex < available - 1 && buffIndex > available - BUFF_EXHAUST_CRITICAL��ʾ��ǰָ�봦���ٽ�����
	* 3.!context.isBufferLocked()��ʾû��segmenter��ռ��buffer
	* Ҫ�жϵ�ǰѭ����bufferҪ������λ�����ٶ�ȡ���ݵĲ���*/
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
		//���������ŵ��ַ�
		if(CHAR_UNKNOWN==this->charTypes[index]){
			index++;
			continue;
		}
		//��pathMap�ҳ���Ӧindexλ�õ�LexemePath
		shared_ptr<LexemePath>path =(this->pathMap[index]);
		if(path!=NULL)
		{
			//���LexemePath�е�lexeme��results����
			shared_ptr<Lexeme>l = path->popHead();
			while(l != NULL)
			{
				this->results.push_back(l);
				//��index����lexeme��
				index=l->getBegin()+l->getLength();
				l=NULL;
				l=path->popHead();
				if(l != NULL)
				{
					//���path�ڲ�����Ԫ����©�ĵ���
					for(;index < l->getBegin();index++){
						this->outputSingleCJK(index);
					}
				}
			}
		}
		else{//pathMap���Ҳ���index��Ӧ��LexemePath
			//�������
			this->outputSingleCJK(index);
			index++;
		}
	}
	//��յ�ǰ��Map
	this->pathMap.clear();
}
static int k=0;
shared_ptr<Lexeme> AnalyzeContext::getNextLexeme()
{
	//�ӽ����ȡ�������Ƴ���һ��Lexme
	if (results.size()==0)
	{
		return NULL;
	}
	shared_ptr<Lexeme>result=this->results.front();
	this->results.pop_front();
	while(result!=NULL)
	{
		//�����ʺϲ�
		this->compound(result);
		if(Dictionary::getSingleton()->isStopWord(&(this->segmentBuff[0]),result->getBegin(),result->getLength()))
		{
			//��ֹͣ�ʼ���ȡ�б����һ��
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
			//����ֹͣ��, ����lexeme�Ĵ�Ԫ�ı�,���
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
	//�����ʺϲ�����
	if(this->results.size())
	{

		if(TYPE_ARABIC == result->getLexemeType())
		{
			shared_ptr<Lexeme>nextLexeme = this->results.front();
			bool appendOk = false;
			if(TYPE_CNUM == nextLexeme->getLexemeType())
			{
				//�ϲ�Ӣ������+��������
				appendOk=result->append(*nextLexeme,TYPE_CNUM);
			}
			else if(TYPE_COUNT==nextLexeme->getLexemeType())
			{
				//�ϲ�Ӣ������+��������
				appendOk=result->append(*nextLexeme,TYPE_CQUAN);
			}
			if(appendOk)
			{
				//����
				this->results.pop_front(); 
			}
		}

		//���ܴ��ڵڶ��ֺϲ�
		if(TYPE_CNUM==result->getLexemeType()&&this->results.size())
		{
			shared_ptr<Lexeme>nextLexeme = this->results.front();
			bool appendOk = false;
			if(TYPE_COUNT == nextLexeme->getLexemeType())
			{
				//�ϲ���������+��������
				appendOk = result->append(*nextLexeme,TYPE_CQUAN);
			}  
			if(appendOk)
			{
				//����
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
