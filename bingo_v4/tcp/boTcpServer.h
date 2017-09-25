/*
 * tcp_server.h
 *
 *  Created on: 2016-6-30
 *      Author: root
 */

#ifndef BINGO_TCP_SERVER_HEADER_H_
#define BINGO_TCP_SERVER_HEADER_H_

#include "bingo_v4/boType.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/boTimer.h"

#ifndef WIN32

#include <iostream>
using namespace std;

namespace bingo {
        namespace TCP {
                template<typename HANDLER, typename SOCKET_MANAGER, typename PARSER>
                class boTcpServer {
                public:
                        class HeartJumpTmer : public boTimer {
                        public:
                                HeartJumpTmer(BO_ASYNC_SERVICE& io_service, SOCKET_MANAGER* p) : boTimer(io_service), p_(p) {
                                }
                                virtual ~HeartJumpTmer() {

                                }
                                void timeout_handler(time_escape& time) override {
#ifdef BINGO_DEBUG_TCP_SERVER
                                        bo_debug_out("start: check_heartjump");
#endif
                                        p_->check_heartjump();
                                }

                        protected:
                                SOCKET_MANAGER* p_;
                        };
                        class AuthenticateTmer : public boTimer {
                        public:
                                AuthenticateTmer(BO_ASYNC_SERVICE& io_service, SOCKET_MANAGER* p) : boTimer(io_service), p_(p) {

                                }
                                virtual ~AuthenticateTmer() {

                                }
                                void timeout_handler(time_escape& time) override {
#ifdef BINGO_DEBUG_TCP_SERVER
                                        bo_debug_out("start: check_authentication_pass");
#endif
                                        p_->check_authentication_pass();
                                }
                        protected:
                                SOCKET_MANAGER* p_;
                        };
                public:
                        typedef BO_SHARED_PTR<HANDLER> pointer;
                        explicit boTcpServer(BO_ASYNC_SERVICE& io_service, string& ipv4, u16_t& port) : ios_(io_service),
                             heartjump_timer_(io_service, &scoket_manager_),
                             authentication_timer_(io_service, &scoket_manager_),
                             acceptor_(io_service,
                                       ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(ipv4), port)) {

                                start_accept();

                                // Start to inspect heartjump
                                start_heartjump();

                                // Start to inspect authentication pass
                                start_authentication();
                        }
                        virtual ~boTcpServer() {
                        }

                        // Start to listening port.
                        void start_accept() {

#ifdef BINGO_DEBUG_TCP_SERVER
                                bo_debug_out("call start_accept()");
#endif

                                // Make new tcp_svr_handler object.
                                pointer new_handler(new HANDLER(ios_,
                                                                PARSER::max_wait_for_heartjump_seconds,
                                                                PARSER::max_wait_for_authentication_pass_seconds,
                                                                bind(&boTcpServer::remove_socket, this, _1),
                                                                this
                                                                ));

                                // Start to wait for connect.
                                acceptor_.async_accept(new_handler->socket(),
                                                       boost::bind(&boTcpServer::accept_handler, this, new_handler,
                                                                   boost::asio::placeholders::error));
                        }

                        // Get socket manager.
                        SOCKET_MANAGER& socket_manager() {
                                return scoket_manager_;
                        }

                private:
                        // Accept connect callback.
                        void accept_handler(pointer new_handler,
                                            const boost::system::error_code& ec) {

                                if (!ec) {

#ifdef BINGO_DEBUG_TCP_SERVER
                                        bo_debug_out("call accept_handler()");
#endif

                                        boErrorWhat e_what;

                                        // Check max socket number.
                                        if (PARSER::max_socket_size > 0
                                                && scoket_manager_.size() == PARSER::max_socket_size) {

#ifdef BINGO_DEBUG_TCP_SERVER
                                                bo_debug_out("reach max socket limit! will close new socket");
#endif
                                                e_what.err_no(TCP_ERROR_TYPE_SERVER_MAX_OF_SOCKET_LIMIT);
                                                e_what.err_message("reach max of socket limit!");

                                                new_handler->catch_error(e_what);

                                                // Active close socket.
                                                new_handler->close_socket();

                                        } else {
                                                // Call accept_handle_success_func()
                                                if (accept_success_func(new_handler, e_what) == 0) {
                                                        // Start to aync-read.
                                                        new_handler->start();

                                                        // Save socket to manager.
                                                        add_socket(new_handler);

                                                } else {

                                                        new_handler->catch_error(e_what);

                                                        // Active close socket.
                                                        new_handler->close_socket();
                                                }
                                        }
                                } else {
                                        // Call accept_error_func()
                                        accept_error_func(ec.value());
                                }

                                // Start another one.
                                start_accept();
                        }

                        // Check authentication timer.
                        void start_authentication() {
                                if (PARSER::max_wait_for_authentication_pass_seconds > 0) {

                                        boTimer::time_escape time;
                                        time.type = boTimer::TIMER_ESCAPE_TYPE_SECOND;
                                        time.interval = PARSER::max_wait_for_authentication_pass_seconds;
                                        authentication_timer_.start(time);

                                }
                        }

                        // Check heartjump timer.
                        void start_heartjump() {
                                if (PARSER::max_wait_for_heartjump_seconds > 0) {

                                        boTimer::time_escape time;
                                        time.type = boTimer::TIMER_ESCAPE_TYPE_SECOND;
                                        time.interval = PARSER::max_wait_for_heartjump_seconds;
                                        heartjump_timer_.start(time);
                                }
                        }

                        // Add socket to manger.
                        void add_socket(pointer new_handler) {
                                string uuid = new_handler->uuid();
                                void* hdr = new_handler.get();
                                scoket_manager_.add(uuid, hdr);
                        }

                        // Remove socket from manger
                        void remove_socket(string& uuid) {
                                scoket_manager_.erase(uuid);
                        }

                protected:
                        // These method will been called after inner handle.
                        virtual int accept_success_func(pointer /*ptr*/, boErrorWhat& /*e_what*/) {
                                return 0;
                        };
                        virtual void accept_error_func(int /*ec*/) {
                        };

                protected:
                        BO_ASYNC_SERVICE& ios_;
                        ip::tcp::acceptor acceptor_;

                        HeartJumpTmer heartjump_timer_;
                        AuthenticateTmer authentication_timer_;

                        SOCKET_MANAGER scoket_manager_;
                };

        }
}

#endif

#endif /* BINGO_TCP_SERVER_HEADER_H_ */
