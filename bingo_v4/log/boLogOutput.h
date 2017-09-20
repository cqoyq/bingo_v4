/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bo_log_output.h
 * Author: root
 *
 * Created on 2017年9月2日, 上午10:05
 */

#ifndef BINGO_LOG_LOG_OUTPUT_H_
#define BINGO_LOG_LOG_OUTPUT_H_

#include "bingo_v4/boString.h"

namespace bingo {
        namespace log {
                class boLogOutput {
                public:
                        boLogOutput() {
                        };
                        virtual ~boLogOutput() {
                        };

                        virtual void transfer(string& info) = 0;

                };

                // Output to file.
                class boLogFileOutput : public boLogOutput {
                public:
                        boLogFileOutput() {
                        };
                        virtual ~boLogFileOutput() {
                        };

                        virtual void transfer(string& info);

                        void set_path(string& path);

                protected:
                        string path_;
                };
                // Output to console.
                class boLogConsoleOutput : public boLogOutput {
                public:
                        boLogConsoleOutput() {
                        };
                        virtual ~boLogConsoleOutput() {
                        };

                        virtual void transfer(string& info);
                };

                // Output template class.
                template<typename T = boLogFileOutput>
                class boLogOutputType : public boLogOutput {
                public:
                        boLogOutputType() : boLogOutput() {
                        };
                        virtual ~boLogOutputType() {
                        };
                        virtual void transfer(string& info) {
                                t. transfer(info);
                        }
                protected:
                        T t;
                };

        }
}

#endif /* BINGO_LOG_LOG_OUTPUT_H_ */

