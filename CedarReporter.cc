// CedarReporter.cc

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

#include "CedarReporter.h"
#include "BESInternalError.h"
#include "BESDataNames.h"
#include "CedarDB.h"
#include "TheBESKeys.h"
#include "BESContextManager.h"
#include "BESDataNames.h"
#include "BESLog.h"

CedarReporter::CedarReporter()
    : BESReporter(),
      _file_buffer( 0 )
{
    _db = CedarDB::DB( "Reporter" ) ;
    if( !_db )
    {
	string s = "Unable to open Cedar Reporter database" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }

    // If we are unable to log to the cedar reporter database then we will
    // log to the cedar log file. Not both!
    bool found = false ;
    TheBESKeys::TheKeys()->get_value( "Cedar.LogName", _log_name, found );
    if( _log_name == "" )
    {
        string err = (string)"Could not determine Cedar log name, "
                     + "not found in configuration file" ;
        throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }
    else
    {
        _file_buffer = new ofstream( _log_name.c_str(), ios::out | ios::app ) ;
        if( !(*_file_buffer) )
        {
            string s = "Unable to open Cedar log file " + _log_name ;;
            throw BESInternalError( s, __FILE__, __LINE__ ) ;
        }
    }
}

CedarReporter::~CedarReporter()
{
    // The database is closed in the CedarModule class
    if( _file_buffer )
    {
        delete _file_buffer ;
        _file_buffer = 0 ;
    }
}

/** @brief reports any data request information to the Cedar Report database
 *
 * Reports the username, action (type of data requested), symbolic names of
 * the containers requested (represents the data files, the first three
 * character representing the instrument), and any constraint
 *
 * @param dhi structure that contains all information pertaining to the data
 * request
 */
void
CedarReporter::report( BESDataHandlerInterface &dhi )
{
    // Only report if no errors occurred
    if( dhi.error_info )
	return ;

    // Only report a data request (get command) for ascii, das, dds, ddx,
    // datadds, tab, flat, stream, info, etc... The first three characters
    // of the action should be get.
    if( dhi.action.substr( 0, 3 ) != "get" )
	return ;

    // The action string is something like get.tab. The data product
    // requested is the information after the dot, after the first 4
    // characters.
    string product = dhi.action.substr( 4, dhi.action.length() - 4 ) ;

    // Get the user name from the data element of dhi. If doesn't exist or
    // is empty then don't log the entry.
    bool found = false ;
    string context = USER_NAME ;
    string username = BESContextManager::TheManager()->get_context( context,
								    found ) ;
    if( username.empty() )
	return ;

    // Get the symbolic names of the containers included in this request.
    // These represent the data files used, and the first three characters
    // represent the instrument. The constraints are all the same for all the
    // containers, so just grab the first constraint.
    string requested ;
    string constraint ;
    bool isfirst = true ;
    dhi.first_container() ;
    while( dhi.container )
    {
	if( !isfirst )
	    requested += ", " ;
	else
	    constraint = dhi.container->get_constraint() ;
	isfirst = false ;
	requested += dhi.container->get_symbolic_name() ;
	dhi.next_container() ;
    }

    // Build the vector of columns to be inserted. There is only one row
    // being inserted, so only one vector of that vector.
    vector< vector<CedarDBColumn> > flds ;
    vector<CedarDBColumn> fld_set ;
    fld_set.push_back( CedarDBColumn( "user", username ) ) ;
    fld_set.push_back( CedarDBColumn( "requested", requested ) ) ;
    fld_set.push_back( CedarDBColumn( "data_product", product ) ) ;
    fld_set.push_back( CedarDBColumn( "constraint_expr", constraint ) ) ;
    flds.push_back( fld_set ) ;

    // If there is a problem reporting to the database then report to the
    // log file.
    try
    {
	_db->insert( "tbl_report", flds ) ;
    }
    catch( BESError &e )
    {
	(*BESLog::TheLog()) << "Failed to report to cedar database: "
			    << e.get_message() << endl ;
	report_to_log( dhi, username, requested, product, constraint ) ;
    }
    catch( ... )
    {
	(*BESLog::TheLog()) << "Failed to report to cedar database: "
			    << "Unknown exception caught" << endl ;
	report_to_log( dhi, username, requested, product, constraint ) ;
    }
}

/** @brief reports request information to the cedar log if problem inserting
 * into database.
 *
 * The line in the log file will look like:
 *
 * [MDT Mon Apr 21 16:14:05 2008] pwest - mfp920504a - tab - date(1992,504,0,0,1992,603,2359,5999);record_type(5340/7001);parameters(21,34,800,810,1410,1420,2506)
 *
 * @param dhi structure that contains all information pertaining to the data
 * request
 * @param username user making the data request
 * @param requested symbolic names of files requested (basename of files)
 * @param product data product requested (das, dds, tab, flat, etc...)
 * @param constraint if any specified, the constraint on the data sets
 */
void
CedarReporter::report_to_log( BESDataHandlerInterface &dhi,
			      const string &username,
			      const string &requested,
			      const string &product,
			      const string &constraint )
{
    const time_t sctime = time( NULL ) ;
    const struct tm *sttime = localtime( &sctime ) ; 
    char zone_name[10] ;
    strftime( zone_name, sizeof( zone_name ), "%Z", sttime ) ;
    char *b = asctime( sttime ) ;
    *(_file_buffer) << "[" << zone_name << " " ;
    for(register int j = 0; b[j] != '\n'; j++ )
        *(_file_buffer) << b[j] ;
    *(_file_buffer) << "] " ;

    *(_file_buffer) << username << " - "
		    << requested << " - "
		    << product << " - "
		    << constraint << endl ;
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance along with information about
 * the containers stored in this volatile list.
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
CedarReporter::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "CedarReporter::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    if( _db ) _db->dump( strm ) ;
    BESIndent::UnIndent() ;
}

