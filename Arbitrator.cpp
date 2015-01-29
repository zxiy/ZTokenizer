#include "Arbitrator.h"



void Arbitrator::backPath(shared_ptr<Lexeme>l,shared_ptr<LexemePath> option )
{
	while(option->checkCross(l))
	{
		option->removeTail();
	}
}

stack<shared_ptr<Cell> > Arbitrator::forwardPath(shared_ptr<Cell>lexemeCell ,shared_ptr<LexemePath>option )
{
	//发生冲突的Lexeme栈
	stack<shared_ptr<Cell> > conflictStack;
	shared_ptr<Cell> c= lexemeCell;
	//迭代遍历Lexeme链表
	while(c != NULL && c->getLexeme() != NULL)
	{
		if(!option->addNotCrossLexeme(c->getLexeme()))
		{
			//词元交叉，添加失败则加入lexemeStack栈
			conflictStack.push(c);
		}
		c = c->getNext();
	}
	return conflictStack;
}

shared_ptr<LexemePath> Arbitrator::judge(shared_ptr<Cell> lexemeCell , int fullTextLength )
{
	//候选路径集合
	vector<shared_ptr<LexemePath> > pathOptions;
	//候选结果路径
	shared_ptr<LexemePath> option(new LexemePath());
	//对crossPath进行一次遍历,同时返回本次遍历中有冲突的Lexeme栈
	stack<shared_ptr<Cell> >lexemeStack = this->forwardPath(lexemeCell,option);
	//当前词元链并非最理想的，加入候选路径集合
	pathOptions.push_back(option->copy());
	//存在歧义词，处理
	shared_ptr<Cell>c;
	while(!lexemeStack.empty())
	{
		c=lexemeStack.top();
		lexemeStack.pop();
		//回滚词元链
		this->backPath(c->getLexeme(),option);
		//从歧义词位置开始，递归，生成可选方案
		this->forwardPath(c,option);
		pathOptions.push_back(option->copy());
//		delete c;
	}
	//返回集合中的最优方案
	sort(pathOptions.begin(),pathOptions.end(),[](shared_ptr<LexemePath> a,shared_ptr<LexemePath> b){return a->compareTo(*b)<0;});
	auto tmp_iter=pathOptions.begin();
	//删除废弃的方案释放内存
//	for_each(++tmp_iter,pathOptions.end(),[](LexemePath* a){delete a;});
	return pathOptions[0];
}

void Arbitrator::process(shared_ptr<AnalyzeContext>context , bool useSmart )
{
	shared_ptr<QuickSortSet> orgLexemes = context->getOrgLexemes();
	shared_ptr<Lexeme> orgLexeme = orgLexemes->popHead();

	shared_ptr<LexemePath>crossPath(new LexemePath());
	while(orgLexeme != NULL)
	{
		if(!crossPath->addCrossLexeme(orgLexeme))
		{
			//找到与crossPath不相交的下一个crossPath	
			if(crossPath->getSize() == 1 || !useSmart)
			{
				//crossPath没有歧义 或者 不做歧义处理
				//直接输出当前crossPath
				context->addLexemePath(crossPath);
			}else{
				//对当前的crossPath进行歧义处理
				shared_ptr<Cell> headCell = crossPath->getHead();
				shared_ptr<LexemePath> judgeResult = this->judge(headCell, crossPath->getPathLength());
				//输出歧义处理结果judgeResult
				context->addLexemePath(judgeResult);
			}

			//把orgLexeme加入新的crossPath中
			crossPath.reset(new LexemePath());
			crossPath->addCrossLexeme(orgLexeme);
		}
		orgLexeme = orgLexemes->popHead();
	}


	//处理最后的path
	if(crossPath->getSize()==1||!useSmart)
	{
		//crossPath没有歧义 或者 不做歧义处理
		//直接输出当前crossPath
		context->addLexemePath(crossPath);
	}
	else
	{
		//对当前的crossPath进行歧义处理
		shared_ptr<Cell> headCell = crossPath->getHead();
		shared_ptr<LexemePath> judgeResult = this->judge(headCell, crossPath->getPathLength());
		//输出歧义处理结果judgeResult
		context->addLexemePath(judgeResult);
	}
}

Arbitrator::Arbitrator()
{
}
