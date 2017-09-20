/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_file.h
 * Author: root
 *
 * Created on 2017年9月2日, 上午8:34
 */

#ifndef BINGO_LOG_LOG_TO_FILE_H_
#define BINGO_LOG_LOG_TO_FILE_H_

#include "bingo_v4/log/boLogHandler.h"
#include "bingo_v4/log/boLogInfo.h"
#include "bingo_v4/log/boLogOutput.h"

namespace bingo {
        namespace log {
                class boLogToFile : public boLogHandler, public boLogInfoType<>, public boLogOutputType<> {
                public:
                        boLogToFile();
                        virtual ~boLogToFile();

                        virtual void handle(int level, const char* tag, std::string& info);

                        void set_path(string& path);
                };

        }
}

#endif /* BINGO_LOG_LOG_TO_FILE_H_ */

