// CedarReadParcods.h

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

#ifndef CedarReadParcods_h_
#define CedarReadParcods_h_ 1

#include <map>
#include <string>

using std::map ;
using std::string ;

class CedarReadParcods
{
private:
    typedef struct _cedar_parameter
    {
	int id ;
	string short_name ;
	string long_name ;
	string madrigal_name ;
	string units ;
	string scale ;
    } CedarParameter ;

    static map<int,CedarReadParcods::CedarParameter> stored_list ;

				CedarReadParcods() {}
    static void			Load_Parameter( int param_id ) ;
public:
    static string		Get_Code_as_String( int param_id ) ;
    static string		Get_Shortname( int param_id ) ;
    static string		Get_Longname( int param_id ) ;
    static string		Get_Madrigalname( int param_id ) ;
    static string		Get_Scale( int param_id ) ;
    static string		Get_Unit_Label( int param_id ) ;
};

#endif // CedarReadParcods_h_

