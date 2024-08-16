#ifndef HIRZEL_FS_FILE_TYPE_HPP
#define HIRZEL_FS_FILE_TYPE_HPP

namespace hirzel::fs
{
	enum class FileType: unsigned char
	{
		Regular,
		Source,
		Json
	};
}

#endif
