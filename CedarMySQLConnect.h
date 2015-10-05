// CedarMySQLConnect.h

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

#ifndef CedarMySQLConnect_h_
#define CedarMySQLConnect_h_ 1

#include <string>

using std::string ;

#include <mysql.h>

class CedarMySQLQuery ;

class CedarMySQLConnect
{
private:
    string		_server, _user, _database, _mysql_sock ;
    int			_mysql_port ;
    string		_error ;
    bool		_channel_open ;
    bool		_has_log ;
    MYSQL *		_the_channel ;
    MYSQL		_mysql ;
    int			_count ;

public:
    			CedarMySQLConnect() ;
			~CedarMySQLConnect() ;
    void		open(const string &server, const string &user,
			     const string &password, const string &database,
			     int mysql_port, const string &mysql_sock ) ;
    void		close ();
    int			is_channel_open() const { return _channel_open ; }
    string		get_error();
    MYSQL *		get_channel() { return _the_channel ; }
    string		get_server() { return _server ; }
    string		get_user() { return _user ; }
    string		get_database() { return _database ; }
};

#endif // CedarMySQLConnect_h_

