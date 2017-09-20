/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbPublishReceiver.cpp
 * Author: root
 * 
 * Created on 2017年9月20日, 上午11:22
 */

#include "boRbPublishReceiver.h"

using namespace bingo::RabbitMQ::imp ;

boRbPublishReceiver::boRbPublishReceiver( boRbConnectDetail& conn , boLogHandler* logger ) :
     boRbReceiver( conn , logger ) {
}

boRbPublishReceiver::~boRbPublishReceiver( ) {
}

void boRbPublishReceiver::connet_to_server( ) {
        // Make connect service
        while ( true ) {

                {
                        BO_ASYNC_SERVICE ioService ;
                        string ipv4 = conn_.ip.c_str( ) ;
                        u16_t port = conn_.port ;
                        boRbAsioClient cet( ioService , ipv4 , port , boRbAsioClient::make_heartjump ) ;

                        AMQP::Connection connection( cet.handler( ) , AMQP::Login(
                                                                                   conn_.username.c_str( ) , conn_.pwd.c_str( ) ) , conn_.host.c_str( ) ) ;

                        AMQP::Channel channel( &connection ) ;

                        // Callback operation when a message was received
                        auto messageCb = [&]( const AMQP::Message &message , uint64_t deliveryTag , bool redelivered ) {
                                string s_msg = message.message( ) ;
                                read_message_complete( s_msg ) ;

                                // acknowledge the message
                                //        channel.ack(deliveryTag);
                        } ;

                        AMQP::QueueCallback callback = [&]( const std::string &name , int msgcount , int consumercount ) {

                                string s_msg = "consume operation started! key:" ;
                                s_msg.append( conn_.key ) ;
                                to_info( s_msg ) ;

                                channel.bindQueue( conn_.exchange.c_str( ) , name , "" ) ;
                                channel.consume( name , AMQP::noack ).
                                        onReceived( messageCb ) ;
                        } ;

                        AMQP::SuccessCallback success = [&]( ) {
                                channel.declareQueue( AMQP::exclusive ).onSuccess( callback ) ;
                        } ;

                        channel.declareExchange( conn_.exchange.c_str( ) , AMQP::fanout ).onSuccess( success ) ;

                        ioService.run( ) ;
                }


                {
                        string s_msg = "ioService is exit!" ;
                        to_info( s_msg ) ;
                }

                BO_THIS_THREAD::sleep( seconds( 2 ) ) ;
        }
}
