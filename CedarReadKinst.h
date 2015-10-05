// CedarReadKinst.h

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

#ifndef CedarReadKinst_h_
#define CedarReadKinst_h_ 1

#include <map>
#include <string>

using std::map ;
using std::string ;

class CedarReadKinst
{
private:
    typedef struct _cedar_instrument
    {
	int kinst ;
	string name ;
	string prefix ;
	int lat_degrees ;
	int lat_minutes ;
	int lat_seconds ;
	int lon_degrees ;
	int lon_minutes ;
	int lon_seconds ;
	double altitude ;
    } CedarInstrument ;

    static map<int,CedarReadKinst::CedarInstrument> stored_list ;

				CedarReadKinst() {}
    static void			Load_Instrument( int kinst ) ;
public:
  /// Returns all the information for an instrument given its numeric id.
    static string		Get_Kinst_as_String( int kinst ) ;
    static string		Get_Name( int kinst ) ;
    static string		Get_Prefix( int kinst ) ;
    static void			Get_Longitude( int kinst, int &degrees,
					       int &minutes, int &seconds ) ;
    static string		Get_Longitude_as_String( int kinst ) ;
    static void			Get_Latitude( int kinst, int &degrees,
					      int &minutes, int &seconds) ;
    static string		Get_Latitude_as_String( int kinst ) ;
    static double		Get_Altitude( int kinst ) ;
    static string		Get_Altitude_as_String( int kinst ) ;
};

#endif // CedarReadKinst_h_

