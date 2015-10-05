// CedarMySQLConnect.cc

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

using std::endl ;

#include "CedarMySQLConnect.h"
#include "BESInternalError.h"
#include "BESLog.h"
#include "CedarEncode.h"

CedarMySQLConnect::CedarMySQLConnect()
{
    _count = 0 ;
    _channel_open = false ;
    _has_log = false ;
    _the_channel = 0 ;
    mysql_init( &_mysql ) ;
}

CedarMySQLConnect::~CedarMySQLConnect()
{
    if( _channel_open )
    {
	mysql_close( _the_channel ) ;
	if( BESLog::TheLog()->is_verbose() )
	{
	    (*BESLog::TheLog()) << "MySQL channel disconnected from:" << endl
			         << "  server = " << _server << endl
			         << "  user = " << _user << endl
			         << "  database = " << _database << endl
				 << "  port = " << _mysql_port << endl
				 << "  socket = " << _mysql_sock << endl ;
	}
    }
}

void
CedarMySQLConnect::open( const string &server, const string &user,
		        const string &password, const string &database,
			int mysql_port, const string &mysql_sock )
{
    _count++ ;
    if( !_channel_open )
    {
	_server = server ;
	_user = user ;
	_database = database ;
	_mysql_port = mysql_port ;
	_mysql_sock = mysql_sock ;
	string dp = CedarEncode::decode( password.c_str(), "disp_key" ) ;
	_the_channel = mysql_real_connect( &_mysql, server.c_str(),
					   user.c_str(), dp.c_str(),
					   database.c_str(),
					   mysql_port, mysql_sock.c_str(), 0 ) ;
	if( !_the_channel )
	{
	    string serr = "can not get connected to MySQL engine." ;
	    string mysql_error = get_error() ;
	    serr += "\n" + mysql_error ;
	    throw BESInternalError( serr, __FILE__, __LINE__ ) ;
	}
	else
	{
	    if( BESLog::TheLog()->is_verbose() )
	    {
		(*BESLog::TheLog()) << "MySQL channel connected to:" << endl
				     << "  server = " << _server << endl
				     << "  user = " << _user << endl
				     << "  database = " << _database << endl
				     << "  port = " << _mysql_port << endl
				     << "  socket = " << _mysql_sock << endl ;
	    }
	    _channel_open = true ;
	}
    }
}

void
CedarMySQLConnect::close()
{
    _count-- ;
    if( _count == 0 && _channel_open )
    {
	mysql_close( _the_channel ) ;
	_channel_open = false ;
	(*BESLog::TheLog()) << "MySQL channel disconnected from:" << endl
			     << "  server = " << _server << endl
			     << "  user = " << _user << endl
			     << "  database = " << _database << endl
			     << "  port = " << _mysql_port << endl
			     << "  socket = " << _mysql_sock << endl ;
    }
}

string
CedarMySQLConnect::get_error()
{
    if( _channel_open )
	_error = mysql_error( _the_channel ) ;      
    else
	_error = mysql_error( &_mysql ) ;
    return _error ;
}

