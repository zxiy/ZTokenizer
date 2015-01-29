#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__
#include "DictSegment.h"
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;
std::wstring ANSI_To_UNICODE( const char* cStr );

class LockGuard
{
	CRITICAL_SECTION *lock;
public:
	LockGuard(CRITICAL_SECTION *lock)
	{
		this->lock=lock;
		EnterCriticalSection(lock);
	}
	~LockGuard()
	{
		LeaveCriticalSection(lock);
	}
};

CRITICAL_SECTION init_critical_section();


//主词典
class Dictionary
{
private:
	static Dictionary *SINGLETON_DICTIONARY;
	Dictionary();
	DictSegment *_MainDict;
	DictSegment *_StopWordDict;
	DictSegment *_QuantifierDict;
	void loadMainDict();//加载主词典及扩展词典
	void loadStopWordDict();//加载用户扩展的停止词词典
	void loadQuantifierDict();//加载量词词典
	static CRITICAL_SECTION singleton_lock;
public:
	/**
	 * 词典初始化
	 * 采用Dictionary类的静态方法进行词典初始化
	 * 只有当Dictionary类被实际调用时，才会开始载入词典，
	 * 这将延长首次分词操作的时间
	 * 该方法提供了一个在应用加载阶段就初始化字典的手段
	 *//*
	Dictionary* initial();*/
	char *DICT_PATH = "";
	static Dictionary* getSingleton();//获取词典单子实例指针
	static void reloadDictionary();
	void addWords(vector<wstring>words);//批量加载新词条
	void disableWords(vector<wstring> words);//批量移除（屏蔽）词条
	shared_ptr<Hit> matchInMainDict(wchar_t *charArray);//检索匹配主词典 返回Hit 匹配结果描述
	shared_ptr<Hit> matchInMainDict(wchar_t *charArray,int begin,int length);//检索匹配主词典 返回Hit 匹配结果描述
	shared_ptr<Hit> matchInQuantifierDict(wchar_t *charArray , int begin, int length);//检索匹配量词词典 返回Hit 匹配结果描述
	shared_ptr<Hit> matchWithHit(wchar_t *charArray , int currentIndex ,shared_ptr<Hit> matchedHit);//从已匹配的Hit中直接取出DictSegment，继续向下匹配
	bool isStopWord(wchar_t *charArray , int begin, int length);//判断是否是停止词
	~Dictionary();
};
#endif