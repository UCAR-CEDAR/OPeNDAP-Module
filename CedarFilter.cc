// CedarFilter.cc

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

#ifdef __GNUG__
#pragma implementation
#endif

#include "CedarFilter.h"
#include "CedarResponseNames.h"
#include "CedarTab.h"
#include "CedarFlat.h"

CedarFilter::CedarFilter()
    : DODSFilter()
{
}

CedarFilter::CedarFilter(int argc, char *argv[]) throw(Error)
    : DODSFilter( )
{
    initialize( argc, argv ) ;
}

CedarFilter::~CedarFilter()
{
}

/** Set the response to be returned to TAB_Response if "TAB" or "tab"
    or to FLAT_Response if "FLAT" or "flat"
    or call parent set_response to check for other options.
    @param o The name of the object. 
    @exceptoion InternalErr Thrown if the response is not one of the valid
    names. */
void CedarFilter::set_response(const string &r) throw(Error)
{
    if (r == "TAB" || r == "tab")
    {
	d_response = (DODSFilter::Response)CedarFilter::TAB_Response;
	d_action = TAB_RESPONSE ;
    }
    else if (r == "FLAT" || r == "flat")
    {
	d_response = (DODSFilter::Response)CedarFilter::FLAT_Response;
	d_action = FLAT_RESPONSE ;
    }
    else if (r == "STREAM" || r == "stream")
    {
	d_response = (DODSFilter::Response)CedarFilter::STREAM_Response;
	d_action = STREAM_RESPONSE ;
    }
    else if (r == "INFO" || r == "info")
    {
	d_response = (DODSFilter::Response)CedarFilter::INFO_Response;
	d_action = INFO_RESPONSE ;
    }
    else
	DODSFilter::set_response( r ) ;
}

