/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boTcpClient.h
 * Author: root
 *
 * Created on 2017年9月7日, 下午1:35
 */

#ifndef BINGO_TCP_CLIENT_HEADER_H_
#define BINGO_TCP_CLIENT_HEADER_H_

#include "bingo_v4/boType.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/boTimer.h"
#include "boTcpError.h"

#ifndef WIN32

#include <iostream>
using namespace std;

#include <boost/lexical_cast.hpp>

namespace bingo {
        namespace TCP {
                template<typename HANDLER, typename PARSER>
                class boTcpClient {
                public:
                        typedef boTcpClient<HANDLER, PARSER> TCP_CLIENT_TYPE;
                        class InspectHeartJumpTmer : public boTimer {
                        public:
                                InspectHeartJumpTmer(BO_ASYNC_SERVICE& io_service, TCP_CLIENT_TYPE* p) : boTimer(io_service), p_(p) {
                                }
                                virtual ~InspectHeartJumpTmer() {

                                }
                                void timeout_handler(time_escape& time) override {
#ifdef BINGO_DEBUG_TCP_SERVER
                                        bo_debug_out("start: check_heartjump");
#endif
                                        p_->check_heartjump();
                                }

                        protected:
                                TCP_CLIENT_TYPE* p_;
                        };
                        class SendHeartJumpTmer : public boTimer {
                        public:
                                SendHeartJumpTmer(BO_ASYNC_SERVICE& io_service, TCP_CLIENT_TYPE* p) : boTimer(io_service), p_(p) {
                                }
                                virtual ~SendHeartJumpTmer() {

                                }
                                void timeout_handler(time_escape& time) override {
                                        p_->send_heartjump();
                                }

                        protected:
                                TCP_CLIENT_TYPE* p_;
                        };
                        class ReconnectTmer : public boTimer {
                        public:
                                ReconnectTmer(BO_ASYNC_SERVICE& io_service, TCP_CLIENT_TYPE* p) : boTimer(io_service), p_(p) {
                                }
                                virtual ~ReconnectTmer() {

                                }
                                void timeout_handler(time_escape& time) override {
                                        p_->start_connect();
                                }

                        protected:
                                TCP_CLIENT_TYPE* p_;
                        };
                public:
                        typedef boost::shared_ptr<HANDLER> pointer;
                        typedef boost::function<void(string&) > make_heartjump_message_func;

                        // Construct boTcpClient
                        explicit boTcpClient(BO_ASYNC_SERVICE& io_service, string& ipv4, u16_t& port, make_heartjump_message_func f1) :
                             ios_(io_service),
                             ipv4_(ipv4), port_(port),
                             timer0_(io_service, this),
                             timer1_(io_service, this),
                             timer2_(io_service, this),
                             is_authenticate_pass_(false),
                             retry_delay_(PARSER::retry_delay_seconds),
                             f1_(f1) {

                                start_connect();

                                // Start send heartjump package
                                start_timer_to_send_heartjump();

                                // Start check heartjump 
                                start_timer_to_check_heartjump();

                        }
                        virtual ~boTcpClient() {
                        }

                        // Start to connect to server.
                        void start_connect() {
                                // Make new tcp_connection object.
                                pointer new_handler(new HANDLER(ios_,
                                                                PARSER::max_interval_seconds_check_heartjump,
                                                                boost::bind(&boTcpClient::reconnect, this),
                                                                boost::bind(&boTcpClient::set_authenticate_pass, this, _1)));

                                // Save hdr to handler_
                                handler_ = new_handler.get();

                                // Start to connect.
                                new_handler->socket().async_connect(
                                                                    ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(ipv4_), port_),
                                                                    boost::bind(&boTcpClient::connect_handler, this,
                                                                                new_handler,
                                                                                boost::asio::placeholders::error));

                        }

                        //Set authenticate status.
                        void set_authenticate_pass(bool is_pass) {
                                this->is_authenticate_pass_ = is_pass;
                        }



                        // Send heartjump.
                        void send_heartjump() {
                                if (is_authenticate_pass_) {
                                        string out;
                                        f1_(out);
                                        if (out.length() > 0) {
                                                handler_->send_data_in_thread(out.c_str(), out.length());
                                        }
                                }
                        }

