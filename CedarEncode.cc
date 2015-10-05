// CedarEncode.cc

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

#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using std::string ;
using std::strlen ;
using std::strncpy ;
using std::exit ;
using std::cerr ;
using std::cout ;
using std::endl ;

#include "CedarEncode.h"

char
CedarEncode::sap_bit( unsigned char val, int pos )
{
    int result ;
    if( ( pos > 7 ) ||( pos < 0 ) ) 
    {
	cerr << "pos not in [0,7]" << endl ;
	exit( 1 ) ;
    }
    switch( pos )
    {
	case 0:
	    result = ( val&1 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 1 ) return '\1' ;
	    break ;
	case 1:
	    result = ( val&2 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 2 ) return '\1' ;
	    break ;
	case 2:
	    result = ( val&4 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 4 ) return '\1' ;
	    break ;
	case 3:
	    result = ( val&8 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 8 ) return '\1' ;
	    break ;
	case 4:
	    result = ( val&16 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 16 ) return '\1' ;
	    break ;
	case 5:
	    result = ( val&32 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 32 ) return '\1' ;
	    break ;
	case 6:
	    result = ( val&64 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 64 ) return '\1' ;
	    break ;
	case 7:
	    result = ( val&128 ) ;
	    if( result == 0 ) return '\0' ;
	    else if( result == 128 ) return '\1' ;
	    break ;
    }
    return 0 ;
}

void
CedarEncode::my_encode( const char * text, const char *key, char *encoded_text )
{
    char bit_control[64] ;
    char key_control[64] ;
    int j = 0 ;
    for( int i = 0 ; i<64 ; i++ )
    {
	bit_control[i] = CedarEncode::sap_bit( text[i/8], j ) ;
	key_control[i] = CedarEncode::sap_bit( key [i/8], j ) ;
	if( ++j == 8 ) j = 0 ;
    }
    setkey( key_control ) ;
    encrypt( bit_control, 0 ) ;
    for( int j = 0 ; j<64 ; j++ )
    {
	if( bit_control[j] == '\0' ) encoded_text[j] = '0' ;
	else if( bit_control[j] == '\1' ) encoded_text[j] = '1' ;
    }
}

void
CedarEncode::my_decode( const char * encoded_text, const char *key,
                       char *decoded_text )
{
    char bit_control[64] ;
    char key_control[64] ;
    int j = 0 ;
    for( int i = 0 ; i<64 ; i++ )
    {
	key_control[i] = CedarEncode::sap_bit( key[i/8], j ) ;
	if( ++j == 8 ) j = 0 ;
	if( encoded_text[i] == '0' ) bit_control[i] = '\0' ;
	else if( encoded_text[i] == '1' ) bit_control[i] = '\1' ;
    }
    setkey( key_control ) ;
    encrypt( bit_control, 1 ) ;
    int pos0, pos1, pos2, pos3, pos4, pos5, pos6, pos7 ;
    pos0 = pos1 = pos2 = pos3 = pos4 = pos5 = pos6 = pos7 = 0 ;
    int k = 0 ;
    for( int j = 0 ; j<64 ; j+= 8 )
    {
	if( bit_control[j+0] == '\0' ) pos0 = 0 ;
	else if( bit_control[j+0] == '\1' ) pos0 = 1 ;
	if( bit_control[j+1] == '\0' ) pos1 = 0 ;
	else if( bit_control[j+1] == '\1' ) pos1 = 1 ;
	if( bit_control[j+2] == '\0' ) pos2 = 0 ;
	else if( bit_control[j+2] == '\1' ) pos2 = 1 ;
	if( bit_control[j+3] == '\0' ) pos3 = 0 ;
	else if( bit_control[j+3] == '\1' ) pos3 = 1 ;
	if( bit_control[j+4] == '\0' ) pos4 = 0 ;
	else if( bit_control[j+4] == '\1' ) pos4 = 1 ;
	if( bit_control[j+5] == '\0' ) pos5 = 0 ;
	else if( bit_control[j+5] == '\1' ) pos5 = 1 ;
	if( bit_control[j+6] == '\0' ) pos6 = 0 ;
	else if( bit_control[j+6] == '\1' ) pos6 = 1 ;
	if( bit_control[j+7] == '\0' ) pos7 = 0 ;
	else if( bit_control[j+7] == '\1' ) pos7 = 1 ;
	int total = ( 128*pos7 ) + ( 64*pos6 ) + ( 32*pos5 ) +
	            ( 16*pos4 ) + ( 8*pos3 ) + ( 4*pos2 ) +
		    ( 2*pos1 ) + ( 1*pos0 ) ;
	decoded_text[k++] = total ;
    }
}

void
CedarEncode::encode( const char *text, const char *key, char *encoded_text )
{
    // take the text and break it up into bits of 8
    // if size is less than 8 then pad with spaces
    if( strlen( text ) < 8 )
    {
	char new_text[9] ;
	int i = 0 ;
	for( ; i < 8; i++ )
	{
	    if( text[i] == '\0' )
	    {
		break ;
	    }
	    new_text[i] = text[i] ;
	}
	for( ; i < 8; i++ )
	{
	    new_text[i] = ' ' ;
	}
	new_text[i] = '\0' ;
	CedarEncode::my_encode( new_text, key, encoded_text ) ;
	encoded_text[64] = '\0' ;
    }
    else if( strlen( text ) > 8 )
    {
	CedarEncode::encode( text+8, key, encoded_text+64 ) ;
	char new_t[9] ;
	strncpy( new_t, text, 8 ) ;
	new_t[8] = '\0' ;
	CedarEncode::my_encode( new_t, key, encoded_text ) ;
    }
    else
    {
	CedarEncode::my_encode( text, key, encoded_text ) ;
	encoded_text[64] = '\0' ;
    }
}

void
CedarEncode::decode( const char * encoded_text, const char *key,
		    char *decoded_text )
{
    // take encoded_text. take 64 chars at a time and call my_decode with
    // it, contact results together
    string ets = encoded_text ;
    if( strlen( encoded_text ) > 64 )
    {
	CedarEncode::decode( encoded_text+64, key, decoded_text+8 ) ;
	char new_et[65] ;
	strncpy( new_et, encoded_text, 64 ) ;
	new_et[64] = '\0' ;
	CedarEncode::my_decode( new_et, key, decoded_text ) ;
    }
    else if( strlen( encoded_text ) < 64 )
    {
	decoded_text = 0 ;
	return ;
    }
    else
    {
	CedarEncode::my_decode( encoded_text, key, decoded_text ) ;
	decoded_text[8] = '\0' ;
	int i = 7 ;
	while( decoded_text[i] == ' ' )
	{
	    decoded_text[i] = '\0' ;
	    i-- ;
	}
    }
}

string
CedarEncode::encode( const string &text, const string &key )
{
    char b1[2048] ;
    CedarEncode::encode( text.c_str(), key.c_str(), b1 ) ;
    return b1 ;
}

string
CedarEncode::decode( const string &encoded_text, const string &key )
{
    char b2[2048] ;
    CedarEncode::decode( encoded_text.c_str(), key.c_str(), b2 ) ;
    return b2 ;
}

