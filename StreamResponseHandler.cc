// StreamResponseHandler.cc

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

#include "StreamResponseHandler.h"
#include "BESRequestHandlerList.h"

StreamResponseHandler::StreamResponseHandler( const string &name )
    : BESResponseHandler( name )
{
}

StreamResponseHandler::~StreamResponseHandler( )
{
}

void
StreamResponseHandler::execute( BESDataHandlerInterface &dhi )
{
    _response = 0 ;
    BESRequestHandlerList::TheList()->execute_each( dhi ) ;
}

void
StreamResponseHandler::transmit( BESTransmitter *transmitter,
                                 BESDataHandlerInterface & )
{
    // The Data is transmitted when it is read, dumped to stdout
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance and calls dump on the parent
 * class.
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
StreamResponseHandler::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "StreamResponseHandler::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    BESResponseHandler::dump( strm ) ;
    BESIndent::UnIndent() ;
}

BESResponseHandler *
StreamResponseHandler::StreamResponseBuilder( const string &handler_name )
{
    return new StreamResponseHandler( handler_name ) ;
}

