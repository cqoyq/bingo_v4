/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <boost/test/unit_test.hpp>

#include "../../bingo_v4/tcp/boTcpClient.h"
#include "../../bingo_v4/tcp/boTcpCetHandler.h"
#include "../../bingo_v4/tcp/boTcpError.h"
#include "../../bingo_v4/boString.h"
using namespace bingo::TCP ;
using namespace bingo ;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time ;

BOOST_AUTO_TEST_SUITE( test_tcp_client )

struct cet_parser {
        static int retry_delay_seconds ; // when client connect fail, reconnect wait for max seconds, if the value is 0, then reconnect don't call.
        static int max_retry_delay_seconds ;

        static int max_interval_seconds_send_heartjump ; // If value is 0, then don't send heartjump.
        static int max_interval_seconds_check_heartjump ; // If value is 0, then don't check heartjump.
} ;
int cet_parser::retry_delay_seconds = 1 ;
int cet_parser::max_retry_delay_seconds = 60 ;
int cet_parser::max_interval_seconds_send_heartjump = 4 ;
int cet_parser::max_interval_seconds_check_heartjump = 6 ;

// Test tcp client, the client connect to server fail, the client auto reconnect .

class cet_hdr1 : public boTcpCetHandler {
public:

        cet_hdr1( BO_ASYNC_SERVICE& io_service ,
                  int max_interval_seconds_check_heartjump ,
                  reconnect_func f1 ,
                  authenticate_pass_func f2 ) : boTcpCetHandler( io_service , max_interval_seconds_check_heartjump , f1 , f2 ) {
        }

        virtual ~cet_hdr1( ) {
        }

        virtual void catch_error_func( pointer ptr , boErrorWhat& e ) {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual void close_complete_func( pointer ptr , int& ec_value ) {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), ec_value:" )->add( ec_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }


} ;

typedef boTcpClient<cet_hdr1 , cet_parser> tcp_cet_base1 ;

class cet1 : public tcp_cet_base1 {
public:

        cet1( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port , make_heartjump_message_func f1 ) :
             tcp_cet_base1( io_service , ipv4 , port , f1 ) {
        }

        virtual ~cet1( ) {
        }

        virtual void connet_fail_func( pointer ptr , int& retry_delay_seconds ) {
                boStringAppend ap ;
                ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect fail, reconnect after " )->add( retry_delay_seconds )->add( " seconds" ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual int connet_success_func( pointer ptr , boErrorWhat& e ) {
                boStringAppend ap ;
                ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect success!" ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        static void make_heartjump( string& out ) {

        }
} ;

BOOST_AUTO_TEST_CASE( t1 ) {
        BO_ASYNC_SERVICE io_service ;
        string ipv4 = "127.0.0.1" ;
        u16_t port = 17015 ;
        cet1 c( io_service , ipv4 , port , cet1::make_heartjump ) ;

        io_service.run( ) ;
}

// Test tcp client, it is success that the client connect to server, and the server disconnect, the client auto reconnect, the server is t_tcp_server/t9.

BOOST_AUTO_TEST_CASE( t2 ) {
        BO_ASYNC_SERVICE io_service ;
        string ipv4 = "127.0.0.1" ;
        u16_t port = 17015 ;
        cet1 c( io_service , ipv4 , port , cet1::make_heartjump ) ;

        io_service.run( ) ;
}

// Test tcp client, the client connect to server, the message is transfer both server and server, the server is t_tcp_server/t9.

class cet_hdr3 : public boTcpCetHandler {
public:

        cet_hdr3( BO_ASYNC_SERVICE& io_service ,
                  int max_interval_seconds_check_heartjump ,
                  reconnect_func f1 ,
                  authenticate_pass_func f2 ) : boTcpCetHandler( io_service , max_interval_seconds_check_heartjump , f1 , f2 ) {
        }

        virtual ~cet_hdr3( ) {
        }

