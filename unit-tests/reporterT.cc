// reporterT.cc

#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit ;

#include <stdlib.h>

#include <iostream>

using std::cerr ;
using std::endl ;

#include "CedarReporter.h"
#include "CedarMySQLDB.h"
#include "ContainerStorageCedar.h"
#include "BESDataHandlerInterface.h"
#include "BESDebug.h"
#include "TheBESKeys.h"
#include "BESDataNames.h"
#include "BESContextManager.h"
#include "test_config.h"

class reporterT: public TestFixture {
private:

public:
    reporterT() {}
    ~reporterT() {}

    void setUp()
    {
    } 

    void tearDown()
    {
    }

    CPPUNIT_TEST_SUITE( reporterT ) ;

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
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CPPUNIT_ASSERT( !"Failed to create database object" ) ;
        }

        try
        {
            cerr << "create the reporter" << endl ;
            CedarReporter reporter ;
            cerr << "create the data handler interface" << endl ;
            BESDataHandlerInterface dhi ;
            cerr << "create the container storage" << endl ;
            ContainerStorageCedar csc( "cedar" ) ;

            BESContextManager::TheManager()->set_context( USER_NAME, "pwest" ) ;
            dhi.action = "get.tab" ;

            cerr << "find mfp920504a" << endl ;
            BESContainer *c = csc.look_for( "mfp920504a" ) ;
            CPPUNIT_ASSERT( c ) ;
            c->set_constraint( "date(1992,504,0,0,1992,603,2359,5999);record_type(5340/7001);parameters(21,34,800,810,1410,1420,2506)") ;
            dhi.containers.push_back( c ) ;

            cerr << "find mfp911104a" << endl ;
            c = csc.look_for( "mfp911104a" ) ;
            CPPUNIT_ASSERT( c ) ;
            c->set_constraint( "date(1992,504,0,0,1992,603,2359,5999);record_type(5340/7001);parameters(21,34,800,810,1410,1420,2506)") ;
            dhi.containers.push_back( c ) ;

            cerr << "find mfp920603a" << endl ;
            c = csc.look_for( "mfp920603a" ) ;
            CPPUNIT_ASSERT( c ) ;
            c->set_constraint( "date(1992,504,0,0,1992,603,2359,5999);record_type(5340/7001);parameters(21,34,800,810,1410,1420,2506)") ;
            dhi.containers.push_back( c ) ;

            reporter.report( dhi ) ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"Failed to create database object" ) ;
        }

        try
        {
            CedarDB::Close() ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CPPUNIT_ASSERT( !"Failed to close the database" ) ;
        }
    }

} ;

CPPUNIT_TEST_SUITE_REGISTRATION( reporterT ) ;

int 
main( int, char** )
{
    CppUnit::TextTestRunner runner ;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() ) ;

    bool wasSuccessful = runner.run( "", false )  ;

    return wasSuccessful ? 0 : 1 ;
}

