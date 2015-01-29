#include "DictSegment.h"
#pragma warning(disable:4267)
map<wchar_t,wchar_t>DictSegment::charMap;/*���溺�ֵ��ֵ���ʼ��*/
DictSegment::DictSegment(wchar_t NodeChar)
{
	storeSize=0;
	NodeState=0;
	this->NodeChar=NodeChar;
}
DictSegment::DictSegment()
{
}

wchar_t DictSegment::getNodeChar()
{
	return this->NodeChar;
}

bool DictSegment::hasChildNode()
{
	return this->storeSize>0;
}

	/* ����HitResult�ͷ�����������ͬ�����ڵݹ�ʹ�úͲ�������.
	 * STEP1:�����в��ҹؼ��ֶ�Ӧ������
	 * STEP2:�ҵ���Ӧ���������жϴʵ�ƥ��״̬�����ݽ�������Ǽ����ݹ飬���Ƿ��ؽ��
	 * STEP3:���û���ҵ���Ӧ�������� ��HIT����Ϊ��ƥ��
	 * param charArray ������Ҫ����ƥ��Ĵʶε�����
	 * param begin charArray����ĵ�begin��Ԫ��Ϊʵ�ʵ���ʼƥ��λ��
	 * param length ��Ҫ����ƥ����ֶγ���
	 * param HitResult �ֶ�ƥ������������뷽����������ͬ ##�����ʾȫ������##
	 * return HitTarget ������� �����һ��������ͬ
	 * */
shared_ptr<Hit> DictSegment::match(const wchar_t *charArray,int begin,int length,shared_ptr<Hit> HitResult) 
{
	if (HitResult==NULL)
	{
		HitResult.reset(new Hit());//���Ϊ�����½�
		HitResult->setBegin(begin);
	}
	else
	{
		HitResult->setUnmatch();/*����ƥ��ǰ��ȷ������״̬Ϊδƥ��*/
	}
	HitResult->setEnd(begin);/*����ƥ�䵱ǰ����λ��*/
	wchar_t keyChar=charArray[begin];/*Ҫ���ҵĹؼ���*/
	shared_ptr<DictSegment> resDicSeg;	/*�ҵ�������*/
	vector<shared_ptr<DictSegment> > &segArray=this->childArray;	/*��ֹ����µ�ͬ���������bug����ʵ����������Ϊ���ر���*/
	map<wchar_t,shared_ptr<DictSegment> > &segMap=this->childMap;
	/*STEP1��������˵��*/
	if(segArray.size())	/*�������в���*/
	{
		int position=-1;/*��¼�ҵ����ֵ�����λ��*/
		for(int i=0;i<this->storeSize;i++)
		{
			if(keyChar == segArray[i]->NodeChar)
				position = i;
		}
		if(position>=0)
		{
			resDicSeg=segArray[position];
		}
	}
	else if(segMap.size())/*��HASHMAP�в���*/
		if(segMap.find(keyChar)!=segMap.end())/*map::find() ������ҵ�ĳԪ�ؾͷ��ظ�Ԫ�صĵ����������򷵻�ָ��mapĩβ�ĵ��������ɴ��ж�map���Ƿ�����һ��Ԫ��*/
			resDicSeg=segMap[keyChar];
	/*STEP2��������˵��*/
	if(resDicSeg!=NULL)
	{
		if(length>1)/*�ʲ�û��ƥ����ʱ�������½��еݹ�����*/
		{
			return resDicSeg->match(charArray,begin+1,length-1,HitResult);
		}
		else if(length==1)/*ֻʣ��һ����ʱ���������һ����*/
		{
			if(resDicSeg->NodeState==1)/*�жϴӸ��ڵ㵽��ǰ�ڵ��Ƿ�ɴ�*/
			{
				HitResult->setMatch();/*�������״̬Ϊ��ȫƥ��*/
			}
			if(resDicSeg->hasChildNode())/*����ڵ�ƥ���ҵ�ǰ�ڵ㻹���ӽڵ�*/
			{
				HitResult->setPrefix();/*����״̬Ϊǰ׺ƥ��*/
				HitResult->setMatchedDictSegment(resDicSeg);/*������״̬��¼��ǰλ�ýڵ�,��Ϊ�Ѿ�ƥ�䵽�Ľڵ�*/
			}
			return HitResult;
		}
	}
	/*STEP3*/
	return HitResult;	/*�Ҳ�������֣�����״̬�ǲ�ƥ�䣬ֱ�ӷ���*/
}
shared_ptr<Hit> DictSegment::match(const wchar_t *charArray,int begin,int length)
{
	return this->match(charArray,begin,length,NULL);
}
shared_ptr<Hit> DictSegment::match(const wchar_t *charArray)
{
	return this->match(charArray,0,wcslen(charArray), NULL);
}

