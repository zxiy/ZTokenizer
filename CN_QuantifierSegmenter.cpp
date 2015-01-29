#include "CN_QuantifierSegmenter.h"


void CN_QuantifierSegmenter::outputNumLexeme(shared_ptr<AnalyzeContext> context )
{
	if(nStart > -1 && nEnd > -1)
	{
		//�������
		shared_ptr<Lexeme>newLexeme (new Lexeme(context->getBufferOffset() , nStart , nEnd - nStart + 1,TYPE_CNUM));
		context->addLexeme(newLexeme);
	}
}

void CN_QuantifierSegmenter::processCNumber(shared_ptr<AnalyzeContext> context )
{
	if(nStart == -1 && nEnd == -1)
	{//��ʼ״̬
		if(CHAR_CHINESE == context->getCurrentCharType() 
			&& ChnNumberChars.count(context->getCurrentChar()))
		{
			//��¼���ʵ���ʼ������λ��
			nStart = context->getCursor();
			nEnd = context->getCursor();
		}
	}
	else
	{//���ڴ���״̬
		if(CHAR_CHINESE == context->getCurrentCharType() 
			&& ChnNumberChars.count(context->getCurrentChar()))
		{
			//��¼���ʵĽ���λ��
			nEnd = context->getCursor();
		}
		else
		{
			//�������
			this->outputNumLexeme(context);
			//����ͷβָ��
			nStart = -1;
			nEnd = -1;
		}
	}

	//�������Ѿ����꣬������δ���������
	if(context->isBufferConsumed())
	{
		if(nStart != -1 && nEnd != -1)
		{
			//�������
			outputNumLexeme(context);
			//����ͷβָ��
			nStart = -1;
			nEnd = -1;
		}
	}
}

bool CN_QuantifierSegmenter::needCountScan( shared_ptr<AnalyzeContext> context )
{
	if((nStart!=-1&&nEnd!=-1)||!countHits.empty())
	{
		//���ڴ�����������,�������ڴ�������
		return true;
	}
	else
	{
		//�ҵ�һ�����ڵ�����
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
	// �ж��Ƿ���Ҫ��������ɨ��
	if(!this->needCountScan(context))
	{
		return;
	}
	if(CHAR_CHINESE == context->getCurrentCharType())
	{
		//���ȴ���countHits�е�hit
		if(!this->countHits.empty())
		{
			//����ʶζ���
			vector<shared_ptr<Hit> >tmpArray(this->countHits.begin(),this->countHits.end());
			for(shared_ptr<Hit>hit:tmpArray)
			{
				hit = Dictionary::getSingleton()->matchWithHit(context->getSegmentBuff(),context->getCursor(),hit);
				if(hit->isMatch())
				{
					//�����ǰ�Ĵ�
					shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),hit->getBegin(),context->getCursor() - hit->getBegin() + 1 ,TYPE_COUNT));
					context->addLexeme(newLexeme);
					if(!hit->isPrefix())
					{//���Ǵ�ǰ׺��hit����Ҫ����ƥ�䣬�Ƴ�
						this->countHits.remove(hit);
					}
				}
				else if(hit->isUnmatch())
				{
					//hit���Ǵʣ��Ƴ�
					this->countHits.remove(hit);
				}					
			}
		}				
		//*********************************
		//�Ե�ǰָ��λ�õ��ַ����е���ƥ��
		shared_ptr<Hit> singleCharHit = Dictionary::getSingleton()->matchInQuantifierDict(context->getSegmentBuff(), context->getCursor(),1);
		if(singleCharHit->isMatch())
		{//���ֳ����ʴ�
			//�����ǰ�Ĵ�
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),context->getCursor(),1,TYPE_COUNT));
			context->addLexeme(newLexeme);
			//ͬʱҲ�Ǵ�ǰ׺
			if(singleCharHit->isPrefix())
			{
				//ǰ׺ƥ�������hit�б�
				this->countHits.push_back(singleCharHit);
			}
		}
		else if(singleCharHit->isPrefix())
		{//����Ϊ����ǰ׺
			//ǰ׺ƥ�������hit�б�
			this->countHits.push_back(singleCharHit);
		}
	}
	else
	{
		//����Ĳ��������ַ�
		//���δ���ε�����
		this->countHits.clear();
	}
	//�����������Ѿ����꣬������δ���������
	if(context->isBufferConsumed())
	{
		//���δ���ε�����
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
	//������������
	this->processCNumber(context);
	//������������
	this->processCount(context);
	//�ж��Ƿ�����������
	if(this->nStart == -1 && this->nEnd == -1&& countHits.empty())
	{
		//�Ի���������
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
