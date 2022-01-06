#pragma once

#include <string>
#include <memory>
#include <locale>

namespace bemong
{
	class String
	{
	public:
		template<typename ... Types>
		std::string StringFormat(
			const std::string& format, 
			Types ... args);
	};
};

#include "UtilString.hpp"