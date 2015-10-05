// CedarEncode.h

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

#ifndef I_CedarEncode_h
#define I_CedarEncode_h 1

#include <string>

using std::string ;

class CedarEncode
{
private:
    static char sap_bit(unsigned char val, int pos) ;
    static void encode( const char * text, const char *key,
		        char *encoded_text ) ;
    static void my_encode( const char * text, const char *key,
                           char *encoded_text ) ;
    static void decode( const char * encoded_text, const char *key,
		        char *decoded_text ) ;
    static void my_decode( const char * encoded_text, const char *key,
                           char *decoded_text ) ;

public:
    // text MUST be buffer of 8 bytes, key MUST be buffer with 8 bytes,
    // encoded_text MUST be buffer with 64 bytes
    // not buffer overflow check is done!
    static string encode( const string &text, const string &key ) ;

    // text MUST be buffer of 64 bytes, key MUST be buffer with 8 bytes, 
    // decoded_text MUST be buffer with 8 bytes
    // no buffer overflow check is done!
    // encoded_text MUST have only 0s and 1s, otherwise behavior is not 
    // predicted.
    static string decode( const string &encoded_text, const string &key ) ;
} ;

#endif // I_CedarEncode_h