        void catch_error_func( pointer ptr , boErrorWhat& e ) override {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        void close_complete_func( pointer ptr , int& ec_value ) override {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), ec_value:" )->add( ec_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        int make_first_sended_package_func( string& out , boErrorWhat& e ) override {
                char auth_msg[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                out.append( auth_msg , 4 ) ;
                return 0 ;
        }

        void write_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , int error_code ) override {
                boStringAppend ap ;
                boString t ;
                ap.add( "snd data:" )->add( t.stream_to_string( data , data_size ) )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) override {
                {
                        boStringAppend ap ;
                        boString t ;
                        ap.add( "rev data:" )->add( t.stream_to_string( data , data_size ) )->add( ",hdr:" )->add( ptr.get( ) ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }
        }

        bool message_authenticate_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                char msg2[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                if ( data_size == 4 && memcmp( data , msg2 , 4 ) == 0 ) {

                        this->set_authenticate_pass( true ) ;

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

        void message_split_action( const char* in , size_t in_size , std::vector<string>& message_set ) override {
                u64_t max = in_size / 4 ;
                u64_t n = 0 ;
                while ( n < max ) {
                        string msg ;
                        msg.append( in + n * 4 , 4 ) ;
                        message_set.push_back( msg ) ;
                        n++ ;
                }
        }

} ;
typedef boTcpClient<cet_hdr3 , cet_parser> tcp_cet_base3 ;

class cet3 : public tcp_cet_base3 {
public:

        cet3( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port , make_heartjump_message_func f1 ) :
             tcp_cet_base3( io_service , ipv4 , port , f1 ) {
        }

        virtual ~cet3( ) {
        }

        virtual void connet_fail_func( pointer ptr , int& retry_delay_seconds ) {
                boStringAppend ap ;
                ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect fail, reconnect after " )->add( retry_delay_seconds )->add( " seconds" ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual int connet_success_func( pointer ptr , boErrorWhat& e ) {
                boStringAppend ap ;
                ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect success!" ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        static void make_heartjump( string& out ) {

        }
} ;

cet3* pCet3 = 0 ;

void send_msg3( ) {
        this_thread::sleep( seconds( 6 ) ) ;

        int n = 0 ;
        while ( n < 10 ) {
                char data[4] = {0x01 , 0x02 , 0x03 , 0xDD} ;
                memcpy( data + 1 , &n , 1 ) ;

                pCet3->send_data( data , 4 ) ;

                n++ ;
        }

}

BOOST_AUTO_TEST_CASE( t3 ) {
        BO_THREAD t1( send_msg3 ) ;

        BO_ASYNC_SERVICE io_service ;
        string ipv4 = "127.0.0.1" ;
        u16_t port = 17015 ;
        cet3 c( io_service , ipv4 , port , cet1::make_heartjump ) ;

        pCet3 = &c ;

        io_service.run( ) ;
}

// Test tcp client, the client connect to server, the client send authentication message to server by first, 
// and then wait for message from the server,  the server is t_tcp_server/t9.

BOOST_AUTO_TEST_CASE( t4 ) {
        // Same to t3.
}

// Test tcp client, the client connect to server, authenticae is pass, the client auto send heart-jump.

class cet_hdr5 : public boTcpCetHandler {
public:

        cet_hdr5( BO_ASYNC_SERVICE& io_service ,
                  int max_interval_seconds_check_heartjump ,
                  reconnect_func f1 ,
                  authenticate_pass_func f2 ) : boTcpCetHandler( io_service , max_interval_seconds_check_heartjump , f1 , f2 ) {
        }

        virtual ~cet_hdr5( ) {
        }

        virtual void catch_error_func( pointer ptr , boErrorWhat& e ) {
                boStringAppend ap ;
                ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                        ->add( ",err_no:" )->add( e.err_no( ) )
                        ->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual void close_complete_func( pointer ptr , int& ec_value ) {
                boStringAppend ap ;
                ap.add( "call close_complete_func(), ec_value:" )->add( ec_value )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual int make_first_sended_package_func( string& out , boErrorWhat& e ) {
                char auth_msg[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                out.append( auth_msg , 4 ) ;
                return 0 ;
        }

        virtual void write_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , const boost::system::error_code& e ) {
                boStringAppend ap ;
                boString t ;
                ap.add( "snd data:" )->add( t.stream_to_string( data , data_size ) )->add( ",hdr:" )->add( ptr.get( ) ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual int read_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , boErrorWhat& e ) {
                {
                        boStringAppend ap ;
                        boString t ;
                        ap.add( "rev data:" )->add( t.stream_to_string( data , data_size ) )->add( ",hdr:" )->add( ptr.get( ) ) ;
                        ::bo_debug_out( ap.to_string( ) ) ;
                }

                char auth_msg[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                if ( data_size == 4 && memcmp( data , auth_msg , 4 ) == 0 ) {
                        this->set_authenticate_pass( true ) ;
                        {
                                boStringAppend ap ;
                                ap.add( "authenticate is pass" ) ;
                                ::bo_debug_out( ap.to_string( ) ) ;
                        }
                }

                char heartjump_msg[4] = {0x01 , 0x02 , 0x03 , 0xFE} ;
                if ( data_size == 4 && memcmp( data , heartjump_msg , 4 ) == 0 ) {
                        this->set_heartjump_datetime( ) ;
                        {
                                boStringAppend ap ;
                                ap.add( "heartjump is pass" ) ;
                                ::bo_debug_out( ap.to_string( ) ) ;
                        }
                }
        }

        bool message_authenticate_action( string& message ) override {
                const char* data = message.c_str( ) ;
                size_t data_size = message.length( ) ;

                char msg2[4] = {0x01 , 0x02 , 0x03 , 0xFF} ;
                if ( data_size == 4 && memcmp( data , msg2 , 4 ) == 0 ) {

                        this->set_authenticate_pass( true ) ;

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

                char msg2[4] = {0x01 , 0x02 , 0x03 , 0xFE} ;
                if ( data_size == 4 && memcmp( data , msg2 , 4 ) == 0 ) {

                        this->set_heartjump_datetime( ) ;

                        {
                                boString t ;
                                boStringAppend ap ;
                                ap.add( "call heartjump action, data:" )->add( t.stream_to_string( data , data_size ) )
                                        ->add( ",hdr:" )->add( this ) ;
                                ::bo_debug_out( ap.to_string( ) ) ;
                        }

                        return true ;
                } else {
                        return false ;
                }
        }

        void message_split_action( const char* in , size_t in_size , std::vector<string>& message_set ) override {

                u64_t max = in_size / 4 ;
                u64_t n = 0 ;
                while ( n < max ) {
                        string msg ;
                        msg.append( in + n * 4 , 4 ) ;
                        message_set.push_back( msg ) ;
                        n++ ;
                }
        }

} ;
typedef boTcpClient<cet_hdr5 , cet_parser> tcp_cet_base5 ;

class cet5 : public tcp_cet_base5 {
public:

        cet5( BO_ASYNC_SERVICE& io_service , string& ipv4 , u16_t& port , make_heartjump_message_func f1 ) :
             tcp_cet_base5( io_service , ipv4 , port , f1 ) {
        }

        virtual ~cet5( ) {
        }

        virtual void connet_fail_func( pointer ptr , int& retry_delay_seconds ) {
                boStringAppend ap ;
                ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect fail, reconnect after " )->add( retry_delay_seconds )->add( " seconds" ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        virtual int connet_success_func( pointer ptr , boErrorWhat& e ) {
                boStringAppend ap ;
                ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect success!" ) ;
                ::bo_debug_out( ap.to_string( ) ) ;
        }

        static void make_heartjump( string& out ) {
                char heartjump_msg[4] = {0x01 , 0x02 , 0x03 , 0xFE} ;
                out.append( heartjump_msg , 4 ) ;
        }
} ;

BOOST_AUTO_TEST_CASE( t5 ) {
        BO_ASYNC_SERVICE io_service ;
        string ipv4 = "127.0.0.1" ;
        u16_t port = 17015 ;
        cet5 c( io_service , ipv4 , port , cet5::make_heartjump ) ;

        io_service.run( ) ;
}

// Test tcp client, the client connect to server, authenticae isn't pass, the client no send heart-jump, the server is t_tcp_server/t9.

BOOST_AUTO_TEST_CASE( t6 ) {
        // Same to t3
}

BOOST_AUTO_TEST_SUITE_END( )
