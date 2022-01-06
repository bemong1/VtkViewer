#pragma once

#include <vector>
#include <map>

#include "ListManagerDefine.h"
#include "ListManagerMutex.h"

namespace bemong
{
	namespace map
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

			LM_RESULT GetDataByKey(TKey tKey, TData& tData);
			LM_RESULT GetKeyByData(const TData& tData, std::vector<TKey>& tKey);

			LM_RESULT RemoveDataByKey(TKey tKey);

			LM_RESULT GetListSize(int& nSize);
			LM_RESULT CheckValidOfList();

			LM_RESULT ClearList();

		private:
			using MapKey = typename std::map<TKey, TData>;
			MapKey m_mapStElement;

		private:
			ListManagerMutex m_cMutex;
		};
	};
};

#include "ListManagerImplementationAsMap.hpp"
