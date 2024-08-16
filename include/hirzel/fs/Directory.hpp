#ifndef HIRZEL_FS_DIRECTORY_HPP
#define HIRZEL_FS_DIRECTORY_HPP

#include "hirzel/fs/File.hpp"

#include <vector>
#include <cstdint>

namespace hirzel::fs
{
    class Directory
    {
        std::vector<File> _files;
        std::vector<Directory> _subdirectories;
        Path _path;
        uint16_t _projectOffset;

    private:

        Directory(std::vector<File>&& files, std::vector<Directory>&& subdirectories, Path&& path, uint16_t projectOffset);

        static Directory readSubdirectory(const Path& path, uint16_t projectOffset);

    public:

        Directory(Directory&&) = default;
        Directory(const Directory&) = delete;

        static Directory read(const Path& path, uint16_t projectOffset = 0);
        static Directory createTestSrcDirectory(const Path& projectPath);

        void addFile(File&& file);

        std::string getRelativePath() const;
        std::string getName() const;

        const auto& path() const { return _path; }
        const auto& files() const { return _files; }
        const auto& subdirectories() const { return _subdirectories; }
    };
}

#endif
