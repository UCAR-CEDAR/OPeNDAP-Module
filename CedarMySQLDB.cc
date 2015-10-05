// CedarMySQLDB.cc

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

#include <vector>
#include <cstdlib>

using std::vector ;
using std::atoi ;
using std::getenv ;

#include "CedarMySQLDB.h"
#include "CedarMySQLConnect.h"
#include "TheBESKeys.h"
#include "BESDebug.h"
#include "BESInternalError.h"

CedarMySQLDB::CedarMySQLDB( const string &db_name )
{
    bool found = false ;
    string key = "Cedar.DB." + db_name + "." ;

    TheBESKeys::TheKeys()->get_value( key + "Server", _mysql_server, found ) ;
    if( found == false )
    {
	string s = (string)"MySQL server not set in BES configuration file" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    BESDEBUG( "cedar", "MySQL server = " << _mysql_server << endl ) ;

    TheBESKeys::TheKeys()->get_value( key + "User", _mysql_user, found  ) ;
    if( found == false )
    {
	string s = (string)"MySQL user not set in BES configuration file" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    BESDEBUG( "cedar", "MySQL user = " << _mysql_user << endl ) ;

    TheBESKeys::TheKeys()->get_value( key + "Password", _mysql_pwd, found  ) ;
    if( found == false )
    {
	string s = (string)"MySQL password not set in BES configuration file" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    BESDEBUG( "cedar", "MySQL password = " << _mysql_pwd << endl ) ;

    TheBESKeys::TheKeys()->get_value( key + "Database", _mysql_db, found ) ;
    if( found == false )
    {
	string s = (string)"MySQL database not set in BES configuration file" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    BESDEBUG( "cedar", "MySQL database = " << _mysql_db << endl ) ;
    
    bool port_found = false ;
    string my_sport ;
    TheBESKeys::TheKeys()->get_value( key + "Port", my_sport, port_found ) ;
    _mysql_port = 0 ;
    if( port_found && !my_sport.empty() )
    {
	_mysql_port = atoi( my_sport.c_str() ) ;
	if( _mysql_port == 0 )
	{
	    string s = "MySQL port " + my_sport
	               + " is not valid in BES configuration file" ;
	    throw BESInternalError( s, __FILE__, __LINE__ ) ;
	}
	BESDEBUG( "cedar", "MySQL port = " << _mysql_port << endl ) ;
    }

    TheBESKeys::TheKeys()->get_value( key + "Socket", _mysql_socket, found ) ;
    if( !found && !port_found )
    {
	char *mysql_home = getenv( "MYSQL_HOME" ) ;
	if( !mysql_home )
	{
	    _mysql_socket = "/tmp/mysql.sock" ;
	}
	else
	{
	    _mysql_socket = string( mysql_home ) + "/mysql.sock" ;
	}
    }
    else if( found && _mysql_socket != "" )
    {
	_mysql_port = 0 ;
    }
    else if( found && _mysql_socket == "" )
    {
	string s = (string)"MySQL socket is not set in BES configuration file" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    BESDEBUG( "cedar", "MySQL socket = " << _mysql_socket << endl ) ;
}

CedarMySQLDB::~CedarMySQLDB()
{
    if( is_open() )
	close() ;
}

void
CedarMySQLDB::open()
{
    if( !is_open() )
    {
	_connection = new CedarMySQLConnect( ) ;
	_connection->open( _mysql_server, _mysql_user, _mysql_pwd, _mysql_db,
			   _mysql_port, _mysql_socket ) ;
	set_is_open( true ) ;
    }
}

void
CedarMySQLDB::close()
{
    if( _connection )
    {
	delete _connection ;
	_connection = 0 ;
    }
    set_is_open( false ) ;
}

CedarDBResult *
CedarMySQLDB::run_query( const string &query )
{
    BESDEBUG( "cedar", "MYSQL query = " << query << endl ) ;
    CedarMySQLResult *ret_result = 0 ;
    if( !is_open() )
    {
	open() ;
    }

    BESDEBUG( "cedar", "MYSQL query making query" << endl ) ;
    MYSQL *sql_channel = _connection->get_channel() ;
    if( mysql_query( sql_channel, query.c_str() ) )
    {
	throw BESInternalError( _connection->get_error(), __FILE__, __LINE__ ) ;
    }

    BESDEBUG( "cedar", "MYSQL query getting result" << endl ) ;
    MYSQL_RES *result = 0 ;
    MYSQL_ROW row ;
    result = mysql_store_result( sql_channel ) ;
    int n_rows = mysql_num_rows( result) ;
    int n_fields = mysql_num_fields( result ) ;

    vector<string> result_fields ;
    MYSQL_FIELD *field;
    while( ( field = mysql_fetch_field( result ) ) )
    {
	result_fields.push_back( field->name ) ;
    }

    BESDEBUG( "cedar", "MYSQL query setting results" << endl ) ;
    ret_result = new CedarMySQLResult( n_rows, n_fields, result_fields ) ;
    ret_result->first_row() ;
    while( ( row = mysql_fetch_row( result ) ) != NULL )
    {
	vector<string> result_row ;
	for( int i=0; i<n_fields; i++ )
	{
	    if( row[i] )
		result_row.push_back( row[i] ) ;
	    else
		result_row.push_back( "" ) ;
	}
	ret_result->set_row( result_row ) ;
	ret_result->next_row() ;
    }
    mysql_free_result( result ) ;

    BESDEBUG( "cedar", "MYSQL query returning" << endl ) ;
    return ret_result ;
}

unsigned int
CedarMySQLDB::insert( const string &table_name,
		      const vector< vector<CedarDBColumn> > &flds )
{
    BESDEBUG( "cedar", "MYSQL insert" << endl ) ;

    // there must be fields to insert
    if( flds.empty() )
    {
	string err = (string)"No fields specified in the insert to table "
	             + table_name ;
	throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }

    // if the database connection hasn't been made, make it. open will throw
    // an exception if there is a problem.
    if( !is_open() )
    {
	open() ;
    }

    string query = "INSERT INTO " + table_name + " ( " ;
    string values = "VALUES " ;
    vector< vector<CedarDBColumn> >::const_iterator oi = flds.begin() ;
    vector< vector<CedarDBColumn> >::const_iterator oe = flds.end() ;
    bool firstset = true ;
    for( ; oi != oe; oi++ )
    {
	if( !firstset )
	    values += ", " ;

	vector<CedarDBColumn>::const_iterator i = (*oi).begin() ;
	vector<CedarDBColumn>::const_iterator e = (*oi).end() ;
	bool isfirst = true ;
	for( ; i != e; i++ )
	{
	    if( firstset )
	    {
		if( !isfirst )
		    query += ", " ;
		query += (*i).name() ;
	    }

	    if( !isfirst )
		values += ", " ;
	    else
		values += "( " ;
	    isfirst = false ;
	    values += (*i).insert() ;
	}
	values += " )" ;
	firstset = false ;
    }
    query += " ) " + values ;
    BESDEBUG( "cedar", "MYSQL insert: query = " << query << endl ) ;

    // make the insert
    MYSQL *sql_channel = _connection->get_channel() ;
    if( mysql_query( sql_channel, query.c_str() ) )
    {
	throw BESInternalError( _connection->get_error(), __FILE__, __LINE__ ) ;
    }

    BESDEBUG( "cedar", "MYSQL insert done" << endl ) ;
    return mysql_affected_rows( sql_channel ) ;
}

unsigned int
CedarMySQLDB::update( const string &table_name,
		      const vector<CedarDBColumn> &flds,
		      const vector<CedarDBWhere> &where )
{
    BESDEBUG( "cedar", "MYSQL update" << endl ) ;

    // there must be fields to insert
    if( flds.empty() )
    {
	string err = (string)"No fields specified in the update to table "
	             + table_name ;
	throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }
    // there must be a where clause
    if( where.empty() )
    {
	string err = (string)"No where clause specified in the update to table "
	             + table_name ;
	throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }

    // if the database hasn't been opened, open it.
    if( !is_open() )
    {
	open() ;
    }

    // build the sql statement
    string query = "UPDATE " + table_name + " SET " ;
    vector<CedarDBColumn>::const_iterator i = flds.begin() ;
    vector<CedarDBColumn>::const_iterator e = flds.end() ;
    bool isfirst = true ;
    for( ; i != e; i++ )
    {
	if( !isfirst )
	{
	    query += ", " ;
	}
	isfirst = false ;
	query += (*i).update() ;
    }
    query += " WHERE " ;
    vector<CedarDBWhere>::const_iterator wi = where.begin() ;
    vector<CedarDBWhere>::const_iterator we = where.end() ;
    for( ; wi != we; wi++ )
    {
	query += (*wi).where() ;
    }
    BESDEBUG( "cedar", "MYSQL update query = " << query << endl ) ;

    // make the update
    MYSQL *sql_channel = _connection->get_channel() ;
    if( mysql_query( sql_channel, query.c_str() ) )
    {
	throw BESInternalError( _connection->get_error(), __FILE__, __LINE__ ) ;
    }

    BESDEBUG( "cedar", "MYSQL update done" << endl ) ;
    return mysql_affected_rows( sql_channel ) ;
}

unsigned int
CedarMySQLDB::del( const string &table_name,
		   const vector<CedarDBWhere> &where )
{
    BESDEBUG( "cedar", "MYSQL delete" << endl ) ;

    // there must be a where clause
    if( where.empty() )
    {
	string err = (string)"No where clause specified in the delete to table "
	             + table_name ;
	throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }

    string query = "DELETE FROM " + table_name + " WHERE " ;
    vector<CedarDBWhere>::const_iterator wi = where.begin() ;
    vector<CedarDBWhere>::const_iterator we = where.end() ;
    for( ; wi != we; wi++ )
    {
	query += (*wi).where() ;
    }
    BESDEBUG( "cedar", "MYSQL update query = " << query << endl ) ;

    // make the update
    MYSQL *sql_channel = _connection->get_channel() ;
    if( mysql_query( sql_channel, query.c_str() ) )
    {
	throw BESInternalError( _connection->get_error(), __FILE__, __LINE__ ) ;
    }

    BESDEBUG( "cedar", "MYSQL delete done" << endl ) ;
    return mysql_affected_rows( sql_channel ) ;
}

void
CedarMySQLDB::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "CedarMySQLDB::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    strm << BESIndent::LMarg << "server = " << _mysql_server << endl ;
    strm << BESIndent::LMarg << "user = " << _mysql_user << endl ;
    strm << BESIndent::LMarg << "password = " << _mysql_pwd << endl ;
    strm << BESIndent::LMarg << "database = " << _mysql_db << endl ;
    strm << BESIndent::LMarg << "port = " << _mysql_port << endl ;
    strm << BESIndent::LMarg << "socket = " << _mysql_socket << endl ;
    BESIndent::UnIndent() ;
}

CedarDB *
CedarMySQLDB::BuildMySQLDB( const string &db_name )
{
    return new CedarMySQLDB( db_name ) ;
}

