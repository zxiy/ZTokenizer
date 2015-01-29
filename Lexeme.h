
#ifndef __LEXEME_H__
#define __LEXEME_H__

#include <string>
//字符类型常量
static const int TYPE_UNKNOWN=0/*未知类型*/
	,TYPE_ENGLISH=1/*英文*/
	,TYPE_ARABIC=2/*阿拉伯数字*/
	,TYPE_LETTER=4/*英文数字混合的字符*/
	,TYPE_CNWORD=8/*中文词素*/
	,TYPE_CNCHAR=16/*中文单字*/
	,TYPE_CNUM=32/*中文数词*/
	,TYPE_COUNT=64/*中文量词*/
	,TYPE_CQUAN=128/*中文数量词*/
	,TYPE_OTHERWCHAR=256;/*其他宽字符，如日韩蒙藏彝文*/

//单字类
class Lexeme
{
private:
	int offset;/*词素起始位置偏移*/
	int begin;/*词素相对起始位置*/
	int length;/*词素的长度*/
	int lexemeType;/*词素的类型*/
	std::wstring lexemeText;/*词素的文本*/

public:
	Lexeme(int offset,int begin,int length,int lexemeType);
	Lexeme(int offset,int begin,int length,std::wstring text);
	Lexeme(int offset,int begin,int length,wchar_t *text);
	~Lexeme();
	int getOffset() const;
	int getBegin() const;
	int getLength() const;
	int getBeginPosition() const;
	bool operator == (const Lexeme &other) const;/*重载==运算符，判断词素相等：起始位置、偏移位置、长度都相等就判定为相等*/
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

