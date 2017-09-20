/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbTaskData.h
 * Author: root
 *
 * Created on 2017年9月15日, 下午3:43
 */

#ifndef BINGO_RABBITMQ_TASK_DATA_H_
#define BINGO_RABBITMQ_TASK_DATA_H_

#include "bingo_v4/thread/boThreadAction.h"
using namespace bingo::thread;

namespace bingo {
        namespace RabbitMQ {
                class boRbTaskData : public boThreadTaskData {
                public:
                        boRbTaskData() {
                                type = TASK_DATA_TYPE_UNKNOWN;
                        }
                        boRbTaskData(int type) {
                                this->type = type;
                        }
                        boRbTaskData(int type, string& value) {
                                this->type = type;
                                this->body = value;
                        }
                        virtual ~boRbTaskData() {
                        }

                        int type;
                        string body;
                        string to_string() override {
                                string out;
                                boString t;
                                out = t.stream_to_string(body.c_str(), body.length());

                                return out;
                        }
                        enum {
                                TASK_DATA_TYPE_UNKNOWN = 0x00,
                                TASK_DATA_TYPE_MESSAGE = 0x01,
                                TASK_DATA_TYPE_EXIT_FLAG = 0x02,
                        };
                };

        }
}

#endif /* BINGO_RABBITMQ_TASK_DATA_H_ */

