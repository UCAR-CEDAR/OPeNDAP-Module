// cedar_read_attributes.h

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

#ifndef cedar_read_attributes_h_
#define cedar_read_attributes_h_ 1


#include "DAS.h"
#include "CedarDataRecord.h"

using namespace libdap ;

/**
  Reads the attribute data from a cbf file and loads it into a DAS object.
  This is the entry point for reading the attribute data from a given cbf
  file "filename" into and DAS object "das."

  The algorithm parses sequentially the cbf file to extract the data records
  from it; at every step if successful the data record is loaded into das
  using the function "load_das(...)". 

  @param das: reference the the DAS object to be loaded with the attribute data.
  @param filename: full qualify path to the cbf file where the data resides.
  @param error: Pointer to string object where a error string can be loaded indicating the reason for failure of this function.
  @return bool: True if the process have not problem loading the data, false otherwise.
  @see load_das
  */
bool cedar_read_attributes( DAS &das, const string &filename,
                            string &cedar_error ) ;

/**
  loads a single data record into a DAS object.
 
  By calling member methods of CedarDataRecord class in the Cedar api core
  and methods from the libdap core, a Data Attribute object (DAS) is built.
  Here lays the algorithm in which a data record parameters information is
  mapped to Attributes Tables. Even thought you may think this is the same
  as read_descriptors and load_dds (in fact we tried to keep the function
  prototypes very similar) inside they are substantially different because
  of the nature of the data they manipulate.
  @param das: A reference to the DDS object where to load the data.
  @param my_data_record: A reference to a CedarDataRecord object which contains the data to be loaded.
  @return void: At this point no return value is present until a more firm policy exist about what to do with corrupted data records. The idea is that if this function returns false then its master user (read_attributes) will spoil the whole dataset just because one record.
  @see read_attributes
  */
void load_das(DAS &das,CedarDataRecord *dr);

/**
  Mantains records about which data records has been used to build the
  DAS object.
  
  This method is used by load_das.

  Everytime a data record is extracted from a cbf file, it is parse in
  order to extract its attributes into an Attribute table. However from
  attributes point of view two data records are considered the same if
  they have the same KINDAT. In order to improve speed a static data
  structure is mantained by this function in which KINDATs are stored if
  their corresponding data records have been parsed. 

  This method will return 0 (false) if the KINDAT which is passed has not
  been logged, therefore giving a green light to load_das to parse the
  data record and add it to the DAS object.

  @param dat: the KINDAT value for the corresponding data record which is about to be parse by load_das.
  @returns: 0 (false) if the KINDAT is not already logged. 1 (true) if the KINDAT is already logged.
  @see load_das
  */
int logged(int dat);

#endif // cedar_read_attributes_h_

