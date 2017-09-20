/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbTopicReceiver.cpp
 * Author: root
 * 
 * Created on 2017年9月20日, 下午1:17
 */

#include "boRbTopicReceiver.h"

using namespace bingo::RabbitMQ::imp ;

#include "../asio/asiohandler.h"
#include "bingo_v4/boForeach.h"

boRbTopicReceiver::boRbTopicReceiver( boRbConnectDetail& conn , boLogHandler* logger ) :
     boRbReceiver( conn , logger ) {
}

boRbTopicReceiver::~boRbTopicReceiver( ) {
}

void boRbTopicReceiver::connet_to_server( ) {
        // Make connect service
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

                        auto receiveMessageCallback = [&]( const AMQP::Message &message , uint64_t deliveryTag , bool redelivered ) {
                                string s_msg = message.message( ) ;
                                read_message_complete( s_msg ) ;
                        } ;

                        AMQP::QueueCallback callback = [&]( const std::string &name , int msgcount , int consumercount ) {

                                string s_msg = "consume operation started! key:" ;
                                s_msg.append( conn_.key ) ;
                                to_info( s_msg ) ;

                                foreach_( string& n , conn_.routingkey ) {
                                        channel.bindQueue( conn_.exchange.c_str( ) , name , n.c_str( ) ) ;
                                        channel.consume( name , AMQP::noack ).onReceived( receiveMessageCallback ) ;
                                }

                        } ;

                        AMQP::SuccessCallback success = [&]( ) {
                                channel.declareQueue( AMQP::exclusive ).onSuccess( callback ) ;
                        } ;

                        channel.declareExchange( conn_.exchange.c_str( ) , AMQP::topic ).onSuccess( success ) ;

                        ioService.run( ) ;
                }


                {
                        string s_msg = "ioService is exit!" ;
                        to_info( s_msg ) ;
                }

                BO_THIS_THREAD::sleep( seconds( 2 ) ) ;
        }
}