/*
 * db_field.h
 *
 *  Created on: 2016-9-21
 *      Author: root
 */

#ifndef BINGO_DATABASE_DB_FIELD_H_
#define BINGO_DATABASE_DB_FIELD_H_

#include <memory.h>
#include <string>
using namespace std;

#include "bingo_v4/boType.h"

namespace bingo {
        namespace database {
                struct boDbFieldHeader {
                public:
                        boDbFieldHeader(const char* s) {
                                name = s;
                        }
                        string name;
                };
                class boDbField {
                public:
                        boDbField();
                        boDbField(const char* data, size_t data_size);
                        virtual ~boDbField();

                        const char* data();

                        size_t data_size();

                protected:
                        char* data_;
                        size_t data_size_;
                };

        }
}

#endif /* BINGO_DATABASE_DB_FIELD_H_ */
