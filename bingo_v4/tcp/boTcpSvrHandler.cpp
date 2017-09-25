/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>

#include "boTcpSvrHandler.h"

#include "boTcpError.h"

#ifndef WIN32

using namespace bingo::TCP ;

#include "bingo_v4/boString.h"
#include "bingo_v4/boForeach.h"
#include "bingo_v4/algorithm/boAlgorithmFactory.h"
using namespace bingo::algorithm ;

boTcpSvrHandler::boTcpSvrHandler( boost::asio::io_service& io_service ,
                                  int max_wait_for_heartjump_seconds ,
                                  int max_wait_for_authentication_pass_seconds ,
                                  remove_socket_func f1 ,
                                  void* acceptor ) :
     ios_( io_service ) ,
     socket_( io_service ) ,
     is_valid_( true ) ,
     is_authentication_pass_( false ) ,
     max_wait_for_heartjump_seconds_( max_wait_for_heartjump_seconds ) ,
     max_wait_for_authentication_pass_seconds_( max_wait_for_authentication_pass_seconds ) ,
     f1_( f1 ) ,
     acceptor_( acceptor ) {

        memset( message_block , 0x00 , MESSAGE_MAX_OF_SIZE ) ;

        // Save uuid.
        uuid_flag_ = boAlgorithmFactory::uuid->make_random_uuid( ) ;

        set_heartjump_datetime( ) ;
        set_active_heartjump_datetime( ) ;
        set_authentication_pass_datetime( ) ;
}

boTcpSvrHandler::~boTcpSvrHandler( ) {
#ifdef BINGO_DEBUG_TCP_SERVER
        boStringAppend ap ;
        ap.add( "hdr:" )->add( this )->add( ",destory!" ) ;
        bo_debug_out( ap.to_string( ) ) ;
#endif
}

void boTcpSvrHandler::start( ) {
        // Set active heart-jump start to time.
        set_active_heartjump_datetime( ) ;

        // Set authentication start to time.
        set_authentication_pass_datetime( ) ;

        // Start async read data, read finish to call read_handler().
        socket_.async_read_some(
                                 boost::asio::buffer( message_block , MESSAGE_MAX_OF_SIZE ) ,
                                 boost::bind( &boTcpSvrHandler::read_handler ,
                                              this->shared_from_this( ) ,
                                              boost::asio::placeholders::error ,
                                              boost::asio::placeholders::bytes_transferred ) ) ;
}

void boTcpSvrHandler::close_socket( ) {
        socket_.close( ) ;
}

void boTcpSvrHandler::catch_error( boErrorWhat& e_what ) {
        catch_error_func( this->shared_from_this( ) , e_what ) ;
}

bool boTcpSvrHandler::check_heartjump_timeout( ) {
        if ( is_authentication_pass_ ) {
                ptime now = boost::posix_time::microsec_clock::local_time( ) ;
                ptime p1 = now - seconds( max_wait_for_heartjump_seconds_ ) ;

#ifdef BINGO_DEBUG_TCP_SERVER
                boStringAppend oss ;
                oss.add( "check heartjump time, p1_:" )->add( to_iso_extended_string( p1_ ) )
                        ->add( ",p1:" )->add( to_iso_extended_string( p1 ) )
                        ->add( ",p1_< p1:" )->add( p1_ < p1 )
                        ->add( ",hdr:" )->add( this ) ;
                bo_debug_out( oss.to_string( ) ) ;
#endif

                if ( p1_ < p1 ) {
                        return true ;
                } else {
                        return false ;
                }

        } else {
                return false ;
        }
}

void boTcpSvrHandler::set_heartjump_datetime( ) {
        p1_ = boost::posix_time::microsec_clock::local_time( ) ;

#ifdef BINGO_DEBUG_TCP_SERVER
        boStringAppend oss ;
        oss.add( "set heartjump time:" )->add( to_iso_extended_string( p1_ ) )->add( ",hdr:" )->add( this ) ;
        bo_debug_out( oss.to_string( ) ) ;
#endif
}

void boTcpSvrHandler::set_active_heartjump_datetime( ) {
        p3_ = boost::posix_time::microsec_clock::local_time( ) ;
}

bool boTcpSvrHandler::check_authentication_pass( ) {
        if ( !is_authentication_pass_ ) {
                // Check whether authentication pass is timeout.
                ptime now = boost::posix_time::microsec_clock::local_time( ) ;
                ptime p2 = now - seconds( max_wait_for_authentication_pass_seconds_ ) ;

                if ( p2_ < p2 )
                        return false ;
                else
                        return true ;

        } else {
                return true ;
        }
}

void boTcpSvrHandler::set_authentication_pass( ) {
        is_authentication_pass_ = true ;

        // Set heart-jump start to time.
        set_heartjump_datetime( ) ;
}

bool boTcpSvrHandler::get_authentication_pass( ) {
        return is_authentication_pass_ ;
}

void boTcpSvrHandler::set_authentication_pass_datetime( ) {
        p2_ = boost::posix_time::microsec_clock::local_time( ) ;

#ifdef BINGO_DEBUG_TCP_SERVER
        boStringAppend oss ;
        oss.add( "set authentication pass time : " )->add( to_iso_extended_string( p2_ ) )->add( ",hdr: " )->add( this ) ;
        bo_debug_out( oss.to_string( ) ) ;
#endif
}

