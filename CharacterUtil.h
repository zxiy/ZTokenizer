#ifndef __CHARACTERUTIL_H_
#define __CHARACTERUTIL_H_


#include "Lexeme.h"

static const int CHAR_UNKNOWN=0,
				CHAR_ARABIC=1,
				CHAR_ENGLISH=2,
				CHAR_CHINESE=4,
				CHAR_OTHERCJK=8;
//字类型处理器
class CharacterUtil
{
private:
public:
	int identifyCharType(wchar_t input);
	wchar_t regularize(wchar_t input);

};

#endif