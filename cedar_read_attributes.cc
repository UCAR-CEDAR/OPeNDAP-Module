// cedar_read_attributes.cc

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

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <string>
#include <new>
#include <memory>

using std::string ;
using std::bad_alloc ;
using std::auto_ptr ;

#include "CedarReadKinst.h"
#include "CedarReadParcods.h"
#include "cedar_read_attributes.h"
#include "CedarException.h"
#include "CedarFile.h"
#include "BESError.h"

static const char STRING[]="String";
static const char BYTE[]="Byte";
static const char INT16[]="Int16";
static const char FLOAT64[]="Float64";

bool cedar_read_attributes( DAS &das, const string &filename, string &error )
{
    try
    {
	CedarFile file;
	file.open_file (filename.c_str());
	const CedarLogicalRecord* lr=file.get_first_logical_record();
	if(lr)
	{
	    if(lr->get_type()==1) load_das(das, (CedarDataRecord*)lr);
	    while (!file.end_dataset())
	    {
		lr=file.get_next_logical_record();
		if(lr) 
		    if(lr->get_type()==1) load_das(das, (CedarDataRecord*)lr);
	    }
	    return true;
	}
	else
	{
	    error="Can not connect to file-> ";
	    error+=filename;
	    error+=" .Non existent file or not a cbf file.\n";
	    return false;
	}
    }
    catch (CedarException &cedarex)
    {
	error = "The requested dataset produces the following exception: ";
	error += cedarex.get_description() + (string)"\n" ;
	return false;
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
	return 0;
    }
    catch (...)
    {
	error="The requested dataset produces an unknown exception\n";
	return 0;
    }

}

int logged(int dat)
{
    static CedarArray<field>reg(1,0);
    int max_elem=reg.get_size();
    for (int i=0; i<max_elem-1;i++)
    {
	if (dat==reg[i])
	    return 1;
    }
    reg.set_size(max_elem+1);
    reg[max_elem-1]=dat;
    return 0;
}

