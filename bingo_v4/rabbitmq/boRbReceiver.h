/*
 * receiver.h
 *
 *  Created on: 2016-9-2
 *      Author: root
 */

#ifndef BINGO_RABBITMQ_RECEIVER_H_
#define BINGO_RABBITMQ_RECEIVER_H_

#include "bingo_v4/boType.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/log/boLogHandler.h"
#include "bingo_v4/log/boLogBase.h"
using namespace bingo::log;

#include "boRbConnectDetail.h"

namespace bingo {
        namespace RabbitMQ {
                class boRbReceiver : public boLogBase {
                public:
                        boRbReceiver(boRbConnectDetail& conn, boLogHandler* logger) {
                                // Set logger.
                                set_log_environment(RABBITMQ_LOG_TAG_RECEIVER, logger);

                                conn.clone(this->conn_);
                        }
                        virtual ~boRbReceiver() {
                        }

                        // Retrival err information.
                        boErrorWhat& err() {
                                return e_what_;
                        }

                public:
                        // Connect to MQ server.
                        virtual void connet_to_server() = 0;

                        // Receive callback.
                        virtual void read_message_complete(string& message) {

                        }
                protected:
                        boErrorWhat e_what_;
                        boRbConnectDetail conn_;


                };

        }
}

#endif /* BINGO_RABBITMQ_RECEIVER_H_ */
