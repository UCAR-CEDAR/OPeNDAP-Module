// CedarModule.cc

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

using std::endl ;

#include "CedarModule.h"
#include <BESRequestHandlerList.h>
#include "CedarRequestHandler.h"
#include <BESResponseHandlerList.h>
#include "FlatResponseHandler.h"
#include "TabResponseHandler.h"
#include "StreamResponseHandler.h"
#include "InfoResponseHandler.h"
#include <BESResponseNames.h>
#include <BESTransmitterNames.h>
#include "CedarResponseNames.h"
#include <BESReporterList.h>
#include "CedarReporter.h"
#include <BESInterface.h>
#include <BESExceptionManager.h>
#include "CedarAuthenticateException.h"
#include "ContainerStorageCedar.h"
#include <BESContainerStorageList.h>
#include "CedarMySQLDB.h"
#include <BESDapService.h>
#include <BESServiceRegistry.h>
#include <BESReturnManager.h>

#include "BESDebug.h"

void
CedarModule::initialize( const string &modname )
{
    BESDEBUG( "cedar", "Initializing Cedar module " << modname << endl ) ;

    BESDEBUG( "cedar", "    adding mysql database" << endl ) ;
    CedarDB::Add_DB_Builder( "mysql", CedarMySQLDB::BuildMySQLDB ) ;

    BESDEBUG( "cedar", "    adding cedar debug context" << endl ) ;
    BESDebug::Register( "cedar" ) ;

    BESDEBUG( "cedar", modname << " handles dap services" << endl ) ;
    BESDapService::handle_dap_service( modname ) ;

    BESDEBUG( "cedar", "Adding " << CEDAR_SERVICE << " services:" << endl ) ;
    BESServiceRegistry *registry = BESServiceRegistry::TheRegistry() ;
    registry->add_service( CEDAR_SERVICE ) ;
    registry->add_to_service( CEDAR_SERVICE, FLAT_SERVICE,
			      FLAT_DESCRIPT, BASIC_TRANSMITTER ) ;
    registry->add_to_service( CEDAR_SERVICE, TAB_SERVICE,
			      TAB_DESCRIPT, BASIC_TRANSMITTER ) ;
    registry->add_to_service( CEDAR_SERVICE, INFO_SERVICE,
			      INFO_DESCRIPT, BASIC_TRANSMITTER ) ;
    registry->add_to_service( CEDAR_SERVICE, STREAM_SERVICE,
			      STREAM_DESCRIPT, BASIC_TRANSMITTER ) ;
    registry->handles_service( modname, CEDAR_SERVICE ) ;

    BESDEBUG( "cedar", "    adding " << modname <<
		       " request handler" << endl ) ;
    BESRequestHandlerList::TheList()->add_handler( modname, new CedarRequestHandler( modname ) ) ;

    BESDEBUG( "cedar", "    adding " << FLAT_RESPONSE
		       << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->add_handler( FLAT_RESPONSE, FlatResponseHandler::FlatResponseBuilder ) ;

    BESDEBUG( "cear", "    adding " << TAB_RESPONSE
		      << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->add_handler( TAB_RESPONSE, TabResponseHandler::TabResponseBuilder ) ;

    BESDEBUG( "cedar", "    adding " << STREAM_RESPONSE
		       << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->add_handler( STREAM_RESPONSE, StreamResponseHandler::StreamResponseBuilder ) ;

    BESDEBUG( "cear", "    adding " << INFO_RESPONSE
		      << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->add_handler( INFO_RESPONSE, InfoResponseHandler::InfoResponseBuilder ) ;

    BESDEBUG( "cedar", "    adding Cedar reporter" << endl ) ;
    BESReporterList::TheList()->add_reporter( modname, new CedarReporter ) ;

    BESDEBUG( "cedar", "    adding Cedar auth exception callback" << endl ) ;
    BESExceptionManager::TheEHM()->add_ehm_callback( CedarAuthenticateException::handleAuthException ) ;

    BESDEBUG( "cedar", "    adding Cedar Persistence" << endl ) ;
    ContainerStorageCedar *cpf = new ContainerStorageCedar( "Cedar" ) ;
    BESContainerStorageList::TheList()->add_persistence( cpf ) ;

    BESDEBUG( "cedar", "Done Initializing Cedar module " << modname << endl ) ;
}

void
CedarModule::terminate( const string &modname )
{
    BESDEBUG( "cedar", "Cleaning NC module " << modname << endl ) ;

    BESDEBUG( "cedar", "    removing Cedar handler" << modname << endl ) ;
    BESRequestHandler *rh = BESRequestHandlerList::TheList()->remove_handler( modname ) ;
    if( rh ) delete rh ;

    BESDEBUG( "cedar", "    removing " << FLAT_RESPONSE
		       << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->remove_handler( FLAT_RESPONSE ) ;
    BESDEBUG( "cedar", "    removing " << TAB_RESPONSE
		       << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->remove_handler( TAB_RESPONSE ) ;
    BESDEBUG( "cedar", "    removing " << STREAM_RESPONSE
		       << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->remove_handler( STREAM_RESPONSE ) ;
    BESDEBUG( "cedar", "    removing " << INFO_RESPONSE
		       << " response handler" << endl ) ;
    BESResponseHandlerList::TheList()->remove_handler( INFO_RESPONSE ) ;

    BESDEBUG( "cedar", "    removing Cedar reporter" << endl ) ;
    BESReporter *r = BESReporterList::TheList()->remove_reporter( modname ) ;
    if( r ) delete r ;

    /* no way to remove this
    BESDEBUG( "cedar", "    adding Cedar authenticate exception callback" << endl ) ;
    BESExceptionManager::TheEHM()->add_ehm_callback( CedarAuthenticateException::handleAuthException ) ;
    */

    BESDEBUG( "cedar", "    adding Cedar Persistence" << endl ) ;
    BESContainerStorageList::TheList()->deref_persistence( "Cedar" ) ;

    BESDEBUG( "cedar", "    closing databases" << endl ) ;
    CedarDB::Close() ;

    BESDEBUG( "cedar", "Done Cleaning NC module " << modname << endl ) ;
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
CedarModule::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "CedarModule::dump - ("
			     << (void *)this << ")" << endl ;
}

extern "C"
{
    BESAbstractModule *maker()
    {
	return new CedarModule ;
    }
}

