#include "DictSegment.h"
#pragma warning(disable:4267)
map<wchar_t,wchar_t>DictSegment::charMap;/*储存汉字的字典表初始化*/
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

	/* 参数HitResult和方法返回项相同，便于递归使用和参数传递.
	 * STEP1:在树中查找关键字对应的子树
	 * STEP2:找到对应的子树，判断词的匹配状态，根据结果决定是继续递归，还是返回结果
	 * STEP3:如果没有找到对应的子树， 将HIT设置为不匹配
	 * param charArray 保存需要进行匹配的词段的数组
	 * param begin charArray数组的第begin个元素为实际的起始匹配位置
	 * param length 需要进行匹配的字段长度
	 * param HitResult 字段匹配命中情况，与方法返回项相同 ##点击显示全部内容##
	 * return HitTarget 命中情况 与最后一个参数相同
	 * */
shared_ptr<Hit> DictSegment::match(const wchar_t *charArray,int begin,int length,shared_ptr<Hit> HitResult) 
{
	if (HitResult==NULL)
	{
		HitResult.reset(new Hit());//如果为空则新建
		HitResult->setBegin(begin);
	}
	else
	{
		HitResult->setUnmatch();/*进行匹配前，确保命中状态为未匹配*/
	}
	HitResult->setEnd(begin);/*设置匹配当前处理位置*/
	wchar_t keyChar=charArray[begin];/*要查找的关键字*/
	shared_ptr<DictSegment> resDicSeg;	/*找到的子树*/
	vector<shared_ptr<DictSegment> > &segArray=this->childArray;	/*防止因更新的同步问题产生bug，将实例变量引用为本地变量*/
	map<wchar_t,shared_ptr<DictSegment> > &segMap=this->childMap;
	/*STEP1，见函数说明*/
	if(segArray.size())	/*在数组中查找*/
	{
		int position=-1;/*记录找到的字的数组位置*/
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
	else if(segMap.size())/*在HASHMAP中查找*/
		if(segMap.find(keyChar)!=segMap.end())/*map::find() 如果能找到某元素就返回该元素的迭代器，否则返回指向map末尾的迭代器，由此判断map中是否有这一个元素*/
			resDicSeg=segMap[keyChar];
	/*STEP2，见函数说明*/
	if(resDicSeg!=NULL)
	{
		if(length>1)/*词并没有匹配完时继续往下进行递归搜索*/
		{
			return resDicSeg->match(charArray,begin+1,length-1,HitResult);
		}
		else if(length==1)/*只剩下一个的时候搜索最后一个字*/
		{
			if(resDicSeg->NodeState==1)/*判断从父节点到当前节点是否成词*/
			{
				HitResult->setMatch();/*添加命中状态为完全匹配*/
			}
			if(resDicSeg->hasChildNode())/*如果节点匹配且当前节点还有子节点*/
			{
				HitResult->setPrefix();/*命中状态为前缀匹配*/
				HitResult->setMatchedDictSegment(resDicSeg);/*在命中状态记录当前位置节点,作为已经匹配到的节点*/
			}
			return HitResult;
		}
	}
	/*STEP3*/
	return HitResult;	/*找不到这个字，命中状态是不匹配，直接返回*/
}
shared_ptr<Hit> DictSegment::match(const wchar_t *charArray,int begin,int length)
{
	return this->match(charArray,begin,length,NULL);
}
shared_ptr<Hit> DictSegment::match(const wchar_t *charArray)
{
	return this->match(charArray,0,wcslen(charArray), NULL);
}

void DictSegment::fillSegment(const wchar_t *charArray , int begin , int length , int disableFlag)/*填充或屏蔽词典的具体实现，disableFlag为1时填充，为0时屏蔽*/
{
	/*获取字典中的汉字对象*/
	wchar_t beginChar=charArray[begin];
	wchar_t keyChar=charMap[beginChar];
	if(keyChar==NULL)/*如果字典中没有这个字，那么就把这个字加入到字典中*/
	{
		charMap[beginChar]=beginChar;
		keyChar=beginChar;
	}	
	shared_ptr<DictSegment> DicSeg=findSegment(keyChar,disableFlag);
	if(DicSeg!=NULL)/*处理keychar对应的segment*/
	{
		if(length>1)/*词还没有完全加入字典树*/
			DicSeg->fillSegment(charArray,begin+1,length-1,disableFlag);
		else if(length==1)/*表明当前的字已经是词素的最后一个字，设置当前节点状态为disableFlag*/
			DicSeg->NodeState=disableFlag;
			/*disableFlag=1表明当前为一个词，disableFlag=0表明屏蔽词典中当前的词*/
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
	if(storeSize<=ArrayLengthLimit)/*当前使用容器储存的情况*/
	{//获取容器，没有则创建一个
		int position=-1;/*记录找到的字的数组位置*/
		for(int i=0;i<this->storeSize;i++)
			if(keyChar == childArray[i]->NodeChar)
				position = i;
		if(position>=0)
			DicSeg=childArray[position];
		if (DicSeg==NULL&&create==1)/*遍历容器后没有找到字对应的节点*/
		{
/*new*/		DicSeg.reset(new DictSegment(keyChar));
			if (this->storeSize<ArrayLengthLimit)/*数组没有满，使用数组保存*/
			{
				childArray.push_back(DicSeg);/*把节点插入数组*/
				this->storeSize++;
				//sort(childArray.begin(),childArray.end());
			} 
			else/*数组已满，使用map保存*/
			{
				for(shared_ptr<DictSegment> tmpdicseg:childArray)/*把容器的元素转移到map中*/
					childMap[tmpdicseg->NodeChar]=tmpdicseg;
				childMap[keyChar]=DicSeg;/*放入新的节点*/
				this->storeSize++;
				childArray.clear();/*释放容器空间*/
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