void DictSegment::fillSegment(const wchar_t *charArray , int begin , int length , int disableFlag)/*�������δʵ�ľ���ʵ�֣�disableFlagΪ1ʱ��䣬Ϊ0ʱ����*/
{
	/*��ȡ�ֵ��еĺ��ֶ���*/
	wchar_t beginChar=charArray[begin];
	wchar_t keyChar=charMap[beginChar];
	if(keyChar==NULL)/*����ֵ���û������֣���ô�Ͱ�����ּ��뵽�ֵ���*/
	{
		charMap[beginChar]=beginChar;
		keyChar=beginChar;
	}	
	shared_ptr<DictSegment> DicSeg=findSegment(keyChar,disableFlag);
	if(DicSeg!=NULL)/*����keychar��Ӧ��segment*/
	{
		if(length>1)/*�ʻ�û����ȫ�����ֵ���*/
			DicSeg->fillSegment(charArray,begin+1,length-1,disableFlag);
		else if(length==1)/*������ǰ�����Ѿ��Ǵ��ص����һ���֣����õ�ǰ�ڵ�״̬ΪdisableFlag*/
			DicSeg->NodeState=disableFlag;
			/*disableFlag=1������ǰΪһ���ʣ�disableFlag=0�������δʵ��е�ǰ�Ĵ�*/
	}
}
void DictSegment::fillSegment(const wchar_t *charArray)
{
	this->fillSegment(charArray,0,wcslen(charArray),1);
}
void DictSegment::disableSegment(const wchar_t *charArray)
{
	this->fillSegment(charArray,0,wcslen(charArray),0);
}

shared_ptr<DictSegment> DictSegment::findSegment(wchar_t keyChar,int create)
{
	shared_ptr<DictSegment> DicSeg=NULL;
	if(storeSize<=ArrayLengthLimit)/*��ǰʹ��������������*/
	{//��ȡ������û���򴴽�һ��
		int position=-1;/*��¼�ҵ����ֵ�����λ��*/
		for(int i=0;i<this->storeSize;i++)
			if(keyChar == childArray[i]->NodeChar)
				position = i;
		if(position>=0)
			DicSeg=childArray[position];
		if (DicSeg==NULL&&create==1)/*����������û���ҵ��ֶ�Ӧ�Ľڵ�*/
		{
/*new*/		DicSeg.reset(new DictSegment(keyChar));
			if (this->storeSize<ArrayLengthLimit)/*����û������ʹ�����鱣��*/
			{
				childArray.push_back(DicSeg);/*�ѽڵ��������*/
				this->storeSize++;
				//sort(childArray.begin(),childArray.end());
			} 
			else/*����������ʹ��map����*/
			{
				for(shared_ptr<DictSegment> tmpdicseg:childArray)/*��������Ԫ��ת�Ƶ�map��*/
					childMap[tmpdicseg->NodeChar]=tmpdicseg;
				childMap[keyChar]=DicSeg;/*�����µĽڵ�*/
				this->storeSize++;
				childArray.clear();/*�ͷ������ռ�*/
			}
		} 
	}
	else
	{
		auto iter=childMap.find(keyChar);
		if(iter!=childMap.end())
			DicSeg=childMap[keyChar];
		else if (create)
		{	
			DicSeg.reset(new DictSegment(keyChar));
			childMap[keyChar]=DicSeg;
			this->storeSize++;
		}
	}
	return DicSeg;
}
bool DictSegment::operator < (const DictSegment &other) const
{
	return this->NodeChar<other.NodeChar;
}