// CedarAuthenticateException.cc

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

#include "CedarAuthenticateException.h"
#include "BESDataNames.h"
#include "BESInfo.h"
#include "BESInfoList.h"
#include "TheBESKeys.h"
#include "config_cedar.h"

#define DEFAULT_ADMINISTRATOR "cedar_db@hao.ucar.edu"

int
CedarAuthenticateException::handleAuthException( BESError &e,
						 BESDataHandlerInterface &dhi )
{
    CedarAuthenticateException *ae =
	dynamic_cast<CedarAuthenticateException*>(&e);

    if( ae )
    {
	dhi.error_info = BESInfoList::TheList()->build_info() ;
	if( dhi.error_info )
	{
	    dhi.error_info->add_data_from_file( "Cedar.LoginScreen",
						"Login Failed" ) ;
	    string administrator = "" ;
	    try
	    {
		bool found = false ;
		TheBESKeys::TheKeys()->get_value( "BES.ServerAdministrator",
						  administrator, found ) ;
	    }
	    catch( ... )
	    {
		administrator = DEFAULT_ADMINISTRATOR ;
	    }
	    dhi.error_info->add_tag( "Error Encountered", ae->get_message() ) ;
	    dhi.error_info->add_break( 1 ) ;
	    string admin_email = "<A HREF=\"mailto:" + administrator + "\">"
	                         + administrator + "</A>" ;
	    dhi.error_info->add_tag( "If you continue to have problems contact",
				     admin_email ) ;
	    dhi.error_info->add_data( "</TD></TR></TABLE>" ) ;
	}
	return CEDAR_AUTHENTICATE_EXCEPTION ;
    }
    return 0 ;

    /*
    if( ae )
    {
	bool ishttp = false ;
	if( dhi.transmit_protocol == "HTTP" )
	    ishttp = true ;

	if( ishttp )
	{
	    set_mime_html( stdout, dods_error ) ;
	    fprintf( stdout, "<HTML>" ) ;
	    fprintf( stdout, "<HEAD></HEAD>" ) ;
	    fprintf( stdout, "<BODY BACKGROUND='http://cedarweb.hao.ucar.edu/images/Texture_lt_gray_004.jpg'>") ;
	    fprintf( stdout, "<TABLE BACKGROUND='http://cedarweb.hao.ucar.edu/images/Texture_lt_gray_004.jpg' BORDER='0' WIDTH='100%%' CELLPADDING='1' CELLSPACING='0'>" ) ;
	    fprintf( stdout, "<TR>" ) ;
	    fprintf( stdout, "<TD WIDTH='20%%' BACKGROUND='http://cedarweb.hao.ucar.edu/images/Texture_lt_gray_004.jpg'>" ) ;
	    fprintf( stdout, "<P ALIGN='center'>" ) ;
	    fprintf( stdout, "<A HREF='http://www.ucar.edu' TARGET='_blank'><IMG SRC='http://cedarweb.hao.ucar.edu/images/CedarwebUCAR.gif' ALT='UCAR' BORDER='0'><BR><FONT SIZE='2'>UCAR</FONT></A>" ) ;
	    fprintf( stdout, "</P>" ) ;
	    fprintf( stdout, "</TD>" ) ;
	    fprintf( stdout, "<TD WIDTH='80%%' BACKGROUND='http://cedarweb.hao.ucar.edu/images/Texture_lt_gray_004.jpg'>" ) ;
	    fprintf( stdout, "<P ALIGN='center'>" ) ;
	    fprintf( stdout, "<IMG BORDER='0' SRC='http://cedarweb.hao.ucar.edu/images/Cedarweb.jpg' ALT='CEDARweb'>" ) ;
	    fprintf( stdout, "</P>" ) ;
	    fprintf( stdout, "</TD>" ) ;
	    fprintf( stdout, "</TR>" ) ;
	    fprintf( stdout, "</TABLE>" ) ;
	    fprintf( stdout, "<BR />" ) ;
	    fprintf( stdout, "<BR />" ) ;
	    fprintf( stdout, "%s %s %s.\n",
	             "We were unable to authenticate your session",
		     "for user",
		     dhi.data[USER_NAME].c_str() ) ;
	    fprintf( stdout, "<BR />\n" ) ;
	    fprintf( stdout, "<BR />\n" ) ;
	    fprintf( stdout, "%s\n", e.get_message().c_str() ) ;
	    fprintf( stdout, "<BR />\n" ) ;
	    fprintf( stdout, "<BR />\n" ) ;
	    fprintf( stdout, "Please follow <A HREF=\"https://cedarweb.hao.ucar.edu:%d/cgi-bin/ion-p?page=login.ion\" TARGET=\"NEW\">this link</A> to login.\n", HTTPS_PORT ) ;
 	    fprintf( stdout, "Then refresh this page to get your data once you have logged in\n" ) ;
	    fprintf( stdout, "</BODY></HTML>" ) ;
	}
	else
	{
	    if( dhi.error_info )
	    {
		dhi.error_info->add_exception( "Authentication", e ) ;
		dhi.error_info->end_response() ;
	    }
	}
	return CEDAR_AUTHENTICATE_EXCEPTION ;
    } 
    */
}