void load_das(DAS &das,CedarDataRecord *dr)
{
    if (!logged(dr->get_record_kind_data()))
    { 
	char tmp[100];
	string name = "" ;
	string info = "" ;
	string type = "String" ;
	string str = "Data_Descriptor_for_KINDAT_" ;
	CedarStringConversions::ltoa(dr->get_record_kind_data(),tmp,10);
	str+=tmp;
	str+="_KINST_";
	CedarStringConversions::ltoa(dr->get_record_kind_instrument(),tmp,10);
	str+=tmp;
	AttrTable *at=new AttrTable(); 
	AttrTable *t1;
	t1=at->append_container("KINST"); 
	// The KINST attribute table will contain the KINST, INST_NAME,
	// PREFIX, LATITUDE, LONGITUDE, and ALTITUDE.

	// KINST
	int kinst = dr->get_record_kind_instrument() ;
	string skinst = CedarReadKinst::Get_Kinst_as_String( kinst ) ;
	info="\"" + skinst + "\"" ;
	t1->append_attr( "KINST", type, info ) ;

	// instrument name
	info = "\"" + CedarReadKinst::Get_Name( kinst ) + "\"" ;
	t1->append_attr( "NAME", type, info ) ;

	// instrument prefix
	info = "\"" + CedarReadKinst::Get_Prefix( kinst ) + "\"" ;
	t1->append_attr( "PREFIX", type, info ) ;

	// latitude
	info = "\"" + CedarReadKinst::Get_Latitude_as_String( kinst ) + "\"" ;
	t1->append_attr( "LATITUDE", type, info ) ;

	// longitude
	info = "\"" + CedarReadKinst::Get_Longitude_as_String( kinst ) + "\"" ;
	t1->append_attr( "LONGITUDE", type, info ) ;

	// altitude
	info = "\"" + CedarReadKinst::Get_Altitude_as_String( kinst ) + "\"" ;
	t1->append_attr( "ALTITUDE", type, info ) ;

	// get jpar attributes
	int njpar=dr->get_jpar();
	auto_ptr<vector<short int> > jparvars( new vector<short int>(njpar));
	if (jparvars.get())
	{
	    // For each of the jpar parameters there will be the code,
	    // longname, scale, and units.
	    dr->load_JPAR_vars(*jparvars);
	    for (int i=0; i<njpar; i++)
	    {
		string nm="JPAR_";
		CedarStringConversions::ltoa(i,tmp,10);
		nm+=tmp;
		t1=at->append_container(nm);

		int code = (*jparvars.get())[i] ;

		// get the parameter code
		string scode = CedarReadParcods::Get_Code_as_String( code ) ;
		info = "\"" + scode + "\"" ;
		t1->append_attr( "CODE", type, info ) ;

		// short name
		string sname = CedarReadParcods::Get_Shortname( code ) ;
		info = "\"" + sname + "\"" ;
		t1->append_attr( "SHORTNAME", type, info ) ;

		// long name
		string lname = CedarReadParcods::Get_Longname( code ) ;
		info = "\"" + lname + "\"" ;
		t1->append_attr( "LONGNAME", type, info ) ;

		// scale
		string scale = CedarReadParcods::Get_Scale( code ) ;
		info = "\"" + scale + "\"" ;
		t1->append_attr( "SCALE", type, info ) ;

		// units
		string unit = CedarReadParcods::Get_Unit_Label( code ) ;
		info = "\"" + unit + "\"" ;
		t1->append_attr( "UNIT", type, info ) ;

		/*
		int val;
		if ((*jparvars.get())[i]<0)
		{
		    val=(*jparvars.get())[i]*-1;
		    info="\"Error in ";
		    name="ERROR_IN_PARAMETER_CODE_";
		}
		else
		{
		    val=(*jparvars.get())[i];
		    info="\"";
		    name="PARAMETER_CODE_";
		}
		info+=CedarReadParcods::Get_Longname(val);
		info+=" ";
		info+=CedarReadParcods::Get_Scale(val);
		info+=" ";
		info+=CedarReadParcods::Get_Unit_Label(val);
		info+="\"";
		CedarStringConversions::ltoa(val,tmp,10);
		name+=tmp;
		t1->append_attr(name, type,info);
		*/
	    }
	}

	// mpar parameter attributes
	int nmpar=dr->get_mpar();
	auto_ptr<vector<short int> > mparvars( new vector<short int>(nmpar) );
	if (mparvars.get())
	{
	    dr->load_MPAR_vars(*mparvars);
	    for (int i=0; i<nmpar; i++)
	    {
		string nm="MPAR_";
		CedarStringConversions::ltoa(i,tmp,10);
		nm+=tmp;
		t1=at->append_container(nm);

		int code = (*mparvars.get())[i] ;

		// get the parameter code
		string scode = CedarReadParcods::Get_Code_as_String( code ) ;
		info = "\"" + scode + "\"" ;
		t1->append_attr( "CODE", type, info ) ;

		// short name
		string sname = CedarReadParcods::Get_Shortname( code ) ;
		info = "\"" + sname + "\"" ;
		t1->append_attr( "SHORTNAME", type, info ) ;

		// long name
		string lname = CedarReadParcods::Get_Longname( code ) ;
		info = "\"" + lname + "\"" ;
		t1->append_attr( "LONGNAME", type, info ) ;

		// scale
		string scale = CedarReadParcods::Get_Scale( code ) ;
		info = "\"" + scale + "\"" ;
		t1->append_attr( "SCALE", type, info ) ;

		// units
		string unit = CedarReadParcods::Get_Unit_Label( code ) ;
		info = "\"" + unit + "\"" ;
		t1->append_attr( "UNIT", type, info ) ;

		/*
		int val;
		if ((*mparvars.get())[i]<0)
		{
		    val=(*mparvars.get())[i]*-1;
		    info="\"Error in ";
		    name="ERROR_IN_PARAMETER_CODE_";
		}
		else
		{
		    val=(*mparvars.get())[i];
		    info="\"";
		    name="PARAMETER_CODE_";
		}
		info+=CedarReadParcods::Get_Longname(val);
		info+=" ";
		info+=CedarReadParcods::Get_Scale(val);
		info+=" ";
		info+=CedarReadParcods::Get_Unit_Label(val);
		info+="\"";
		CedarStringConversions::ltoa(val,tmp,10);
		name+=tmp;
		t1->append_attr(name, type,info);
		*/
	    }
	}
	das.add_table(str, at);
    }
}

