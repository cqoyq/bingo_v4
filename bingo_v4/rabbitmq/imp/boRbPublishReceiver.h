/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbPublishReceiver.h
 * Author: root
 *
 * Created on 2017年9月20日, 上午11:22
 */

#ifndef BINGO_RABBITMQ_IMP_PUBLISH_RECEIVER_H_
#define BINGO_RABBITMQ_IMP_PUBLISH_RECEIVER_H_

#include "../boRbReceiver.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        namespace RabbitMQ {
                namespace imp {
                        class boRbPublishReceiver : public boRbReceiver {
                        public:
                                boRbPublishReceiver(boRbConnectDetail& conn, boLogHandler* logger);
                                virtual ~boRbPublishReceiver();

                                virtual void connet_to_server();

                        };
                }
        }
}

#endif /* BINGO_RABBITMQ_IMP_PUBLISH_RECEIVER_H_ */

