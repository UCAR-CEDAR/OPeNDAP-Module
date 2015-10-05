// dbT.cc

#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit ;

#include <stdlib.h>

#include <iostream>

using std::cerr ;
using std::endl ;

#include "CedarDB.h"
#include "CedarMySQLDB.h"
#include "CedarDBResult.h"
#include "BESInternalError.h"
#include "BESDebug.h"
#include "TheBESKeys.h"
#include "test_config.h"

class dbT: public TestFixture {
private:

public:
    dbT() {}
    ~dbT() {}

    void setUp()
    {
    } 

    void tearDown()
    {
    }

    CPPUNIT_TEST_SUITE( dbT ) ;

    CPPUNIT_TEST( do_conversion ) ;

    CPPUNIT_TEST_SUITE_END() ;

    void do_conversion()
    {
        string bes_conf = (string)TEST_SRC_DIR + "/bes.conf" ;
        TheBESKeys::ConfigFile = bes_conf ;

        BESDebug::SetUp( "cerr,cedar" ) ;
        CedarDB *db = 0 ;
        try
        {
            CedarDB::Add_DB_Builder( "mysql", CedarMySQLDB::BuildMySQLDB ) ;

            db = CedarDB::DB( "Test" ) ;

            if( !db )
            {
                CPPUNIT_ASSERT( !"Database empty" ) ;
            }

            cerr << (*db) << endl ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CPPUNIT_ASSERT( !"Failed to create database object" ) ;
        }

        try
        {
            db->open() ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"Failed to open database" ) ;
        }

        try
        {
            vector< vector<CedarDBColumn> > flds ;
            vector<CedarDBColumn> fld_set ;
            fld_set.push_back( CedarDBColumn( "test_str", "s1" ) ) ;
            fld_set.push_back( CedarDBColumn( "test_num", 1 ) ) ;
            flds.push_back( fld_set ) ;
            fld_set.clear() ;
            fld_set.push_back( CedarDBColumn( "test_str", "s2" ) ) ;
            fld_set.push_back( CedarDBColumn( "test_num", 2 ) ) ;
            flds.push_back( fld_set ) ;
            unsigned int num_rows = db->insert( "cedar_test", flds ) ;
            CPPUNIT_ASSERT( num_rows == 2 ) ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"insert operations failed" ) ;
        }

        try
        {
            vector<CedarDBColumn> fld_set ;
            fld_set.push_back( CedarDBColumn( "test_str", "s3" ) ) ;
            fld_set.push_back( CedarDBColumn( "test_num", 3 ) ) ;

            vector<CedarDBWhere> fld_where ;
            fld_where.push_back( CedarDBWhere( "test_str", "=", "s1" ) ) ;
            fld_where.push_back( CedarDBWhere( "AND", "test_num", "=", 1 ) ) ;

            unsigned int num_rows = db->update( "cedar_test",
                            fld_set, fld_where ) ;
            CPPUNIT_ASSERT( num_rows == 1 ) ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"update operations failed" ) ;
        }

        try
        {
            CedarDBResult *result =
            db->run_query( "SELECT * from cedar_test" ) ;
            if( !result )
            {
            CPPUNIT_ASSERT( !"result set empty" ) ;
            }

            cerr << (*result) << endl ;
            cerr << "num rows = " << result->get_num_rows() << endl ;

            CPPUNIT_ASSERT( result->get_num_rows() == 2 ) ;
            CPPUNIT_ASSERT( result->get_num_fields() == 2 ) ;

            CPPUNIT_ASSERT( result->first_row() == true ) ;
            bool s2 = false ;
            bool s3 = false ;
            if( (*result)["test_str"] == "s3" )
            {
            CPPUNIT_ASSERT( (*result)["test_num"] == "3" ) ;
            s3 = true ;
            }
            else if( (*result)["test_str"] == "s2" )
            {
            CPPUNIT_ASSERT( (*result)["test_num"] == "2" ) ;
            s2 = true ;
            }
            else
            CPPUNIT_ASSERT( !"Neither result is s2 or s3" ) ;

            CPPUNIT_ASSERT( result->next_row() == true ) ;

            if( s2 )
            {
            CPPUNIT_ASSERT( (*result)["test_str"] == "s3" ) ;
            CPPUNIT_ASSERT( (*result)["test_num"] == "3" ) ;
            }
            else if( s3 )
            {
            CPPUNIT_ASSERT( (*result)["test_str"] == "s2" ) ;
            CPPUNIT_ASSERT( (*result)["test_num"] == "2" ) ;
            }
            else
            CPPUNIT_ASSERT( !"Neither result is s2 or s3" ) ;

            CPPUNIT_ASSERT( result->next_row() == false ) ;
            delete result ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"query operations failed" ) ;
        }

        try
        {
            vector<CedarDBWhere> where ;
            where.push_back( CedarDBWhere( "test_str", "!=", "s3" ) ) ;
            unsigned int num_rows = db->del( "cedar_test", where ) ;
            CPPUNIT_ASSERT( num_rows == 1 ) ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"delete operations failed" ) ;
        }

        try
        {
            CedarDBResult *result =
            db->run_query( "SELECT * from cedar_test" ) ;
            if( !result )
            {
            CPPUNIT_ASSERT( !"result set empty" ) ;
            }

            cerr << (*result) << endl ;

            CPPUNIT_ASSERT( result->get_num_rows() == 1 ) ;
            CPPUNIT_ASSERT( result->get_num_fields() == 2 ) ;

            CPPUNIT_ASSERT( result->first_row() == true ) ;
            CPPUNIT_ASSERT( (*result)["test_str"] == "s3" ) ;
            CPPUNIT_ASSERT( (*result)["test_num"] == "3" ) ;
            CPPUNIT_ASSERT( result->next_row() == false ) ;
            delete result ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"query operations failed" ) ;
        }

        try
        {
            vector<CedarDBWhere> where ;
            where.push_back( CedarDBWhere( "test_str", "!=", "" ) ) ;
            unsigned int num_rows = db->del( "cedar_test", where ) ;
            CPPUNIT_ASSERT( num_rows == 1 ) ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"delete operations failed" ) ;
        }

        try
        {
            CedarDBResult *result =
            db->run_query( "SELECT * from cedar_test" ) ;
            if( !result )
            {
            CPPUNIT_ASSERT( !"result set empty" ) ;
            }

            cerr << (*result) << endl ;

            CPPUNIT_ASSERT( result->get_num_rows() == 0 ) ;
            delete result ;
        }
        catch( BESInternalError &e )
        {
            cerr << e << endl ;
            CedarDB::Close() ;
            CPPUNIT_ASSERT( !"query operations failed" ) ;
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

CPPUNIT_TEST_SUITE_REGISTRATION( dbT ) ;

int 
main( int, char** )
{
    CppUnit::TextTestRunner runner ;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() ) ;

    bool wasSuccessful = runner.run( "", false )  ;

    return wasSuccessful ? 0 : 1 ;
}

