/*
 * Server. Listens to incoming requests.
 * Copyright (C) 2017  Kelvin Ng
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <tcsmr/base/net/server_completion_queue.hpp>

#include <tcsmr/base/utils/endpoint.hpp>

#include <grpc++/grpc++.h>
#include <glog/logging.h>

namespace tcsmr {

class Server {
   public:
    Server(const Endpoint& endpoint);
    Server(const Endpoints& endpoints);

    template <typename ServiceT>
    inline void register_service(ServiceT* service) {
        builder_.RegisterService(service);
    }

    /*inline std::unique_ptr<grpc::ServerCompletionQueue> add_completion_queue() {
        return builder_.AddCompletionQueue();
    }*/
    
    inline ServerCompletionQueue add_completion_queue() {
        return ServerCompletionQueue(builder_.AddCompletionQueue());
    }

    inline void serve() {
        server_ = builder_.BuildAndStart();
        LOG(INFO) << "Mailbox serving";
    }

   protected:
    grpc::ServerBuilder builder_;
    std::unique_ptr<grpc::Server> server_;
};

}  // namespace tcsmr

