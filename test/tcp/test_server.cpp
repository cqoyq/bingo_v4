/*
 * test_string_ex.cpp
 *
 *  Created on: 2016-9-13
 *      Author: root
 */

#include <boost/test/unit_test.hpp>

#include "../../bingo_v4/tcp/boScoketManager.h"
#include "../../bingo_v4/tcp/boTcpServer.h"
#include "../../bingo_v4/tcp/boTcpSvrHandler.h"
#include "../../bingo_v4/tcp/boTcpError.h"
#include "../../bingo_v4/boString.h"
using namespace bingo::TCP ;
using namespace bingo ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

BOOST_AUTO_TEST_SUITE( test_tcp_server )

struct svr_parser {
        static int max_wait_for_heartjump_seconds ; // If the value is 0, then server don't check heartjump.
        static int max_wait_for_authentication_pass_seconds ; // If the value is 0, then server don't check authentication pass.
        static size_t max_socket_size ; // If the value is 0, then no limit.
} ;
//int svr_parser::max_wait_for_authentication_pass_seconds = 4 ;
//int svr_parser::max_wait_for_heartjump_seconds = 8 ;
int svr_parser::max_wait_for_authentication_pass_seconds = 0 ;
int svr_parser::max_wait_for_heartjump_seconds = 0 ;
size_t svr_parser::max_socket_size = 3 ;

// Test tcp server, check authenticate and heartjump in period.

class svr_hdr1 : public boTcpSvrHandler {
public:

        svr_hdr1( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr1( ) {
        }
} ;

typedef boScoketManager<svr_hdr1 > manager1 ;
typedef boTcpServer<svr_hdr1 , manager1 , svr_parser> tcp_server_base1 ;

class svr1 : public tcp_server_base1 {
public:

        svr1( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base1( io_service , ipv4 , port ) {
        }

        virtual ~svr1( ) {
        }

} ;

BOOST_AUTO_TEST_CASE( t1 ) {

        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr1 server( io , ip , port ) ;
        io.run( ) ;

}

// Test tcp server, the client connect success, and accept_success_func() return -1, 
// the server disconnect.

class svr_hdr2 : public boTcpSvrHandler {
public:

        svr_hdr2( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr2( ) {
        }

        void catch_error_func( pointer ptr , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }
} ;

typedef boScoketManager<svr_hdr2 > manager2 ;
typedef boTcpServer<svr_hdr2 , manager2 , svr_parser> tcp_server_base2 ;

class svr2 : public tcp_server_base2 {
public:

        svr2( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base2( io_service , ipv4 , port ) {
        }

        virtual ~svr2( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func return -1, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;

                e.err_no( TCP_ERROR_TYPE_SERVER_UNKNOWN ) ;
                e.err_message( "acceptor error" ) ;
                return -1 ;
        }


} ;

BOOST_AUTO_TEST_CASE( t2 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr2 server( io , ip , port ) ;
        io.run( ) ;
}

// Test tcp server, the client connect success, and the client active disconnect.

class svr_hdr3 : public boTcpSvrHandler {
public:

        svr_hdr3( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr3( ) {
        }

        void catch_error_func( pointer ptr , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }
} ;

typedef boScoketManager<svr_hdr3> manager3 ;
typedef boTcpServer<svr_hdr3 , manager3 , svr_parser> tcp_server_base3 ;

class svr3 : public tcp_server_base3 {
public:

        svr3( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base3( io_service , ipv4 , port ) {
        }

        virtual ~svr3( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func success, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
                return 0 ;
        }
} ;

BOOST_AUTO_TEST_CASE( t3 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr3 server( io , ip , port ) ;
        io.run( ) ;
}

// Test tcp server, the client connect success, the server disconnect because authenticate.

class svr_hdr4 : public boTcpSvrHandler {
public:

