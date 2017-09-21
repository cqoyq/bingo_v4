/*
 * type.h
 *
 *  Created on: 2016-6-29
 *      Author: root
 */

#ifndef BINGO_COLLECTION_HEADER_H_
#define BINGO_COLLECTION_HEADER_H_

#include "thread/boThreadAction.h"
#include "boForeach.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/filtered.hpp>

namespace bingo {
        template<typename T>
        class boCollection {
        public:
                boCollection() {
                };
                virtual ~boCollection() {
                };

                // Push one item, return this.
                boCollection* push_back(T* n) {
                        collection_.push_back(n);
                        return this;
                }

                // Get item by index, fail return 0.
                T* at(int index) {
                        int max = collection_.size() - 1;
                        if (index <= max)
                                return &(collection_[index]);
                        else
                                return 0;
                }

                // Retrival collection size.
                size_t size() {
                        return collection_.size();
                }
                boost::ptr_vector<T> & collection() {
                        return collection_;
                }

        protected:
                boost::ptr_vector<T> collection_;
        };
        template<typename T>
        class boCollectionMutex {
        public:
                boCollectionMutex() {
                };
                virtual ~boCollectionMutex() {
                };

                // Push one item, return this.
                boCollectionMutex* push_back(T* n) {
                        // lock part field.
                        BO_SCOPE_LOCK_TYPE lock(mtx_);

                        collection_.push_back(n);
                        return this;
                }

                // Get item by index, fail return 0.
                T* operator[](int index) {
                        // lock part field.
                        BO_SCOPE_LOCK_TYPE lock(mtx_);

                        int max = collection_.size() - 1;
                        if (index <= max)
                                return &(collection_[index]);
                        else
                                return 0;
                }

                // Retrival collection size.
                size_t size() {
                        // lock part field.
                        BO_SCOPE_LOCK_TYPE lock(mtx_);

                        return collection_.size();
                }

        protected:
                boost::ptr_vector<T> collection_;
                BO_MUTEX_TYPE mtx_;
        };

}


#endif /* BINGO_COLLECTION_HEADER_H_ */
