/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boUUID.h
 * Author: root
 *
 * Created on 2017年9月5日, 上午10:19
 */

#ifndef BINGO_ALGORITHM_UUID_HEADER_H_
#define BINGO_ALGORITHM_UUID_HEADER_H_

#include "bingo_v4/boType.h"

#include <boost/uuid/uuid.hpp>     //uuid使用
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
using namespace boost::uuids;

namespace bingo {
        namespace algorithm {
                class boUUID {
                public:
                        boUUID();
                        virtual ~boUUID();

                        string uuid_to_string(boost::uuids::uuid& in);
                        void string_to_uuid(string& in, boost::uuids::uuid& out);
                        string make_random_uuid();
                        template<typename KEY>
                        string make_name_uuid(string& name) {
                                uuid name_com = string_generator()(KEY::value);
                                name_generator ngen(name_com);

                                uuid u1 = ngen(name);

                                return uuid_to_string(u1);
                        }
                };
        }
}

#endif /* BINGO_ALGORITHM_UUID_HEADER_H_ */

