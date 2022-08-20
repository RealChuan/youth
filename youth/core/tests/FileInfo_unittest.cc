#include <ios>
#include <youth/core/Dir.h>
#include <youth/core/FileInfo.hpp>

#include <iostream>

using namespace youth::core;

void printFileInfo(const FileInfo &fileInfo)
{
    std::cout << "absuleDir: " << fileInfo.absoluteDir().absolutePath() << std::endl;
    std::cout << "absoluteFilePath: " << fileInfo.absoluteFilePath() << std::endl;
    std::cout << "absolutePath: " << fileInfo.absolutePath() << std::endl;
    std::cout << "canonicalFilePath: " << fileInfo.canonicalFilePath() << std::endl;
    std::cout << "canonicalPath: " << fileInfo.canonicalPath() << std::endl;
    std::cout << "symlinkTarget: " << fileInfo.symlinkTarget() << std::endl;

    std::cout << "fileName: " << fileInfo.fileName() << std::endl;
    std::cout << "bundleName: " << fileInfo.bundleName() << std::endl;
    std::cout << "suffix: " << fileInfo.suffix() << std::endl;

    std::cout << "isAbsolute: " << std::boolalpha << fileInfo.isAbsolute() << std::endl;
    std::cout << "isBlockFile: " << std::boolalpha << fileInfo.isBlockFile() << std::endl;
    std::cout << "isBundle: " << std::boolalpha << fileInfo.isBundle() << std::endl;
    std::cout << "isCharacterFile: " << std::boolalpha << fileInfo.isCharacterFile() << std::endl;
    std::cout << "isDir: " << std::boolalpha << fileInfo.isDir() << std::endl;
    std::cout << "isEmptyFileOrDir: " << std::boolalpha << fileInfo.isEmptyFileOrDir() << std::endl;
    std::cout << "isFifo: " << std::boolalpha << fileInfo.isFifo() << std::endl;
    std::cout << "isFile: " << std::boolalpha << fileInfo.isFile() << std::endl;
    std::cout << "isHidden: " << std::boolalpha << fileInfo.isHidden() << std::endl;
    std::cout << "isOther: " << std::boolalpha << fileInfo.isOther() << std::endl;
    std::cout << "isRoot: " << std::boolalpha << fileInfo.isRoot() << std::endl;
    std::cout << "isShortcut: " << std::boolalpha << fileInfo.isShortcut() << std::endl;
    std::cout << "isSocket: " << std::boolalpha << fileInfo.isSocket() << std::endl;
    std::cout << "isSymLink: " << std::boolalpha << fileInfo.isSymLink() << std::endl;

    std::cout << "lastModified: " << fileInfo.lastModified().toStandardFormat() << std::endl;
    std::cout << "size: " << fileInfo.size() << std::endl;
}

void testListAll()
{
    std::cout << "testListAll" << std::endl;
    Dir dir("./../../");
    auto list = dir.entryInfoList({});
    for (const auto &fileInfo : list) {
        printFileInfo(fileInfo);
        std::cout << std::endl;
    }
    std::cout << "testListAll end" << std::endl;
}

void testListFromNameFilters()
{
    std::cout << "testListFromNameFilters" << std::endl;
    Dir dir("./../../");
    auto list = dir.entryInfoList({"*.cpp", "*.h"});
    for (const auto &fileInfo : list) {
        std::cout << fileInfo.fileName() << std::endl;
    }
    std::cout << "testListFromNameFilters end" << std::endl;
}

void testListFromFilterType()
{
    std::cout << "testListFromFilterType" << std::endl;
    Dir dir("./../../");
    auto filterType = static_cast<Dir::FilterType>(
        Dir::FilterType::FilterType_All | Dir::FilterType_NoDotAndDotDot | Dir::FilterType_Hidden);
    auto list = dir.entryInfoList({}, filterType);
    for (const auto &fileInfo : list) {
        std::cout << fileInfo.fileName() << std::endl;
    }
    std::cout << "testListFromFilterType end" << std::endl;
}

void testListFromSortType()
{
    std::cout << "testListFromSortType" << std::endl;
    Dir dir("./../../");
    auto sortType = Dir::SortType_Name;
    auto list = dir.entryInfoList({}, Dir::FilterType_All, sortType);
    for (const auto &fileInfo : list) {
        std::cout << fileInfo.fileName() << std::endl;
    }
    std::cout << "testListFromSortType end" << std::endl;
}

void testListFromAll()
{
    std::cout << "testListFromAll" << std::endl;
    Dir dir("./../../");
    auto filterType = static_cast<Dir::FilterType>(Dir::FilterType_NoDotAndDotDot
                                                   | Dir::FilterType_Hidden);
    auto list = dir.entryInfoList({"*.cpp", "*.h"}, filterType, Dir::SortType_Name);
    for (const auto &fileInfo : list) {
        std::cout << fileInfo.fileName() << std::endl;
    }
    std::cout << "testListFromAll end" << std::endl;
}

int main()
{
    testListAll();
    testListFromNameFilters();
    testListFromFilterType();
    testListFromSortType();
    testListFromAll();
    return 0;
}