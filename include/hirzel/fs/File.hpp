#ifndef HIRZEL_FS_FILE_HPP
#define HIRZEL_FS_FILE_HPP

#include "ErrorCallback.hpp"
#include "hirzel/fs/Path.hpp"
#include "hirzel/fs/FileType.hpp"

#include <string>
#include <cstdint>

namespace hirzel::fs
{
	class File
	{
		std::string _text;
		Path _path;
        uint16_t _projectOffset;
		FileType _type;

	private:

		File(std::string&& text, const Path& path, uint16_t projectOffset, FileType fileType);

	public:

		File(File&&) = default;
		File(const File&) = delete;

		static File read(const Path& filepath, uint16_t relativePathOffset = 0);
		static File createTestMainSrcFile(const Path& directoryPath, uint16_t relativePathOffset, std::string&& text);
		static void setErrorCallback(ErrorCallback&& errorCallback);

		void write();
		std::string getRelativePath() const;
		
		const char& operator[](size_t index) const;

		const auto& text() const { return _text; }
		const auto& path() const { return _path; }
		const auto& type() const { return _type; }
		auto length() const { return _text.length(); }
	};
}

#endif
