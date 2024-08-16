#include "hirzel/fs/Directory.hpp"
#include "hirzel/fs/ErrorCallback.hpp"

#include <cstring>
#include <filesystem>

namespace hirzel::fs
{
	extern ErrorCallback _errorCallback;

	Directory::Directory(std::vector<File>&& files, std::vector<Directory>&& subdirectories, Path&& path, uint16_t projectOffset):
        _files(std::move(files)),
        _subdirectories(std::move(subdirectories)),
		_path(std::move(path)),
		_projectOffset(projectOffset)
	{}

	Directory Directory::readSubdirectory(const Path& path, uint16_t projectOffset)
	{
		auto iterator = std::filesystem::directory_iterator(path.text());
		auto files = std::vector<File>();
		auto subdirectories = std::vector<Directory>();

		for (const auto& entry : iterator)
		{
			const auto& entryPath = Path(entry.path().string());

			if (entry.is_regular_file())
			{
				auto file = File::read(entryPath, projectOffset);

				files.emplace_back(std::move(file));

			}
			else if (entry.is_directory())
			{
				auto subdirectory = readSubdirectory(std::move(entryPath), projectOffset);
				
				subdirectories.emplace_back(std::move(subdirectory));
			}
		}

		auto directory = Directory(std::move(files), std::move(subdirectories), Path(path), projectOffset);

		return directory;
	}

	static uint16_t getNameOffset(const std::string& absolutePath)
	{
		// TODO: Fix safety where we're assuming the string doesn't end with a '/'
		for (auto i = (uint16_t)absolutePath.size(); i-- > 0;)
		{
			if (absolutePath[i] == '/')
				return i + 1;
		}

		return 0;
	}

	Directory Directory::read(const Path& absolutePath, uint16_t projectOffset)
	{
		// if (!std::filesystem::exists(absolutePath.text()))
		// 	log::directoryOpenError(absolutePath.text().c_str());

		auto directory = readSubdirectory(absolutePath, projectOffset);

		return directory;
	}	

	Directory Directory::createTestSrcDirectory(const Path& projectPath)
	{
		auto absolutePath = projectPath / "src";
		auto directory = Directory({}, {}, std::move(absolutePath), 0);
		
		return directory;
	}

	void Directory::addFile(File&& file)
	{
		_files.emplace_back(std::move(file));
	}
}
