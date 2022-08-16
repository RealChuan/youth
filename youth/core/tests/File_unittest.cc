#include <youth/core/File.hpp>

#include <iostream>

using namespace youth::core;

void writeTest()
{
    File file("write.txt");
    if (file.open(File::OpenMode::WriteOnly)) {
        int count = 10;
        while (count--) {
            file.write(std::to_string(count) + " hello world \n");
        }
        file.close();
    } else {
        std::cout << "Failed to open file" << std::endl;
    }
}

void readLineTest()
{
    File file("write.txt");
    if (file.open(File::OpenMode::ReadOnly)) {
        std::string line = file.readLine();
        while (!line.empty()) {
            std::cout << line << std::endl;
            line = file.readLine();
        }
        file.close();
    } else {
        std::cout << "Failed to open file" << std::endl;
    }
}

void readAllTest()
{
    File file("write.txt");
    if (file.open(File::OpenMode::ReadOnly)) {
        std::string all = file.readAll();
        std::cout << all << std::endl;
        file.close();
    } else {
        std::cout << "Failed to open file" << std::endl;
    }
}

void seekTest()
{
    File file("write.txt");
    if (file.open(File::OpenMode::ReadOnly)) {
        file.seek(File::SeekType::End);
        std::cout << file.tell() << std::endl;
        file.seek(File::SeekType::Begin);
        std::cout << file.tell() << std::endl;
        file.seek(File::SeekType::Current, -5);
        std::cout << file.tell() << std::endl;
        file.close();
    } else {
        std::cout << "Failed to open file" << std::endl;
    }
}

void existsTest()
{
    if (File("write.txt").exists()) {
        std::cout << "File exists" << std::endl;
    } else {
        std::cout << "File not exists" << std::endl;
    }

    if (File("not_exists.txt").exists()) {
        std::cout << "File exists" << std::endl;
    } else {
        std::cout << "File not exists" << std::endl;
    }
}

void copyTest()
{
    if (File::copy("write.txt", "copy.txt")) {
        std::cout << "Copy success" << std::endl;
    } else {
        std::cout << "Copy failed" << std::endl;
    }
}

void renameTest()
{
    if (File::rename("copy.txt", "rename.txt")) {
        std::cout << "Rename success" << std::endl;
    } else {
        std::cout << "Rename failed" << std::endl;
    }
}

void removeTest()
{
    if (File::remove("write.txt")) {
        std::cout << "Remove success" << std::endl;
    } else {
        std::cout << "Remove failed" << std::endl;
    }
}

int main()
{
    writeTest();
    readLineTest();
    readAllTest();
    seekTest();
    std::cout << File("write.txt").size() << std::endl;
    existsTest();
    copyTest();
    renameTest();
    removeTest();

    return 0;
}