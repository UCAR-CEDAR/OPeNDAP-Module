// CedarAuthenticate.h

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

#ifndef A_CedarAuthenticate_h
#define A_CedarAuthenticate_h 1

#include "BESDataHandlerInterface.h"

/** @brief Authentication is done through the use of MySQL database.
 *
 * This class provides a mechanism of authentication using a MySQL database,
 * specified through the dods initialization file. The database is specified
 * with the following keys in the initialization file:
 *
 * Cedar.Authenticate.MySQL.server=&lt;serverName&gt;
 * Cedar.Authenticate.MySQL.user=&lt;userName&gt;
 * Cedar.Authenticate.MySQL.password=&lt;encryptedPassword&gt;
 * Cedar.Authenticate.MySQL.database=&lt;databaseName&gt;
 * Cedar.Authenticate.MySQL.mode=&lt;on|off&gt;
 *
 * The relevant columns for authentication in the session table tbl_session,
 * are USER_NAME and TOKEN.
 * 
 * CedarAuthenticate can be specified on the link line as the
 * authentication mechanism by linking in the object module
 * mysql_authenticator.o. If, during development and testing of the server,
 * you wish to turn off authentication using mysql, set the mode to off.
 *
 * The password is encrypted.
 *
 * @see TheBESKeys
 */
class CedarAuthenticate
{
public:
    static bool		authenticate( BESDataHandlerInterface &dhi ) ;
} ;

#endif // A_CedarAuthenticate_h

