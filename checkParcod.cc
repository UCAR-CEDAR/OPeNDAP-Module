// checkParcod.cc

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
#include <cstdlib>

using std::cout ;
using std::endl ;
using std::cerr ;
using std::atoi ;

#include "CedarReadParcods.h"
#include "CedarMySQLDB.h"
#include "BESError.h"

int
main (int argc, char *argv[])
{
    if( argv[1] == NULL )
    {
	cerr << "You must provide a number as a parameter id." << endl ;
	return 1 ;
    }

    try
    {
	CedarDB::Add_DB_Builder( "mysql", CedarMySQLDB::BuildMySQLDB ) ;

	int param_id = atoi( argv[1] ) ;
	cout << "Input parameter code: " << param_id << endl ;
	string long_name = CedarReadParcods::Get_Longname( param_id ) ;
	cout << "Long Name: " << long_name << endl ;
	string short_name = CedarReadParcods::Get_Shortname( param_id ) ;
	cout << "Short Name: " << short_name << endl ;
	string madrigal_name = CedarReadParcods::Get_Madrigalname( param_id ) ;
	cout << "Madrigal Name: " << madrigal_name << endl ;
	string scale = CedarReadParcods::Get_Scale( param_id ) ;
	cout << "Multiplier: " << scale << endl ;
	string unit = CedarReadParcods::Get_Unit_Label( param_id ) ;
	cout << "Unit Label: " << unit << endl ;
    }
    catch( BESError &e )
    {
	cerr << e << endl ;
	return 1 ;
    }

    return 0 ;
}

