#pragma warning(disable:4267)
#include "Lexeme.h"

Lexeme::Lexeme( int offset,int begin,int length,int lexemeType )
{
	this->offset=offset;
	this->begin=begin;
	this->length=length;
	this->lexemeType=lexemeType;
}
Lexeme::~Lexeme()
{
}
Lexeme::Lexeme( int offset,int begin,int length,std::wstring text )
{
	this->offset=offset;
	this->begin=begin;
	this->length=length;
	this->lexemeType=lexemeType;
	this->lexemeText=text;
}

Lexeme::Lexeme( int offset,int begin,int length,wchar_t *text )
{
	this->offset=offset;
	this->begin=begin;
	this->length=length;
	this->lexemeType=lexemeType;
	this->lexemeText.assign(text);
}

int Lexeme::getOffset() const
{
	return offset;
}

int Lexeme::getBegin() const
{
	return begin;
}

int Lexeme::getLength() const
{
	return length;
}

int Lexeme::getBeginPosition() const
{
	return offset+begin;
}

bool Lexeme::operator==( const Lexeme &other ) const
{
	if(this->offset==other.getOffset()&&
		this->begin==other.getBegin()&&
		this->length==other.getLength())
		return true;
	else return false;
}

int Lexeme::getEndPosition() const
{
	return offset+begin+length;
}

int Lexeme::compareTo( Lexeme &other ) const
{
	//起始位置优先
	if(this->begin < other.getBegin())
	{
		return -1;
	}
	else if(this->begin == other.getBegin())
	{
		//词素长度优先
		if(this->length > other.getLength())
		{
			return -1;
		}
		else if(this->length == other.getLength())
		{
			return 0;
		}
		else 
		{
			return 1;
		}

	}
	else
	{
		return 1;
	}
}

int Lexeme::hashCode()
{
	int absbegin=getBeginPosition();
	int absend=getEndPosition();
	return absbegin*37+absend*31+absbegin*absend%getLength()*11;
}

void Lexeme::setOffset( int offset )
{
	this->offset=offset;
}

void Lexeme::setBegin( int begin )
{
	this->begin=begin;
}

std::wstring Lexeme::getLexemeText()
{
	return lexemeText;
}
void Lexeme::setLexemeText( wchar_t *str )
{
	lexemeText.assign(str);
	length=lexemeText.size();
}
void Lexeme::setLexemeText( std::wstring str )
{
	lexemeText.assign(str);
	length=lexemeText.size();
}

bool Lexeme::append( Lexeme l1,int lexemeType )
{
	if(this->getEndPosition()==l1.getBeginPosition())
	{
		this->length+=l1.getLength();
		this->lexemeType=lexemeType;
		return true;
	}
	else return false;
}

void Lexeme::setLexemeType( int lexemeType )
{
	this->lexemeType;
}

int Lexeme::getLexemeType()
{
	return lexemeType;
}

void Lexeme::setLength( int length )
{
	this->length=length;
}
