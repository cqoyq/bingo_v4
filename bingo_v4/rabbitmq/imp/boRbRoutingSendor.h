/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbRoutingSendor.h
 * Author: root
 *
 * Created on 2017年9月20日, 下午1:05
 */

#ifndef BINGO_RABBITMQ_IMP_ROUTING_SENDOR_H_
#define BINGO_RABBITMQ_IMP_ROUTING_SENDOR_H_

#include "../boRbSendor.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        namespace RabbitMQ {
                namespace imp {
                        class boRbRoutingSendor : public boRbSendor {
                        public:
                                class SendTask : public boRbSendTask<boRbRoutingSendor> {
                                public:
                                        SendTask(boRbTaskData* exit_thr_flag, boRbRoutingSendor* p);
                                        virtual ~SendTask();

                                        void handle(const char* data, size_t data_size) override;
                                };
                        public:
                                boRbRoutingSendor(boRbConnectDetail& conn, boLogHandler* logger);
                                virtual ~boRbRoutingSendor();

                                // Override sendor::connet_to_server() method.
                                void connet_to_server() override;

                                // Override sendor::send_data() method, fail to check err().
                                bool send_data(const char* data, size_t data_size) override;

                        protected:

                                // Make send thread service.
                                void make_send_svc();
                                SendTask* send_task_;
                        };
                }
        }
}
#endif /* BINGO_RABBITMQ_IMP_ROUTING_SENDOR_H_ */

