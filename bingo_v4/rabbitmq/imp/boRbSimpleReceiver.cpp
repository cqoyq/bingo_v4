/*
 * simple_receiver.cpp
 *
 *  Created on: 2016-9-2
 *      Author: root
 */
#include "boRbSimpleReceiver.h"

using namespace bingo::RabbitMQ::imp ;

#include "../asio/asiohandler.h"

boRbSimpleReceiver::boRbSimpleReceiver( boRbConnectDetail& conn , boLogHandler* logger ) :
     boRbReceiver( conn , logger ) {
        // TODO Auto-generated constructor stub
}

boRbSimpleReceiver::~boRbSimpleReceiver( ) {
        // TODO Auto-generated destructor stub
}

void boRbSimpleReceiver::connet_to_server( ) {
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
                        channel.declareQueue( conn_.routingkey[0].c_str( ) ) ;

                        // Callback function that is called when the consume operation starts
                        auto startCb = [&]( const std::string & consumertag ) {

                                string s_msg = "consume operation started! key:" ;
                                s_msg.append( conn_.key ) ;
                                to_info( s_msg ) ;
                        } ;

                        // Callback function that is called when the consume operation failed
                        auto errorCb = [&]( const char *message ) {

                                boStringAppend ap ;
                                ap.add( "channel error, err_msg:" )->add( message ) ;
                                to_error( ap.to_string( ) ) ;
                        } ;

                        // Callback operation when a message was received
                        auto messageCb = [&]( const AMQP::Message &message , uint64_t deliveryTag , bool redelivered ) {
                                string s_msg = message.message( ) ;
                                read_message_complete( s_msg ) ;

                                // acknowledge the message
                                //        channel.ack(deliveryTag);
                        } ;


                        channel.consume( "" , AMQP::noack )
                                .onReceived( messageCb )
                                .onSuccess( startCb )
                                .onError( errorCb ) ;

                        ioService.run( ) ;
                }


                {
                        string s_msg = "ioService is exit!" ;
                        to_info( s_msg ) ;
                }

                BO_THIS_THREAD::sleep( seconds( 2 ) ) ;
        }
}


