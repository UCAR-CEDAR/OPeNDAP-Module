// CedarMySQLQuery.cc

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

#include <new>

using std::bad_alloc ;

#include "CedarMySQLQuery.h"
#include "BESInternalFatalError.h"
#include "CedarMySQLConnect.h"
#include "BESInternalError.h"

CedarMySQLQuery::CedarMySQLQuery(const string &server, const string &user,
			       const string &password, const string &database,
			       int mysql_port, const string &mysql_sock )
{
    _my_connection = 0 ;
    _results = 0 ;

    try
    {
	_my_connection = new CedarMySQLConnect ;
    }
    catch( bad_alloc::bad_alloc )
    {
	string s = "Could not allocate memory for MySQL Connect object" ;
	throw BESInternalFatalError( s, __FILE__, __LINE__ ) ;
    }

    try
    {
	_my_connection->open( server, user, password, database,
			      mysql_port, mysql_sock ) ;
    }
    catch( BESError &ce )
    {
	if( _my_connection ) delete _my_connection ;
	_my_connection = 0 ;
	throw ce ;
    }
}

CedarMySQLQuery::~CedarMySQLQuery()
{
    if( _results ) 
    {
	delete _results;
    }
    if( _my_connection )
    {
	delete _my_connection;
    }
}

void
CedarMySQLQuery::run_query(const string &query)
{
    MYSQL *sql_channel = _my_connection->get_channel() ;
    if( mysql_query( sql_channel, query.c_str() ) )
    {
	throw BESInternalError( _my_connection->get_error(), __FILE__, __LINE__ ) ;
    }
    else
    {
	if( _results ) delete _results ;
	_results = 0 ;
	MYSQL_RES *result = 0 ;
	MYSQL_ROW row ;
	result = mysql_store_result( sql_channel ) ;
	int n_rows = mysql_num_rows( result) ;
	int n_fields = mysql_num_fields( result ) ;
	try
	{
	    _results = new CedarMySQLResult( n_rows, n_fields ) ;
	}
	catch(bad_alloc::bad_alloc)
	{
	    string s = "Could not allocate memory for MySQL result object" ;
	    throw BESInternalFatalError( s, __FILE__, __LINE__ ) ;
	}
	_results->first_row() ;
	while( (row=mysql_fetch_row( result )) != NULL )
	{
	    _results->first_field() ;
	    for( int i=0; i<n_fields; i++ )
	    {
		_results->set_field( row[i] ) ;
		_results->next_field() ;
	    }
	    _results->next_row() ;
	}
	mysql_free_result( result ) ;
    }
}

