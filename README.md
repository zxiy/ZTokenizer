# ZTokenizer

ZTokenizer是一个简单的，基于字典匹配的中文分词器。

分词有两种模式，智能模式和搜索引擎模式，在使用时传参数来确定。

智能模式会将句子直接切分，搜索引擎模式会把所有的单词匹配结果都输出出来。

分词器的内部编码方式为Unicode。

### 使用范例

```
#include <iostream>
#include <string>
#include "ZTokenizer.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "chs");
	wstring str = L"永和服装饰品有限公司";

	ZTokenizer zTokenizer(str, true);

	while (true)
	{
		auto result = zTokenizer.next();
		if (result.length() > 0)
			wcout << result << endl;
		else
			break;
	}
	zTokenizer.reset(str, false);
	wcout << "---------" << endl;
	while (true)
	{
		auto result = zTokenizer.next();
		if (result.length() > 0)
			wcout << result << endl;
		else
			break;
	}
	return 0;
}
```
### 词典配置

路径下有3个词典，分别是主词典`main2012.dic`，量词词典`quantifier.dic`和屏蔽词词典`stopword.dic`，可以在里面随便添加新词。

### 其他

设计思路参考了[IK分词器](https://code.google.com/p/ik-analyzer/)。
