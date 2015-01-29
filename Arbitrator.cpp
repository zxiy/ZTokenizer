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
	//������ͻ��Lexemeջ
	stack<shared_ptr<Cell> > conflictStack;
	shared_ptr<Cell> c= lexemeCell;
	//��������Lexeme����
	while(c != NULL && c->getLexeme() != NULL)
	{
		if(!option->addNotCrossLexeme(c->getLexeme()))
		{
			//��Ԫ���棬���ʧ�������lexemeStackջ
			conflictStack.push(c);
		}
		c = c->getNext();
	}
	return conflictStack;
}

shared_ptr<LexemePath> Arbitrator::judge(shared_ptr<Cell> lexemeCell , int fullTextLength )
{
	//��ѡ·������
	vector<shared_ptr<LexemePath> > pathOptions;
	//��ѡ���·��
	shared_ptr<LexemePath> option(new LexemePath());
	//��crossPath����һ�α���,ͬʱ���ر��α������г�ͻ��Lexemeջ
	stack<shared_ptr<Cell> >lexemeStack = this->forwardPath(lexemeCell,option);
	//��ǰ��Ԫ������������ģ������ѡ·������
	pathOptions.push_back(option->copy());
	//��������ʣ�����
	shared_ptr<Cell>c;
	while(!lexemeStack.empty())
	{
		c=lexemeStack.top();
		lexemeStack.pop();
		//�ع���Ԫ��
		this->backPath(c->getLexeme(),option);
		//�������λ�ÿ�ʼ���ݹ飬���ɿ�ѡ����
		this->forwardPath(c,option);
		pathOptions.push_back(option->copy());
//		delete c;
	}
	//���ؼ����е����ŷ���
	sort(pathOptions.begin(),pathOptions.end(),[](shared_ptr<LexemePath> a,shared_ptr<LexemePath> b){return a->compareTo(*b)<0;});
	auto tmp_iter=pathOptions.begin();
	//ɾ�������ķ����ͷ��ڴ�
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
			//�ҵ���crossPath���ཻ����һ��crossPath	
			if(crossPath->getSize() == 1 || !useSmart)
			{
				//crossPathû������ ���� �������崦��
				//ֱ�������ǰcrossPath
				context->addLexemePath(crossPath);
			}else{
				//�Ե�ǰ��crossPath�������崦��
				shared_ptr<Cell> headCell = crossPath->getHead();
				shared_ptr<LexemePath> judgeResult = this->judge(headCell, crossPath->getPathLength());
				//������崦����judgeResult
				context->addLexemePath(judgeResult);
			}

			//��orgLexeme�����µ�crossPath��
			crossPath.reset(new LexemePath());
			crossPath->addCrossLexeme(orgLexeme);
		}
		orgLexeme = orgLexemes->popHead();
	}


	//��������path
	if(crossPath->getSize()==1||!useSmart)
	{
		//crossPathû������ ���� �������崦��
		//ֱ�������ǰcrossPath
		context->addLexemePath(crossPath);
	}
	else
	{
		//�Ե�ǰ��crossPath�������崦��
		shared_ptr<Cell> headCell = crossPath->getHead();
		shared_ptr<LexemePath> judgeResult = this->judge(headCell, crossPath->getPathLength());
		//������崦����judgeResult
		context->addLexemePath(judgeResult);
	}
}

Arbitrator::Arbitrator()
{
}
