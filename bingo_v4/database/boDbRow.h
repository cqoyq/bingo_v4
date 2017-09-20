/*
 * db_row.h
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#ifndef BINGO_DATABASE_DB_ROW_H_
#define BINGO_DATABASE_DB_ROW_H_

#include <string>
using namespace std;

#include "bingo_v4/boCollection.h"
#include "bingo_v4/boType.h"
#include "boDbField.h"

namespace bingo {
        namespace database {
                class boDbRow {
                public:
                        boDbRow(void* res);
                        virtual ~boDbRow();

                        void add_field(boDbField* field);

                        boDbField* get_field(int index);
                        boDbField* get_field(const char* field_name);

                        int size();

                protected:
                        boCollection<boDbField> fields_;

                        // db_result pointer.
                        void* res_;
                };

        }
}

#endif /* BINGO_DATABASE_DB_ROW_H_ */
