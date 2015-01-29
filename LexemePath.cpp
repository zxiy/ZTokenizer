#include "LexemePath.h"



LexemePath::LexemePath()
{
	this->pathBegin=-1;
	this->pathEnd=-1;
	this->payloadLength=0;
}

bool LexemePath::addCrossLexeme(shared_ptr<Lexeme> lexeme )
{
	if (this->isEmpty())
	{
		this->addLexeme(lexeme);
		this->pathBegin=lexeme->getBegin();
		this->pathEnd=this->pathBegin+lexeme->getLength();
		this->payloadLength=lexeme->getLength();
		return true;
	}
	else if (this->checkCross(lexeme))
	{
		this->addLexeme(lexeme);
		if (lexeme->getBegin()+lexeme->getLength()>this->pathEnd)
		{
			this->pathEnd=lexeme->getBegin()+lexeme->getLength();
		}
		this->payloadLength=this->pathEnd-this->pathBegin;
		return true;
	}
	else return false;
}

bool LexemePath::checkCross( shared_ptr<Lexeme> lexeme )
{
	return (lexeme->getBegin() >= this->pathBegin && lexeme->getBegin() < this->pathEnd)
		||(this->pathBegin >= lexeme->getBegin() && this->pathBegin < lexeme->getBegin()+ lexeme->getLength());
}

bool LexemePath::addNotCrossLexeme(shared_ptr<Lexeme>lexeme )
{
	if(this->isEmpty())
	{
		this->addLexeme(lexeme);
		this->pathBegin=lexeme->getBegin();
		this->pathEnd=this->pathBegin+lexeme->getLength();
		this->payloadLength+=lexeme->getLength();
		return true;
	}
	else if (this->checkCross(lexeme))
	{
		return false;
	}
	else
	{
		this->addLexeme(lexeme);
		this->payloadLength+=lexeme->getLength();
		shared_ptr<Lexeme> head=this->peekFirst();
		this->pathBegin=head->getBegin();
		shared_ptr<Lexeme> tail=this->peekLast();
		this->pathEnd = tail->getBegin()+tail->getLength();
		return true;
	}
}

shared_ptr<Lexeme> LexemePath::removeTail()
{
	shared_ptr<Lexeme> tail = this->popTail();
	if(this->isEmpty())
	{
		this->pathBegin = -1;
		this->pathEnd = -1;
		this->payloadLength = 0;			
	}
	else
	{		
		this->payloadLength-=tail->getLength();
		shared_ptr<Lexeme> newTail = this->peekLast();
		this->pathEnd = newTail->getBegin() + newTail->getLength();
	}
	return tail;
}

int LexemePath::getPayloadLength()
{
	return this->payloadLength;
}

int LexemePath::getPathLength()
{
	return this->pathEnd=this->pathBegin;
}

int LexemePath::getXWeight()
{
	int product=1;
	shared_ptr<Cell> c = this->getHead();
	while(c!=NULL&&c->getLexeme()!=NULL)
	{
		product*=c->getLexeme()->getLength();
		c = c->getNext();
	}
	return product;
}

int LexemePath::getPWeight()
{
	int pWeight = 0;
	int p = 0;
	shared_ptr<Cell> c=this->getHead();
	while(c!=NULL&&c->getLexeme()!=NULL)
	{
		p++;
		pWeight+=p*c->getLexeme()->getLength() ;
		c=c->getNext();
	}
	return pWeight;
}

shared_ptr<LexemePath> LexemePath::copy()
{
	shared_ptr<LexemePath> theCopy(new LexemePath());
	theCopy->pathBegin = this->pathBegin;
	theCopy->pathEnd = this->pathEnd;
	theCopy->payloadLength = this->payloadLength;
	shared_ptr<Cell> c=this->getHead();
	while(c!=NULL&&c->getLexeme()!=NULL)
	{
		theCopy->addLexeme(c->getLexeme());
		c=c->getNext();
	}
	return theCopy;
}

int LexemePath::compareTo( LexemePath &o )
{
	//比较有效文本长度
	if(this->payloadLength > o.payloadLength)
	{
		return -1;
	}
	else if(this->payloadLength < o.payloadLength)
	{
		return 1;
	}
	else
	{
		//比较词元个数，越少越好
		if(this->getSize()<o.getSize())
		{
			return -1;
		}
		else if(this->getSize()>o.getSize())
		{
			return 1;
		}
		else
		{
			//路径跨度越大越好
			if(this->getPathLength()>o.getPathLength())
			{
				return -1;
			}
			else if(this->getPathLength()<o.getPathLength())
			{
				return 1;
			}
			else 
			{
				//根据统计学结论，逆向切分概率高于正向切分，因此位置越靠后的优先
				if(this->pathEnd>o.pathEnd)
				{
					return -1;
				}
				else if(pathEnd < o.pathEnd)
				{
					return 1;
				}
				else
				{
					//词长越平均越好
					if(this->getXWeight()>o.getXWeight())
					{
						return -1;
					}
					else if(this->getXWeight()<o.getXWeight())
					{
						return 1;
					}
					else
					{
						//词元位置权重比较
						if(this->getPWeight()>o.getPWeight())
						{
							return -1;
						}
						else if(this->getPWeight()<o.getPWeight())
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

int LexemePath::getPathBegin()
{
	return pathBegin;
}

int LexemePath::getPathEnd()
{
	return pathEnd;
}

