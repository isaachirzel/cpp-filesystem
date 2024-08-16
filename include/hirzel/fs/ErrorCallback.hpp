#ifndef HIRZEL_FS_ERROR_CALLBACK_HPP
#define HIRZEL_FS_ERROR_CALLBACK_HPP

#include "ErrorType.hpp"
#include <functional>

namespace hirzel::fs
{
	using ErrorCallback = std::function<void(ErrorType, const char* path)>;
}

#endif
