#include "ListManagerMutex.h"

namespace bemong
{
	namespace mutex
	{
		ListManagerMutex::ListManagerMutex(bool bUseMutex)
		{
			SetUseMutex(bUseMutex);
		}

		LM_RESULT ListManagerMutex::SetUseMutex(bool bUseMutex)
		{
			m_bUseMutex = bUseMutex;
			return LM_RESULT::OK;
		}

		LM_RESULT ListManagerMutex::MutexLock()
		{
			if (m_bUseMutex == true)
			{
				m_mDataManagerMutex.lock();
				return LM_RESULT::OK;
			}
			else
			{
				return LM_RESULT::ERROR_MUTEX_IS_DISABLED;
			}
		}

		LM_RESULT ListManagerMutex::MutexUnlock()
		{
			if (m_bUseMutex == true)
			{
				m_mDataManagerMutex.unlock();
				return LM_RESULT::OK;
			}
			else
			{
				return LM_RESULT::ERROR_MUTEX_IS_DISABLED;
			}
		}
	}
}

