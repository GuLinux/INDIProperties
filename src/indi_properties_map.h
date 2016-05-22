/*
    Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
*/

#ifndef INDI_PROPERTIES_LIB_INDI_PROPERTIES_MAP
#define INDI_PROPERTIES_LIB_INDI_PROPERTIES_MAP

#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include "c++/containers_streams.h"
#include <algorithm>
#include "indi_properties.h"

namespace INDI {
namespace Properties {
template<typename key_type = std::string, typename sub_key_type = std::string>
class PropertiesMap {
public:
  typedef std::pair<key_type, Properties<sub_key_type>> each;
  Properties<sub_key_type> &operator[](const key_type &key) { return m_map[key]; }
  void clear() { for(auto &e: m_map) e.second.clear(); }
  void clear(const key_type &key) { m_map[key].clear(); }
  template<typename ...Args>
  bool update(const std::string &device, const std::string &name, Args ...args) {
    return GuLinux::make_stream(m_map).any([&](const each &e) { return e.second.update(device, name, args...); });
  }

  void save_config(FILE *fp) const {
    GuLinux::make_stream(m_map).for_each([&](const each &e) { return e.second.save_config(fp); });
  }
private:
  std::unordered_map<key_type, Properties<sub_key_type>> m_map;
};
}
}

#endif