// kinstT.cc

#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdlib.h>

#include <iostream>

using std::cout ;
using std::endl ;

#include "CedarReadKinst.h"
#include "CedarMySQLDB.h"
#include "BESDebug.h"
#include "TheBESKeys.h"
#include "BESError.h"

#include "test_config.h"

using namespace CppUnit ;

class kinstT: public TestFixture {
private:

public:
    kinstT() {}
    ~kinstT() {}

    void setUp()
    {
    } 

    void tearDown()
    {
    }

    CPPUNIT_TEST_SUITE( kinstT ) ;

    CPPUNIT_TEST( do_conversion ) ;

    CPPUNIT_TEST_SUITE_END() ;

    void do_conversion()
    {
        cout << endl << "*****************************************" << endl;
        cout << "Entered kinstT unit test do_conversion" << endl;
        string bes_conf = (string)TEST_SRC_DIR + "/bes.conf" ;
        TheBESKeys::ConfigFile = bes_conf ;

        BESDebug::SetUp( "cerr,cedar" ) ;
        try
        {
            cout << endl << "*****************************************" << endl;
            cout << "adding MySQL database" << endl;
            CedarDB::Add_DB_Builder( "mysql", CedarMySQLDB::BuildMySQLDB ) ;
        }
        catch( BESError &e )
        {
            cout << e << endl ;
            CPPUNIT_ASSERT( !"Caught BES exception" ) ;
        }

        try
        {
            cout << endl << "*****************************************" << endl;
            cout << "retrieve instrument 5340 with site" << endl;
            string inst = CedarReadKinst::Get_Kinst_as_String( 5340 ) ;
            cout << "instrument = " << inst << endl ;
            CPPUNIT_ASSERT( inst == "5340" ) ;
            string name = CedarReadKinst::Get_Name( 5340 ) ;
            cout << "name = " << name << endl ;
            CPPUNIT_ASSERT( name == "Millstone Hill Fabry-Perot" ) ;
            string prefix = CedarReadKinst::Get_Prefix( 5340 ) ;
            cout << "prefix = " << prefix << endl ;
            CPPUNIT_ASSERT( prefix == "MFP" ) ;
            string lat = CedarReadKinst::Get_Latitude_as_String( 5340 ) ;
            cout << "lat = " << lat << endl ;
            CPPUNIT_ASSERT( lat == "42 36'43\\\"" ) ;
            string lon = CedarReadKinst::Get_Longitude_as_String( 5340 ) ;
            cout << "lon = " << lon << endl ;
            CPPUNIT_ASSERT( lon == "-71 29'05\\\"" ) ;
            string alt = CedarReadKinst::Get_Altitude_as_String( 5340 ) ;
            cout << "alt = " << alt << endl ;
            CPPUNIT_ASSERT( alt == "0.146" ) ;
        }
        catch( BESError &e )
        {
            cout << e << endl ;
            CPPUNIT_ASSERT( !"Caught BES exception" ) ;
        }

        try
        {
            cout << endl << "*****************************************" << endl;
            cout << "retrieve instrument that doesn't exist" << endl;
            string inst = CedarReadKinst::Get_Kinst_as_String( 1 ) ;
            CPPUNIT_ASSERT( !"found the instrument, shouldn't have" ) ;
        }
        catch( BESError &e )
        {
            cout << e << endl ;
            CPPUNIT_ASSERT( "Caught BES exception, good" ) ;
        }

        try
        {
            cout << endl << "*****************************************" << endl;
            cout << "retrieve instrument 120 without a site" << endl;
            string inst = CedarReadKinst::Get_Kinst_as_String( 120 ) ;
            cout << "instrument = " << inst << endl ;
            CPPUNIT_ASSERT( inst == "120" ) ;
            string name = CedarReadKinst::Get_Name( 120 ) ;
            cout << "name = " << name << endl ;
            CPPUNIT_ASSERT( name == "Interplanetary Mag Fld and Solar Wind" ) ;
            string prefix = CedarReadKinst::Get_Prefix( 120 ) ;
            cout << "prefix = " << prefix << endl ;
            CPPUNIT_ASSERT( prefix == "IMF" ) ;
            string lat = CedarReadKinst::Get_Latitude_as_String( 120 ) ;
            cout << "lat = " << lat << endl ;
            CPPUNIT_ASSERT( lat == "0 00'00\\\"" ) ;
            string lon = CedarReadKinst::Get_Longitude_as_String( 120 ) ;
            cout << "lon = " << lon << endl ;
            CPPUNIT_ASSERT( lon == "0 00'00\\\"" ) ;
            string alt = CedarReadKinst::Get_Altitude_as_String( 120 ) ;
            cout << "alt = " << alt << endl ;
            CPPUNIT_ASSERT( alt == "0" ) ;
        }
        catch( BESError &e )
        {
            cout << e << endl ;
            CPPUNIT_ASSERT( !"Caught BES exception" ) ;
        }

        try
        {
            cout << endl << "*****************************************" << endl;
            cout << "Close the database" << endl;
            CedarDB::Close() ;
        }
        catch( BESError &e )
        {
            cout << e << endl ;
            CPPUNIT_ASSERT( !"Caught BES exception" ) ;
        }
    }

} ;

CPPUNIT_TEST_SUITE_REGISTRATION( kinstT ) ;

int 
main( int, char** )
{
    CppUnit::TextTestRunner runner ;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() ) ;

    bool wasSuccessful = runner.run( "", false )  ;

    return wasSuccessful ? 0 : 1 ;
}

