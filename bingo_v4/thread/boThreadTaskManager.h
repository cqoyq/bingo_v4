/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boThreadTaskManager.h
 * Author: root
 *
 * Created on 2017年12月13日, 下午2:29
 */

#ifndef BINGO_THREAD_THREAD_TASK_MANAGER_H_
#define BINGO_THREAD_THREAD_TASK_MANAGER_H_

#include "bingo_v4/boString.h"
#include "bingo_v4/boCollection.h"
using namespace bingo;

#include "boThreadAction.h"

namespace bingo {
        namespace thread {
                class boThreadTaskManager : public boCollection<boThreadTaskSvrAction> {
                public:
                        struct is_equal_key {
                                is_equal_key(const char* key) {
                                        key_ = key;
                                }
                                bool operator()(boThreadTaskSvrAction& n)const {
                                        return (n.get_key().compare(key_.c_str()) == 0) ? true : false;
                                }
                        private:
                                string key_;
                        };
                public:
                        boThreadTaskManager();
                        virtual ~boThreadTaskManager();

                        // Success return no null-pointer, otherwise return nullptr.
                        boThreadTaskSvrAction* get(const char* key);
                };
        }
}

#endif /* BINGO_THREAD_THREAD_TASK_MANAGER_H_ */

