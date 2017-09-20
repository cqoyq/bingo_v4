/*
 * sendor.h
 *
 *  Created on: 2016-9-2
 *      Author: root
 */

#ifndef BINGO_RABBITMQ_SENDOR_H_
#define BINGO_RABBITMQ_SENDOR_H_

#include "bingo_v4/boType.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/log/boLogHandler.h"
#include "bingo_v4/log/boLogBase.h"
using namespace bingo::log;
#include "bingo_v4/thread/boThreadTask.h"
using namespace bingo::thread;

#include "boRbTaskData.h"
#include "boRbConnectDetail.h"

namespace bingo {
        namespace RabbitMQ {
                // Sendtask base class.
                template<typename T>
                class boRbSendTask : public boThreadTask {
                public:
                        boRbSendTask(boRbTaskData* exit_thr_flag, T* p) : boThreadTask(exit_thr_flag), p_(p) {
                        }
                        virtual ~boRbSendTask() {
                        }

                        // Override boThreadTask's method
                        void top(boThreadTaskData*& data) override {
                                boRbTaskData* p = dynamic_cast<boRbTaskData*> (data);
                                if (p) {
                                        handle(p->body.c_str(), p->body.length());
                                }

                        }
                        bool is_exit_data(boThreadTaskData*& data) override {
                                boRbTaskData* p = dynamic_cast<boRbTaskData*> (data);
                                if (p && p->type == boRbTaskData::TASK_DATA_TYPE_EXIT_FLAG) {
                                        return true;
                                } else {
                                        return false;
                                }
                        }

                        // Handle top data.
                        virtual void handle(const char* data, size_t data_size) = 0;

                protected:
                        T* p_;
                };

                // Sendor base class.
                class boRbSendor : public boLogBase {
                public:
                        boRbSendor(boRbConnectDetail& conn, boLogHandler* logger) {
                                // Set logger.
                                set_log_environment(RABBITMQ_LOG_TAG_SENDOR, logger);

                                is_valid_ = false;
                                conn.clone(this->conn_);
                        }
                        virtual ~boRbSendor() {
                        };

                        // Connect to MQ server.
                        virtual void connet_to_server() = 0;

                        // Send data to MQ server by routingkey.
                        virtual bool send_data(const char* data, size_t data_size) = 0;

                        // Retrival err information.
                        boErrorWhat& err() {
                                return e_what_;
                        }

                        // Get connect-detail
                        boRbConnectDetail& conn() {
                                return conn_;
                        }

                        // Get channel.
                        AMQP::Channel* channel() {
                                return conn_.channel;
                        }

                        // Whether rabbitmq connect is valid.
                        bool is_valid() {
                                return is_valid_;
                        }

                protected:
                        boErrorWhat e_what_;
                        bool is_valid_;

                        boRbConnectDetail conn_;
                };

        }
}

#endif /* BINGO_RABBITMQ_SENDOR_H_ */
