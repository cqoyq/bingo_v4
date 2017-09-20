/*
 * simple_sendor.h
 *
 *  Created on: 2016-9-2
 *      Author: root
 */

#ifndef BINGO_RABBITMQ_IMP_SIMPLE_SENDOR_H_
#define BINGO_RABBITMQ_IMP_SIMPLE_SENDOR_H_

#include "../boRbSendor.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        namespace RabbitMQ {
                namespace imp {
                        class boRbSimpleSendor : public boRbSendor {
                        public:
                                class SendTask : public boRbSendTask<boRbSimpleSendor> {
                                public:
                                        SendTask(boRbTaskData* exit_thr_flag, boRbSimpleSendor* p);
                                        virtual ~SendTask();

                                        void handle(const char* data, size_t data_size) override;
                                };
                        public:
                                boRbSimpleSendor(boRbConnectDetail& conn, boLogHandler* logger);
                                virtual ~boRbSimpleSendor();

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

#endif /* BINGO_RABBITMQ_IMP_SIMPLE_SENDOR_H_ */
