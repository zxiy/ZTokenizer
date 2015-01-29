#include "CJKSegmenter.h"


void CJKSegmenter::analyze(shared_ptr<AnalyzeContext>context )
{
	if(CHAR_UNKNOWN!= context->getCurrentCharType())
	{
		//���ȴ���tmpHits�е�hit
		if(!this->tmpHits.empty())
		{
			//����ʶζ���
			vector<shared_ptr<Hit> >tmpArray(this->tmpHits.begin(),this->tmpHits.end());
			for(shared_ptr<Hit> hit : tmpArray)
			{
				hit = Dictionary::getSingleton()->matchWithHit(context->getSegmentBuff(),context->getCursor(),hit);
				if(hit->isMatch())
				{
					//�����ǰ�Ĵ�
					shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),hit->getBegin(),context->getCursor()-hit->getBegin()+1,TYPE_CNWORD));
					context->addLexeme(newLexeme);
					if(!hit->isPrefix())
					{//���Ǵ�ǰ׺��hit����Ҫ����ƥ�䣬�Ƴ�
						this->tmpHits.remove(hit);
					}

				}
				else if(hit->isUnmatch())
				{
					//hit���Ǵʣ��Ƴ�
					this->tmpHits.remove(hit);
				}
			}
		}			
		//*********************************
		//�ٶԵ�ǰָ��λ�õ��ַ����е���ƥ��
		shared_ptr<Hit>singleCharHit = Dictionary::getSingleton()->matchInMainDict(context->getSegmentBuff(),context->getCursor(), 1);
		if(singleCharHit->isMatch())
		{//���ֳɴ�
			//�����ǰ�Ĵ�
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),context->getCursor(),1,TYPE_CNWORD));
			context->addLexeme(newLexeme);
			//ͬʱҲ�Ǵ�ǰ׺
			if(singleCharHit->isPrefix())
			{
				//ǰ׺ƥ�������hit�б�
				this->tmpHits.push_back(singleCharHit);
			}
			else
			{
				//����ǰ׺��ɾ���ͷſռ�
				singleCharHit.reset();
			}
		}
		else if(singleCharHit->isPrefix())
		{//����Ϊ��ǰ׺
			//ǰ׺ƥ�������hit�б�
			this->tmpHits.push_back(singleCharHit);
		}
		else
		{
			//��ƥ�䣬ɾ���ͷſռ�
			singleCharHit.reset();
		}
	}
	else
	{
		//����CHAR_USELESS�ַ�
		//��ն���
		//for_each(tmpHits.begin(),tmpHits.end(),[](Hit* a){delete a;});
		this->tmpHits.clear();
	}
	//�жϻ������Ƿ��Ѿ�����
	if(context->isBufferConsumed())
	{
		//��ն���
		//for_each(tmpHits.begin(),tmpHits.end(),[](Hit* a){delete a;});
		this->tmpHits.clear();
	}

	//�ж��Ƿ�����������
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
	//��ն���
	this->tmpHits.clear();
}
