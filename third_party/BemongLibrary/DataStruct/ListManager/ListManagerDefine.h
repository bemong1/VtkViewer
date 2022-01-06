#pragma once

#include <string>
#include <ostream>
//using namespace std;

namespace bemong
{
	namespace custom_key
	{
		const bool		USING_OR_KEY = true;
		const int		NULL_INT_KEY = INT_MIN + 1;
		const std::string	NULL_STR_KEY = "NULL_KEY";

		class StPairKey
		{
		public:
			StPairKey(int _nKey, std::string _strKey)
				: nKey(_nKey), strKey(_strKey) {}

			bool operator < (const StPairKey& stPairKey) const
			{
				if (USING_OR_KEY)
				{
					if (stPairKey.strKey.compare(NULL_STR_KEY) == 0 ||
						strKey.compare(NULL_STR_KEY) == 0)
					{
						if (nKey < stPairKey.nKey)
							return true;
						else
							return false;
					}

					else if (stPairKey.nKey == NULL_INT_KEY ||
						nKey == NULL_INT_KEY)
					{
						if (strKey.compare(stPairKey.strKey) < 0)
							return true;
						else
							return false;
					}

					else
					{
						if (nKey < stPairKey.nKey)
							return true;
						else if (nKey == stPairKey.nKey)
							if (strKey.compare(stPairKey.strKey) < 0)
								return true;
						return false;
					}
				}
				else
				{
					if (nKey < stPairKey.nKey)
						return true;
					else if (nKey == stPairKey.nKey)
						if (strKey.compare(stPairKey.strKey) < 0)
							return true;
					return false;
				}
			}
			
			bool operator == (const StPairKey& stPairKey) const
			{
				if (USING_OR_KEY)
				{
					if (stPairKey.strKey.compare(NULL_STR_KEY) == 0 ||
						strKey.compare(NULL_STR_KEY) == 0)
					{
						if (nKey == stPairKey.nKey)
							return true;
						else
							return false;
					}

					else if (stPairKey.nKey == NULL_INT_KEY ||
						nKey == NULL_INT_KEY)
					{
						if (strKey.compare(stPairKey.strKey) == 0)
							return true;
						else
							return false;
					}

					else
					{
						if (nKey == stPairKey.nKey &&
							(strKey.compare(stPairKey.strKey) == 0))
							return true;
						else
							return false;
					}
				}
				else
				{
					if (nKey == stPairKey.nKey &&
						(strKey.compare(stPairKey.strKey) == 0))
						return true;
					else
						return false;
				}
			}

			friend std::ostream& operator << (std::ostream& out, const StPairKey& stPairKey)
			{
				out << "[nKey:" << stPairKey.nKey << "], [strKey:" << stPairKey.strKey << "]";
				return out;
			}

		public:
			int nKey;
			std::string strKey;
		};
	};
};

namespace bemong
{
	namespace enum_class
	{
		const int nMinCount = -0x06;
		enum class LM_RESULT
		{
			ERROR_DATA_IS_NOT_EXIST = nMinCount,
			ERROR_DATA_ALREADY_EXIST,
			ERROR_LIST_IS_EMPTY,
			ERROR_LIST_IS_NOT_VALID,
			ERROR_INDEX_IS_NEGATIVE,

			ERROR_MUTEX_IS_DISABLED,

			OK,
		};
	};
};