                        // Check heartjump.
                        void check_heartjump() {
                                if (is_authenticate_pass_
                                        && handler_->check_heartjump_timeout()) {
                                        boErrorWhat e_what;
                                        e_what.err_no(TCP_ERROR_TYPE_CLIENT_CLOSE_SOCKET_BECAUSE_HEARTJUMP);
                                        e_what.err_message("client close socket because heartjump");
                                        handler_->send_close_in_thread(e_what);
                                }
                        }


                        // Send message.
                        void send_data(const char* data, size_t data_size) {
                                if (is_authenticate_pass_) {
                                        handler_->send_data_in_thread(data, data_size);
                                }
                        }


                        // Get handler pointer.
                        HANDLER* handler() {
                                return handler_;
                        }
                protected:

                        // Call the method after connect to server success.
                        void connect_handler(pointer new_handler,
                                             const boost::system::error_code& ec) {
                                if (!ec) {

                                        // Call connet_success_func()
                                        boErrorWhat e_what;
                                        if (connet_success_func(new_handler, e_what) == 0) {
                                                // Start to aync-read.
                                                new_handler->start();

                                        } else {

                                                new_handler->catch_error(e_what);

                                                // Active close socket.
                                                new_handler->close_socket();
                                        }

                                } else {

                                        // If retry_delay_seconds is 0, then don't reconnect.
                                        if (PARSER::retry_delay_seconds == 0) {
                                                ios_.stop();
                                                return;
                                        }

                                        // Begin to reconnect.
                                        retry_delay_ *= 2;
                                        if (retry_delay_ > PARSER::max_retry_delay_seconds)
                                                retry_delay_ = PARSER::max_retry_delay_seconds;

                                        connet_fail_func(new_handler, retry_delay_);

                                        // Start to reconnet.
                                        boTimer::time_escape time;
                                        time.type = boTimer::TIMER_ESCAPE_TYPE_SECOND;
                                        time.interval = retry_delay_;
                                        timer0_.start(time);

                                        return;
                                }
                        }

                        // Start send heartjump timer.
                        void start_timer_to_send_heartjump() {
                                // If max_interval_seconds_send_heartjump is 0, then don't send heartjump.
                                if (PARSER::max_interval_seconds_send_heartjump > 0) {
                                        boTimer::time_escape time;
                                        time.type = boTimer::TIMER_ESCAPE_TYPE_SECOND;
                                        time.interval = PARSER::max_interval_seconds_send_heartjump;
                                        timer1_.start(time);
                                }
                        }

                        // Start check heartjump timer.
                        void start_timer_to_check_heartjump() {
                                // If max_interval_seconds_check_heartjump is 0, then don't check heartjump.
                                if (PARSER::max_interval_seconds_check_heartjump > 0) {
                                        boTimer::time_escape time;
                                        time.type = boTimer::TIMER_ESCAPE_TYPE_SECOND;
                                        time.interval = PARSER::max_interval_seconds_check_heartjump;
                                        timer2_.start(time);
                                }
                        }

                        // Call the function when connect in handler again.
                        void reconnect() {
                                retry_delay_ = PARSER::retry_delay_seconds;

                                // If retry_delay_seconds is 0, then don't reconnect.
                                if (retry_delay_ == 0)
                                        return;

                                start_connect();
                        }

                protected:
                        virtual int connet_success_func(pointer /*ptr*/, boErrorWhat& /*err_code*/) {
                                return 0;
                        }
                        virtual void connet_fail_func(pointer /*ptr*/, int& /*retry_delay_seconds*/) {
                        }

                protected:
                        BO_ASYNC_SERVICE& ios_;
                        HANDLER* handler_;

                        string ipv4_;
                        u16_t port_;

                        int retry_delay_; // reconnect delay seconds.
                        ReconnectTmer timer0_; // reconnect timer.

                        bool is_authenticate_pass_; // If authenticate is pass, then send heartbeat package.
                        SendHeartJumpTmer timer1_; // heartjump timer.
                        InspectHeartJumpTmer timer2_; // inspect heartjump timer.
                        make_heartjump_message_func f1_;
                };
        }
}

#endif


#endif /* BINGO_TCP_CLIENT_HEADER_H_ */

