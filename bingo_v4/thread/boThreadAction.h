/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_thread.h
 * Author: root
 *
 * Created on 2017年9月2日, 上午10:52
 */

#ifndef BINGO_THREAD_HEADER_H_
#define BINGO_THREAD_HEADER_H_

#include "bingo_v4/boType.h"

// Thread error type.
enum {
        THREAD_ERROR_TYPE_THREAD_HAS_EXITED = 0x01,

};

namespace bingo {
        namespace thread {
                // Thread task base class.
                class boThreadTaskData {
                public:
                        virtual ~boThreadTaskData() {
                        }
                        virtual string to_string() {
                                return string("");
                        }
                };

                // Thread task top action.
                class boThreadTaskTopAction {
                public:
                        virtual ~boThreadTaskTopAction() {
                        }
                        virtual void top(boThreadTaskData*& data) {

                        }
                };

                // Thread task check exit flag action.
                class boThreadTaskCheckExitFlagAction {
                public:
                        virtual ~boThreadTaskCheckExitFlagAction() {
                        }
                        virtual bool is_exit_data(boThreadTaskData*& data) {
                                return false;
                        }
                };
        }
}

#endif /* BINGO_THREAD_HEADER_H_ */

