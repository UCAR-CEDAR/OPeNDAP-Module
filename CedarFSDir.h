// CedarFSDir.h

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the Cedar Data
// Access Protocol.

// Copyright (c) 2002,2003 Cedar, Inc.
// Author: Patrick West <pwest@ucar.edu>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact Cedar, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

// (c) COPYRIGHT URI/MIT 1994-1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>

#ifndef I_CedarFSDir_h
#define I_CedarFSDir_h

#include <list>
#include <string>

using std::list ;
using std::string ;

#include "CedarFSFile.h"

class CedarFSDir
{
private:
    string    _dirName ;
    string    _fileExpr ;
    list<CedarFSFile>   _fileList ;
    list<CedarFSDir>   _dirList ;
    bool    _dirLoaded ;

    void    loadDir() ;
public:
    CedarFSDir(const string &dirName) ;
    CedarFSDir(const string &dirName,
               const string &fileExpr) ;
    CedarFSDir(const CedarFSDir &copyFrom) ;
    virtual    ~CedarFSDir() ;

    typedef list<CedarFSDir>::iterator dirIterator ;
    virtual CedarFSDir::dirIterator beginOfDirList() ;
    virtual CedarFSDir::dirIterator endOfDirList() ;

    typedef list<CedarFSFile>::iterator fileIterator ;
    virtual CedarFSDir::fileIterator beginOfFileList() ;
    virtual CedarFSDir::fileIterator endOfFileList() ;

    virtual string   getDirName()
    {
        return _dirName ;
    }
} ;

#endif // I_CedarFSDir_h

