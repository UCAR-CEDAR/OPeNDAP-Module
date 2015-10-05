// cedar_read_tab.cc

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

#include <exception>
#include <cstddef>
//#include <new>
#include <sstream>
#include <memory>

using std::ostringstream ;
using std::auto_ptr ;

#include "CedarFile.h"
#include "CedarTab.h"
#include "cedar_read_tab.h"
#include "CedarException.h"
#include "cedar_read_descriptors.h"
#include "cedar_read_tab_support.h"
#include "CedarParameter.h"
#include "CedarConstraintEvaluator.h"
#include "BESError.h"

struct _printable_parameter
{
    field par_value;
    bool is_valid;
};

typedef _printable_parameter printable_parameter;

void send_tab_data(CedarTab &tab_object, CedarDataRecord &dr, CedarConstraintEvaluator &qa)
{
    if (qa.validate_record(dr))
    {
	ostringstream oss;
	unsigned int w = 0 ;
	int y,z = 0 ;
	CedarDate bdate,edate;
	dr.get_record_begin_date(bdate);
	dr.get_record_end_date(edate);
	oss<<"KINST"<<'\t'<<"KINDAT"<<'\t'<<"IBYRT"<<'\t'<<"IBDTT"<<'\t'<<"IBHMT"<<'\t'<<"IBCST"<<'\t'<<"IEYRT"<<'\t'<<"IEDTT"<<'\t'<<"IEHMT"<<'\t'<<"IECST"<<'\t'<<"JPAR"<<'\t'<<"MPAR"<<'\t'<<"NROWS"<<endl;
	oss<<dr.get_record_kind_instrument()<<'\t';
	oss<<dr.get_record_kind_data()<<'\t';
	oss<< bdate.get_year()<<'\t';
	oss<<bdate.get_month_day()<<'\t';
	oss<<bdate.get_hour_min()<<'\t';
	oss<<bdate.get_second_centisecond()<<'\t';
	oss<<edate.get_year()<<'\t';
	oss<<edate.get_month_day()<<'\t';
	oss<<edate.get_hour_min()<<'\t';
	oss<<edate.get_second_centisecond()<<'\t';
	oss<<dr.get_jpar()<<'\t';
	oss<<dr.get_mpar()<<'\t';
	oss<<dr.get_nrows()<<endl;

	unsigned int jpar_value=dr.get_jpar();
	auto_ptr<vector<short int> > pJparData( new vector<short int>(jpar_value) );
	if (!pJparData.get())
	{
	    throw bad_alloc();
	}
	dr.load_JPAR_data(*pJparData);

	auto_ptr<vector<short int> > pJparVars( new vector<short int>(jpar_value) );
	if (!pJparVars.get())
	{
	    throw bad_alloc();
	}
	dr.load_JPAR_vars(*pJparVars);

	for (w=0; w<jpar_value; w++)
	{ 
	    string JparVarName="";
	    string the_var="";
	    if (qa.validate_parameter((*pJparVars)[w]))
	    {
		int val;
		if ((*pJparVars)[w]<0)
		{
		    val=(*pJparVars)[w]*-1;
		    the_var+="e_";
		}
		else
		{
		    val=(*pJparVars)[w];
		}
		get_name_for_parameter(JparVarName,val);
		the_var+=JparVarName;
		oss<<the_var;
		if(w<(jpar_value-1))
		{
		    oss<<'\t';
		}
	    }
	}
	oss<<endl;

	for (w=0; w<jpar_value; w++)
	{ 
	    if (qa.validate_parameter((*pJparVars)[w]))
	    {
		oss<<(*pJparVars)[w];
		if(w<(jpar_value-1))
		{
		    oss<<'\t';
		}
	    }
	}
	oss<<endl;
	for (w=0; w<jpar_value; w++)
	{ 
	    if (qa.validate_parameter((*pJparVars)[w]))
	    {
		oss<<(*pJparData)[w];
		if(w<(jpar_value-1))
		{
		    oss<<'\t';
		}
	    }
	}
	oss<<endl;

	int mpar_value=dr.get_mpar();
	int nrow_value=dr.get_nrows();
	if ((mpar_value>0) && (nrow_value>0))
	{
	    auto_ptr<vector<short int> > pMparVars( new vector<short int>(mpar_value) );
	    if (!pMparVars.get())
	    {
		throw bad_alloc();
	    }
	    dr.load_MPAR_vars(*pMparVars);

	    auto_ptr<vector<short int> > pMparData( new vector<short int>(nrow_value*mpar_value) );
	    if (!pMparData.get())
	    {
		throw bad_alloc();
	    }
	    dr.load_MPAR_data(*pMparData);

	    for (y=0; y<mpar_value;y++)
	    {
		string MparVarName="";
		string the_var="";
		if (qa.validate_parameter((*pMparVars)[y]))
		{
		    int val;
		    if ((*pMparVars)[y]<0)
		    {
			val=(*pMparVars)[y]*-1;
			the_var+="e_";
		    }
		    else
		    {
			val=(*pMparVars)[y];
		    }
		    get_name_for_parameter(MparVarName,val);
		    the_var+=MparVarName;
		    oss<<the_var;
		    if(y<(mpar_value-1))
		    {
			oss<<'\t';
		    }
		}
	    }
	    oss<<endl;

	    unsigned int number_of_valid_variables=0;

	    printable_parameter valid_printable_parameters[mpar_value];

	    for (y=0; y<mpar_value;y++)
	    {
		valid_printable_parameters[y].is_valid=0;
	    }

	    for (y=0; y<mpar_value;y++)
	    {
		if (qa.validate_parameter((*pMparVars)[y]))
		{
		    number_of_valid_variables++;
		    oss<<(*pMparVars)[y];
		    if(y<(mpar_value-1))
		    {
			oss<<'\t';
		    }
		    valid_printable_parameters[y].is_valid=1;
		    valid_printable_parameters[y].par_value=(*pMparVars)[y];
		}
	    }
	    oss<<endl;

	    // Print the data

	    CedarParameter pp;

	    unsigned int number_of_printable_items;

	    bool print_row;

	    for (y=0; y<nrow_value;y++)
	    {
		print_row=1;

		if (qa.got_parameter_constraint())
		{
		    number_of_printable_items=0;
		    for (z=0; z<mpar_value;z++)
		    {
			if(valid_printable_parameters[z].is_valid)
			{
			    pp=qa.get_parameter(valid_printable_parameters[z].par_value);
			    if(pp.validateValue((*pMparData)[z+(y*mpar_value)]))
			    {
				number_of_printable_items++;
			    }
			}
		    }

		    if(number_of_printable_items!=number_of_valid_variables)
		    {
			print_row=0;
		    }
		}

		if (print_row)
		{
		    for (z=0; z<mpar_value;z++)
		    {
			if(valid_printable_parameters[z].is_valid)
			{
			    oss<<(*pMparData)[z+(y*mpar_value)];

			    if(z<(mpar_value-1))
			    {
				oss<<'\t';
			    }
			}
		    }
		    oss<<endl;
		}
	    }

	    oss<<endl;
	}
	//oss<<'\0';
	tab_object.add_data( oss.str() ) ;
    }
}

