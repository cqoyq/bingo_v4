/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_info.h
 * Author: root
 *
 * Created on 2017年9月2日, 上午8:50
 */

#ifndef BINGO_LOG_LOG_INFO_H_
#define BINGO_LOG_LOG_INFO_H_

#include "bingo_v4/boString.h"

namespace bingo {
        namespace log {
                class boLogInfo {
                public:
                        boLogInfo() {
                        };
                        virtual ~boLogInfo() {
                        };

                        virtual string make_log_info(int level, const char*& tag, string& info) = 0;

                protected:
                        string get_now();
                };

                // Normal log format: 2017-05-02 11:22:33.123@tag@debug@info
                class boLogNormalInfo : public boLogInfo {
                public:
                        boLogNormalInfo() {
                        };
                        virtual ~boLogNormalInfo() {
                        };

                        virtual string make_log_info(int level, const char*& tag, string& info);
                };

                // Thread log format: 2017-05-02 11:22:33.123@thread_id@tag@debug@info
                class boLogNormalThreadInfo : public boLogInfo {
                public:
                        boLogNormalThreadInfo() {
                        };
                        virtual ~boLogNormalThreadInfo() {
                        };

                        virtual string make_log_info(int level, const char*& tag, string& info);

                };

                // Simple log format: 2017-05-02 11:22:33.123@info
                class boLogSimpleInfo : public boLogInfo {
                public:
                        boLogSimpleInfo() {
                        };
                        virtual ~boLogSimpleInfo() {
                        };

                        virtual string make_log_info(int level, const char*& tag, string& info);

                };

                // Simple log format: 2017-05-02 11:22:33.123@thread_id@info
                class boLogSimpleThreadInfo : public boLogInfo {
                public:
                        boLogSimpleThreadInfo() {
                        };
                        virtual ~boLogSimpleThreadInfo() {
                        };

                        virtual string make_log_info(int level, const char*& tag, string& info);

                };

                // Info template class.
                template<typename T = boLogNormalInfo>
                class boLogInfoType : public boLogInfo {
                public:
                        boLogInfoType() : boLogInfo() {
                        };
                        virtual ~boLogInfoType() {
                        };
                        virtual string make_log_info(int level, const char*& tag, string& info) {
                                return t.make_log_info(level, tag, info);
                        }
                protected:
                        T t;
                };

        }
}

#endif /* BINGO_LOG_LOG_INFO_H_ */

