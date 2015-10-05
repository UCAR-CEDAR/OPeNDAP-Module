// cedar_read_descriptors.cc

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

#include <memory>
#include <vector>

using std::auto_ptr ;
using std::vector ;

#include <mime_util.h>

#include "CedarReadParcods.h"
#include "CedarConstraintEvaluator.h"
#include "Array.h"
#include "Structure.h"
#include "Int16.h"
#include "UInt16.h"

#include "cedar_read_descriptors.h"

#include "CedarException.h"
#include "BESError.h"
#include "TheBESKeys.h"

bool cedar_read_descriptors( DDS &dds, const string &filename,
                             const string &name, const string &query,
			     string &error )
{
    int i=0;
    CedarConstraintEvaluator qa;
    auto_ptr<Structure> container( new Structure( name ) ) ;

    try
    {
	qa.parse(query.c_str());
    }
    catch (CedarException &ex)
    {
	error=ex.get_description();
	return false;
    }

    try
    {
	CedarFile file;
	file.open_file (filename.c_str());
	const CedarLogicalRecord* lr=file.get_first_logical_record();
	if(lr)
	{
	    if( lr->get_type() == 1 )
	    {
		load_dds( *(container.get()), (CedarDataRecord*)lr, qa, i ) ;
	    }
	    while (!file.end_dataset())
	    {
		lr = file.get_next_logical_record();
		if(lr) 
		{
		    if( lr->get_type() == 1 )
		    {
			load_dds( *(container.get()), (CedarDataRecord*)lr, qa, i ) ;
		    }
		}
	    }
	}
	else
	{
	    error = "Can not connect to file-> " ;
	    error += filename ;
	    error += " .Non existent file or no a cbf file.\n" ;
	    return false ;
	}
    }
    catch (CedarException &cedarex)
    {
	error = "The requested dataset produced the following exception: " ;
	error += cedarex.get_description() + (string)"\n" ;
	return false;
    }
    catch( BESError &beserr )
    {
	error = "The requested dataset produced the following exception: " ;
	error += beserr.get_message() + (string)"\n" ;
	return false ;
    }
    catch (bad_alloc::bad_alloc)
    {
	error = "There has been a memory allocation error.\n" ;
	return false ;
    }
    catch (...)
    {
	error = "The requested dataset produces an unknown exception\n" ;
	return false ;
    }


    BaseType *bt = container.get() ;
    dds.add_var( bt ) ;
    dds.set_dataset_name(name_path(filename));

    return true;
}

void get_name_for_parameter(string &str, int par)
{
    str="";
    if (par<0)
    {
	par=par*-1;
	str="e_";
    }
    string madnam=CedarReadParcods::Get_Madrigalname(par);
    if (madnam=="")
    {
	madnam="var_";
	char tmp[100];
	CedarStringConversions::ltoa(par,tmp,10);
	madnam+=tmp;
    }
    str+=madnam;
}
  
