// CedarFilter.h

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

#ifndef I_CedarFilter_h
#define I_CedarFilter_h

#ifndef __POWERPC__
#ifdef __GNUG__
#pragma interface
#endif
#endif

#include "DODSFilter.h"

class CedarTab ;
class CedarFlat ;

using namespace libdap ;

class CedarFilter : public DODSFilter {
public:
    /** Types of responses DODSFilter know about. */
    enum Response {
	TAB_Response = DODSFilter::Version_Response + 1,
	FLAT_Response,
	STREAM_Response,
	INFO_Response
    };

public:
    CedarFilter();
    CedarFilter(int argc, char *argv[]) throw(Error);

    virtual ~CedarFilter();

    virtual void set_response(const string &r) throw(Error);
};

#endif // I_CedarFilter_h
