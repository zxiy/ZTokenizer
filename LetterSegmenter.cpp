#pragma warning(disable:4267)
#include "LetterSegmenter.h"



LetterSegmenter::LetterSegmenter()
{
	this->start = -1;
	this->end = -1;
	this->englishStart = -1;
	this->englishEnd = -1;
	this->arabicStart = -1;
	this->arabicEnd = -1;
}


bool LetterSegmenter::isLetterConnector( wchar_t input )
{
	int index = Letter_Connector.find(input);
	return index >= 0;
}

bool LetterSegmenter::isNumConnector( wchar_t input )
{
	int index=Num_Connector.find(input);
	return index >= 0;
}

bool LetterSegmenter::processMixLetter(shared_ptr<AnalyzeContext>context)
{
	bool needLock = false;

	if(this->start==-1)//��ǰ�ķִ�����δ��ʼ�����ַ�
	{
		if(CHAR_ARABIC==context->getCurrentCharType()
			||CHAR_ENGLISH==context->getCurrentCharType())
		{
			//��¼��ʼָ���λ��,�����ִ������봦��״̬
			this->start=context->getCursor();
			this->end=start;
		}
	}
	else//��ǰ�ķִ������ڴ����ַ�
	{			
		if(CHAR_ARABIC == context->getCurrentCharType()
			||CHAR_ENGLISH == context->getCurrentCharType())
		{
			//��¼�¿��ܵĽ���λ��
			this->end = context->getCursor();
		}
		else if(CHAR_UNKNOWN==context->getCurrentCharType()
			&&this->isLetterConnector(context->getCurrentChar()))
		{
			//��¼�¿��ܵĽ���λ��
			this->end = context->getCursor();
		}
		else
		{
			//������Letter�ַ��������Ԫ
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),this->start,this->end-this->start+1,TYPE_LETTER));
			context->addLexeme(newLexeme);
			this->start=-1;
			this->end=-1;
		}			
	}

	//�жϻ������Ƿ��Ѿ�����
	if(context->isBufferConsumed())
	{
		if(this->start != -1 && this->end != -1)
		{
			//�����Զ��꣬�����Ԫ
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),this->start,this->end-this->start+1,TYPE_LETTER));
			context->addLexeme(newLexeme);
			this->start = -1;
			this->end = -1;
		}
	}

	//�ж��Ƿ�����������
	if(this->start == -1 && this->end == -1)
	{
		//�Ի���������
		needLock = false;
	}
	else
	{
		needLock = true;
	}
	return needLock;
}

bool LetterSegmenter::processEnglishLetter(shared_ptr<AnalyzeContext> context)
{
	bool needLock = false;

	if(this->englishStart == -1)//��ǰ�ķִ�����δ��ʼ����Ӣ���ַ�
	{
		if(CHAR_ENGLISH == context->getCurrentCharType())
		{
			//��¼��ʼָ���λ��,�����ִ������봦��״̬
			this->englishStart = context->getCursor();
			this->englishEnd = this->englishStart;
		}
	}
	else 
	{//��ǰ�ķִ������ڴ���Ӣ���ַ�	
		if(CHAR_ENGLISH == context->getCurrentCharType())
		{
			//��¼��ǰָ��λ��Ϊ����λ��
			this->englishEnd =  context->getCursor();
		}
		else
		{
			//������English�ַ�,�����Ԫ
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),this->englishStart,this->englishEnd-this->englishStart+1 ,TYPE_ENGLISH));
			context->addLexeme(newLexeme);
			this->englishStart = -1;
			this->englishEnd= -1;
		}
	}

	//�жϻ������Ƿ��Ѿ�����
	if(context->isBufferConsumed())
	{
		if(this->englishStart != -1 && this->englishEnd != -1){
			//�����Զ��꣬�����Ԫ
			shared_ptr<Lexeme>newLexeme(new Lexeme(context->getBufferOffset(),this->englishStart,this->englishEnd-this->englishStart + 1,TYPE_ENGLISH));
			context->addLexeme(newLexeme);
			this->englishStart = -1;
			this->englishEnd= -1;
		}
	}	

	//�ж��Ƿ�����������
	if(this->englishStart == -1 && this->englishEnd == -1)
	{
		//�Ի���������
		needLock = false;
	}
	else
	{
		needLock = true;
	}
	return needLock;
}

bool LetterSegmenter::processArabicLetter(shared_ptr<AnalyzeContext> context)
{
	bool needLock = false;
	if(this->arabicStart == -1)
	{//��ǰ�ķִ�����δ��ʼ���������ַ�	
		if(CHAR_ARABIC == context->getCurrentCharType())
		{
			//��¼��ʼָ���λ��,�����ִ������봦��״̬
			this->arabicStart = context->getCursor();
			this->arabicEnd = this->arabicStart;
		}
	}
	else
	{//��ǰ�ķִ������ڴ��������ַ�	
		if(CHAR_ARABIC == context->getCurrentCharType())
		{
			//��¼��ǰָ��λ��Ϊ����λ��
			this->arabicEnd = context->getCursor();
		}
		else if(CHAR_UNKNOWN == context->getCurrentCharType()
			&& this->isNumConnector(context->getCurrentChar()))
		{
			//��������֣�������ǽ���
		}
		else
		{
			////������Arabic�ַ�,�����Ԫ
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset() ,this->arabicStart,this->arabicEnd-this->arabicStart + 1,TYPE_ARABIC));
			context->addLexeme(newLexeme);
			this->arabicStart = -1;
			this->arabicEnd = -1;
		}
	}
	//�жϻ������Ƿ��Ѿ�����
	if(context->isBufferConsumed())
	{
		if(this->arabicStart != -1 && this->arabicEnd != -1)
		{
			//�������зֵĴ�Ԫ
			shared_ptr<Lexeme> newLexeme(new Lexeme(context->getBufferOffset(),this->arabicStart,this->arabicEnd - this->arabicStart + 1,TYPE_ARABIC));
			context->addLexeme(newLexeme);
			this->arabicStart = -1;
			this->arabicEnd = -1;
		}
	}

	//�ж��Ƿ�����������
	if(this->arabicStart == -1 && this->arabicEnd == -1)
	{
		//�Ի���������
		needLock = false;
	}
	else
	{
		needLock = true;
	}
	return needLock;
}

void LetterSegmenter::analyze(shared_ptr<AnalyzeContext> context)
{
	bool bufferLockFlag = false;
	//����Ӣ����ĸ
	bufferLockFlag = this->processEnglishLetter(context) || bufferLockFlag;
	//����������ĸ
	bufferLockFlag = this->processArabicLetter(context) || bufferLockFlag;
	//��������ĸ
	bufferLockFlag = this->processMixLetter(context) || bufferLockFlag;

	//�ж��Ƿ�����������
	if(bufferLockFlag)
	{
		context->lockBuffer(SEGMENTER_NAME_LS);
	}
	else
	{
		//�Ի���������
		context->unlockBuffer(SEGMENTER_NAME_LS);
	}
}

void LetterSegmenter::reset()
{
	this->start = -1;
	this->end = -1;
	this->englishStart = -1;
	this->englishEnd = -1;
	this->arabicStart = -1;
	this->arabicEnd = -1;
}
