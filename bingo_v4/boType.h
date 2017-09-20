/*
 * type.h
 *
 *  Created on: 2016-6-29
 *      Author: root
 */

#ifndef BINGO_TYPE_HEADER_H_
#define BINGO_TYPE_HEADER_H_

#include <string>
using namespace std;

// Log to console
void bo_debug_out(const char* debug_info);
void bo_debug_out(string& debug_info);
void bo_debug_out(stringstream& debug_info);
// Get current thread id.
string bo_thread_id();

#ifdef WIN32
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
#else
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;

#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace boost::asio;

#define BO_ASYNC_SERVICE boost::asio::io_service
#define BO_ASYNC_TIMER deadline_timer

#include <boost/thread.hpp>
using namespace boost;

#define BO_MUTEX_TYPE boost::mutex
#define BO_SCOPE_LOCK_TYPE mutex::scoped_lock

#define BO_THREAD boost::thread
#define BO_THREAD_CONDITION_VARIABLE_ANY boost::condition_variable_any
#define BO_BIND boost::bind
#define BO_THIS_THREAD boost::this_thread

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#define BO_SHARED_PTR boost::shared_ptr
#define BO_ENABLE_SHARED_FROM_THIS  boost::enable_shared_from_this

#endif



#endif /* BINGO_TYPE_HEADER_H_ */
