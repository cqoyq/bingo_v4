/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boTcpCetHandler.cpp
 * Author: root
 * 
 * Created on 2017年9月7日, 下午1:54
 */

#include "boTcpCetHandler.h"

#include "boTcpError.h"

#ifndef WIN32

using namespace bingo::TCP ;

#include "bingo_v4/boString.h"
#include "bingo_v4/boForeach.h"

boTcpCetHandler::boTcpCetHandler( boost::asio::io_service& io_service ,
                                  int max_interval_seconds_check_heartjump ,
                                  reconnect_func f1 ,
                                  authenticate_pass_func f2 ) :
     ios_( io_service ) ,
     socket_( io_service ) ,
     is_valid_( true ) ,
     f1_( f1 ) ,
     f2_( f2 ) ,
     max_interval_seconds_check_heartjump_( max_interval_seconds_check_heartjump ) {

}

boTcpCetHandler::~boTcpCetHandler( ) {
#ifdef BINGO_DEBUG_TCP_SERVER
        boStringAppend ap ;
        ap.add( "hdr:" )->add( this )->add( ",destory!" ) ;
        bo_debug_out( ap.to_string( ) ) ;
#endif
}

void boTcpCetHandler::start( ) {
        string pk ;
        boErrorWhat e_what ;
        if ( make_first_sended_package_func( pk , e_what ) == 0 ) {

                // If pk have no data, then start to asyc-read.
                if ( pk.length( ) == 0 ) {

                        // Start async read data, read finish to call read_handler().
                        socket_.async_read_some(
                                                 boost::asio::buffer( message_block , MESSAGE_MAX_OF_SIZE ) ,
                                                 boost::bind( &boTcpCetHandler::read_handler ,
                                                              this->shared_from_this( ) ,
                                                              boost::asio::placeholders::error ,
                                                              boost::asio::placeholders::bytes_transferred ) ) ;
                } else {
                        boost::asio::async_write( socket_ ,
                                                  buffer( pk.c_str( ) , pk.length( ) ) ,
                                                  boost::bind( &boTcpCetHandler::write_first_handler ,
                                                               this->shared_from_this( ) ,
                                                               boost::asio::placeholders::error ,
                                                               boost::asio::placeholders::bytes_transferred ,
                                                               pk ) ) ;
                }

        } else {

                // Throw exception, err_code is error_tcp_client_close_socket_because_make_first_package.
                catch_error( e_what ) ;

                close_socket( ) ;
                close_completed( 0 ) ;
        }
}

void boTcpCetHandler::catch_error( boErrorWhat& e_what ) {
        catch_error_func( this->shared_from_this( ) , e_what ) ;
}

void boTcpCetHandler::close_socket( ) {
        socket_.close( ) ;
}

void boTcpCetHandler::close( const boost::system::error_code& ec ) {
        using namespace boost::system::errc ;
        if ( ec.value( ) != operation_canceled ) {

                boErrorWhat e_what ;
                e_what.err_no( TCP_ERROR_TYPE_CLIENT_PASSIVE_CLOSE_SOCKET ) ;
                e_what.err_message( "tcp client socket is been passive closed!" ) ;

                catch_error( e_what ) ;

                // Passive close
                close_socket( ) ;
        }

        close_completed( ec.value( ) ) ;
}

void boTcpCetHandler::close_completed( int ec_value ) {
        is_valid_ = false ;
        set_authenticate_pass( false ) ;

        close_complete_func( this->shared_from_this( ) , ec_value ) ;

        // Start to reconnet.
        reconnet( ) ;
}

void boTcpCetHandler::reconnet( ) {
        f1_( ) ;
}

void boTcpCetHandler::set_authenticate_pass( bool is_pass ) {
        f2_( is_pass ) ;

        // Set heart-jump start to time.
        set_heartjump_datetime( ) ;
}

void boTcpCetHandler::read_handler( const boost::system::error_code& ec , size_t bytes_transferred ) {
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
                        e_what.err_no( TCP_ERROR_TYPE_CLIENT_MESSAGE_FORMAT_HAS_ERROR ) ;
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
                                 boost::bind( &boTcpCetHandler::read_handler ,
                                              this->shared_from_this( ) ,
                                              boost::asio::placeholders::error ,
                                              boost::asio::placeholders::bytes_transferred ) ) ;
}

void boTcpCetHandler::write_handler( const boost::system::error_code& ec , size_t bytes_transferred , string& pk ) {
        write_pk_full_complete_func( this->shared_from_this( ) , pk.c_str( ) , bytes_transferred , ec.value( ) ) ;
}

void boTcpCetHandler::write_first_handler( const boost::system::error_code& ec , size_t bytes_transferred , string& pk ) {
        write_pk_full_complete_func( this->shared_from_this( ) , pk.c_str( ) , bytes_transferred , ec.value( ) ) ;

        // Start async read data, read finish to call read_handler().
        socket_.async_read_some(
                                 boost::asio::buffer( message_block , MESSAGE_MAX_OF_SIZE ) ,
                                 boost::bind( &boTcpCetHandler::read_handler ,
                                              this->shared_from_this( ) ,
                                              boost::asio::placeholders::error ,
                                              boost::asio::placeholders::bytes_transferred ) ) ;
}

void boTcpCetHandler::send_data_in_thread( const char* data , size_t data_size ) {

        string in_data ;
        in_data.append( data , data_size ) ;
        ios_.post( BO_BIND( &boTcpCetHandler::active_send , this->shared_from_this( ) , in_data ) ) ;
}

void boTcpCetHandler::send_close_in_thread( boErrorWhat& e_what ) {
        boErrorWhat* p_err = new boErrorWhat( ) ;
        e_what.clone( *p_err ) ;
        ios_.post( BO_BIND( &boTcpCetHandler::active_close , this->shared_from_this( ) , p_err ) ) ;
}

void boTcpCetHandler::active_send( string& pk ) {
        if ( is_valid_ ) {
                boErrorWhat e_what ;
                if ( active_send_in_ioservice_func( this->shared_from_this( ) , pk , e_what ) == -1 ) {

                        catch_error( e_what ) ;

                        return ;
                }

                boost::asio::async_write( socket_ ,
                                          buffer( pk.c_str( ) , pk.length( ) ) ,
                                          boost::bind( &boTcpCetHandler::write_handler ,
                                                       this->shared_from_this( ) ,
                                                       boost::asio::placeholders::error ,
                                                       boost::asio::placeholders::bytes_transferred ,
                                                       pk ) ) ;

        }
}

void boTcpCetHandler::active_close( boErrorWhat*& e_what ) {
        if ( is_valid_ ) {
                catch_error( *e_what ) ;
                close_socket( ) ;
                close_completed( 0 ) ;
        }

        delete e_what ;
}

void boTcpCetHandler::set_heartjump_datetime( ) {
        p1_ = boost::posix_time::microsec_clock::local_time( ) ;
}

bool boTcpCetHandler::check_heartjump_timeout( ) {
        ptime now = boost::posix_time::microsec_clock::local_time( ) ;
        ptime p1 = now - seconds( max_interval_seconds_check_heartjump_ ) ;

        if ( p1_ < p1 ) {
                return true ;
        } else {
                return false ;
        }
}

#endif