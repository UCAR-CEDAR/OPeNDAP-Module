// ContainerStorageCedar.cc

// This file is part of the OPeNDAP Cedar data handler, providing data
// access views for CedarWEB data

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.edu> and Jose Garcia <jgarcia@ucar.edu>
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
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301
 
// (c) COPYRIGHT University Corporation for Atmostpheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>
//      jgarcia     Jose Garcia <jgarcia@ucar.edu>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sstream>
#include <fstream>
#include <iostream>

using std::stringstream ;
using std::ifstream ;

#include "ContainerStorageCedar.h"
#include <BESFileContainer.h>
#include <TheBESKeys.h>
#include <BESInternalError.h>
#include <BESSyntaxUserError.h>
#include <BESForbiddenError.h>
#include <BESInfo.h>
#include <BESServiceRegistry.h>
#include "CedarFSDir.h"
#include "CedarFSFile.h"
#include "CedarResponseNames.h"

ContainerStorageCedar::ContainerStorageCedar( const string &n )
    : BESContainerStorage( n )
{
    string key = "Cedar.BaseDir" ;
    bool found = false ;
    TheBESKeys::TheKeys()->get_value( key, _cedar_base, found ) ;
    if( _cedar_base == "" )
    {
	string s = key + " not defined in bes configuration file"
	           + ", unable to determine Cedar base directory" ;
	throw BESSyntaxUserError( s, __FILE__, __LINE__ ) ;
    }
    key = "Madrigal.BaseDir" ;
    found = false ;
    TheBESKeys::TheKeys()->get_value( key, _madrigal_base, found ) ;
}

ContainerStorageCedar::~ContainerStorageCedar()
{
}

BESContainer *
ContainerStorageCedar::look_for( const string &sym_name )
{
    // This should be the full path to the file. Make sure it exists. If
    // it doesn't, then return null and perhaps another container store
    // can handle it. If not, then an exception will be thrown.
    string real_name = _cedar_base + "/" + sym_name + ".cbf" ;
    int is_accessible = access( real_name.c_str(), R_OK ) ;
    if( is_accessible == -1 && _madrigal_base.empty() )
    {
        return 0 ;
    }
    else if( is_accessible == -1 && !_madrigal_base.empty() )
    {
        real_name = _madrigal_base + "/" + sym_name + ".001" ;
        is_accessible = access( real_name.c_str(), R_OK ) ;
        if( is_accessible == -1 )
        {
            real_name = _madrigal_base + "/" + sym_name + ".cbf" ;
            is_accessible = access( real_name.c_str(), R_OK ) ;
            if( is_accessible == -1 )
            {
                // In this case, the file is not accessible or does not exist
                // using cedar base directory. So return 0 and allow another
                // container store a chance to get to it.
                return 0 ;
            }
        }
    }
    // the file is accessible, let's use it
    BESContainer *c = new BESFileContainer( sym_name, real_name, "cedar" ) ;
    return c ;
}

void
ContainerStorageCedar::add_container( const string &s_name,
				      const string &r_ame,
				      const string &type )
{
    string s = "Unable to add a container to Cedar Persistence"  ;
    throw BESForbiddenError( s, __FILE__, __LINE__ ) ;
}

bool
ContainerStorageCedar::del_container( const string &s_name )
{
    string s = "Unable to remove a container from Cedar Persistence"  ;
    throw BESForbiddenError( s, __FILE__, __LINE__ ) ;
    return false ;
}

bool
ContainerStorageCedar::del_containers( )
{
    string s = "Unable to remove a containers from Cedar Persistence" ;
    throw BESForbiddenError( s, __FILE__, __LINE__ ) ;
    return false ;
}

/** @brief determine if the given container is data and what servies
 * are available for it
 *
 * @param inQuestion the container in question
 * @param provides an output parameter for storing the list of
 * services provided for this container
 */
bool
ContainerStorageCedar::isData( const string &inQuestion,
			       list<string> &provides )
{
    bool isit = false ;
    BESContainer *c = look_for( inQuestion ) ;
    if( c )
    {
	isit = true ;
	string node_type = c->get_container_type() ;
	BESServiceRegistry::TheRegistry()->services_handled( node_type,
							     provides ) ;
    }
    return isit ;
}

void
ContainerStorageCedar::show_containers( BESInfo &info )
{
    info.add_data( get_name() ) ;
    info.add_data( "\n" ) ;

    // need to get every file name in the directory and display
    try
    {
	CedarFSDir d( _cedar_base, ".*\\.cbf$" ) ;
	CedarFSDir::fileIterator i ;
	for( i = d.beginOfFileList(); i != d.endOfFileList(); i++ )
	{
	    string sym = (*i).getBaseName() ;
	    string real = (*i).getFullPath() ;
	    show_container( sym, real, CEDAR_NAME, info ) ;
	}
    }
    catch( const string &err_str )
    {
	info.add_data( err_str ) ;
    }
    catch( ... )
    {
	info.add_data( "Failed to retrieve containers for cedar" ) ;
    }

    try
    {
	if( !_madrigal_base.empty() )
	{
	    CedarFSDir d( _madrigal_base, ".*\\.001$" ) ;
	    CedarFSDir::fileIterator i ;
	    for( i = d.beginOfFileList(); i != d.endOfFileList(); i++ )
	    {
		string sym = (*i).getBaseName() ;
		string real = (*i).getFullPath() ;
		show_container( sym, real, CEDAR_NAME, info ) ;
	    }
	}
    }
    catch( const string &err_str )
    {
	info.add_data( err_str ) ;
    }
    catch( ... )
    {
	info.add_data( "Failed to retrieve containers for cedar" ) ;
    }

    try
    {
	if( !_madrigal_base.empty() )
	{
	    CedarFSDir d( _madrigal_base, ".*\\.cbf$" ) ;
	    CedarFSDir::fileIterator i ;
	    for( i = d.beginOfFileList(); i != d.endOfFileList(); i++ )
	    {
		string sym = (*i).getBaseName() ;
		string real = (*i).getFullPath() ;
		show_container( sym, real, CEDAR_NAME, info ) ;
	    }
	}
    }
    catch( const string &err_str )
    {
	info.add_data( err_str ) ;
    }
    catch( ... )
    {
	info.add_data( "Failed to retrieve containers for cedar" ) ;
    }
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance and the Cedar root data
 * directory
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
ContainerStorageCedar::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "ContainerStorageCedar::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    strm << BESIndent::LMarg << "Cedar base directory: " << _cedar_base << endl;
    BESIndent::UnIndent() ;
}

