#include "QuickSortSet.h"



QuickSortSet::QuickSortSet()
{
	head=tail=NULL;
	size=0;
}

bool QuickSortSet::addLexeme(shared_ptr<Lexeme> lexeme)
{
	shared_ptr<Cell>newCell(new Cell(lexeme));
	if (this->size==0)
	{
		this->head=newCell;
		this->tail=newCell;
		this->size++;
		return true;
	}
	else
	{
		if(!(this->tail->compareTo(newCell)))
		{
			return false;
		}
		else if(this->tail->compareTo(newCell) <0)
		{
			this->tail->next=newCell;
			newCell->prev=this->tail;
			this->tail=newCell;
			this->size++;
			return true;
		}
		else if (this->head->compareTo(newCell) >0)
		{
			this->head->prev=newCell;
			newCell->next=this->head;
			this->head=newCell;
			this->size++;
			return true;
		}
		else
		{
			shared_ptr<Cell>index=this->tail;
			while(index!=NULL&&index->compareTo(newCell)>0)
			{
				index=index->prev;
			}
			if(!index->compareTo(newCell))
			{
				return false;
			}
			else if(index->compareTo(newCell) <0)
			{
				newCell->prev=index;
				newCell->next=index->next;
				index->next->prev=newCell;
				index->next=newCell;
				this->size++;
				return true;
			}
		}
	}
	return false;
}

shared_ptr<Lexeme> QuickSortSet::peekFirst()
{
	return this->head->lexeme;
}

shared_ptr<Lexeme> QuickSortSet::peekLast()
{
	return this->tail->lexeme;
}

shared_ptr<Lexeme> QuickSortSet::popHead()
{
	if (this->size==1)
	{
		shared_ptr<Lexeme>first=this->head->lexeme;
		this->head=this->tail=NULL;
		this->size=0;
		return first;
	}
	else if(this->size>1)
	{
		shared_ptr<Lexeme>first=this->head->lexeme;
		this->head=this->head->next;
		this->head->prev=NULL;
		this->size--;
		return first;
	}
	else return NULL;
}

shared_ptr<Lexeme> QuickSortSet::popTail()
{
	if (this->size==1)
	{
		shared_ptr<Lexeme>last=this->tail->lexeme;
		this->head=this->tail=NULL;
		this->size=0;
		return last;
	}
	else if (this->size>1)
	{
		shared_ptr<Lexeme>last=this->tail->lexeme;
		this->tail=this->tail->prev;
		this->tail->next=NULL;
		this->size--;
		return last;
	}
	else return NULL;
}

int QuickSortSet::getSize()
{
	return this->size;
}

bool QuickSortSet::isEmpty()
{
	return !size;
}

shared_ptr<Cell> QuickSortSet::getHead()
{
	return this->head;
}

shared_ptr<Cell> QuickSortSet::getTail()
{
	return this->tail;
}

Cell::Cell(shared_ptr<Lexeme>lexeme )
{
	this->lexeme=lexeme;
}

int Cell::compareTo(shared_ptr<Cell>o  )
{
	return this->lexeme->compareTo(*o->lexeme);
}

shared_ptr<Cell> Cell::getPrev()
{
	return prev;
}

shared_ptr<Cell> Cell::getNext()
{
	return next;
}

shared_ptr<Lexeme> Cell::getLexeme()
{
	return lexeme;
}
