#include "hirzel/fs/FileSnippet.hpp"
#include <cassert>

namespace hirzel::fs
{
	FileSnippet::FileSnippet(const FilePosition& position, uint32_t length):
		_position(position),
		_length(length)
	{}

	FileSnippet FileSnippet::sub(size_t offset, size_t length) const
	{
		assert(offset + length <= _position.index());
		return FileSnippet(_position + offset, length);
	}

	FileSnippet& FileSnippet::operator=(const FileSnippet& other)
	{
		_position = other._position;
		_length = other._length;

		return *this;
	}

	FileSnippet FileSnippet::operator+(const FileSnippet& other) const
	{
		assert(&_position.file() == &other._position.file());

		const auto& firstSnippet = _position <= other._position
			? *this
			: other;
		const auto position = firstSnippet._position;
		const auto thisEnd = _position.index() + _length;
		const auto otherEnd = other._position.index() + other._length;
		const auto end = std::max(thisEnd, otherEnd);
		const auto length = end - position.index();
		const auto snippet = FileSnippet(position, length);

		return snippet;
	}

	FileSnippet& FileSnippet::operator+=(const FileSnippet& other)
	{
		*this = *this + other;

		return *this;
	}
	
	bool FileSnippet::operator==(const FileSnippet& other) const
	{
		return _position == other._position && _length == other._length;
	}

	bool FileSnippet::operator!=(const FileSnippet& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& out, const FileSnippet& snippet)
	{
		const auto *start = snippet.begin();
		
		out.write(start, snippet._length);

		return out;
	}
}
