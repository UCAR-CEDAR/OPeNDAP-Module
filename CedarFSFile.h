// CedarFSFile.h

#ifndef I_CedarFSFile_h
#define I_CedarFSFile_h

#include <string>

using std::string ;

#include "CedarFSFile.h"

class CedarFSFile
{
private:
    string    _dirName ;
    string    _fileName ;
    string    _baseName ;
    string    _extension ;

    void    breakApart(const string &fullPath) ;
    void    breakExtension() ;
public:
    CedarFSFile(const string &fullPath) ;
    CedarFSFile(const string &dirName,
                const string &fileName) ;
    CedarFSFile(const CedarFSFile &copyFrom) ;
    virtual    ~CedarFSFile() ;

    virtual string   getDirName() ;
    virtual string   getFileName() ;
    virtual string   getBaseName() ;
    virtual string   getExtension() ;
    virtual string   getFullPath() ;
} ;

#endif // I_CedarDir_h

