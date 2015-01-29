#ifndef __QUICKSORTSET_H__
#define __QUICKSORTSET_H__
#include <list>
#include <map>
#include <memory>
#include "Lexeme.h"
using namespace std;
class Cell;
//分词结果集排序
class QuickSortSet
{
private:
	shared_ptr<Cell> head,tail;
	int size;
	
public:
	QuickSortSet();
	bool addLexeme(shared_ptr<Lexeme> lexeme);
	shared_ptr<Lexeme> peekFirst();
	shared_ptr<Lexeme> peekLast();
	shared_ptr<Lexeme> popHead();
	shared_ptr<Lexeme> popTail();
	int getSize();
	bool isEmpty();
	shared_ptr<Cell> getHead();
	shared_ptr<Cell> getTail();
};

class Cell
{
public:
	shared_ptr<Cell> prev;
	shared_ptr<Cell> next;
	shared_ptr<Lexeme> lexeme;
	Cell(shared_ptr<Lexeme>lexeme);
	int compareTo(shared_ptr<Cell>o);
	shared_ptr<Cell> getPrev();
	shared_ptr<Cell> getNext();
	shared_ptr<Lexeme> getLexeme();
};

#endif