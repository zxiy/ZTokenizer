#include "CharacterUtil.h"



int CharacterUtil::identifyCharType( wchar_t input )
{
	if (input>='0'&&input<='9')
	{
		return CHAR_ARABIC;
	}
	else if ((input>='a'&&input<='z')||(input>=0x21&&input<=0x2f)||(input>=0x3a&&input<=0x40))
	{
		return CHAR_ENGLISH;
	}
	else if((input>=0x4e00&&input<=0x9fa5)//CJK Unified Ideographs 1.1
		||(input>=0xf900&&input<=0xfa2d)//CJK Compatibility Ideographs 1.1
		||(input>=0x3400&&input<=0x4DB5)//CJK Unified Ideographs Extension A 3.0
		||(input>=0xfa30&&input<=0xfa6a)//CJK Compatibility Ideographs 3.2
		||(input>=0x9fa6&&input<=0x9fbb)//CJK Unified Ideographs 4.1
		||(input>=0xfa70&&input<=0xfad9))//CJK Compatibility Ideographs 4.1
	{
		return CHAR_CHINESE;
	}
	else if((input>=0x3040&&input<=0x309f)//ÈÕÎÄÆ½¼ÙÃû
		||(input>=0x30a0&&input<=0x30ff)//ÈÕÎÄÆ¬¼ÙÃû
		||(input>=0x31f0&&input<=0x31ff)//ÈÕÎÄÆ¬¼ÙÃûÆ´ÒôÀ©Õ¹
		||(input>=0xac00&&input<=0xd7af)//º«ÎÄÆ´Òô
		||(input>=0x1100&&input<=0x11ff)//º«ÎÄ×ÖÄ¸
		||(input>=0x3130&&input<=0x318f)//º«ÎÄ¼æÈİ×ÖÄ¸
		)
	{
		return CHAR_OTHERCJK;
	}
	else return CHAR_UNKNOWN;
}

wchar_t CharacterUtil::regularize( wchar_t input )
{
	if(input==0x3000)//È«½Ç¿Õ¸ñ
		return 0x20;
	else if(input>0xff00&&input<0xff5f)//È«½ÇÊı×Ö×ÖÄ¸·ûºÅ
		input-=0xfee0;
	if (input>='A'&&input<='Z')
	{
		input+=32;
	}
	return input;
}
