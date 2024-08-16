#ifndef HIRZEL_FS_FILE_SNIPPET_HPP
#define HIRZEL_FS_FILE_SNIPPET_HPP

#include "hirzel/fs/FilePosition.hpp"

#include <ostream>

namespace hirzel::fs
{
	class FileSnippet
	{
		FilePosition _position;
		size_t _length;

	public:

		FileSnippet(const FilePosition& position, uint32_t length);
		FileSnippet(FileSnippet&&) = default;
		FileSnippet(const FileSnippet&) = default;
		FileSnippet& operator=(const FileSnippet& other);

		const char *ptr() const { return &_position.file()[_position.index()]; }
		const char *begin() const { return &_position.file()[_position.index()]; }
		const char *end() const { return &_position.file()[_position.index() + _length]; }

		const auto& file() const { return _position.file(); }
		const auto& position() const { return _position; }
		const auto& length() const { return _length; }
		
		FileSnippet sub(size_t offset, size_t length) const;

		FileSnippet operator+(const FileSnippet& other) const;

		FileSnippet& operator+=(const FileSnippet& other);

		bool operator==(const FileSnippet& other) const;
		bool operator!=(const FileSnippet& other) const;


		const auto& operator[](size_t index) const { return _position.file()[_position.index() + index]; }
		friend std::ostream& operator<<(std::ostream& out, const FileSnippet& snippet);
	};
}

#endif
