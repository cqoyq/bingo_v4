/*
 * log_handler.h
 *
 *  Created on: 2016-11-8
 *      Author: root
 */

#ifndef BINGO_LOG_LOG_HANDLER_H
#define BINGO_LOG_LOG_HANDLER_H

#include <string>
using namespace std;

#include "boLogLevel.h"

namespace bingo {
        namespace log {
                class boLogHandler {
                public:
                        boLogHandler() {
                                level_ = LOG_LEVEL_OFF;
                        };
                        virtual ~boLogHandler() {
                        };

                        virtual void handle(int level, const char* tag, std::string& info) = 0;
                        virtual void set_level(int level) {
                                level_ = level;
                        };

                protected:
                        int level_;
                };

        }
}

#endif /* BINGO_LOG_LOG_HANDLER_H */
