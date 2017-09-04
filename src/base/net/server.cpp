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

#include <tcsmr/base/net/server.hpp>
#include <glog/logging.h>

namespace tcsmr {

Server::Server(const Endpoint& endpoint) {
    LOG(INFO) << "Will listen on " << endpoint.str();
    builder_.AddListeningPort(endpoint.str(), grpc::InsecureServerCredentials());
}

Server::Server(const Endpoints& endpoints) {
    for (const auto& endpoint : endpoints) {
        LOG(INFO) << "Will listen on " << endpoint.str();
        builder_.AddListeningPort(endpoint.str(), grpc::InsecureServerCredentials());
    }
}

}  // namespace tcsmr

