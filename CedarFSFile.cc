// CedarFSFile.cc

#include "CedarFSFile.h"

CedarFSFile::CedarFSFile(const string &fullPath)
        : _dirName(""),
        _fileName(""),
        _baseName(""),
        _extension("")
{
    breakApart(fullPath) ;
}

CedarFSFile::CedarFSFile(const string &dirName, const string &fileName)
        : _dirName(dirName),
        _fileName(fileName),
        _baseName(""),
        _extension("")
{
    breakExtension() ;
}

CedarFSFile::CedarFSFile(const CedarFSFile &copyFrom)
        : _dirName(copyFrom._dirName),
        _fileName(copyFrom._fileName),
        _baseName(copyFrom._baseName),
        _extension(copyFrom._extension)
{}

CedarFSFile::~CedarFSFile()
{}

string
CedarFSFile::getDirName()
{
    return _dirName ;
}

string
CedarFSFile::getFileName()
{
    return _fileName ;
}

string
CedarFSFile::getBaseName()
{
    return _baseName ;
}

string
CedarFSFile::getExtension()
{
    return _extension ;
}

string
CedarFSFile::getFullPath()
{
    return _dirName + "/" + _fileName ;
}

void
CedarFSFile::breakApart(const string &fullPath)
{
    string::size_type pos = fullPath.rfind("/") ;
    if (pos != string::npos) {
        _dirName = fullPath.substr(0, pos) ;
        _fileName = fullPath.substr(pos + 1, fullPath.length() - pos) ;
    }
    else {
        _dirName = "./" ;
        _fileName = fullPath ;
    }

    breakExtension() ;
}

void
CedarFSFile::breakExtension()
{
    string::size_type pos = _fileName.rfind(".") ;
    if (pos != string::npos) {
        _baseName = _fileName.substr(0, pos) ;
        _extension = _fileName.substr(pos + 1, _fileName.length() - pos) ;
    }
    else {
        _baseName = _fileName ;
    }
}

