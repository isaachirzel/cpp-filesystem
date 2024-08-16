#ifndef HIRZEL_FS_ERROR_TYPE_HPP
#define HIRZEL_FS_ERROR_TYPE_HPP

namespace hirzel::fs
{
	enum class ErrorType
	{
		FileOpen,
		FileWrite,
		FileRead,
		FileStat,
		DirectoryOpen
	};
}

#endif
