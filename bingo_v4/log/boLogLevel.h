/*
 * log_level.h
 *
 *  Created on: 2016-11-8
 *      Author: root
 */

#ifndef BINGO_LOG_LOG_LEVEL_H_
#define BINGO_LOG_LOG_LEVEL_H_

#include "bingo_v4/boString.h"

namespace bingo {
        namespace log {
                enum {
                        LOG_LEVEL_OFF = 0x00,
                        LOG_LEVEL_FATAL = 0x01,
                        LOG_LEVEL_ERROR = 0x02,
                        LOG_LEVEL_INFO = 0x03,
                        LOG_LEVEL_DEBUG = 0x04,
                        LOG_LEVEL_ALL = 0x05,
                        LOG_LEVEL_UNKNOWN = 0xFF,
                };

                // log_level -> string
                string loglevel_to_string(int level);

                // string -> log_level
                int string_to_loglevel(string& s);
        }
}


#endif /* BINGO_LOG_LOG_LEVEL_H_ */
