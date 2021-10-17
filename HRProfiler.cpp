#include "stdafx.h"
#include "HRProfiler.h"

namespace hr {
	__int64 GetMicroSecond()
	{
		LARGE_INTEGER frequency;
		LARGE_INTEGER now;

		if (!QueryPerformanceFrequency(&frequency))
			return (__int64)GetTickCount();

		if (!QueryPerformanceCounter(&now))
			return (__int64)GetTickCount();

		return ((now.QuadPart) / (frequency.QuadPart / 1000000));
	}

	void HRProfiler::InitProfileObjects()
	{
		for (int i = 0; i < eProfileObjectMax; i++) {
			_profile[i].ThreadID = eINVALID_THREAD_ID;
		}
	}
	HRProfiler::ProfileObject * HRProfiler::FindProfileObject(DWORD threadID, WCHAR * name)
	{
		for (int i = 0; i < eProfileObjectMax; i++) {
			if (_profile[i].ThreadID != threadID)
				continue;

			if (wcscmp(_profile[i].Name, name) != 0)
				continue;

			return &_profile[i];
		}
		return NULL;
	}
	HRProfiler::ProfileObject * HRProfiler::GetEmptyProfileObject()
	{
		for (int i = 0; i < eProfileObjectMax; i++) {
			if (_profile[i].ThreadID != eINVALID_THREAD_ID)
				continue;

			_profile[i].Average = eINVALID_DATA;
			_profile[i].Min = eINVALID_DATA;
			_profile[i].Max = eINVALID_DATA;
			_profile[i].Call = 0;
			_profile[i].BeginTime = eINVALID_DATA;

			return &_profile[i];
		}
		return NULL;
	}
	HRProfiler::HRProfiler()
	{
		InitProfileObjects();
	}
	HRProfiler::~HRProfiler()
	{
		WCHAR fileName[50];
		time_t t = time(0);   // get time now
		tm* now = localtime(&t);

		wsprintfW(fileName, L"Profile_%4d%02d%02d.txt", now->tm_year + 1900, now->tm_mon, now->tm_mday);
		wprintf(fileName);
		FILE* pFile = _wfopen(fileName, L"w");
		if (pFile == NULL) return;

		fwprintf(pFile, L" %9s | %15s | %12s | %12s | %12s | %9s \n",
			L"Thread ID", L"Name", L"Average", L"Min", L"Max", L"Call");
		fwprintf(pFile, L"-----------+-----------------+--------------+--------------+--------------+-----------\n");

		for (int i = 0; i < eProfileObjectMax; i++) {
			if (_profile[i].ThreadID == eINVALID_THREAD_ID)
				continue;

			fwprintf(pFile, L" %9d | %15s | %10.4fus | %10.4fus | %10.4fus | %9d \n",
				_profile[i].ThreadID, _profile[i].Name, _profile[i].Average, _profile[i].Min, _profile[i].Max, _profile[i].Call);
		}
		fwprintf(pFile, L"-----------+-----------------+--------------+--------------+--------------+-----------\n");

		fclose(pFile);
	}
	void HRProfiler::ProfileBegin(WCHAR * TagName)
	{
		DWORD threadID = GetCurrentThreadId();
		ProfileObject* profileObj = FindProfileObject(threadID, TagName);
		if (profileObj == NULL) {
			profileObj = GetEmptyProfileObject();
			if (profileObj == NULL) return;

			profileObj->ThreadID = threadID;
			wcscpy(profileObj->Name, TagName);
		}

		profileObj->BeginTime = GetMicroSecond();
	}

	void HRProfiler::ProfileEnd(WCHAR * TagName)
	{
		DWORD threadID = GetCurrentThreadId();
		ProfileObject* profileObj = FindProfileObject(threadID, TagName);
		if (profileObj == NULL)
			return;
		if (profileObj->BeginTime == eINVALID_DATA)
			return;
		
		__int64 SpendTime = GetMicroSecond() - profileObj->BeginTime;

		profileObj->Average = profileObj->Average == eINVALID_DATA ? SpendTime :
			profileObj->Average + (SpendTime - profileObj->Average) / (profileObj->Call + 1);
		profileObj->Min = profileObj->Min == eINVALID_DATA ? SpendTime : min(profileObj->Min, SpendTime);
		profileObj->Max = profileObj->Max == eINVALID_DATA ? SpendTime : max(profileObj->Max, SpendTime);
		profileObj->Call++;

		profileObj->BeginTime = eINVALID_DATA;
	}
}