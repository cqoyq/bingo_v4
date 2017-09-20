/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbPublishSendor.cpp
 * Author: root
 * 
 * Created on 2017年9月14日, 下午4:00
 */

#include "boRbPublishSendor.h"

using namespace bingo::RabbitMQ::imp ;

#include "../asio/asiohandler.h"

// --------------------------------- SendTask ----------------------------- //

boRbPublishSendor::SendTask::SendTask( boRbTaskData* exit_thr_flag , boRbPublishSendor* p ) :
     boRbSendTask<boRbPublishSendor>( exit_thr_flag , p ) {

}

boRbPublishSendor::SendTask::~SendTask( ) {
}

void boRbPublishSendor::SendTask::handle( const char* data , size_t data_size ) {
        if ( p_->is_valid( ) ) {
                // Send message body.
                bool re = p_->channel( )->publish( p_->conn( ).exchange.c_str( ) , "" , data , data_size ) ;
                if ( !re ) {
                        boString t ;
                        boStringAppend ap ;
                        ap.add( "send data fail!,data:" )->add( t.stream_to_string( data , data_size ) ) ;
                        p_->to_error( ap.to_string( ) ) ;
                }
        }
}

// --------------------------------- boRbPublishSendor ----------------------------- //

boRbPublishSendor::boRbPublishSendor( boRbConnectDetail& conn , boLogHandler* logger ) :
     boRbSendor( conn , logger ) {
        send_task_ = 0 ;

        // Make send service.
        make_send_svc( ) ;
}

boRbPublishSendor::~boRbPublishSendor( ) {
        if ( send_task_ ) delete send_task_ ;
}

void boRbPublishSendor::connet_to_server( ) {
        while ( true ) {

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
                        channel.declareExchange( conn_.exchange.c_str( ) , AMQP::fanout ).onSuccess( [&]( ) {
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

bool boRbPublishSendor::send_data( const char* data , size_t data_size ) {
        string in ;
        in.append( data , data_size ) ;

        // Make task data.        
        return (send_task_->put( new boRbTaskData( boRbTaskData::TASK_DATA_TYPE_MESSAGE , in ) ,
                                 this->e_what_ ) == 0 ) ? true : false ;
}

void boRbPublishSendor::make_send_svc( ) {

        // Define exit-flag.
        send_task_ = new SendTask( new boRbTaskData( boRbTaskData::TASK_DATA_TYPE_EXIT_FLAG ) , this ) ;
}

