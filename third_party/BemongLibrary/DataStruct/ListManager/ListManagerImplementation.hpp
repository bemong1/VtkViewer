#include "ListManager.h"

namespace bemong
{
	namespace vector
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
				StElement stElement(tKey, tData);

				m_cMutex.MutexLock();
				m_vecStElement.push_back(stElement);
				m_cMutex.MutexUnlock();

				return LM_RESULT::OK;
			}
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::SetData(TKey tKey, const TData& tData)
		{
			LM_RESULT nRet;
			int nIndex = 0;
			nRet = getIndexBy(tKey, nIndex);

			if (nRet != LM_RESULT::OK) return nRet;
			
			StElement stElement(tKey, tData);

			m_cMutex.MutexLock();
			m_vecStElement[nIndex] = stElement;
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::IsExistData(TKey tKey)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			for (int i = 0; i < m_vecStElement.size(); i++)
			{
				if (m_vecStElement[i].tKey == tKey)
				{
					m_cMutex.MutexUnlock();
					return LM_RESULT::OK;
				}
			}
			m_cMutex.MutexUnlock();

			return LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetKeyByIndex(int nIndex, TKey& tKey)
		{
			LM_RESULT nRet = CheckValidOfIndex(nIndex);
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			tKey = m_vecStElement[nIndex].tKey;
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetDataByIndex(int nIndex, TData& tData)
		{
			LM_RESULT nRet = CheckValidOfIndex(nIndex);
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			tData = m_vecStElement[nIndex].tData;
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetDataByKey(TKey tKey, TData& tData)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			for (int i = 0; i < m_vecStElement.size(); i++)
			{
				if (m_vecStElement[i].tKey == tKey)
				{
					tData = m_vecStElement[i].tData;
					m_cMutex.MutexUnlock();
					return LM_RESULT::OK;
				}
			}
			m_cMutex.MutexUnlock();

			return LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
		}
		
		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetKeyByData(const TData& tData, std::vector<TKey>& tKey)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			nRet = LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
			tKey.clear();

			m_cMutex.MutexLock();
			for (int i = 0; i < m_vecStElement.size(); i++)
			{
				if (tData == m_vecStElement[i].tData)
				{
					tKey.push_back(m_vecStElement[i].tKey);
					nRet = LM_RESULT::OK;
				}
			}
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::RemoveDataByKey(TKey tKey)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			nRet = LM_RESULT::ERROR_DATA_IS_NOT_EXIST;

			m_cMutex.MutexLock();
			for (int i = 0; i < m_vecStElement.size(); i++)
			{
				if (tKey == m_vecStElement[i].tKey)
				{
					m_vecStElement.erase(m_vecStElement.begin() + i);
					nRet = LM_RESULT::OK;
				}
			}
			m_cMutex.MutexUnlock();

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::GetListSize(int& nSize)
		{
			m_cMutex.MutexLock();
			nSize = static_cast<int>(m_vecStElement.size());
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::AscSortByKey()
		{
			m_cMutex.MutexLock();
			sort(m_vecStElement.begin(), m_vecStElement.end(),
				[](const auto& stElement1, const auto& stElement2)
			{
				return stElement1.tKey < stElement2.tKey;
			}
			);
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::DesSortByKey()
		{
			m_cMutex.MutexLock();
			sort(m_vecStElement.begin(), m_vecStElement.end(),
				[](const auto& stElement1, const auto& stElement2)
			{
				return stElement1.tKey > stElement2.tKey;
			}
			);
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::ShiftKeyToBegin(TKey& tKey)
		{
			int nIndex = 0;
			LM_RESULT nRet = getIndexBy(tKey, nIndex);
			if (nRet != LM_RESULT::OK) return nRet;

			nRet = shiftIndexToBegin(nIndex);
			if (nRet != LM_RESULT::OK) return nRet;

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::ClearList()
		{
			m_cMutex.MutexLock();
			m_vecStElement.clear();
			m_cMutex.MutexUnlock();

			return LM_RESULT::OK;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::getIndexBy(TKey tKey, int& nIndex)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			m_cMutex.MutexLock();
			for (int i = 0; i < m_vecStElement.size(); i++)
			{
				if (m_vecStElement[i].tKey == tKey)
				{
					nIndex = i;
					m_cMutex.MutexUnlock();
					return LM_RESULT::OK;
				}
			}
			m_cMutex.MutexUnlock();

			return LM_RESULT::ERROR_DATA_IS_NOT_EXIST;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::shiftIndexToBegin(int nIndex)
		{
			LM_RESULT nRet = CheckValidOfIndex(nIndex);
			if (nRet != LM_RESULT::OK) return nRet;

			for (int i = 0; i < nIndex; i++)
			{
				m_vecStElement.push_back(m_vecStElement[0]);
				m_vecStElement.erase(m_vecStElement.begin());
			}

			return nRet;
		}

		template <class TKey, class TData>
		LM_RESULT ListManager<TKey, TData>::CheckValidOfIndex(int nIndex)
		{
			LM_RESULT nRet = CheckValidOfList();
			if (nRet != LM_RESULT::OK) return nRet;

			int nSize = 0;
			GetListSize(nSize);

			if (nIndex < 0)
				return LM_RESULT::ERROR_INDEX_IS_NEGATIVE;
			if (nIndex >= nSize)
				return LM_RESULT::ERROR_DATA_IS_NOT_EXIST;

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