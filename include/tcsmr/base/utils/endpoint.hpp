/*
 * Endpoint. An abstraction of an endpoint.
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

#include <string>
#include <vector>

namespace tcsmr {

class Endpoint {
 public:
    Endpoint(const std::string& endpoint_str);
    Endpoint(const std::string& host, unsigned short port);

    inline std::string str() const {
        return host_ + ":" + std::to_string(port_);
    }

    inline const std::string& get_host() const {
        return host_;
    }

    inline unsigned short get_port() const {
        return port_;
    }

 private:
    std::string host_;
    unsigned short port_;
};

using Endpoints = std::vector<Endpoint>;

}  // namespace tcsmr

