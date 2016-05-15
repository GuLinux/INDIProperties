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

#ifndef INDI_PROPERTIES_LIB_INDI_PROPERTIES
#define INDI_PROPERTIES_LIB_INDI_PROPERTIES

#include <map>
#include <unordered_map>
#include <string>
#include "indi_property.h"
#include "switch.h"

namespace INDI {
namespace Properties {
template<typename key_type = std::string>
class Properties {
public:
  Property<Switch> &add_switch(const key_type &key, DefaultDevice *device, const Property<Switch>::BaseOptions &options, ISRule rule, Switch::OnUpdate on_update) {
    m_switches[key] = std::make_shared<Property<Switch>>(device, options, rule, on_update);
    return *m_switches[key];
  }
  void remove_switch(const key_type &key) {
    m_switches.erase(key);
  }
  
  Property<Switch> &operator[](const key_type &key) { return *m_switches[key]; }

private:
  std::unordered_map<key_type, Property<Switch>::ptr> m_switches;
};
}
}
#endif
