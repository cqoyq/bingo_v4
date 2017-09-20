/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_timer.h
 * Author: root
 *
 * Created on 2017年9月3日, 下午5:36
 */

#ifndef BINGO_TIMER_H_
#define BINGO_TIMER_H_

#include "boType.h"

namespace bingo {
        class boTimer {
        public:

                // Timer's escape type.
                enum {
                        TIMER_ESCAPE_TYPE_UNKNOWN = 0x00,
                        TIMER_ESCAPE_TYPE_SECOND = 0x01,
                        TIMER_ESCAPE_TYPE_MILLISECOND = 0x02,
                };

                // Timer's escape data.
                struct time_escape {
                        int type;
                        int interval;
                        time_escape() {
                                type = TIMER_ESCAPE_TYPE_UNKNOWN;
                                interval = 2;
                        }
                };
        public:
                explicit boTimer(BO_ASYNC_SERVICE& io_service);
                virtual ~boTimer();

                void start(time_escape& time);

        protected:

                virtual void timeout_handler(time_escape& time) = 0;
#ifdef WIN32
#else
                void on_timeout(const system::error_code& ec);

#endif

                BO_ASYNC_TIMER timer_;
                time_escape interval_time_;
        };

}

#endif /* BINGO_TYPE_TIMER_H_ */

