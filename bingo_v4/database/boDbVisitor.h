/*
 * db_visitor.h
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#ifndef BINGO_DATABASE_DB_VISITOR_H_
#define BINGO_DATABASE_DB_VISITOR_H_

#include <vector>

#include "boDbResult.h"
#include "bingo_v4/boErrorWhat.h"

namespace bingo {
        namespace database {
                enum {
                        DATABASE_ERROR_TYPE_CONNECT_FAIL = 0x01,
                        DATABASE_ERROR_TYPE_QUERY_FAIL = 0x02,
                        DATABASE_ERROR_TYPE_READ_CFG_FAIL = 0x03,
                        DATABASE_ERROR_TYPE_CHECK_CFG_NODE_FAIL = 0x04,
                        DATABASE_ERROR_TYPE_CFG_NODE_NO_EXIST = 0x05,
                        DATABASE_ERROR_TYPE_POOL_IS_EMPTY = 0x06,
                        DATABASE_ERROR_TYPE_FACTORY_NO_INSTANCE = 0x07,
                };
                class boDbVisitor {
                public:
                        boDbVisitor() {
                        }
                        virtual ~boDbVisitor() {
                        };

                        // Query single result, fail and return false, to check err().
                        // @sql is single sql-string, @result is query result, @e_what is error.
                        virtual bool query_result(const char* sql, boDbResult*& result) = 0;

                        // Query multi-result,  fail and return false, to check err().
                        virtual bool query_result(std::vector<string>& sqls, boost::ptr_vector<boDbResult>& results) = 0;

                        // Execute multi-sql-string,  fail and return false, to check err().
                        virtual bool query_result(std::vector<string>& sqls) = 0;

                        // Get error information.
                        boErrorWhat& err() {
                                return e_what_;
                        }

                protected:
                        boErrorWhat e_what_;
                };

        }
}


#endif /* BINGO_DATABASE_DB_VISITOR_H_ */
