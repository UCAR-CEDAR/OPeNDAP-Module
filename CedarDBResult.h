// CedarDBResult.h

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

#ifndef CedarDBResult_h_
#define CedarDBResult_h_ 1

#include <string>
#include <iostream>

using std::string ;
using std::ostream ;

#include "BESObj.h"

class CedarDBResult : public BESObj
{
private:
    				CedarDBResult( const CedarDBResult& )
				    : _nrows( 0 ),
				      _nfields( 0 ) {}
protected:
    int				_nrows ;
    int				_nfields ;
				CedarDBResult()
				    : _nrows( 0 ),
				      _nfields( 0 ) {}
    				CedarDBResult( const int n, const int f )
				    : _nrows( n ),
				      _nfields( f ) {}
public:
    				~CedarDBResult() {} ;

    virtual bool		first_row() = 0 ;
    virtual bool		next_row() = 0 ;
    virtual bool		is_empty_set() { return _nrows == 0 ; }
    virtual int			get_num_rows() { return _nrows ; }
    virtual int			get_num_fields() { return _nfields ; }
    virtual string		operator[]( const string &field ) = 0 ;
    virtual void		dump( ostream &strm ) const = 0 ;
} ;

#endif //CedarDBResult_h_

