/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbWorkReceiver.h
 * Author: root
 *
 * Created on 2017年9月20日, 下午1:32
 */

#ifndef BINGO_RABBITMQ_IMP_WORK_RECEIVER_H_
#define BINGO_RABBITMQ_IMP_WORK_RECEIVER_H_

#include "../boRbReceiver.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        namespace RabbitMQ {
                namespace imp {
                        class boRbWorkReceiver : public boRbReceiver {
                        public:
                                boRbWorkReceiver(boRbConnectDetail& conn, boLogHandler* logger);
                                virtual ~boRbWorkReceiver();

                                // Override connet_to_server() method.
                                virtual void connet_to_server();
                        };
                }
        }
}

#endif /* BINGO_RABBITMQ_IMP_WORK_RECEIVER_H_ */

