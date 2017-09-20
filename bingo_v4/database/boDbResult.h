/*
 * db_result.h
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#ifndef BINGO_DATABASE_DB_RESULT_H_
#define BINGO_DATABASE_DB_RESULT_H_

#include <string>
using namespace std;

#include "bingo_v4/boCollection.h"
#include "bingo_v4/boType.h"
#include "boDbRow.h"

namespace bingo {
        namespace database {
                class boDbResult {
                public:
                        boDbResult();
                        virtual ~boDbResult();

                        void add_header(boDbFieldHeader* header);

                        void add_row(boDbRow* row);

                        // Get db_row pointer, return o if fail.
                        boDbRow* get_row(int index);

                        // Get db_field_header pointer, return 0 if fail.
                        boDbFieldHeader* get_header(int index);

                        // Get index of db_field_header, return -1 if fail.
                        int get_header_index(const char* header_name);

                        int row_amount;
                        int column_amount;

                protected:
                        boCollection<boDbRow> rows_;
                        boCollection<boDbFieldHeader> header_;

                };

        }
}

#endif /* BINGO_DATABASE_DB_RESULT_H_ */
