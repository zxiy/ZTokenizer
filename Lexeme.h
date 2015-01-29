
#ifndef __LEXEME_H__
#define __LEXEME_H__

#include <string>
//�ַ����ͳ���
static const int TYPE_UNKNOWN=0/*δ֪����*/
	,TYPE_ENGLISH=1/*Ӣ��*/
	,TYPE_ARABIC=2/*����������*/
	,TYPE_LETTER=4/*Ӣ�����ֻ�ϵ��ַ�*/
	,TYPE_CNWORD=8/*���Ĵ���*/
	,TYPE_CNCHAR=16/*���ĵ���*/
	,TYPE_CNUM=32/*��������*/
	,TYPE_COUNT=64/*��������*/
	,TYPE_CQUAN=128/*����������*/
	,TYPE_OTHERWCHAR=256;/*�������ַ������պ��ɲ�����*/

//������
class Lexeme
{
private:
	int offset;/*������ʼλ��ƫ��*/
	int begin;/*���������ʼλ��*/
	int length;/*���صĳ���*/
	int lexemeType;/*���ص�����*/
	std::wstring lexemeText;/*���ص��ı�*/

public:
	Lexeme(int offset,int begin,int length,int lexemeType);
	Lexeme(int offset,int begin,int length,std::wstring text);
	Lexeme(int offset,int begin,int length,wchar_t *text);
	~Lexeme();
	int getOffset() const;
	int getBegin() const;
	int getLength() const;
	int getBeginPosition() const;
	bool operator == (const Lexeme &other) const;/*����==��������жϴ�����ȣ���ʼλ�á�ƫ��λ�á����ȶ���Ⱦ��ж�Ϊ���*/
	int getEndPosition() const;
	int hashCode();
	int compareTo( Lexeme &other ) const;
	void setOffset( int offset );
	void setBegin( int begin );
	void setLength( int length );
	std::wstring getLexemeText();
	void setLexemeText(wchar_t *str);	
	void setLexemeText(std::wstring str);
	bool append(Lexeme l1,int lexemeType);
	void setLexemeType( int lexemeType );
	int getLexemeType();

};
#endif

