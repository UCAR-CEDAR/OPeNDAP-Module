// ContainerStorageCedar.h

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

#ifndef I_ContainerStorageCedar_h_
#define I_ContainerStorageCedar_h_ 1

#include <string>

using std::string ;

#include "BESContainerStorage.h"

class ContainerStorageCedar : public BESContainerStorage
{
private:
    string			_cedar_base ;
    string			_madrigal_base ;
public:
    				ContainerStorageCedar( const string &n);
    virtual			~ContainerStorageCedar() ;

    virtual BESContainer * 	look_for( const string &sym_name ) ;
    virtual void		add_container( const string &s_name,
                                               const string &r_ame,
					       const string &type ) ;
    virtual bool		del_container( const string &s_name ) ;
    virtual bool		del_containers( ) ;
    virtual bool		isData( const string &inQuestion,
    					list<string> &provides ) ;
    virtual void		show_containers( BESInfo &info ) ;

    virtual void		dump( ostream &strm ) const ;
};

#endif // I_ContainerStorageCedar_h_

