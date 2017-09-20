/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boRbConnectDetail.h
 * Author: root
 *
 * Created on 2017年9月12日, 上午10:46
 */

#ifndef BINGO_RABBITMQ_CONNET_DETAIL_H_
#define BINGO_RABBITMQ_CONNET_DETAIL_H_

#include "bingo_v4/boType.h"
#include "asio/asiohandler.h"

#define RABBITMQ_LOG_TAG_SENDOR         "rabbitmq_sendor"
#define RABBITMQ_LOG_TAG_RECEIVER       "rabbitmq_receiver" 

namespace bingo {
        namespace RabbitMQ {
                struct boRbConnectDetail {
                        string key;
                        string ip;
                        u16_t port;
                        string username;
                        string pwd;
                        string host;
                        std::vector<string> routingkey;
                        string exchange;
                        AMQP::Channel* channel;
                        boRbConnectDetail() {
                                port = 0;
                                channel = 0;
                        }
                        void clone(boRbConnectDetail& new_obj) {
                                new_obj.key = this->key;
                                new_obj.exchange = this->exchange;
                                new_obj.host = this->host;
                                new_obj.ip = this->ip;
                                new_obj.port = this->port;
                                new_obj.pwd = this->pwd;
                                new_obj.routingkey = this->routingkey;
                                new_obj.username = this->username;
                                new_obj.channel = this->channel;
                        }
                };

        }
}

#endif /* BINGO_RABBITMQ_CONNET_DETAIL_H_ */

