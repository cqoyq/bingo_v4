/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_base.h
 * Author: root
 *
 * Created on 2017年9月1日, 下午5:20
 */

#ifndef BINGO_LOG_LOG_BASE_H_
#define BINGO_LOG_LOG_BASE_H_

#include "boLogHandler.h"

namespace bingo {
        namespace log {
                class boLogBase {
                public:
                        boLogBase();
                        virtual ~boLogBase();

                        void set_log_environment(const char* tag, boLogHandler*& logger);

                        void to_debug(string& info);
                        void to_info(string& info);
                        void to_error(string& info);
                protected:
                        string log_tag_;
                        boLogHandler* logger_;
                };

        }
}
#endif /* BINGO_LOG_LOG_BASE_H_ */

