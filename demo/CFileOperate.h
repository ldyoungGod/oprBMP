#ifndef CFILEOPERATE_H
#define CFILEOPERATE_H
#include <iostream>
#include <fstream>
#include <dirent.h>
typedef unsigned char byte;

enum OptType
{
    AppendEnd,
    Rewrite
};
class CFileOperate
{
public:
    CFileOperate();

    bool writeFileText(const std::string src, const std::string path, OptType type = OptType::Rewrite);
    bool writeFileBinary(const char* pSrc, size_t len, const char* path, OptType type = OptType::Rewrite);

    std::string readFileText(const std::string path);
    size_t readFileBinary(const char* path, char* outStr);//outStr为nullptr时，该函数获取文件大小
};

//std::ios_base::in:  打开文件进行读操作，即读取文件中的数据
//std::ios_base::out: 打开文件进行写操作，即写入数据到文件
//std::ios_base::app: 每次进行写入操作的时候都会重新定位到文件的末尾(only write)
//std::ios_base::ate: 打开文件之后立即定位到文件末尾(read and write)
//std::ios_base::trunc: 打开文件，若文件已存在那么，清空文件内容
//std::ios_base::binary: 以二进制的方式对打开的文件进行读写
#endif // CFILEOPERATE_H
