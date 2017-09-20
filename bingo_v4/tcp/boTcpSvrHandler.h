/*
 * tcp_svr_handler.h
 *
 *  Created on: 2016-6-30
 *      Author: root
 */

#ifndef BINGO_TCP_SVR_HANDLER_HEADER_H_
#define BINGO_TCP_SVR_HANDLER_HEADER_H_

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
                class boTcpSvrHandler : public BO_ENABLE_SHARED_FROM_THIS<boTcpSvrHandler>,
                public boTcpMessageSplitAction,
                public boTcpMessageFormatAction,
                public boTcpHeartJumpAction,
                public boTcpAuthenticateAction {
                public:
                        typedef BO_SHARED_PTR<boTcpSvrHandler> pointer;
                        typedef boost::function<void(string&) > remove_socket_func;

                        // Construct and destruct
                        explicit boTcpSvrHandler(BO_ASYNC_SERVICE& io_service,
                                                 int max_wait_for_heartjump_seconds,
                                                 int max_wait_for_authentication_pass_seconds,
                                                 remove_socket_func f1);
                        virtual ~boTcpSvrHandler();

                        // Get scoket
                        ip::tcp::socket& socket() {
                                return socket_;
                        }

                        // Get uuid
                        string& uuid() {
                                return uuid_flag_;
                        }

                        // Start to handle tcp.
                        void start();

                        // Close socket.
                        // This function is used to close the socket. Any asynchronous send, receive
                        // or connect operations will be cancelled immediately, and will complete
                        // with the boost::asio::error::operation_aborted error.
                        void close_socket();

                        // Catch error infomation.
                        void catch_error(boErrorWhat& e_what);




                        // Call the method from tcp_handler_manager.
                        bool check_heartjump_timeout();

                        // Call this to update p1_ when detect the heartjump package.
                        void set_heartjump_datetime();

                        // Call this to update p3_.
                        void set_active_heartjump_datetime();

                        // Call this from tcp_handler_manager.
                        bool check_authentication_pass();

                        // Call this to update is_authentication_pass_ is true, and then update heartjump datetime.
                        // when detect authentication is pass.
                        void set_authentication_pass();

                        // Retrival is_authentication_pass_ status.
                        bool get_authentication_pass();

                        // Call this to update p2_.
                        void set_authentication_pass_datetime();




                        // These method is called in thread. 
                        void send_heartjump_in_thread();

                        void send_data_in_thread(const char* data, size_t data_size);

                        void send_close_in_thread(boErrorWhat& e_what);




                        // Call the method in ios thread. the method is called by send_heartjump_in_thread().
                        void active_send_heartjump();

                        // Call the method to active close socket ios thread. 
                        void active_close(boErrorWhat*& e_what);

                        // Call the method in ios thread. the method is called by send_data_in_thread().
                        void active_send_data(string& pk);


                protected:
                        // Call the method after receive data.
                        void read_handler(const boost::system::error_code& ec,
                                          size_t bytes_transferred);

                        // Call the method after send data.
                        void write_handler(const boost::system::error_code& ec, size_t bytes_transferred, string& pk);




                        // Call the method to close socket in ios thread, the method is called by inner.
                        void close(const boost::system::error_code& ec);

                        // Call the method to clean something after close socket. 
                        void close_completed(int ec_value);

                protected:
                        // These method will been called after inner handle.
                        virtual void catch_error_func(pointer /*p*/, boErrorWhat& /*e_what*/) {
                        };
                        virtual void close_complete_func(pointer /*p*/, int /*ec_value*/) {
                        };
                        virtual int read_pk_full_complete_func(pointer /*p*/, const char* /*rev_data*/, size_t /*rev_data_size*/, boErrorWhat& /*e_what*/) {
                                return 0;
                        };
                        virtual void write_pk_full_complete_func(pointer /*p*/, const char* /*snd_p*/, size_t /*snd_size*/, int /*ec*/) {
                        };
                        virtual int active_send_data_in_ioservice_func(pointer /*p*/, string& /*pk*/, boErrorWhat& /*e_what*/) {
                                return 0;
                        };
                        virtual void active_send_heartjump_in_ioservice_func(pointer /*p*/) {
                        };

                protected:
                        BO_ASYNC_SERVICE& ios_;
                        ip::tcp::socket socket_;
                        remove_socket_func f1_;

                        bool is_valid_;
                        string uuid_flag_; // uuid's string format, this is boTcpSvrHandler's identify. 

#define      MESSAGE_MAX_OF_SIZE  4096
                        char message_block[MESSAGE_MAX_OF_SIZE];

                        ptime p1_; // Save passive heart jump datetime.
                        ptime p3_; // Save active heart jump datetime.
                        int max_wait_for_heartjump_seconds_;

                        ptime p2_; // Save authentication pass datetime.
                        bool is_authentication_pass_; // When authentication pass, this is true, default is false.
                        int max_wait_for_authentication_pass_seconds_;
                };

        }
}

#endif

#endif /* BINGO_TCP_SVR_HANDLER_HEADER_H_ */
