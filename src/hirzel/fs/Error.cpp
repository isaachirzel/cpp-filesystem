#include "hirzel/fs/Error.hpp"
#include "hirzel/fs/ErrorCallback.hpp"

namespace hirzel::fs
{
	ErrorCallback _callback;

	void onError(ErrorCallback&& callback)
	{
		_callback = std::move(callback);
	}

	void pushError(ErrorType errorType, const char* path)
	{
		if (_callback)
			_callback(errorType, path);
	}
}
