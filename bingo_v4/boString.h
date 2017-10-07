/*
 * string.h
 *
 *  Created on: 2016-6-29
 *      Author: root
 */

#ifndef BINGO_STRING_HEADER_H_
#define BINGO_STRING_HEADER_H_

#include <string>
#include <memory.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
using namespace std;

#include "boType.h"
#include "boErrorWhat.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>       
using namespace boost;

namespace bingo {

        // -------------------------------------------------------  string_ex -------------------------------------------------------- //
        class boString {
        public:
                boString();
                virtual ~boString();

                // byte's stream -> string.
                // for example: {0x00, 0x01, 0x02} -> '00 01 02'
                string stream_to_string(const char* p, size_t p_size);

                // string -> byte's stream.
                // for example: '01 02 03' -> {0x01, 0x02, 0x03}
                // @out must be initialized and allocate memory.
                string string_to_stream(string& in);

                // short -> byte of stream.
                // for example: 260 -> {0x04, 0x01}
                string short_to_stream(u16_t value);

                // byte of stream -> short
                // for example: {0x04, 0x01} -> 260
                u16_t stream_to_short(const char* p);

                // Ip string -> int.
                u32_t ip_to_int(string& s);

                // Int -> ip string
                string int_to_ip(u32_t n);


                // Convert T to string.
                template<typename T>
                string convert(const T& t) {
                        string out;
                        stringstream oss;
                        oss << t;
                        oss >> out;
                        return out;
                }
                string convert(const char*& t);
                string convert(const char& t);
                string convert(const u8_t& t);

                // Convert integer's string to integer.
                template<typename T>
                static std::tuple<bool, T> to_integer(string& in) {
                        T n = 0;
                        bool is_succ = boost::all(in, is_digit());

                        if (in.length() == 0)
                                is_succ = false;
                        else if (in.length() == 1)
                                is_succ = boost::all(in, is_digit());
                        else if (in.substr(0, 1).compare("-") == 0) {
                                // first char is negative.
                                is_succ = boost::all(in.substr(1, in.length() - 1), is_digit());
                        } else
                                is_succ = boost::all(in, is_digit());

                        if (is_succ)
                                n = boost::lexical_cast<T>(in);

                        return std::make_tuple(is_succ, n);
                }



        protected:

                // char -> int
                // for example: 'd' -> 11
                int char_to_int(string chr);
        };

        // -------------------------------------------------------  string_append -------------------------------------------------------- //
        class boStringAppend {
        public:
                template<typename T>
                boStringAppend* add(T t) {
                        boString st;
                        data.append(st.convert(t));
                        return this;
                };
                boStringAppend* add(string s) {
                        data.append(s.c_str());
                        return this;
                };
                string& to_string() {
                        return data;
                }

        private:
                string data;
        };

}

#endif /* BINGO_STRING_HEADER_H_ */
