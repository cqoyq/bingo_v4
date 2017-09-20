/*
 * error_what.h
 *
 *  Created on: 2016-9-16
 *      Author: root
 */

#ifndef BINGO_ERROR_WHAT_H_
#define BINGO_ERROR_WHAT_H_

#include <string>
using namespace std;

namespace bingo {

        // Bingo error information class.
        class boErrorWhat {
        public:
                boErrorWhat();
                virtual ~boErrorWhat();

                // Handle error message.
                string& err_message();
                void err_message(const char* err);

                // Handle error number.
                int err_no();
                void err_no(int n);

                // Clone a new error.
                void clone(boErrorWhat& obj);

        private:
                string err_;
                int err_no_;
        };

}


#endif /* BINGO_ERROR_WHAT_H_ */
