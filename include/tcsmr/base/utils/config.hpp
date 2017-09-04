/*
 * Config. Parsing and storing configurations.
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

#include <tcsmr/base/utils/endpoint.hpp>

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>
#include <unordered_map>

namespace tcsmr {

class Config {
   public:
    using WidT = int;
    using WidsT = std::vector<WidT>;
    using RoleWidsT = std::unordered_map<std::string, WidsT>;

    Config(const std::string& config_path);

    const Endpoints& get_peer_endpoints() const {
        return peer_endpoints_;
    }

    const Endpoints& get_listen_endpoints() const {
        return listen_endpoints_;
    }

    const boost::property_tree::ptree& get_raw_config() const {
        return ptree_;
    }

   protected:
    RoleWidsT role_wids_;
    Endpoints peer_endpoints_;
    Endpoints listen_endpoints_;

    boost::property_tree::ptree ptree_;
};

}  // namespace tcsmr

