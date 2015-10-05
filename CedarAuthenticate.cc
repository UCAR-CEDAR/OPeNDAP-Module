// CedarAuthenticate.cc

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

#include <iostream>
#include <sstream>
#include <new>

using std::cerr ;
using std::endl ;
using std::ostringstream ;
using std::bad_alloc ;

#include <time.h>

#include "CedarAuthenticate.h"
#include "CedarDB.h"
#include "CedarDBResult.h"
#include "TheBESKeys.h"
#include "CedarAuthenticateException.h"
#include "BESInternalFatalError.h"
#include "BESDataNames.h"
#include "BESContextManager.h"
#include "BESLog.h"
#include "BESDebug.h"

/** @brief Cedar authentication using MySQL database
 *
 * First reads the key/value pairs from the opendap initiailization file
 * using TheBESKeys in order to make a connection to the MySQL database
 * and to determine if authentication is turned on or off. The key/value
 * pairs used from the intialization file are:
 *
 * Cedar.Authenticate.MySQL.server=&lt;serverName&gt;
 * Cedar.Authenticate.MySQL.user=&lt;userName&gt;
 * Cedar.Authenticate.MySQL.password=&lt;encryptedPassword&gt;
 * Cedar.Authenticate.MySQL.database=&lt;databaseName&gt;
 * Cedar.Authenticate.MySQL.mode=&lt;on|off&gt;
 *
 * Then authenticates the user specified in the BESDataHandlerInterface
 * using a MySQL database. The table cedar_session in the MySQL database is
 * used to authenticate the user. The session information must be created
 * prior to this method being called.
 *
 * @param dhi contains the user name of the user to be authenticated
 * @throws CedarAuthenticateException if unable to read information from
 * initialization file or if unable to authenticate the user.
 * @throws BESInternalError if unable to connect to the MySQL database.
 * @see _BESDataHandlerInterface
 * @see CedarMySQLQuery
 * @see CedarAuthenticateException
 * @see BESInternalError
 * @see BESKeys
 */
bool
CedarAuthenticate::authenticate( BESDataHandlerInterface &dhi )
{
    if( BESLog::TheLog()->is_verbose() )
    {
	*(BESLog::TheLog()) << "authenticating" << endl ;
    }

    bool enforce_authentication = false ;
    bool found = false ;
    string my_key = "Cedar.Authenticate.Mode" ;
    string mode ;
    try
    {
	TheBESKeys::TheKeys()->get_value( my_key, mode, found ) ;
    }
    catch( ... )
    {
	string s = (string)"Unable to authenticate: "
	           + "Authentication mode (on/off) is not set "
		   + "in BES configuration file" ;
	throw CedarAuthenticateException( s, __FILE__, __LINE__ ) ;
    }
    if( found == false )
    {
	string s = (string)"Unable to authenticate: "
	           + "Authentication mode (on/off) is not set "
		   + "in BES configuration file" ;
	throw CedarAuthenticateException( s, __FILE__, __LINE__ ) ;
    }
    else
    {
	if( mode == "on" )
	{
	    enforce_authentication = true ;
	    BESDEBUG( "cedar", "CEDAR Authentication enabled" << endl ) ;
	}
	else
	{
	    if( mode == "off" )
	    {
		enforce_authentication = false ;
		BESDEBUG( "cedar", "CEDAR Authentication disabled" << endl ) ;
	    }
	    else
	    {
		string s = "Unable to authenticate: Authentication mode set to "
		           + mode + " in BES configuration file"
			   + ", should be set to on or off)" ;
		throw CedarAuthenticateException( s, __FILE__, __LINE__ ) ;
	    }
	}
    }

    if( enforce_authentication )
    {
	// build the query objects, which connects to the database
	CedarDB *db = 0 ;
	try
	{
	    db = CedarDB::DB( "Authenticate" ) ;
	    if( !db )
	    {
		string s = "Unable to authenticate: Failed to access database" ;
		throw CedarAuthenticateException( s, __FILE__, __LINE__ ) ;
	    }
	}
	catch( bad_alloc::bad_alloc )
	{
	    string s = "Can not get memory for MySQL Query object" ;
	    throw BESInternalFatalError( s, __FILE__, __LINE__ ) ;
	}
	catch( BESError &e )
	{
	    if( BESLog::TheLog()->is_verbose() )
	    {
		*(BESLog::TheLog()) << "error logging in: "
		                    << e.get_message() << endl ;
	    }
	    throw CedarAuthenticateException( e.get_message(), __FILE__, __LINE__ ) ;
	}

	string context = USER_NAME ;
	string username = BESContextManager::TheManager()->get_context( context,
								    found ) ;
	if( username.empty() )
	{
	    string s = "Unable to authenticate user, no user name provided" ;
	    throw CedarAuthenticateException( s, __FILE__, __LINE__ ) ;
	}

	context = USER_TOKEN ;
	string token = BESContextManager::TheManager()->get_context( context,
								     found ) ;

	// attempt to get the users session information
	string query_str = "select USER_NAME from cedar_sessions " ;
	query_str += "where USER_NAME=\"" + username + "\"" ;
	if( token != "" )
	{
	    query_str += " AND TOKEN=\"" + token + "\";" ;
	}
	else
	{
	    query_str += " AND TOKEN IS NULL;" ;
	}
	BESDEBUG( "cedar", "authenticating with " << query_str << endl ) ;
	CedarDBResult *result = db->run_query( query_str ) ;
	if( result && !result->is_empty_set() )
	{
	    if( (result->get_num_fields() != 1) )
	    {
		delete result ;
		ostringstream s ;
		s << "Unable to authenticate user "
		  << username << endl
		  << "Invalid data from MySQL: "
		  << result->get_num_rows() << " rows and "
		  << result->get_num_fields() << " fields returned" ;
		throw CedarAuthenticateException( s.str(), __FILE__, __LINE__ );
	    }
	}
	else
	{
	    if( result ) delete result ;
	    ostringstream s ;
	    s << "Unable to authenticate user "
	      << username ;
	    throw CedarAuthenticateException( s.str(), __FILE__, __LINE__ ) ;
	}

	if( result ) delete result ;
	result = 0 ;
    }

    return true ;
}

