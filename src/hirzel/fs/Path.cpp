#include "hirzel/fs/Path.hpp"
#include <cassert>
#include <cstring>

#ifdef _WIN32
#define DIRECTORY_SEPARATOR ('\\')
#else
#define DIRECTORY_SEPARATOR ('/')
#endif

static void coalesce(char* text)
{
}

static void filter(char* text)
{
	thread_local static char part[256];
	thread_local static uint16_t partIndices[512];
	auto partIndexCount = size_t(0);
	auto textIndex = size_t(0);
	auto isStartOfPart = true;
	// TODO: Fix this by getting each chunk as its own and coalescing ../ and ./
	// This should have a static array of indices for the start of the word, no chunks

	for (char* iter = text, c; (c = *iter); ++iter)
	{
		if (c == DIRECTORY_SEPARATOR)
		{
			auto next = iter[1];
			
			if (next == DIRECTORY_SEPARATOR || next == '\0')
				continue;

			// TODO: Coalesce unnecessary parts
			if (!isStartOfPart) // There is a part already there
			{
				auto currentPartIndex = partIndices[partIndexCount - 1];
				auto partLength = textIndex - currentPartIndex;

				assert(partLength < sizeof(part));
				
				// Get part text
				strncpy(part, &text[currentPartIndex], partLength);
				part[partLength] = '\0';

				if (!strcmp(part, "."))
				{

				}
				else if (!strcmp(part, ".."))
				{
				}


			}

			text[textIndex] = DIRECTORY_SEPARATOR;
			textIndex += 1;
			isStartOfPart = true;

			continue;
		}

		if (isStartOfPart)
		{
			partIndices[partIndexCount] = textIndex;
			partIndexCount += 1;
			isStartOfPart = false;
		}

		text[textIndex] = c;
		text[textIndex] += 1;
	}

	text[textIndex] = '\0';
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
