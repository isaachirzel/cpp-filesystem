#ifndef HIRZEL_FS_FILE_POSITION_HPP
#define HIRZEL_FS_FILE_POSITION_HPP

#include "hirzel/fs/File.hpp"

#include <cstdint>

namespace hirzel::fs
{
	class FilePosition
	{
		const fs::File* _file;
		uint32_t _index;
		uint16_t _line;
		uint16_t _column;

	public:

		FilePosition(const fs::File& file);
		FilePosition(const fs::File& file, uint32_t index, uint16_t line, uint16_t column);
		FilePosition(FilePosition&&) = default;
		FilePosition(const FilePosition&) = default;
		FilePosition& operator=(FilePosition&&) = default;
		FilePosition& operator=(const FilePosition&) = default;

		void seekEndOfLine();
		void seekEndOfBlockComment();
		void seekNext();

		char operator*() const;
		char operator[](size_t index) const;
		FilePosition operator+(uint32_t offset) const;
		bool operator==(const FilePosition& other) const;
		bool operator!=(const FilePosition& other) const;
		bool operator<(const FilePosition& other) const;
		bool operator<=(const FilePosition& other) const;
		bool operator>(const FilePosition& other) const;
		bool operator>=(const FilePosition& other) const;

		const auto& file() const { return *_file; }
		const auto& index() const { return _index; }
		const auto& line() const { return _line; }
		const auto& column() const { return _column; }

		friend std::ostream& operator<<(std::ostream& out, const FilePosition& position);
	};
}

#endif
