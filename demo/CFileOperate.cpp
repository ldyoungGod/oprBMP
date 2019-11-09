#include "CFileOperate.h"

CFileOperate::CFileOperate()
{

}

bool CFileOperate::writeFileText(const std::string src, const std::string path, OptType type)
{
    std::ofstream outfile;
    switch (type)
    {
    case OptType::AppendEnd:
        outfile.open(path,std::ios::out|std::ios::app);
        break;
    case OptType::Rewrite:
        outfile.open(path,std::ios::out|std::ios::trunc);
        break;
    default:
        return false;
    }
    if(!outfile.is_open())
        return false;
    outfile.write(src.c_str(),src.length());
    outfile.close();
    return true;
}

bool CFileOperate::writeFileBinary(const char *pSrc, size_t len, const char *path, OptType type)
{
    std::ofstream outfile;
    switch (type)
    {
    case OptType::AppendEnd:
        outfile.open(path,std::ios::out|std::ios::app|std::ios::binary);
        break;
    case OptType::Rewrite:
        outfile.open(path,std::ios::out|std::ios::trunc|std::ios::binary);
        break;
    default:
        return false;
    }
    if(!outfile.is_open())
        return false;
    outfile.write(pSrc,len);
    outfile.close();
    return true;
}

std::string CFileOperate::readFileText(const std::string path)
{
    std::string strRet = "";
    size_t len = this->readFileBinary(path.c_str(),nullptr);
    if(len < 1)
    {
        return strRet;
    }
    char* szStr = new char[len+1];
    szStr[len] = 0;
    this->readFileBinary(path.c_str(),szStr);
    strRet = szStr;
    delete[] szStr;
    return strRet;
}

size_t CFileOperate::readFileBinary(const char *path, char *outStr)
{
    std::ifstream infile;
    infile.open(path,std::ios::in);
    if(!infile.is_open())
        return -1;
    infile.seekg(0,std::ios_base::end);
    size_t totalLen = infile.tellg();
    if(outStr != nullptr)
    {
        infile.seekg(0,std::ios_base::beg);
        infile.read(outStr,totalLen);
    }
    infile.close();
    return totalLen;
}
