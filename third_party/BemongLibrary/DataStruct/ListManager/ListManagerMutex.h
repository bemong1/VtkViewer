#pragma once

#include <mutex>
#include "ListManagerDefine.h"

namespace bemong
{
	namespace mutex
	{
		using namespace enum_class;
		class ListManagerMutex
		{
		public:
			ListManagerMutex(bool bUseMutex = true);
			//~ListManagerMutex();

		public:
			LM_RESULT SetUseMutex(bool bUseMutex);
			LM_RESULT MutexLock();
			LM_RESULT MutexUnlock();

		private:
			bool m_bUseMutex;
			std::mutex m_mDataManagerMutex;
		};
	}
}

