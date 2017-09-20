/*
 * simple_receiver.h
 *
 *  Created on: 2016-9-2
 *      Author: root
 */

#ifndef BINGO_RABBITMQ_IMP_SIMPLE_RECEIVER_H_
#define BINGO_RABBITMQ_IMP_SIMPLE_RECEIVER_H_

#include "../boRbReceiver.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        namespace RabbitMQ {
                namespace imp {
                        class boRbSimpleReceiver : public boRbReceiver {
                        public:
                                boRbSimpleReceiver(boRbConnectDetail& conn, boLogHandler* logger);
                                virtual ~boRbSimpleReceiver();

                                // Override connet_to_server() method.
                                virtual void connet_to_server();
                        };

                }
        }
}

#endif /* BINGO_RABBITMQ_IMP_SIMPLE_RECEIVER_H_ */
