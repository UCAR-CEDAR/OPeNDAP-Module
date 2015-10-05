// CedarReadKinst.cc

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
#include <iomanip>
#include <iostream>
#include <cstdlib>

using std::ostringstream ;
using std::setw ;
using std::setprecision ;
using std::setfill ;
using std::left ;
using std::right ;

#include "CedarReadKinst.h"
#include "CedarDB.h"
#include "CedarDBResult.h"
#include "BESInternalError.h"

map<int,CedarReadKinst::CedarInstrument> CedarReadKinst::stored_list ;

/** @brief given an instrument id (kinst) load the information from the
 * database
 *
 * Loads the information about the given instrument specified by the
 * instrument id (kinst) and stores it in the stored_list. If the instrument
 * information already exists, then just return the stored information, else
 * go get the information from the Catalog database.
 *
 * columns of the table needed are KINST, INST_NAME, PREFIX
 *
 * FIXME: With the re-work of the CEDARCATALOG, LATITUDE, LONGITUDE, and
 * ALTITUDE are no win the tbl_site table, which is not yet populated
 *
 * @param kinst instrument id to look up
 */
void
CedarReadKinst::Load_Instrument( int kinst )
{
    // see if the request kinst is already stored. If not, go get it and
    // store it. Only get what is requested
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	CedarDB *db = CedarDB::DB( "Catalog" ) ;
	if( !db )
	{
	    string err = "Unable to establish connection to Catalog database" ;
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}

	ostringstream iquery ;
	iquery << "SELECT KINST, INST_NAME, PREFIX from tbl_instrument where KINST = " << kinst ;
	CedarDBResult *result = db->run_query( iquery.str() ) ;
	if( !result )
	{
	    db->close() ;
	    string err =
		(string)"Query " + iquery.str() + " failed to return results";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}
	if( result->is_empty_set() )
	{
	    db->close() ;
	    string err =
		(string)"Query " + iquery.str() + " returned the empty set";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}
	if( result->get_num_rows() != 1 )
	{
	    db->close() ;
	    string err =
		(string)"Query " + iquery.str() + " returned too many results";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}

	result->first_row() ;

	CedarReadKinst::CedarInstrument instrument ;
	instrument.kinst = atoi( (*result)["KINST"].c_str() ) ;
	instrument.name = (*result)["INST_NAME"] ;
	instrument.prefix = (*result)["PREFIX"] ;
	delete result ;

	ostringstream squery ;
	squery << "SELECT LAT_DEGREES, LAT_MINUTES, LAT_SECONDS, LON_DEGREES, LON_MINUTES, LON_SECONDS, ALT from tbl_site where KINST = " << kinst ;
	result = db->run_query( squery.str() ) ;
	if( !result )
	{
	    db->close() ;
	    string err =
		(string)"Query " + squery.str() + " failed to return results";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}
	if( result->get_num_rows() > 1 )
	{
	    db->close() ;
	    string err =
		(string)"Query " + squery.str() + " returned too many results";
	    throw BESInternalError( err, __FILE__, __LINE__ ) ;
	}

	instrument.lat_degrees = 0 ;
	instrument.lat_minutes = 0 ;
	instrument.lat_seconds = 0 ;
	instrument.lon_degrees = 0 ;
	instrument.lon_minutes = 0 ;
	instrument.lon_seconds = 0 ;
	instrument.altitude = 0.0 ;
	if( result->get_num_rows() == 1 )
	{
	    result->first_row() ;
	    instrument.lat_degrees = atoi( (*result)["LAT_DEGREES"].c_str() ) ;
	    instrument.lat_minutes = atoi( (*result)["LAT_MINUTES"].c_str() ) ;
	    instrument.lat_seconds = atoi( (*result)["LAT_SECONDS"].c_str() ) ;
	    instrument.lon_degrees = atoi( (*result)["LON_DEGREES"].c_str() ) ;
	    instrument.lon_minutes = atoi( (*result)["LON_MINUTES"].c_str() ) ;
	    instrument.lon_seconds = atoi( (*result)["LON_SECONDS"].c_str() ) ;
	    instrument.altitude = atof( (*result)["ALT"].c_str() ) ;
	}
	CedarReadKinst::stored_list[kinst] = instrument ;

	db->close() ;
    }
}

