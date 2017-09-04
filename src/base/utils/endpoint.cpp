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

#include <tcsmr/base/utils/endpoint.hpp>

namespace tcsmr {

Endpoint::Endpoint(const std::string& endpoint_str) {
    auto colon_pos = endpoint_str.find(":");
    host_ = endpoint_str.substr(0, colon_pos);
    port_ = std::stoi(endpoint_str.substr(colon_pos + 1));
}

Endpoint::Endpoint(const std::string& host, unsigned short port)
    : host_(host), port_(port) {}

}  // namespace tcsmr

