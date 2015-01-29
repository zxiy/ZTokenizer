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


//���ʵ�
class Dictionary
{
private:
	static Dictionary *SINGLETON_DICTIONARY;
	Dictionary();
	DictSegment *_MainDict;
	DictSegment *_StopWordDict;
	DictSegment *_QuantifierDict;
	void loadMainDict();//�������ʵ估��չ�ʵ�
	void loadStopWordDict();//�����û���չ��ֹͣ�ʴʵ�
	void loadQuantifierDict();//�������ʴʵ�
	static CRITICAL_SECTION singleton_lock;
public:
	/**
	 * �ʵ��ʼ��
	 * ����Dictionary��ľ�̬�������дʵ��ʼ��
	 * ֻ�е�Dictionary�౻ʵ�ʵ���ʱ���ŻῪʼ����ʵ䣬
	 * �⽫�ӳ��״ηִʲ�����ʱ��
	 * �÷����ṩ��һ����Ӧ�ü��ؽ׶ξͳ�ʼ���ֵ���ֶ�
	 *//*
	Dictionary* initial();*/
	char *DICT_PATH = "";
	static Dictionary* getSingleton();//��ȡ�ʵ䵥��ʵ��ָ��
	static void reloadDictionary();
	void addWords(vector<wstring>words);//���������´���
	void disableWords(vector<wstring> words);//�����Ƴ������Σ�����
	shared_ptr<Hit> matchInMainDict(wchar_t *charArray);//����ƥ�����ʵ� ����Hit ƥ��������
	shared_ptr<Hit> matchInMainDict(wchar_t *charArray,int begin,int length);//����ƥ�����ʵ� ����Hit ƥ��������
	shared_ptr<Hit> matchInQuantifierDict(wchar_t *charArray , int begin, int length);//����ƥ�����ʴʵ� ����Hit ƥ��������
	shared_ptr<Hit> matchWithHit(wchar_t *charArray , int currentIndex ,shared_ptr<Hit> matchedHit);//����ƥ���Hit��ֱ��ȡ��DictSegment����������ƥ��
	bool isStopWord(wchar_t *charArray , int begin, int length);//�ж��Ƿ���ֹͣ��
	~Dictionary();
};
#endif