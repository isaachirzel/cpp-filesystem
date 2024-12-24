#ifndef HIRZEL_FS_ERROR_HPP
#define HIRZEL_FS_ERROR_HPP

#include "hirzel/fs/ErrorCallback.hpp"
namespace hirzel::fs
{
	void onError(ErrorCallback&& callback);
	void pushError(ErrorType errorType, const char* path);
}

#endif
