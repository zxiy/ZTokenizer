#ifndef __ARBITRATOR_H__
#define __ARBITRATOR_H__
#include "AnalyzeContext.h"
#include <stack>
//ÆçÒå²Ã¾öÆ÷
class Arbitrator
{
private:
	void backPath(shared_ptr<Lexeme> l,shared_ptr<LexemePath> option);
	stack<shared_ptr<Cell> > forwardPath(shared_ptr<Cell>lexemeCell , shared_ptr<LexemePath> option);
	shared_ptr<LexemePath> judge(shared_ptr<Cell> lexemeCell , int fullTextLength);
public:
	Arbitrator();
	void process(shared_ptr<AnalyzeContext>context , bool useSmart);
};

#endif