string
CedarReadKinst::Get_Kinst_as_String( int kinst )
{
    CedarReadKinst::Load_Instrument( kinst ) ;
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve instrument kinst for kinst " << kinst ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    ostringstream strm ;
    strm << kinst ;
    return strm.str() ;
}

string
CedarReadKinst::Get_Name( int kinst )
{
    CedarReadKinst::Load_Instrument( kinst ) ;
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve instrument name for kinst " << kinst ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.name ;
}

string
CedarReadKinst::Get_Prefix( int kinst )
{
    CedarReadKinst::Load_Instrument( kinst ) ;
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve instrument prefix for kinst " << kinst ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.prefix ;
}

void
CedarReadKinst::Get_Longitude( int kinst,
			       int &degrees, int &minutes, int &seconds )
{
    CedarReadKinst::Load_Instrument( kinst ) ;
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve instrument longitude for kinst " << kinst ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    degrees = iter->second.lon_degrees ;
    minutes = iter->second.lon_minutes ;
    seconds = iter->second.lon_seconds ;
}

string
CedarReadKinst::Get_Longitude_as_String( int kinst )
{
    // first, get the longitude. If the kinst doesn't exist, then calling
    // Get_Longitude will throw an exception, no need to check here.
    int degrees, minutes, seconds = 0 ;
    CedarReadKinst::Get_Longitude( kinst, degrees, minutes, seconds ) ;
    ostringstream strm ;
    strm << degrees << " " ;
    strm << setw( 2 ) << setfill( '0' ) << minutes << "'" ;
    strm << setw( 2 ) << setfill( '0' ) << seconds << "\\\"" ;
    return strm.str() ;
}

void
CedarReadKinst::Get_Latitude( int kinst,
			      int &degrees, int &minutes, int &seconds )
{
    CedarReadKinst::Load_Instrument( kinst ) ;
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve instrument latitude for kinst " << kinst ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    degrees = iter->second.lat_degrees ;
    minutes = iter->second.lat_minutes ;
    seconds = iter->second.lat_seconds ;
}

string
CedarReadKinst::Get_Latitude_as_String( int kinst )
{
    // first, get the longitude. If the kinst doesn't exist, then calling
    // Get_Latitude will throw an exception, no need to check here.
    int degrees, minutes, seconds = 0 ;
    CedarReadKinst::Get_Latitude( kinst, degrees, minutes, seconds ) ;
    ostringstream strm ;
    strm << degrees << " " ;
    strm << setw( 2 ) << setfill( '0' ) << minutes << "'" ;
    strm << setw( 2 ) << setfill( '0' ) << seconds << "\\\"" ;
    return strm.str() ;
}

double
CedarReadKinst::Get_Altitude( int kinst )
{
    CedarReadKinst::Load_Instrument( kinst ) ;
    map<int,CedarReadKinst::CedarInstrument>::iterator iter ;
    iter = CedarReadKinst::stored_list.find( kinst ) ;
    if( iter == CedarReadKinst::stored_list.end() )
    {
	ostringstream err ;
	err << "Failed to retrieve instrument altitude for kinst " << kinst ;
	throw BESInternalError( err.str(), __FILE__, __LINE__ ) ;
    }
    return iter->second.altitude ;
}

string
CedarReadKinst::Get_Altitude_as_String( int kinst )
{
    // first, get the altitude. If the kinst doesn't exist then calling
    // Get_Altitude will throw an exception, no need to check here.
    double alt = CedarReadKinst::Get_Altitude( kinst ) ;
    ostringstream strm ;
    strm << setprecision( 5 ) << alt ;
    return strm.str() ;
}