        svr_hdr4( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr4( ) {
        }

        void catch_error_func( pointer ptr , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) override {
                char auth_msg[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                if ( data_size == 4 && memcmp( data , auth_msg , 4 ) == 0 ) {
                        this->set_authentication_pass( ) ;
                        this->send_data_in_thread( data , 4 ) ;
                } else {
                        this->send_data_in_thread( data , data_size ) ;
                }

                return 0 ;
        }

} ;

typedef boScoketManager<svr_hdr4> manager4 ;
typedef boTcpServer<svr_hdr4 , manager4 , svr_parser> tcp_server_base4 ;

class svr4 : public tcp_server_base4 {
public:

        svr4( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base4( io_service , ipv4 , port ) {
        }

        virtual ~svr4( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func success, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
                return 0 ;
        }
} ;

BOOST_AUTO_TEST_CASE( t4 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr4 server( io , ip , port ) ;
        io.run( ) ;
}

// Test tcp server, the client connect success, the server disconnect because heartjump.

class svr_hdr5 : public boTcpSvrHandler {
public:

        svr_hdr5( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr5( ) {
        }

        int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "rec success,hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;

                return 0 ;
        }

        void catch_error_func( pointer ptr , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void message_split_action( const char* in , size_t in_size , std::vector<string>& message_set ) override {

                {
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "split data,hdr:" )->add( this )->add( ",data:" )->add( t.stream_to_string( in , in_size ) ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }

                string msg ;
                msg.append( in , in_size ) ;
                message_set.push_back( msg ) ;
        }

        bool message_authenticate_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                char heartjump[4] = {0x01 , 0x02 , 0x03 , 0x04} ;
                if ( data_size == 4 && memcmp( data , heartjump , 4 ) == 0 ) {
                        this->set_authentication_pass( ) ;

                        boStringAppend ap ;
                        ap.add( "authenticate success,hdr:" )->add( this ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                        return true ;
                } else {
                        return false ;
                }
        }

} ;

typedef boScoketManager<svr_hdr5> manager5 ;
typedef boTcpServer<svr_hdr5 , manager5 , svr_parser> tcp_server_base5 ;

class svr5 : public tcp_server_base5 {
public:

        svr5( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base5( io_service , ipv4 , port ) {
        }

        virtual ~svr5( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func success, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
                return 0 ;
        }
} ;

BOOST_AUTO_TEST_CASE( t5 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr5 server( io , ip , port ) ;
        io.run( ) ;
}

// Test tcp server, the client connect success, and then send message to server. the server find that the
// message's format has mistake, and to disconnect.

class svr_hdr6 : public boTcpSvrHandler {
public:

        svr_hdr6( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr6( ) {
        }

        int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) override {

                boStringAppend ap ;
                ap.add( "rec success,hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;


                return 0 ;
        }

        void catch_error_func( pointer ptr , boErrorWhat & e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        bool message_format_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                char msg2[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                if ( data_size == 4 && memcmp( data , msg2 , 4 ) == 0 ) {

                        return false ;
                } else {
                        return true ;
                }
        }

        bool message_authenticate_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                char auth[4] = {0x01 , 0x02 , 0x03 , 0x04} ;
                if ( data_size == 4 && memcmp( data , auth , 4 ) == 0 ) {
                        this->set_authentication_pass( ) ;

                        boStringAppend ap ;
                        ap.add( "authenticate success,hdr:" )->add( this ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                        return true ;
                } else {
                        return false ;
                }
        }

        void message_split_action( const char* in , size_t in_size , std::vector<string>& message_set ) override {
                int n = 0 ;
                while ( n < 2 ) {
                        string msg ;
                        msg.append( in + n * 4 , 4 ) ;
                        message_set.push_back( msg ) ;
                        n++ ;
                }
        }


} ;

typedef boScoketManager<svr_hdr6> manager6 ;
typedef boTcpServer<svr_hdr6 , manager6 , svr_parser> tcp_server_base6 ;

class svr6 : public tcp_server_base6 {
public:

        svr6( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) : tcp_server_base6( io_service , ipv4 , port ) {
        }

        virtual ~svr6( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func success, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
                return 0 ;
        }
} ;

BOOST_AUTO_TEST_CASE( t6 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr6 server( io , ip , port ) ;
        io.run( ) ;
}

// Test tcp server, the four client connect success. the server will send message to every client 10 times, 
// and then the server disconnect.

class svr_hdr7 : public boTcpSvrHandler {
public:

        svr_hdr7( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr7( ) {
        }

        int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) override {

                boStringAppend ap ;
                ap.add( "rec success,hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;

                return 0 ;
        }

        void catch_error_func( pointer ptr , boErrorWhat & e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }
} ;

typedef boScoketManager<svr_hdr7> manager7 ;
typedef boTcpServer<svr_hdr7 , manager7 , svr_parser> tcp_server_base7 ;

class svr7 : public tcp_server_base7 {
public:

        svr7( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base7( io_service , ipv4 , port ) {
        }

        virtual ~svr7( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func success, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
                return 0 ;
        }
} ;
svr7* pSvr7 = 0 ;

void run_thread7( int n ) {
        this_thread::sleep( seconds( 20 ) ) ;

        u32_t idx = n - 1 ;
        string uuid = pSvr7->socket_manager( ).collection( )[idx]->uuid ;

        for ( int i = 0 ; i < 10 ; ++i ) {

                char data[16] = {
                        0x68 , 0x01 , 0x00 , 0x00 ,
                        0xd9 , 0x8c , 0xee , 0x47 ,
                        0x16 , 0x01 , 0x01 , 0x8f ,
                        0x72 , 0xd8 , 0x0d , 0x16
                } ;
                size_t data_size = 16 ;

                data[1] = ( u8_t ) n ;
                data[3] = ( u8_t ) i ;

                pSvr7->socket_manager( ).send_data_in_thread( uuid , data , data_size ) ;
        }

        pSvr7->socket_manager( ).send_close_in_thread( uuid ) ;
}

BOOST_AUTO_TEST_CASE( t7 ) {

        BO_THREAD t1( run_thread7 , 1 ) ;
        BO_THREAD t2( run_thread7 , 2 ) ;
        BO_THREAD t3( run_thread7 , 3 ) ;
        BO_THREAD t4( run_thread7 , 4 ) ;

        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr7 server( io , ip , port ) ;

        pSvr7 = &server ;

        io.run( ) ;
}

// Test tcp server, five client connect to server. the server allow to connect max of three client, 
// more connect will been diconnected.

BOOST_AUTO_TEST_CASE( t8 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr7 server( io , ip , port ) ;

        io.run( ) ;
}

// Test tcp server, five client connect to server. the client send big-data, 
// server is receive correct.

class svr_hdr9 : public boTcpSvrHandler {
public:

        svr_hdr9( BO_ASYNC_SERVICE& io_service , int max_wait_for_heartjump_seconds , int max_wait_for_authentication_pass_seconds , boTcpSvrHandler::remove_socket_func f1 ) :
             boTcpSvrHandler( io_service , max_wait_for_heartjump_seconds , max_wait_for_authentication_pass_seconds , f1 ) {
        }

        virtual ~svr_hdr9( ) {
        }

        int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) override {
                {
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "call read_pk_full_complete_func, data:" )->add( t.stream_to_string( data , data_size ) )
                                ->add( ",hdr:" )->add( ptr.get( ) ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }
                // Response message.
                this->send_data_in_thread( data , data_size ) ;

                return 0 ;
        }

        void write_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , int err_value ) override {
                {
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "call write_pk_full_complete_func, data:" )->add( t.stream_to_string( data , data_size ) )
                                ->add( ",hdr:" )->add( ptr.get( ) ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }
        }

        void catch_error_func( pointer ptr , boErrorWhat & e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int err_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), err_value:" )->add( err_value )->add( ",hdr:" )->add( ptr.get( ) ) ->add( ",rev_num:" )->add( rev_num ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        bool message_authenticate_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                char msg2[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                if ( data_size == 4 && memcmp( data , msg2 , 4 ) == 0 ) {

                        this->set_authentication_pass( ) ;

                        // Response message.
                        this->send_data_in_thread( data , data_size ) ;

                        {
                                boString t ;
                                boStringAppend ap ;
                                ap.add( "call authenticate action, data:" )->add( t.stream_to_string( data , data_size ) )
                                        ->add( ",data_size:" )->add( data_size )
                                        ->add( ",hdr:" )->add( this ) ;
                                ::bo_debug_out( ap.to_string( ) ) ;
                        }

                        return true ;
                } else {
                        return false ;
                }
        }

        bool message_heartjump_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                {
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "call heartjump action, data:" )->add( t.stream_to_string( data , data_size ) )
                                ->add( ",hdr:" )->add( this ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }

                char msg2[4] = {0x01 , 0x02 , 0x03 , 0xFE} ;
                if ( data_size == 4 && memcmp( data , msg2 , 4 ) == 0 ) {

                        this->set_heartjump_datetime( ) ;

                        // Response message.
                        this->send_data_in_thread( data , data_size ) ;

                        return true ;
                } else {
                        return false ;
                }
        }

        void message_split_action( const char* in , size_t in_size , std::vector<string>& message_set ) override {
                rev_num += in_size ;

                u64_t max = in_size / 4 ;
                u64_t n = 0 ;
                while ( n < max ) {
                        string msg ;
                        msg.append( in + n * 4 , 4 ) ;
                        message_set.push_back( msg ) ;
                        n++ ;
                }

                boStringAppend ap ;
                ap.add( "call split message action, message size:" )->add( message_set.size( ) )->add( ",in_size:" )->add( in_size )
                        ->add( ",hdr:" )->add( this ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        static u64_t rev_num ;

} ;
u64_t svr_hdr9::rev_num = 0 ;

typedef boScoketManager<svr_hdr9> manager9 ;
typedef boTcpServer<svr_hdr9 , manager9 , svr_parser> tcp_server_base9 ;

class svr9 : public tcp_server_base9 {
public:

        svr9( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port ) :
             tcp_server_base9( io_service , ipv4 , port ) {
        }

        virtual ~svr9( ) {
        }

        int accept_success_func( pointer ptr , boErrorWhat& ) override {
                boStringAppend ap ;
                ap.add( "call accept_success_func success, hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
                return 0 ;
        }
} ;

BOOST_AUTO_TEST_CASE( t9 ) {
        string ip = "0.0.0.0" ;
        u16_t port = 17015 ;
        BO_ASYNC_SERVICE io ;
        svr9 server( io , ip , port ) ;

        io.run( ) ;
}

BOOST_AUTO_TEST_SUITE_END( )
