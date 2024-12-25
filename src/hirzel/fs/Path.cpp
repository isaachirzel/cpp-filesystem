#include "hirzel/fs/Path.hpp"
#include <cstring>

#ifdef _WIN32
#define DIRECTORY_SEPARATOR ('\\')
#else
#define DIRECTORY_SEPARATOR ('/')
#endif

static void filter(char* text)
{
	// TODO: Fix this by getting each chunk as its own and coalescing ../ and ./
	auto o = size_t(0);

	for (char* i = text, c; (c = *i); ++i)
	{
		if (c == '/')
		{
			auto next = i[1];
			
			if (next == '/' || next == '\0')
				continue;
		}

		text[o] = c;
		o += 1;
	}

	text[o] = '\0';
}

namespace hirzel::fs
{
	Path::Path():
		_text("."),
		_nameOffset(0),
		_extensionOffset(1)
	{}

	Path::Path(const char *text):
		_text(text),
		_nameOffset(0),
		_extensionOffset(0)
	{
		filter(_text.data());
	}

	Path::Path(const std::string& text):
		Path(text.c_str())
	{}

	bool Path::hasExtension(const char *ext) const
	{
		const auto* currentExt = extension();
		const auto hasExt = !std::strcmp(currentExt, ext);

		return hasExt;
	}

	Path Path::operator/(const char *other) const
	{
		auto path = *this;

		path /= other;

		return path;
	}

	Path& Path::operator/=(const char *other)
	{
		_text += DIRECTORY_SEPARATOR;
		_text += other;

		filter(_text.data());

		return *this;
	}

	std::ostream& operator<<(std::ostream& out, const Path& path)
	{
		out << path._text;

		return out;
	}
}
