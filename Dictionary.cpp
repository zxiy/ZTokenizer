#include "Dictionary.h"

Dictionary* Dictionary::SINGLETON_DICTIONARY=NULL;
CRITICAL_SECTION Dictionary::singleton_lock=init_critical_section();
std::wstring ANSI_To_UNICODE( const char* cStr )
{
	int slen = strlen(cStr);
	if(slen==0)
		return L"";
	wchar_t* buffer = new wchar_t[slen];
	int len = MultiByteToWideChar(CP_ACP,0,cStr,strlen(cStr),buffer,slen);
	if(len>0 && len < slen)
		buffer[len]='\0';
	wstring ws(buffer,len);
	delete [] buffer;
	return ws;
}

CRITICAL_SECTION init_critical_section()
{
	CRITICAL_SECTION t;
	InitializeCriticalSectionAndSpinCount(&t,4000);
	return t;
}

void Dictionary::loadMainDict()
{
	//建立一个主词典实例
	_MainDict = new DictSegment('0');
	//读取主词典文件
	string path=DICT_PATH;
	path+="main2012.dic";
	ifstream dicin(path,ios::in);
	while(!dicin.eof() && dicin.good()) //20140729添加
	{
		string term;
		dicin>>term;
		if(term.size()!=0)
			_MainDict->fillSegment(ANSI_To_UNICODE(term.c_str()).c_str());
	}
	dicin.close();
}

void Dictionary::loadStopWordDict()
{
	//建立一个主词典实例
	_StopWordDict = new DictSegment('0');
	//读取主词典文件
	string path=DICT_PATH;
	path+="stopword.dic";
	ifstream dicin(path,ios::in);
	while(!dicin.eof())
	{
		string term;
		dicin>>term;
		if(term.size()!=0)
			_StopWordDict->fillSegment(ANSI_To_UNICODE(term.c_str()).c_str());
	}
	dicin.close();
}

void Dictionary::loadQuantifierDict()
{
	//建立一个主词典实例
	_QuantifierDict = new DictSegment('0');
	//读取主词典文件
	string path=DICT_PATH;
	path+="quantifier.dic";
	ifstream dicin(path,ios::in);
	while(!dicin.eof())
	{
		string term;
		dicin>>term;
		if(term.size()!=0)
			_QuantifierDict->fillSegment(ANSI_To_UNICODE(term.c_str()).c_str());
	}
	dicin.close();
}


void Dictionary::disableWords( vector<wstring> words )
{
	if(words.size()>0)
		for(wstring word:words)
			if (word.size()>0)
				this->_MainDict->disableSegment(word.c_str());
}

shared_ptr<Hit> Dictionary::matchInMainDict( wchar_t *charArray )
{
	return this->_MainDict->match(charArray);
}

shared_ptr<Hit> Dictionary::matchInMainDict( wchar_t *charArray,int begin,int length )
{
	return this->_MainDict->match(charArray,begin,length);
}

shared_ptr<Hit> Dictionary::matchInQuantifierDict( wchar_t *charArray , int begin, int length )
{
	return this->_QuantifierDict->match(charArray,begin,length);
}

shared_ptr<Hit> Dictionary::matchWithHit( wchar_t *charArray , int currentIndex ,shared_ptr<Hit> matchedHit )
{
	shared_ptr<DictSegment> ds = matchedHit->getMatchedDictSegment();
	return ds->match(charArray, currentIndex,1,matchedHit);
}

bool Dictionary::isStopWord( wchar_t *charArray , int begin, int length )
{
	return this->_StopWordDict->match(charArray,begin,length)->isMatch();
}

void Dictionary::addWords( vector<wstring>words ) /*批量加载新词条 */
{
	if(words.size()>0)
		for(wstring word:words)
			if (word.size()>0)
				this->_MainDict->fillSegment(word.c_str());
}

Dictionary::Dictionary()
{
	this->loadMainDict();
	this->loadStopWordDict();
	this->loadQuantifierDict();
}

void Dictionary::reloadDictionary()
{
	if (SINGLETON_DICTIONARY!=NULL)
	{
		delete SINGLETON_DICTIONARY;
	}
	SINGLETON_DICTIONARY=new Dictionary;
}

Dictionary* Dictionary::getSingleton()
{
	if (SINGLETON_DICTIONARY==NULL)
	{
		LockGuard lock(&singleton_lock);
		if (SINGLETON_DICTIONARY==NULL)
			SINGLETON_DICTIONARY=new Dictionary;
		else return SINGLETON_DICTIONARY;
	}
	return SINGLETON_DICTIONARY;
}

Dictionary::~Dictionary()
{
	if (_MainDict!=NULL)
	{
		delete _MainDict;
		_MainDict=NULL;
	}
	if (_StopWordDict!=NULL)
	{
		delete _StopWordDict;
		_StopWordDict=NULL;
	}
	if (_QuantifierDict!=NULL)
	{
		delete _QuantifierDict;
		_QuantifierDict=NULL;
	}
}