void boTcpSvrHandler::send_heartjump_in_thread( ) {
        ios_.post( BO_BIND( &boTcpSvrHandler::active_send_heartjump , this->shared_from_this( ) ) ) ;
}

void boTcpSvrHandler::send_data_in_thread( const char* data , size_t data_size ) {
        string in_data ;
        in_data.append( data , data_size ) ;

        ios_.post( BO_BIND( &boTcpSvrHandler::active_send_data , this->shared_from_this( ) , in_data ) ) ;
}

void boTcpSvrHandler::send_close_in_thread( boErrorWhat& e_what ) {
        boErrorWhat* p_err = new boErrorWhat( ) ;
        e_what.clone( *p_err ) ;
        ios_.post( BO_BIND( &boTcpSvrHandler::active_close , this->shared_from_this( ) , p_err ) ) ;
}

void boTcpSvrHandler::active_send_heartjump( ) {
        if ( is_valid_ ) {
                active_send_heartjump_in_ioservice_func( this->shared_from_this( ) ) ;
        }
}

void boTcpSvrHandler::active_close( boErrorWhat*& e_what ) {
        if ( is_valid_ ) {

                catch_error( *e_what ) ;
                close_socket( ) ;
                close_completed( 0 ) ;

        }

        delete e_what ;
}

void boTcpSvrHandler::active_send_data( string& pk ) {
        if ( is_valid_ ) {
                boErrorWhat e_what ;
                if ( active_send_data_in_ioservice_func( this->shared_from_this( ) , pk , e_what ) == -1 ) {

                        catch_error( e_what ) ;

                        return ;
                }

                boost::asio::async_write( socket_ ,
                                          buffer( pk.c_str( ) , pk.length( ) ) ,
                                          boost::bind( &boTcpSvrHandler::write_handler ,
                                                       this->shared_from_this( ) ,
                                                       boost::asio::placeholders::error ,
                                                       boost::asio::placeholders::bytes_transferred ,
                                                       pk ) ) ;

        }
}

void boTcpSvrHandler::read_handler( const boost::system::error_code& ec , size_t bytes_transferred ) {
        if ( !is_valid_ )
                return ;

        // Check error.
        if ( ec ) {
                close( ec ) ;
                return ;
        }

        // Split message-block.
        std::vector<string> msgs ;
        message_split_action( message_block , bytes_transferred , msgs ) ;

        // Handle msgs.

        foreach_( string& n , msgs ) {
                // Check message format.
                if ( !message_format_action( n ) ) {

                        //  Throw error, heartjump format is error.
                        boErrorWhat e_what ;
                        e_what.err_no( TCP_ERROR_TYPE_SERVER_MESSAGE_FORMAT_HAS_ERROR ) ;
                        e_what.err_message( "message format is error" ) ;
                        catch_error( e_what ) ;

                        close_socket( ) ;
                        close_completed( ec.value( ) ) ;

                        return ;
                }

                // Authenticate message.
                if ( message_authenticate_action( n ) ) {
                        continue ;
                }

                // Heartjump message.
                if ( message_heartjump_action( n ) ) {
                        continue ;
                }


                if ( !this->get_authentication_pass( ) )
                        continue ;

                // Check common message.
                boErrorWhat e_what ;
                if ( read_pk_full_complete_func(
                                                 this->shared_from_this( ) ,
                                                 n.c_str( ) ,
                                                 n.length( ) ,
                                                 e_what ) == -1 ) {

                        //  Throw error, and err_code is error_tcp_package_body_is_wrong.
                        catch_error( e_what ) ;

                        close_socket( ) ;
                        close_completed( ec.value( ) ) ;

                        return ;
                }
        }


        // Start async read data, read finish to call read_handler().
        socket_.async_read_some(
                                 boost::asio::buffer( message_block , MESSAGE_MAX_OF_SIZE ) ,
                                 boost::bind( &boTcpSvrHandler::read_handler ,
                                              this->shared_from_this( ) ,
                                              boost::asio::placeholders::error ,
                                              boost::asio::placeholders::bytes_transferred ) ) ;


}

void boTcpSvrHandler::write_handler( const boost::system::error_code& ec , size_t bytes_transferred , string& pk ) {
        write_pk_full_complete_func( this->shared_from_this( ) , pk.c_str( ) , bytes_transferred , ec.value( ) ) ;
}

void boTcpSvrHandler::close( const boost::system::error_code& ec ) {
        using namespace boost::system::errc ;
        if ( ec.value( ) != operation_canceled ) {

                boErrorWhat e_what ;
                e_what.err_no( TCP_ERROR_TYPE_SERVER_PASSIVE_CLOSE_SOCKET ) ;
                e_what.err_message( "tcp server socket is passive closed!" ) ;

                catch_error( e_what ) ;

                // Passive close
                close_socket( ) ;
        }

        close_completed( ec.value( ) ) ;
}

void boTcpSvrHandler::close_completed( int ec_value ) {

        if ( f1_ ) f1_( this->uuid_flag_ ) ;

        is_valid_ = false ;

        close_complete_func( this->shared_from_this( ) , ec_value ) ;
}

#endif