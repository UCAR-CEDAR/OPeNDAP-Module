// cedar_read_info.cc

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

#include <string>
#include <sstream>

using std::string ;
using std::ostringstream ;
using std::bad_alloc ;

#include "CedarBlock.h"
#include "CedarFile.h"
#include "CedarVersion.h"
#include "CedarErrorHandler.h"
#include "cedar_read_info.h"
#include "CedarException.h"
#include "CedarConstraintEvaluator.h"
#include "BESError.h"

void
route_buffer( const CedarLogicalRecord *pLogRec, BESInfo &info )
{
    ostringstream *oss = new ostringstream() ;
    switch( pLogRec->get_type() )
    {
	case 2:
	    (*oss) << "\n" ;
	    pLogRec->save_prologue_header( oss->rdbuf() ) ;
	    pLogRec->save_prologue( oss->rdbuf() ) ;
	    pLogRec->save_data_buffer( oss->rdbuf() ) ;
	    break;
	case 3:
	    (*oss) << "\n" ;
	    pLogRec->save_prologue_header( oss->rdbuf() ) ;
	    pLogRec->save_prologue( oss->rdbuf() ) ;
	    pLogRec->save_data_buffer( oss->rdbuf() ) ;
	    break ;
    }
    //(*oss) << '\0' ;
    info.add_data( oss->str() ) ;
    delete oss ;
}

inline void
deal_with_record( CedarBlock &block,
                  CedarConstraintEvaluator &qa,
		  BESInfo &info )
{
    const CedarLogicalRecord *pLogRec = block.get_first_record() ;
    if( pLogRec )
    {
	if( qa.validate_record( pLogRec ) )
	route_buffer( pLogRec, info ) ;
	pLogRec = block.get_next_record() ;
	while( pLogRec )
	{
	    if( qa.validate_record( pLogRec ) )
		route_buffer( pLogRec, info ) ;
	    pLogRec = block.get_next_record() ;
	}
    }
}

bool
cedar_read_info( BESInfo &info, const string &filename,
                 const string &name, const string &query, string &error )
{
    CedarConstraintEvaluator qa ;
    try
    {
	qa.parse( query.c_str() ) ;
    }
    catch( CedarException &ex )
    {
	error = ex.get_description() ;
	return false ;
    }

    CedarFile file ;
    try
    {
	info.add_data( "CATALOG AND HEADER RECORD CORRESPONDING TO: " ) ;
	info.add_data( name ) ;
	info.add_data( "\n" ) ;
	file.open_file( filename.c_str() ) ;
	const CedarLogicalRecord *lr = file.get_first_logical_record() ;

	if( lr )
	{
	    if( qa.validate_record( lr ) )
		route_buffer( lr, info ) ;
	    while( !file.end_dataset() )
	    {
		lr = file.get_next_logical_record() ;
		if( lr )
		    if( qa.validate_record( lr ) )
			route_buffer( lr, info ) ;
	    }
	}
	else 
	{
	    error = (string)"Failure reading data from file "
		    + filename
		    + ", corrupted file or not a cbf file.\n" ;
	    return false ;
	}
    }
    catch( CedarException &ex )
    {
	error = "The requested dataset produces the following exception: " ;
	error += ex.get_description() + (string)"\n" ;
	return false ;
    }
    catch( BESError &beserr )
    {
	error = "The requested dataset produces the following exception: " ;
	error += beserr.get_message() + (string)"\n" ;
	return false ;
    }
    catch (bad_alloc::bad_alloc)
    {
	error="There has been a memory allocation error.\n";
	return false;
    }
    catch (...)
    {
	error="The requested dataset produces an unknown exception\n";
	return false;
    }

    return true ;
}

