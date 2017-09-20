/*
 * test_simple.cpp
 *
 *  Created on: 2016-9-13
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include "../../bingo_v4/boSingleton.h"
#include "../../bingo_v4/boString.h"
#include "../../bingo_v4/boType.h"
#include "../../bingo_v4/boActionFactory.h"
#include "../../bingo_v4/rabbitmq/imp/boRbSimpleSendor.h"
#include "../../bingo_v4/rabbitmq/imp/boRbSimpleReceiver.h"
using namespace bingo::RabbitMQ ;
using namespace bingo::RabbitMQ::imp ;
using namespace bingo::log ;
using namespace bingo ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

// Rabbitmq simple mode is single-to-single connect.
// transfer message example:
// one sendor :   01 00 00 00 ff
//                      02 00 00 00 ff

// one receiver:  01 00 00 00 ff
//                      02 00 00 00 ff

BOOST_AUTO_TEST_SUITE( test_rb_simple )

int max = 65535 ;
int nCount = 0 ;

// Test rabbitmq receiver.

class my_rev : public boRbSimpleReceiver {
public:

        my_rev( boRbConnectDetail& conn , boLogHandler* logger ) : boRbSimpleReceiver( conn , logger ) {

        }

        virtual ~my_rev( ) {

        }

        void read_message_complete( string& message ) override {

                {
                        nCount++ ;
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "rev data:" )->add( t.stream_to_string( message.c_str( ) , message.length( ) ) )->add( ",nCount:" )->add( nCount ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }


                //                nCount++ ;
                //                if ( nCount == ( max - 1 ) ) {
                //                        boStringAppend ap ;
                //                        ap.add( "rev complete, total rev num:" )->add( nCount ) ;
                //                        ::bo_debug_out( ap.to_string( ) ) ;
                //                }
        }
} ;

BOOST_AUTO_TEST_CASE( t_rev ) {

        boRbConnectDetail conn ;
        conn.ip = "127.0.0.1" ;
        conn.port = 5672 ;
        conn.username = "guest" ;
        conn.pwd = "guest" ;
        conn.host = "/" ;
        conn.routingkey.push_back( string( "test_simple" ) ) ;
        conn.key = "simple" ;
        boLogHandler* logger = boActionFactory::make_console_logger( ) ;
        my_rev rev( conn , logger ) ;
        rev.connet_to_server( ) ;

        delete logger ;
}

// Test rabbitmq sendor

boRbSimpleSendor* pSend = 0 ;

void send_data_thr( ) {

        BO_THIS_THREAD::sleep( seconds( 10 ) ) ;

        while ( true ) {
                int n = 0 ;
                while ( n < max ) {
                        char data[4] = {0x01 , 0x02 , 0x03 , 0xDD} ;
                        memcpy( data , &n , 3 ) ;

                        pSend->send_data( data , 4 ) ;
                        BO_THIS_THREAD::sleep( seconds( 1 ) ) ;

                        n++ ;
                }


                BO_THIS_THREAD::sleep( seconds( 10 ) ) ;
        }

}

BOOST_AUTO_TEST_CASE( t_snd ) {

        BO_THREAD t1( send_data_thr ) ;

        boRbConnectDetail conn ;
        conn.ip = "127.0.0.1" ;
        conn.port = 5672 ;
        conn.username = "guest" ;
        conn.pwd = "guest" ;
        conn.host = "/" ;
        conn.routingkey.push_back( string( "test_simple" ) ) ;
        conn.key = "simple" ;
        boRbSimpleSendor snd( conn , boActionFactory::make_console_logger( ) ) ;

        pSend = &snd ;

        snd.connet_to_server( ) ;

        t1.join( ) ;
}

BOOST_AUTO_TEST_SUITE_END( )
