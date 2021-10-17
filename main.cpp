// 프로파일러.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

using namespace hr;
HRProfiler HRProfiler::instance;

int main()
{
	srand(time(NULL));
	for (int i = 0; i < 1000;i++) {
		HR_PRO_BEGIN(L"function 1");
		while (true) {
			if (rand() % 100 < 1)
				break;
		}
		HR_PRO_END(L"function 1");
	}
	for (int i = 0; i < 1000; i++) {
		HR_PRO_BEGIN(L"function 2");
		while (true) {
			if (rand() % 100 < 1)
				break;
		}
		HR_PRO_END(L"function 2");
	}
	for (int i = 0; i < 1000; i++) {
		HR_PRO_BEGIN(L"function 3");
		while (true) {
			if (rand() % 100 < 1)
				break;
		}
		HR_PRO_END(L"function 3");
	}
    return 0;
}

