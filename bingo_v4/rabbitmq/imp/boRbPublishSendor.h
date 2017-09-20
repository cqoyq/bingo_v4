/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbPublishSendor.h
 * Author: root
 *
 * Created on 2017年9月14日, 下午4:00
 */

#ifndef BINGO_RABBITMQ_IMP_PUBLISH_SENDOR_H_
#define BINGO_RABBITMQ_IMP_PUBLISH_SENDOR_H_

#include "../boRbSendor.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        namespace RabbitMQ {
                namespace imp {
                        class boRbPublishSendor : public boRbSendor {
                        public:
                                class SendTask : public boRbSendTask<boRbPublishSendor> {
                                public:
                                        SendTask(boRbTaskData* exit_thr_flag, boRbPublishSendor* p);
                                        virtual ~SendTask();

                                        void handle(const char* data, size_t data_size) override;
                                };
                        public:
                                boRbPublishSendor(boRbConnectDetail& conn, boLogHandler* logger);
                                virtual ~boRbPublishSendor();

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

#endif /* BINGO_RABBITMQ_IMP_PUBLISH_SENDOR_H_ */

