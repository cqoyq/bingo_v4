/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boTcpCetHandler.h
 * Author: root
 *
 * Created on 2017年9月7日, 下午1:54
 */

#ifndef BINGO_TCP_CLIENT_HANDLER_HEADER_H_
#define BINGO_TCP_CLIENT_HANDLER_HEADER_H_

#include "bingo_v4/boType.h"
#include "bingo_v4/boErrorWhat.h"
#include "bingo_v4/boTimer.h"
#include "boTcpAction.h"

#ifndef WIN32

#include <iostream>
using namespace std;

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

namespace bingo {
        namespace TCP {
                class boTcpCetHandler : public boost::enable_shared_from_this<boTcpCetHandler>,
                public boTcpMessageSplitAction,
                public boTcpMessageFormatAction,
                public boTcpHeartJumpAction,
                public boTcpAuthenticateAction {
                public:
                        typedef boost::shared_ptr<boTcpCetHandler> pointer;
                        typedef boost::function<void() > reconnect_func;
                        typedef boost::function<void(bool) > authenticate_pass_func;

                        explicit boTcpCetHandler(BO_ASYNC_SERVICE& io_service,
                                                 int max_interval_seconds_check_heartjump,
                                                 reconnect_func f1, // reconnect func
                                                 authenticate_pass_func f2 // authenticate pass func
                                                 );
                        virtual ~boTcpCetHandler();
                        
                        // Get scoket
                        ip::tcp::socket& socket() {
                                return socket_;
                        }

                        // Start to connect to server.
                        void start();


                        // These method will been called in thread. 
                        void send_data_in_thread(const char* data, size_t data_size);
                        void send_close_in_thread(boErrorWhat& e_what);


                        // Heartjump time.
                        void set_heartjump_datetime();
                        bool check_heartjump_timeout();



                        // This function is used to close the socket. Any asynchronous send, receive
                        // or connect operations will be cancelled immediately, and will complete
                        // with the boost::asio::error::operation_aborted error.
                        void close_socket();

                        // Catch error information.
                        void catch_error(boErrorWhat& e_what);

                protected:

                        // The method will been called in inner.
                        void close(const boost::system::error_code& ec);

                        // Clean something after socket close, and reconnect.
                        void close_completed(int ec_value);



                        // The method will been called in ios thread after call send_data_in_thread().
                        void active_send(string& pk);

                        // The method will been called in ios thread after call send_close_in_thread().
                        void active_close(boErrorWhat*& e_what);



                        // Set authenticate status.
                        void set_authenticate_pass(bool is_pass);

                        // Reconnect to server.
                        void reconnet();



                        // Call the method after receive data.
                        void read_handler(const boost::system::error_code& ec, size_t bytes_transferred);

                        // Call the method after send data.
                        void write_handler(const boost::system::error_code& ec, size_t bytes_transferred, string& pk);

                        // Call the method after connect to server success and send first data success.
                        void write_first_handler(const boost::system::error_code& ec, size_t bytes_transferred, string& pk);


                protected:
                        virtual void catch_error_func(pointer /*p*/, boErrorWhat& /*e_what*/) {
                        }
                        virtual void close_complete_func(pointer /*p*/, int& /*ec_value*/) {
                        }
                        virtual int read_pk_full_complete_func(pointer /*p*/, const char* /*rev_data*/, size_t /*rev_data_size*/, boErrorWhat& /*e_what*/) {
                                return 0;
                        }
                        virtual void write_pk_full_complete_func(pointer /*p*/, const char* /*snd_p*/, size_t /*snd_size*/, int /*ec*/) {
                        }
                        virtual int active_send_in_ioservice_func(pointer /*p*/, string& /*pk*/, boErrorWhat& /*e_what*/) {
                                return 0;
                        }
                        virtual int make_first_sended_package_func(string& /*pk*/, boErrorWhat& /*e_what*/) {
                                return 0;
                        }

                protected:
                        BO_ASYNC_SERVICE& ios_;
                        ip::tcp::socket socket_;

                        bool is_valid_;

#define      MESSAGE_MAX_OF_SIZE  4096
                        char message_block[MESSAGE_MAX_OF_SIZE];

                        reconnect_func f1_; // reconnect callback.
                        authenticate_pass_func f2_; // authenticate pass callback

                        ptime p1_; // Save heart jump datetime.
                        int max_interval_seconds_check_heartjump_;
                };
        }
}

#endif

#endif /* BINGO_TCP_CLIENT_HANDLER_HEADER_H_ */

