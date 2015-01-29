#include "Hit.h"

Hit::Hit()
{
	hitState = UNMATCH;
}

bool Hit::isMatch()
{
	return (this->hitState&MATCH) > 0;
}

void Hit::setMatch()
{
	this->hitState = this->hitState | MATCH;
}

bool Hit::isPrefix()
{
	return (this->hitState & PREFIX) > 0;
}

void Hit::setPrefix()
{
	this->hitState = this->hitState | PREFIX;
}

bool Hit::isUnmatch()
{
	return this->hitState == UNMATCH ;
}

void Hit::setUnmatch()
{
	this->hitState = UNMATCH;
}

shared_ptr<DictSegment> Hit::getMatchedDictSegment()
{
	return matchedDictSegment;
}


void Hit::setMatchedDictSegment(shared_ptr<DictSegment> matchedDictSegment )
{
	this->matchedDictSegment = matchedDictSegment;
}

int Hit::getBegin()
{
	return begin;
}

void Hit::setBegin( int begin )
{
	this->begin = begin;
}

int Hit::getEnd()
{
	return end;
}

void Hit::setEnd( int end )
{
	this->end = end;
}
