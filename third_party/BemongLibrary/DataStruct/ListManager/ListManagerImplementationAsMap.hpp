#include "ListManagerAsMap.h"

namespace bemong
{
	namespace map
	{
		template <class TKey, class TData>
		ListManager<TKey, TData>::ListManager(bool bUseMutex)
		{
			m_cMutex.SetUseMutex(bUseMutex);
		}

		template <class TKey, class TData>
		ListManager<TKey, TData>::~ListManager()
		{

		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::AddData(TKey tKey, const TData& tData)
		{
			LM_RESULT nRet = SetData(tKey, tData);

			if (nRet == LM_RESULT::OK)
			{
				return LM_RESULT::ERROR_DATA_ALREADY_EXIST;
			}
			else
			{
				m_cMutex.MutexLock();
				m_mapStElement.insert(pair<TKey, TData>(tKey, tData));
				m_cMutex.MutexUnlock();

				return LM_RESULT::OK;
			}
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::SetData(TKey tKey, const TData& tData)
		{
			LM_RESULT nRet;
			nRet = IsExistData(tKey);
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			m_mapStElement[tKey] = tData;
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::IsExistData(TKey tKey)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			typename MapKey::iterator mapExistCheck = m_mapStElement.find(tKey);
			if (m_mapStElement.end() == mapExistCheck)
				nRet = LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
			else
				nRet = LM_RESULT::OK;
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetDataByKey(TKey tKey, TData& tData)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			typename MapKey::iterator mapExistCheck = m_mapStElement.find(tKey);
			if (m_mapStElement.end() == mapExistCheck)
			{
				nRet = LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
			}
			else
			{
				tData = mapExistCheck->second;
				nRet = LM_RESULT::OK;
			}
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetKeyByData(const TData& tData, std::vector<TKey>& tKey)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			nRet = LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
			tKey.clear();

			m_cMutex.MutexLock();
			typename MapKey::iterator it;
			for (it = m_mapStElement.begin(); it != m_mapStElement.end(); it++)
			{
				if (tData == it->second)
				{
					tKey.push_back(it->first);
					nRet = LM_RESULT::OK;
				}
			}
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::RemoveDataByKey(TKey tKey)
		{
			LM_RESULT lmRet = CheckValidOfList();
			if (lmRet != LM_RESULT::OK) return lmRet;

			lmRet = LM_RESULT::ERROR_DATA_IS_NOT_EXIST;

			m_cMutex.MutexLock();
			if(m_mapStElement.erase(tKey) == 1)
				lmRet = LM_RESULT::OK;
			m_cMutex.MutexUnlock();

			return lmRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetListSize(int& nSize)
		{
			m_cMutex.MutexLock();
			nSize = static_cast<int>(m_mapStElement.size());
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::ClearList()
		{
			m_cMutex.MutexLock();
			m_mapStElement.clear();
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::CheckValidOfList()
		{
			int nSize = 0;
			GetListSize(nSize);
			if (nSize == 0)
				return LM_RESULT::ERROR_LIST_IS_EMPTY;
			if (nSize < 0)
				return LM_RESULT::ERROR_LIST_IS_NOT_VALID;

			return LM_RESULT::OK;
		}
	}
}