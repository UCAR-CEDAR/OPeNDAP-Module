// authT.cc

#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdlib.h>

#include <iostream>

using std::cerr ;
using std::endl ;

#include "CedarAuthenticate.h"
#include "BESDataNames.h"
#include "BESContextManager.h"
#include "BESDataHandlerInterface.h"
#include "BESDebug.h"
#include "TheBESKeys.h"
#include "CedarMySQLDB.h"

#include "test_config.h"

using namespace CppUnit ;

class authT: public TestFixture {
private:

public:
    authT() {}
    ~authT() {}

    void setUp()
    {
    } 

    void tearDown()
    {
    }

    CPPUNIT_TEST_SUITE( authT ) ;

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

            BESDataHandlerInterface dhi ;
            BESContextManager::TheManager()->set_context( USER_NAME, "pwest" ) ;

            CedarAuthenticate::authenticate( dhi ) ;

            CedarDB::Close() ;
        }
        catch( BESError &e )
        {
            cerr << e << endl ;
            CPPUNIT_ASSERT( !"Caught BES exception" ) ;
        }
    }

} ;

CPPUNIT_TEST_SUITE_REGISTRATION( authT ) ;

int 
main( int, char** )
{
    CppUnit::TextTestRunner runner ;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() ) ;

    bool wasSuccessful = runner.run( "", false )  ;

    return wasSuccessful ? 0 : 1 ;
}