void load_dds( Structure &container, CedarDataRecord *my_data_record,
	       CedarConstraintEvaluator &qa, int &i )
{
    if( qa.validate_record( my_data_record ) )
    {
	i++;
	char stuyo [5];
	CedarStringConversions::ltoa(i,stuyo,10);
	string record_name = "data_record_";
	record_name+=stuyo;

	// BEGIN HERE LOADING PROLOGUE
	auto_ptr<Int16> pKINST (new Int16("KINST"));
	pKINST->set_value(my_data_record->get_record_kind_instrument());
	auto_ptr<Int16> pKINDAT(new Int16("KINDAT"));
	pKINDAT->set_value(my_data_record->get_record_kind_data());
	CedarDate bdate,edate;
	my_data_record->get_record_begin_date(bdate);
	my_data_record->get_record_end_date(edate);
	auto_ptr<UInt16> pIBYRT(new UInt16 ("IBYRT"));
	pIBYRT->set_value(bdate.get_year());
	auto_ptr<UInt16> pIBDTT (new UInt16 ("IBDTT")); 
	pIBDTT->set_value(bdate.get_month_day());
	auto_ptr<UInt16> pIBHMT (new UInt16 ("IBHMT")); 
	pIBHMT->set_value(bdate.get_hour_min());
	auto_ptr<UInt16> pIBCST (new UInt16 ("IBCST"));
	pIBCST->set_value(bdate.get_second_centisecond());
	auto_ptr<UInt16> pIEYRT (new UInt16 ("IEYRT"));
	pIEYRT->set_value(edate.get_year());
	auto_ptr<UInt16> pIEDTT (new UInt16 ("IEDTT"));
	pIEDTT->set_value(edate.get_month_day());
	auto_ptr<UInt16> pIEHMT (new UInt16 ("IEHMT"));
	pIEHMT->set_value(edate.get_hour_min());
	auto_ptr<UInt16> pIECST (new UInt16 ("IECST"));
	pIECST->set_value(edate.get_second_centisecond());
	auto_ptr<Structure> pPROLOGUEstructure (new Structure ("prologue"));
	pPROLOGUEstructure -> add_var (pKINST.get());
	pPROLOGUEstructure -> add_var (pKINDAT.get());
	pPROLOGUEstructure -> add_var (pIBYRT.get());
	pPROLOGUEstructure -> add_var (pIBDTT.get());
	pPROLOGUEstructure -> add_var (pIBHMT.get());
	pPROLOGUEstructure -> add_var (pIBCST.get());
	pPROLOGUEstructure -> add_var (pIEYRT.get());
	pPROLOGUEstructure -> add_var (pIEDTT.get());
	pPROLOGUEstructure -> add_var (pIEHMT.get());
	pPROLOGUEstructure -> add_var (pIECST.get());
	// END HERE LOADING PROLOGUE

	// BEGIN HERE LOADING JPAR SECTION
	unsigned int jpar_value=my_data_record->get_jpar();
	auto_ptr<vector<dods_int16> > pJparData(new vector<dods_int16>(jpar_value));
	my_data_record->load_JPAR_data(*pJparData);
	auto_ptr<vector<dods_int16> > pJparVars(new vector<dods_int16>(jpar_value));
	my_data_record->load_JPAR_vars(*pJparVars);
	auto_ptr<Structure>  pJPARstructure (new Structure ("JPAR"));
	string JparVarName;
	int is_JPAR_empty=1;
	for (unsigned int w=0; w<jpar_value; w++)
	{ 
	    if (qa.validate_parameter((*pJparVars.get())[w]))
	    {
		is_JPAR_empty=0;
		get_name_for_parameter(JparVarName,(*pJparVars.get())[w]);
		auto_ptr<Int16> pjpardata (new Int16 (JparVarName));
		pjpardata->set_value((*pJparData.get())[w]);
		pJPARstructure->add_var(pjpardata.get());
	    }
	}
	// END HERE LOADING JPAR SECTION

	// BEGIN HERE LOADING MPAR SECTION
	auto_ptr<Structure> pMPARstructure (new Structure("MPAR"));
	int mpar_value=my_data_record->get_mpar();
	int nrow_value=my_data_record->get_nrows();
	int is_MPAR_empty=1;
	if ((mpar_value>0) && (nrow_value>0))
	{
	    auto_ptr<vector<dods_int16> > pMparVars (new vector<dods_int16>(mpar_value));
	    my_data_record->load_MPAR_vars(*pMparVars);
	    auto_ptr<vector<dods_int16> > pMparData (new vector<dods_int16>(nrow_value*mpar_value));
	    auto_ptr<vector<dods_int16> > pPartialMparData (new vector<dods_int16>(nrow_value));
	    my_data_record->load_MPAR_data(*pMparData);
	    string MparVarName;
	    for (int j=0; j<mpar_value;j++)
	    {
		if (qa.validate_parameter((*pMparVars.get())[j]))
		{
		    is_MPAR_empty=0;
		    get_name_for_parameter(MparVarName,(*pMparVars.get())[j]);
		    BaseType *pMparvar = new Int16( MparVarName ); 
		    auto_ptr<Array> pmpararray( new Array( MparVarName, pMparvar ) ) ;
		    delete pMparvar ; pMparvar = 0 ;
		    pmpararray->append_dim(nrow_value);
		    for (int w=0; w<nrow_value;w++)
		    {
			(*pPartialMparData)[w]=(*pMparData)[j+(w*mpar_value)];
		    }
		    pmpararray->set_value(*pPartialMparData,nrow_value);
		    pMPARstructure -> add_var(pmpararray.get());
		}
	    }
	}
	// END HERE LOADING MPAR SECTION

	auto_ptr<Structure> precord (new Structure(record_name));
	precord -> add_var (pPROLOGUEstructure.get());
	if (!is_JPAR_empty)
	    precord->add_var(pJPARstructure.get());
	if (!is_MPAR_empty)
	    precord->add_var(pMPARstructure.get());
	container.add_var(precord.get());
    }
}

