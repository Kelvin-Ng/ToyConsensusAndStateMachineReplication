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

#include <tcsmr/base/utils/config.hpp>

#include <boost/property_tree/json_parser.hpp>

namespace tcsmr {

Config::Config(const std::string& config_path) {
    boost::property_tree::read_json(config_path, ptree_);

    const auto& listen_endpoints_item = ptree_.get_child_optional("listen_endpoints");
    if (listen_endpoints_item) {
        for (const auto& listen_endpoint_item : listen_endpoints_item.get()) {
            listen_endpoints_.emplace_back(listen_endpoint_item.second.get_value<std::string>());
        }
    }

    const auto& peer_endpoints_item = ptree_.get_child_optional("peer_endpoints");
    if (peer_endpoints_item) {
        for (const auto& peer_endpoint_item : peer_endpoints_item.get()) {
            peer_endpoints_.emplace_back(peer_endpoint_item.second.get_value<std::string>());
        }
    }

    const auto& roles_items = ptree_.get_child_optional("roles");
    if (roles_items) {
        for (const auto& role_items : roles_items.get()) {
            WidsT* wids = &role_wids_[role_items.first];
            for (const auto& wid_item : role_items.second) {
                wids->push_back(wid_item.second.get_value<WidT>());
            }
        }
    }
}

}  // namespace tcsmr

