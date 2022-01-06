#pragma once

#include <vector>

#include "ListManagerDefine.h"
#include "ListManagerMutex.h"

namespace bemong
{
	namespace vector
	{
		using namespace enum_class;
		using namespace mutex;

		template <class TKey, class TData>
		class ListManager
		{
		public:
			ListManager(bool bUseMutex = true);
			~ListManager();

		public:
			LM_RESULT AddData(TKey tKey, const TData& tData);
			LM_RESULT SetData(TKey tKey, const TData& tData);

			LM_RESULT IsExistData(TKey tKey);

			LM_RESULT GetKeyByIndex(int nIndex, TKey& tKey);
			LM_RESULT GetDataByIndex(int nIndex, TData& tData);
			LM_RESULT GetDataByKey(TKey tKey, TData& tData);
			LM_RESULT GetKeyByData(const TData& tData, std::vector<TKey>& tKey);

			LM_RESULT RemoveDataByKey(TKey tKey);

			LM_RESULT GetListSize(int& nSize);
			LM_RESULT CheckValidOfList();
			LM_RESULT CheckValidOfIndex(int nIndex);

			LM_RESULT AscSortByKey();
			LM_RESULT DesSortByKey();
			LM_RESULT ShiftKeyToBegin(TKey& tKey);

			LM_RESULT ClearList();

		private:
			LM_RESULT getIndexBy(TKey tKey, int& nIndex);

			LM_RESULT shiftIndexToBegin(int nIndex);

		private:
			struct StElement
			{
				StElement(TKey _tKey, TData _tData)
					: tKey(_tKey), tData(_tData) {}
				TKey tKey;
				TData tData;
			};
			std::vector<StElement> m_vecStElement;

		private:
			ListManagerMutex m_cMutex;
		};
	};
};

#include "ListManagerImplementation.hpp"
