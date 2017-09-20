/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_thread_task.h
 * Author: root
 *
 * Created on 2017年9月2日, 上午11:03
 */

#ifndef  BINGO_THREAD_THREAD_BIG_DATA_TASK_H_
#define BINGO_THREAD_THREAD_BIG_DATA_TASK_H_

#include <queue>
using namespace std;

#include "boThreadAction.h"
#include "bingo_v4/boType.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/boString.h"
#include "bingo_v4/boTimer.h"
using namespace bingo;

namespace bingo {
        namespace thread {
                class boThreadBigDataTask : public boThreadTaskTopAction, public boThreadTaskCheckExitFlagAction {
                public:
                        class TaskTimer : public boTimer {
                        public:
                                TaskTimer(BO_ASYNC_SERVICE& io_service, boThreadBigDataTask* p);
                                virtual ~TaskTimer();

                                void timeout_handler(time_escape& time) override;

                        protected:
                                boThreadBigDataTask* p_;
                        };
                public:

                        boThreadBigDataTask(boThreadTaskData* exit_thr_flag, BO_ASYNC_SERVICE& io_service);
                        virtual ~boThreadBigDataTask();

                        // The big data task use to_block() and from_block() to handle.
                        void from_block(std::vector<boThreadTaskData*>& out);
                        void to_block(boThreadTaskData* data);

                        // The normal data task use put() to handle.
                        int put(boThreadTaskData* data, boErrorWhat& e_what);

                        // Close task thread.
                        void exit_task();
                protected:
                        // Thread will call this function.
                        void svc();

                        // Free task data callback.
                        void free_data(boThreadTaskData*& data);
                protected:
                        bool is_thread_exit_;
                        boThreadTaskData* exit_thread_flag_;
                        BO_THREAD thr_;

                        BO_MUTEX_TYPE mu_;
                        BO_THREAD_CONDITION_VARIABLE_ANY cond_get_;

                        std::queue<boThreadTaskData*> queue_;

                        TaskTimer timer_;
                        std::vector<boThreadTaskData*> block_;
                        BO_MUTEX_TYPE block_mu_;
                };

        }
}

#endif /* BINGO_THREAD_THREAD_BIG_DATA_TASK_H_ */

