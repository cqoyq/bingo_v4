#include <iostream>
#include <thread>

#include "asiohandler.h"

#ifndef WIN32

// ------------------------------------------ boRbAsioParser -------------------------------------------- //

int boRbAsioParser::retry_delay_seconds = 0 ;
int boRbAsioParser::max_retry_delay_seconds = 60 ;
int boRbAsioParser::max_interval_seconds_send_heartjump = 60 ;
int boRbAsioParser::max_interval_seconds_check_heartjump = 0 ;

// ------------------------------------------ boRbAsioHandler -------------------------------------------- //

boRbAsioHandler::boRbAsioHandler( BO_ASYNC_SERVICE& io_service ,
                                  int max_interval_seconds_check_heartjump ,
                                  reconnect_func f1 ,
                                  authenticate_pass_func f2 ) : boTcpCetHandler( io_service , max_interval_seconds_check_heartjump , f1 , f2 ) {
        is_first_ = true ;
}

boRbAsioHandler::~boRbAsioHandler( ) {
#ifdef BINGO_MQ_DEBUG 
        boStringAppend ap ;
        ap.add( "hdr:" )->add( this )->add( ",destory!" ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif
}

void boRbAsioHandler::onData( Connection* connection , const char* buffer , size_t size ) {

        _connection = connection ;

        if ( is_first_ ) {
                is_first_ = false ;
        } else {
                this->send_data_in_thread( buffer , size ) ;
        }
}

void boRbAsioHandler::onConnected( Connection* connection ) {
        //        cout << "call onConnected,_connection:" << connection << endl ;
}

void boRbAsioHandler::onClosed( Connection* connection ) {
        //        cout << "AMQP closed connection" << endl ;
}

void boRbAsioHandler::onError( Connection* connection , const char* message ) {
        //        string err = "AMQP error, err_msg:" ;
        //        err.append( message ) ;
        //        cout << err << endl ;
}

void boRbAsioHandler::onHeartbeat( Connection* connection ) {
#ifdef BINGO_MQ_DEBUG 
        boStringAppend ap ;
        ap.add( "call onHeartbeat()" ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif

        p1_ = boost::posix_time::microsec_clock::local_time( ) ;
}

void boRbAsioHandler::catch_error_func( pointer ptr , boErrorWhat& e ) {
#ifdef BINGO_MQ_DEBUG 
        boStringAppend ap ;
        ap.add( "call catch_error_func(), err_msg:" )->add( e.err_message( ) )
                ->add( ",err_no:" )->add( e.err_no( ) )
                ->add( ",hdr:" )->add( this ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif
}

void boRbAsioHandler::close_complete_func( pointer ptr , int& err_code ) {
#ifdef BINGO_MQ_DEBUG 
        boStringAppend ap ;
        ap.add( "call close_complete_func(), ec_value:" )->add( err_code )->add( ",hdr:" )->add( this ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif     
        // Close cancel.
        _connection->close( ) ;

        // Stop ios.
        ios_.stop( ) ;
}

void boRbAsioHandler::message_split_action( const char* in , size_t in_size , std::vector<string>& message_set ) {
        string msg ;
        msg.append( in , in_size ) ;
        message_set.push_back( msg ) ;
}

int boRbAsioHandler::read_pk_full_complete_func( pointer ptr , const char* in , size_t in_size , boErrorWhat& e ) {

        // Log output.
        //        {
        //                boString t ;
        //                boStringAppend ap ;
        //                ap.add( "read data:" )->add( t.stream_to_string( in , in_size ) )->add( ",lenght:" )->add( in_size ) ;
        //                bo_debug_out( ap.to_string( ) ) ;
        //        }

        remain_data_.append( in , in_size ) ;

        parse_data( remain_data_.c_str( ) , remain_data_.length( ) ) ;

        return 0 ;
}

void boRbAsioHandler::write_pk_full_complete_func( pointer ptr , const char* data , size_t data_size , int err_code ) {
#ifdef BINGO_MQ_DEBUG 
        boString t ;
        boStringAppend ap ;
        ap.add( "send data to server!data:" )->add( t.stream_to_string( data , data_size ) )->add( ",data_size:" )->add( data_size ) ->add( ",hdr:" )->add( this ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif
}

int boRbAsioHandler::make_first_sended_package_func( string& out , boErrorWhat& e ) {
        char auth[8] = {0x41 , 0x4d , 0x51 , 0x50 , 0x00 , 0x00 , 0x09 , 0x01} ;
        out.append( auth , 8 ) ;
        return 0 ;
}

void boRbAsioHandler::parse_data( const char* in , size_t in_size ) {
        const size_t count = _connection->parse( in , in_size ) ;
        if ( count > 0 ) {
                u32_t remain = in_size - count ;
                remain_data_.clear( ) ;
                remain_data_.append( in + count , remain ) ;
        }
}

// ------------------------------------------ boRbAsioClient -------------------------------------------- //

boRbAsioClient::boRbAsioClient( boost::asio::io_service& io_service , string& ipv4 , u16_t& port , make_heartjump_message_func f1 ) :
     RABBITMQ_CET_BASE_TYPE( io_service , ipv4 , port , f1 ) {

}

boRbAsioClient::~boRbAsioClient( ) {

}

void boRbAsioClient::make_heartjump( string& out ) {
        char heart[8] = {0x08 , 0x00 , 0x00 , 0x00 ,
                0x00 , 0x00 , 0x00 , 0xce} ;

        out.append( heart , 8 ) ;
}

void boRbAsioClient::connet_fail_func( pointer ptr , int& retry_delay_seconds ) {
#ifdef BINGO_MQ_DEBUG 
        boStringAppend ap ;
        ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect fail, reconnect after " )->add( retry_delay_seconds )->add( " seconds" ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif
}

int boRbAsioClient::connet_success_func( pointer ptr , boErrorWhat& e ) {
#ifdef BINGO_MQ_DEBUG 
        boStringAppend ap ;
        ap.add( "hdr:" )->add( ptr.get( ) )->add( " connect success!" ) ;
        ::bo_debug_out( ap.to_string( ) ) ;
#endif
        this->set_authenticate_pass( true ) ;
        return 0 ;
}

#endif