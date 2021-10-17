#pragma once

namespace hr {
#define PROFILE_CHECK
#ifdef PROFILE_CHECK
#define HR_PRO_BEGIN(TagName)	HRProfiler::Call()->ProfileBegin(const_cast<WCHAR*>(TagName))
#define HR_PRO_END(TagName)		HRProfiler::Call()->ProfileEnd(const_cast<WCHAR*>(TagName))
#else
#define HR_PRO_BEGIN(x)	
#define HR_PRO_END(x)		
#endif

	class HRProfiler {
	private:
		struct ProfileObject {
			DWORD ThreadID;
			WCHAR Name[20];
			double Average;
			double Min;
			double Max;
			double Call;

			__int64 BeginTime;
		};
		
		enum HRProfilerNum {
			eProfileObjectMax = 100,
			eINVALID_THREAD_ID = -1,
			eINVALID_DATA = -1
		};

		ProfileObject _profile[eProfileObjectMax];

		static HRProfiler instance;

		void InitProfileObjects();
		ProfileObject* FindProfileObject(DWORD threadID, WCHAR* name);
		ProfileObject* GetEmptyProfileObject();
	public:
		HRProfiler();
		virtual ~HRProfiler();

		static HRProfiler* Call() { return &instance; }

		void ProfileBegin(WCHAR* TagName);
		void ProfileEnd(WCHAR* TagName);
	};
}
