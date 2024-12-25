project('cpp-filesystem', 'cpp')

common_sources = [
	'src/hirzel/fs/Directory.cpp',
	'src/hirzel/fs/Error.cpp',
	'src/hirzel/fs/File.cpp',
	'src/hirzel/fs/FilePosition.cpp',
	'src/hirzel/fs/FileSnippet.cpp',
	'src/hirzel/fs/Path.cpp'
]

test_sources = [
	'test/hirzel/fs/Directory.test.cpp',
	'test/hirzel/fs/File.test.cpp',
	'test/hirzel/fs/FilePosition.test.cpp',
	'test/hirzel/fs/FileSnippet.test.cpp',
	'test/hirzel/fs/Path.test.cpp'
]

fs = import('fs')
include_dirs = include_directories('include', 'src')

library('cpp-filesystem', common_sources, include_directories: include_dirs)

foreach source: test_sources
	source_file_name = fs.name(source).replace('.test.cpp', '')
	unit_test_name = source_file_name
	unit_test_exe_name = source_file_name + '.test'
	unit_test_exe = executable(unit_test_exe_name, source, common_sources, include_directories: include_dirs)
	
	test(unit_test_name, unit_test_exe)
endforeach
