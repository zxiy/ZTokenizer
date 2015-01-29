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