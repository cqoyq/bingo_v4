/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <boost/test/unit_test.hpp>

#include "../../bingo_v4/boSingleton.h"
#include "../../bingo_v4/boString.h"
#include "../../bingo_v4/boType.h"
#include "../../bingo_v4/boActionFactory.h"
#include "../../bingo_v4/rabbitmq/imp/boRbTopicReceiver.h"
#include "../../bingo_v4/rabbitmq/imp/boRbTopicSendor.h"
using namespace bingo::RabbitMQ ;
using namespace bingo::RabbitMQ::imp ;
using namespace bingo::log ;
using namespace bingo ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

// Rabbitmq Publish/Subscribe mode.
// transfer message example:
// first sendor :   01 00 00 00 ff        (routingkey: test1.log)
//                      02 00 00 00 ff        
//                      03 00 00 00 ff        
//                      04 00 00 00 ff        

// twice sendor : 01 00 00 00 dd        (routingkey: test2.log)
//                      02 00 00 00 dd       
//                      03 00 00 00 dd        
//                      04 00 00 00 dd

// third sendor :  01 00 00 00 dc        (routingkey: test2.service)
//                      02 00 00 00 dc       
//                      03 00 00 00 dc        
//                      04 00 00 00 dc

// first receiver:   01 00 00 00 ff        (routingkey: *.log, test2.*)
//                      02 00 00 00 ff
//                      03 00 00 00 ff
//                      04 00 00 00 ff
//                      01 00 00 00 dd
//                      02 00 00 00 dd
//                      03 00 00 00 dd
//                      04 00 00 00 dd
//                      01 00 00 00 dc
//                      02 00 00 00 dc
//                      03 00 00 00 dc
//                      04 00 00 00 dc

// twice receiver: 01 00 00 00 dc        (routingkey: *.service)
//                      02 00 00 00 dc
//                      03 00 00 00 dc
//                      04 00 00 00 dc

BOOST_AUTO_TEST_SUITE( test_rb_topic )

int max = 65535 ;
int nCount = 0 ;

// Test rabbitmq receiver.

class my_rev : public boRbTopicReceiver {
public:

        my_rev( boRbConnectDetail& conn , boLogHandler* logger ) : boRbTopicReceiver( conn , logger ) {

        }

        virtual ~my_rev( ) {

        }

        void read_message_complete( string& message ) override {

                {
                        nCount++ ;
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "rev data:" )->add( t.stream_to_string( message.c_str( ) , message.length( ) ) )->add( ",ptr:" )->add( this ) ;
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

void run_rev( int n ) {
        boRbConnectDetail conn ;
        conn.ip = "127.0.0.1" ;
        conn.port = 5672 ;
        conn.username = "guest" ;
        conn.pwd = "guest" ;
        conn.host = "/" ;
        conn.exchange = "test_topic" ;
        if ( n == 1 ) {
                conn.routingkey.push_back( string( "*.log" ) ) ;
                conn.routingkey.push_back( string( "test2.*" ) ) ;
        } else {
                conn.routingkey.push_back( string( "*.service" ) ) ;
        }

        conn.key = "topic" + lexical_cast<string>( n ) ;
        boLogHandler* logger = boActionFactory::make_console_logger( ) ;
        my_rev rev( conn , logger ) ;
        rev.connet_to_server( ) ;

        delete logger ;
}

BOOST_AUTO_TEST_CASE( t_rev ) {

        BO_THREAD t1( run_rev , 1 ) ;
        BO_THREAD t2( run_rev , 2 ) ;

        t1.join( ) ;
        t2.join( ) ;

}

// Test rabbitmq sendor

boRbTopicSendor* pSend1 = 0 ;
boRbTopicSendor* pSend2 = 0 ;
boRbTopicSendor* pSend3 = 0 ;

void send_data_thr( int index ) {

        BO_THIS_THREAD::sleep( seconds( 10 ) ) ;

        while ( true ) {
                int n = 0 ;
                while ( n < max ) {
                        char data[4] = {0x01 , 0x02 , 0x03 , 0xDD} ;
                        if ( index == 1 )
                                data[3] = 0xFF ;
                        else if ( index == 2 )
                                data[3] = 0xDD ;
                        else
                                data[3] = 0xDC ;
                        memcpy( data , &n , 3 ) ;

                        if ( index == 1 )
                                pSend1->send_data( data , 4 ) ;
                        else if ( index == 2 )
                                pSend2->send_data( data , 4 ) ;
                        else if ( index == 3 )
                                pSend3 -> send_data( data , 4 ) ;
                        BO_THIS_THREAD::sleep( seconds( 1 ) ) ;

                        n++ ;
                }


                BO_THIS_THREAD::sleep( seconds( 10 ) ) ;
        }

}

void make_snd_thr( int n ) {
        boRbConnectDetail conn ;
        conn.ip = "127.0.0.1" ;
        conn.port = 5672 ;
        conn.username = "guest" ;
        conn.pwd = "guest" ;
        conn.host = "/" ;
        conn.exchange = "test_topic" ;
        if ( n == 1 )
                conn.routingkey.push_back( string( "test1.log" ) ) ;
        else if ( n == 2 )
                conn.routingkey.push_back( string( "test2.log" ) ) ;
        else
                conn.routingkey.push_back( string( "test2.service" ) ) ;
        conn.key = "topic" + lexical_cast<string>( n ) ;

        boRbTopicSendor snd( conn , boActionFactory::make_console_logger( ) ) ;

        if ( n == 1 )
                pSend1 = &snd ;
        else if ( n == 2 )
                pSend2 = &snd ;
        else
                pSend3 = &snd ;

        snd.connet_to_server( ) ;
}

BOOST_AUTO_TEST_CASE( t_snd ) {

        BO_THREAD t1( make_snd_thr , 1 ) ;
        BO_THREAD t2( make_snd_thr , 2 ) ;
        BO_THREAD t3( make_snd_thr , 3 ) ;

        BO_THIS_THREAD::sleep( seconds( 4 ) ) ;

        BO_THREAD t4( send_data_thr , 1 ) ;
        BO_THREAD t5( send_data_thr , 2 ) ;
        BO_THREAD t6( send_data_thr , 3 ) ;


        t1.join( ) ;
        t2.join( ) ;
        t3.join( ) ;
        t4.join( ) ;
        t5.join( ) ;
        t6.join( ) ;
}

BOOST_AUTO_TEST_SUITE_END( )