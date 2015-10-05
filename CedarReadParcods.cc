// CedarReadParcods.cc

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

#include <sstream>
#include <iostream>
#include <cstdlib>

using std::ostringstream ;
using std::atoi ;

#include "CedarReadParcods.h"
#include "CedarDB.h"
#include "CedarDBResult.h"
#include "BESInternalError.h"

map<int,CedarReadParcods::CedarParameter> CedarReadParcods::stored_list ;

/** @brief given an parameter id load the information from the database
 *
 * Loads the information about the given parameter specified by the
 * parameter id and stores it in the stored_list. If the parameter
 * information already exists, then just return the stored information, else
 * go get the information from the Catalog database.
 *
 * columns of the table needed are PARAMETER_ID, LONG_NAME, SHORT_NAME,
 * MADRIGAL_NAME, UNITS and SCALE
 *
 * @param param_id parameter id to look up
 */
void
CedarReadParcods::Load_Parameter( int param_id )
{
    // see if the request kinst is already stored. If not, go get it and
    // store it. Only get what is requested
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	CedarDB *db = CedarDB::DB( "Catalog" ) ;
	if( !db )
	{
	    string err = "Unable to establish connection to Catalog database" ;
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}

	ostringstream query ;
	query << "SELECT PARAMETER_ID, LONG_NAME, SHORT_NAME, MADRIGAL_NAME, UNITS, SCALE FROM tbl_parameter_code WHERE PARAMETER_ID = " << param_id ;
	CedarDBResult *result = db->run_query( query.str() ) ;
	if( !result )
	{
	    db->close() ;
	    string err =
		(string)"Query " + query.str() + " failed to return results";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}
	if( result->is_empty_set() )
	{
	    db->close() ;
	    string err =
		(string)"Query " + query.str() + " returned the empty set";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}
	if( result->get_num_rows() != 1 )
	{
	    db->close() ;
	    string err =
		(string)"Query " + query.str() + " returned too many results";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}

	result->first_row() ;

	CedarReadParcods::CedarParameter parameter ;
	parameter.id = atoi( (*result)["PARAMETER_ID"].c_str() ) ;
	parameter.short_name = (*result)["SHORT_NAME"] ;
	parameter.long_name = (*result)["LONG_NAME"] ;
	parameter.madrigal_name = (*result)["MADRIGAL_NAME"] ;
	parameter.units = (*result)["UNITS"] ;
	parameter.scale = (*result)["SCALE"] ;
	CedarReadParcods::stored_list[param_id] = parameter ;

	db->close() ;
    }
}

string
CedarReadParcods::Get_Code_as_String( int param_id )
{
    CedarReadParcods::Load_Parameter( param_id ) ;
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve param code for param " << param_id ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    ostringstream strm ;
    strm << param_id ;
    return strm.str() ;
}

string
CedarReadParcods::Get_Shortname( int param_id )
{
    CedarReadParcods::Load_Parameter( param_id ) ;
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve param short name for param " << param_id ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.short_name ;
}

string
CedarReadParcods::Get_Longname( int param_id )
{
    CedarReadParcods::Load_Parameter( param_id ) ;
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve param long name for param " << param_id ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.long_name ;
}

string
CedarReadParcods::Get_Madrigalname( int param_id )
{
    CedarReadParcods::Load_Parameter( param_id ) ;
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve param madrigal name for param " << param_id ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.madrigal_name ;
}

string
CedarReadParcods::Get_Scale( int param_id )
{
    CedarReadParcods::Load_Parameter( param_id ) ;
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve param scale for param " << param_id ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.scale ;
}

string
CedarReadParcods::Get_Unit_Label( int param_id )
{
    CedarReadParcods::Load_Parameter( param_id ) ;
    map<int,CedarReadParcods::CedarParameter>::iterator iter ;
    iter = CedarReadParcods::stored_list.find( param_id ) ;
    if( iter == CedarReadParcods::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve param units label for param " << param_id ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.units ;
}

