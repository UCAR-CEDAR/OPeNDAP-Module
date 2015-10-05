// CedarMySQLQuery.h

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

#ifndef CedarMySQLQuery_h_
#define CedarMySQLQuery_h_ 1

#include "CedarMySQLResult.h"
#include "BESInternalError.h"

class CedarMySQLConnect ;

class CedarMySQLQuery
{
private:
    CedarMySQLConnect *_my_connection;
    CedarMySQLResult *_results;
    CedarMySQLQuery(const CedarMySQLQuery &){}   //disable copy constructor
    void empty_query()
    {
	string s = "No results loaded for this query" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
public:
    CedarMySQLQuery( const string &server, const string &user,
		    const string &password, const string &database,
		    int mysql_port, const string &mysql_sock );
    ~CedarMySQLQuery();

    void run_query(const string &query);
    string get_field()
    {
	if(!_results)
	    empty_query(); 
	return _results->get_field();
    }
    bool first_field()
    {
	if(!_results)
	    empty_query(); 
	return _results->first_field();
    }
    bool next_field()
    {
	if(!_results)
	    empty_query(); 
	return _results->next_field();
    }
    bool first_row()
    {
	if(!_results)
	    empty_query(); 
	return _results->first_row();
    }
    bool next_row()
    {
	if(!_results)
	    empty_query(); 
	return _results->next_row();
    }
    int get_nrows()
    {
	if(!_results)
	    empty_query();
	return _results->get_nrows();
    }
    int get_nfields()
    {
	if(!_results)
	    empty_query();
	return _results->get_nfields();
    }
    bool is_empty_set()
    {
	if(!_results)
	    empty_query();
	if( (_results->get_nfields()>0) && (_results->get_nrows()>0) )
	    return false;
	return true;
    }
};

#endif //   CedarMySQLQuery_h_

