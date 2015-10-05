// CedarMySQLResult.cc

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

#include <sstream>
#include <iostream>

using std::ostringstream ;
using std::endl ;

#include "CedarMySQLResult.h"
#include "BESInternalError.h"
#include "BESDebug.h"

CedarMySQLResult::CedarMySQLResult( const int n, const int f,
				    const vector<string> &fields )
    : CedarDBResult( n, f ),
      _matrix( 0 ),
      _fields( fields ),
      _row_position( 0 )
{
    _matrix = new matrix ;
    _matrix->resize( _nrows ) ;
}

CedarMySQLResult::~CedarMySQLResult()
{
    delete _matrix;
}

void
CedarMySQLResult::set_row( const vector<string> &fields )
{
    if( fields.size() != _fields.size() )
    {
	ostringstream strm ;
	strm << "Adding row to DB Result, sent " << fields.size()
	     << " fields, expected " << _fields.size() << " fields" ;
	throw BESInternalError( strm.str(), __FILE__, __LINE__ ) ;
    }

    row new_row ;
    vector<string>::const_iterator pi = fields.begin() ;
    vector<string>::const_iterator pe = fields.end() ;
    vector<string>::const_iterator i = _fields.begin() ;
    for( ; pi != pe; pi++, i++ )
    {
	new_row[*i] = *pi ;
    }
    (*_matrix)[_row_position] = new_row ;
}

bool
CedarMySQLResult::first_row()
{
    if( _nrows > 0 )
    {
	_row_position = 0 ;
	return true ;
    }
    return false ;
}

bool
CedarMySQLResult::next_row()
{
    if( ++_row_position < _nrows )
    {
	return true ;
    }
    return false ;
}

string
CedarMySQLResult::operator[]( const string &field )
{
    if( _row_position < _nrows )
	return (*_matrix)[_row_position][field] ;
    return "" ;
}

void
CedarMySQLResult::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "BESMySQLResult::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;

    strm << BESIndent::LMarg << "num_rows : " << _nrows << endl ;
    strm << BESIndent::LMarg << "num_fields : " << _nfields << endl ;
    strm << BESIndent::LMarg << "fields:" << endl ;
    BESIndent::Indent() ;
    int num_fields = _fields.size() ;
    for( int f = 0; f < num_fields; f++ )
    {
	strm << BESIndent::LMarg << _fields[f] << endl ;
    }
    BESIndent::UnIndent() ;

    strm << BESIndent::LMarg << "rows:" << endl ;
    BESIndent::Indent() ;
    for( int i = 0; i < _nrows; i++ )
    {
	strm << BESIndent::LMarg << "[" << i << "]: " ;
	row current_row = (*_matrix)[i] ;
	for( int f = 0; f < num_fields; f++ )
	{
	    string field = _fields[f] ;
	    if( f )
	    {
		strm << ", " ;
	    }
	    strm << current_row[field] ;
	}
	strm << endl ;
    }
    BESIndent::UnIndent() ;
    BESIndent::UnIndent() ;
}

