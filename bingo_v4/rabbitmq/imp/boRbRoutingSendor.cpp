/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbRoutingSendor.cpp
 * Author: root
 * 
 * Created on 2017年9月20日, 下午1:05
 */

#include "boRbRoutingSendor.h"

using namespace bingo::RabbitMQ::imp ;

#include "../asio/asiohandler.h"


// --------------------------------- SendTask ----------------------------- //

boRbRoutingSendor::SendTask::SendTask( boRbTaskData* exit_thr_flag , boRbRoutingSendor* p ) :
     boRbSendTask<boRbRoutingSendor>( exit_thr_flag , p ) {

}

boRbRoutingSendor::SendTask::~SendTask( ) {
}

void boRbRoutingSendor::SendTask::handle( const char* data , size_t data_size ) {
        if ( p_->is_valid( ) && p_->conn( ).routingkey.size( ) == 1 ) {
                // Send message body.
                bool re = p_->channel( )->publish( p_->conn( ).exchange.c_str( ) , p_->conn( ).routingkey[0].c_str( ) , data , data_size ) ;
                if ( !re ) {
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "send data fail!,data:" )->add( t.stream_to_string( data , data_size ) ) ;
                        p_->to_error( ap.to_string( ) ) ;
                }
        }
}

// --------------------------------- boRbPublishSendor ----------------------------- //

boRbRoutingSendor::boRbRoutingSendor( boRbConnectDetail& conn , boLogHandler* logger ) :
     boRbSendor( conn , logger ) {
        send_task_ = 0 ;

        // Make send service.
        make_send_svc( ) ;
}

boRbRoutingSendor::~boRbRoutingSendor( ) {
        if ( send_task_ ) delete send_task_ ;
}

void boRbRoutingSendor::connet_to_server( ) {
        while ( true ) {
                // Check routingkey set size.
                if ( conn_.routingkey.size( ) == 0 ) {
                        string s_msg = "connect to server fail, routingkey size is 0! key:" ;
                        s_msg.append( conn_.key ) ;
                        to_error( s_msg ) ;
                        return ;
                }

                {
                        BO_ASYNC_SERVICE ioService ;
                        string ipv4 = conn_.ip.c_str( ) ;
                        u16_t port = conn_.port ;
                        boRbAsioClient cet( ioService , ipv4 , port , boRbAsioClient::make_heartjump ) ;

                        AMQP::Connection connection( cet.handler( ) , AMQP::Login(
                                                                                   conn_.username.c_str( ) , conn_.pwd.c_str( ) ) , conn_.host.c_str( ) ) ;

                        AMQP::Channel channel( &connection ) ;

                        // Save channel pointer.
                        conn_.channel = &channel ;

                        // Declare an exchange
                        channel.declareExchange( conn_.exchange.c_str( ) , AMQP::direct ).onSuccess( [&]( ) {
                                //                                                channel.publish(p->exchange.c_str(), "", msg);
                                string s_msg = "send ready ok! key:" + conn_.key ;
                                to_info( s_msg ) ;

                                // Set is_valid is ture. 
                                this->is_valid_ = true ;

                        } ) ;

                        ioService.run( ) ;
                }

                {
                        string s_msg = "ioService is exit!" ;
                        to_info( s_msg ) ;
                }

                // Set is_valid is false. 
                this->is_valid_ = false ;

                BO_THIS_THREAD::sleep( seconds( 2 ) ) ;
        }

}

bool boRbRoutingSendor::send_data( const char* data , size_t data_size ) {
        string in ;
        in.append( data , data_size ) ;

        // Make task data.        
        return (send_task_->put( new boRbTaskData( boRbTaskData::TASK_DATA_TYPE_MESSAGE , in ) ,
                                 this->e_what_ ) == 0 ) ? true : false ;
}

void boRbRoutingSendor::make_send_svc( ) {

        // Define exit-flag.
        send_task_ = new SendTask( new boRbTaskData( boRbTaskData::TASK_DATA_TYPE_EXIT_FLAG ) , this ) ;
}
