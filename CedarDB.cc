// CedarDB.cc

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

#include "CedarDB.h"
#include "BESInternalError.h"
#include "TheBESKeys.h"

map<string,CedarDB *>		CedarDB::db_map ;
map<string,p_db_builder>	CedarDB::db_list ;

void
CedarDB::Add_DB_Builder( const string &db_type, p_db_builder builder )
{
    map<string,p_db_builder>::iterator i = db_list.find( db_type ) ;
    if( i != db_list.end() )
    {
	string err = "Database type " + db_type + " is already installed" ;
	throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }
    db_list[db_type] = builder ;
}

CedarDB *
CedarDB::DB( const string &db_name )
{
    CedarDB *ret = 0 ;
    map<string,CedarDB *>::iterator iter = db_map.find( db_name ) ;
    if( iter != db_map.end() )
    {
	ret = iter->second ;
    }
    else
    {
	bool found = false ;
	string my_key = "Cedar.DB." + db_name + ".Type" ;

	string db_type ;
	TheBESKeys::TheKeys()->get_value( my_key, db_type, found ) ;
	if( found == false || db_type.empty() )
	{
	    string err = "Database type is not specified for " + db_name ;
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}
	else
	{
	    map<string,p_db_builder>::iterator liter = db_list.find( db_type ) ;
	    if( liter != db_list.end() )
	    {
		p_db_builder p = (*liter).second ;
		ret = p( db_name ) ;
		db_map[db_name] = ret ;
	    }
	    else
	    {
		string err = "Database type " + db_type + " is not supported" ;
		throw BESInternalError( err, __FILE__, __LINE__ ) ;
	    }
	}
    }

    return ret ;
}

void
CedarDB::Close()
{
    map<string,CedarDB *>::iterator i = db_map.begin() ;
    map<string,CedarDB *>::iterator e = db_map.end() ;
    for( ; i != e; i++ )
    {
	CedarDB *db = i->second ;
	if( db && db->is_open() )
	{
	    db->close() ;
	}
    }
}

