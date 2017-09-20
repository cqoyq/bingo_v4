/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boActionFactory.h
 * Author: root
 *
 * Created on 2017年9月19日, 下午3:21
 */

#ifndef BINGO_ACTION_FACTORY_HEADER_H_
#define BINGO_ACTION_FACTORY_HEADER_H_

#include "bingo_v4/database/boDbConnectDetail.h"
#include "bingo_v4/database/boDbResult.h"
#include "bingo_v4/database/boDbVisitor.h"
using namespace bingo;
using namespace bingo::database;
#include "bingo_v4/log/boLogHandler.h"
using namespace bingo::log;
#include "bingo_v4/configuration/boConfigAction.h"
using namespace bingo::configuration;
#include "bingo_v4/rabbitmq/boRbSendor.h"
#include "bingo_v4/rabbitmq/boRbReceiver.h"
using namespace bingo::RabbitMQ;

namespace bingo {
        class boActionFactory {
        public:
                boActionFactory();
                virtual ~boActionFactory();

                // Db mysql visitor.
                static boDbVisitor* make_mysql_visitor(boDbConnectDetail& conn_detail);

                // Loggger
                static boLogHandler* make_file_logger(string& path);
                static boLogHandler* make_console_logger();

                // Config
                static boConfigAction* make_xml_parser(bool is_pretty);
                static boConfigAction* make_json_parser(bool is_pretty);

                // Rabbitmq
                template<typename T>
                static boRbReceiver* make_rb_receiver(boRbConnectDetail& conn, boLogHandler* logger) {
                        return new T(conn, logger);
                }
                template<typename T>
                static boRbSendor* make_rb_sendor(boRbConnectDetail& conn, boLogHandler* logger) {
                        return new T(conn, logger);
                }
        };
}

#endif /* BINGO_ACTION_FACTORY_HEADER_H_ */

