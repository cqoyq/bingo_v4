/*
 * mysql_visitor.h
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#ifndef BINGO_DATABASE_MYSQL_MYSQL_VISITOR_H_
#define BINGO_DATABASE_MYSQL_MYSQL_VISITOR_H_

#include "bingo_v4/database/boDbResult.h"
#include "bingo_v4/database/boDbVisitor.h"
#include "bingo_v4/database/boDbConnectDetail.h"

namespace bingo {
        namespace database {
                namespace mysql {
                        class boDbMysqlVisitor : public boDbVisitor {
                        public:
                                boDbMysqlVisitor(boDbConnectDetail& conn);
                                virtual ~boDbMysqlVisitor();

                                bool query_result(const char* sql, boDbResult*& result);

                                bool query_result(std::vector<string>& sqls, boost::ptr_vector<boDbResult>& results);

                                bool query_result(std::vector<string>& sqls);

                                // Call free_database() when progress is destroyed.
                                static void free_mysql_database();

                        private:
                                boDbConnectDetail conn_;
                        };

                }
        }
}

#endif /* BINGO_DATABASE_MYSQL_MYSQL_VISITOR_H_ */