int cedar_read_tab( CedarTab &dt, const string &filename,
                    const string &query, string &error )
{
    CedarConstraintEvaluator qa;
    try
    {
	qa.parse(query.c_str());
    }
    catch (CedarException &ex)
    {
	error=ex.get_description();
	return 0;
    }

    CedarFile file;
    try
    {
	file.open_file (filename.c_str());
	const CedarLogicalRecord *lr =file.get_first_logical_record();

	if(lr)
	{
	    if((lr->get_type()==1))
	    {
		send_tab_data(dt, *(CedarDataRecord*)lr,qa);
	    }
	    while (!file.end_dataset())
	    {
		lr=file.get_next_logical_record();
		if(lr)
		{
		    if((lr->get_type()==1))
		    {
			send_tab_data(dt, *(CedarDataRecord*)lr,qa);
		    }
		}
	    }
	}
	else 
	{
	    error = (string)"Failure reading data from file "
		    + filename
		    + ", corrupted file or not a cbf file.\n" ;
	    return false;
	}
    }
    catch (CedarException &cedarex)
    {
	error="The requested dataset produces the following exception ";
	error+=cedarex.get_description() + (string)"\n";
	return 0;
    }
    catch( BESError &beserr )
    {
	error = "The requested dataset produces the following exception: " ;
	error += beserr.get_message() + (string)"\n" ;
	return 0 ;
    }
    catch (bad_alloc::bad_alloc)
    {
	error="There has been a memory allocation error.\n";
	return 0;
    }
    catch (...)
    {
	error="The requested dataset produces an unknown exception.\n";
	return 0;
    }

    return 1;
}
 
