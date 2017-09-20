/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_to_console.h
 * Author: root
 *
 * Created on 2017年9月2日, 下午12:05
 */

#ifndef BINGO_LOG_LOG_TO_CONSOLE_H_
#define BINGO_LOG_LOG_TO_CONSOLE_H_

#include "bingo_v4/log/boLogHandler.h"
#include "bingo_v4/log/boLogInfo.h"
#include "bingo_v4/log/boLogOutput.h"

namespace bingo {
        namespace log {
                class boLogToDebug : public boLogHandler, public boLogInfoType<boLogSimpleThreadInfo>, public boLogOutputType<boLogConsoleOutput> {
                public:
                        boLogToDebug();
                        virtual ~boLogToDebug();

                        virtual void handle(int level, const char* tag, std::string& info);
                };
        }
}

#endif /* BINGO_LOG_LOG_TO_CONSOLE_H_ */

