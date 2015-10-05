// parcodsT.cc

#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdlib.h>

#include <iostream>

using std::cerr ;
using std::endl ;

#include "CedarReadParcods.h"
#include "CedarMySQLDB.h"
#include "BESDebug.h"
#include "TheBESKeys.h"
#include "BESError.h"

#include "test_config.h"

using namespace CppUnit ;

class parcodsT: public TestFixture {
private:

public:
    parcodsT() {}
    ~parcodsT() {}

    void setUp()
    {
    } 

    void tearDown()
    {
    }

    CPPUNIT_TEST_SUITE( parcodsT ) ;

    CPPUNIT_TEST( do_conversion ) ;

    CPPUNIT_TEST_SUITE_END() ;

    void do_conversion()
    {
        string bes_conf = (string)TEST_SRC_DIR + "/bes.conf" ;
        TheBESKeys::ConfigFile = bes_conf ;

        BESDebug::SetUp( "cerr,cedar" ) ;
        try
        {
            CedarDB::Add_DB_Builder( "mysql", CedarMySQLDB::BuildMySQLDB ) ;

            string long_name = CedarReadParcods::Get_Longname( 810 ) ;
            cerr << "long_name = " << long_name << endl ;
            CPPUNIT_ASSERT( long_name == "Neutral temperature" ) ;
            string short_name = CedarReadParcods::Get_Shortname( 810 ) ;
            cerr << "short_name = " << short_name << endl ;
            CPPUNIT_ASSERT( short_name == "Tn" ) ;
            string madrigal_name = CedarReadParcods::Get_Madrigalname( 810 ) ;
            cerr << "madrigal_name = " << madrigal_name << endl ;
            CPPUNIT_ASSERT( madrigal_name == "tn" ) ;
            string units = CedarReadParcods::Get_Unit_Label( 810 ) ;
            cerr << "units = " << units << endl ;
            CPPUNIT_ASSERT( units == "K" ) ;
            string scale = CedarReadParcods::Get_Scale( 810 ) ;
            cerr << "scale = " << scale << endl ;
            CPPUNIT_ASSERT( scale == "1." ) ;

            long_name = CedarReadParcods::Get_Longname( -2506 ) ;
            cerr << "long_name = " << long_name << endl ;
            CPPUNIT_ASSERT( long_name == "ERROR FOR log10 (Relative line/band brightness)" ) ;
            short_name = CedarReadParcods::Get_Shortname( -2506 ) ;
            cerr << "short_name = " << short_name << endl ;
            CPPUNIT_ASSERT( short_name == "error lg(rel br)" ) ;
            madrigal_name = CedarReadParcods::Get_Madrigalname( -2506 ) ;
            cerr << "madrigal_name = " << madrigal_name << endl ;
            CPPUNIT_ASSERT( madrigal_name == "e_rbrl" ) ;
            units = CedarReadParcods::Get_Unit_Label( -2506 ) ;
            cerr << "units = " << units << endl ;
            CPPUNIT_ASSERT( units == "lg" ) ;
            scale = CedarReadParcods::Get_Scale( -2506 ) ;
            cerr << "scale = " << scale << endl ;
            CPPUNIT_ASSERT( scale == "1.E-03" ) ;

            CedarDB::Close() ;
        }
        catch( BESError &e )
        {
            cerr << e << endl ;
            CPPUNIT_ASSERT( !"Caught BES exception" ) ;
        }
    }

} ;

CPPUNIT_TEST_SUITE_REGISTRATION( parcodsT ) ;

int 
main( int, char** )
{
    CppUnit::TextTestRunner runner ;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() ) ;

    bool wasSuccessful = runner.run( "", false )  ;

    return wasSuccessful ? 0 : 1 ;
}

