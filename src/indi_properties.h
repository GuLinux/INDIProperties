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
#include <functional>
#include "indi_property.h"
#include "switch.h"
#include "number.h"
#include "text.h"

namespace INDI {
namespace Properties {
template<typename key_type = std::string>
class Properties {
public:
  Property<Switch> &add_switch(const key_type &key, DefaultDevice *device, const Property<Switch>::BaseOptions &options, ISRule rule, Switch::OnUpdate on_update) {
    return *(m_switches[key] = std::make_shared<Property<Switch>>(device, options, rule, on_update));
  }
  Property<Number> &add_number(const key_type &key, DefaultDevice *device, const Property<Number>::BaseOptions &options, Number::OnUpdate on_update) {
    return *(m_numbers[key] = std::make_shared<Property<Number>>(device, options, on_update));
  }
  Property<Text> &add_text(const key_type &key, DefaultDevice *device, const Property<Text>::BaseOptions &options, Text::OnUpdate on_update) {
    return *(m_texts[key] = std::make_shared<Property<Text>>(device, options, on_update));
  }
  
  void remove(const key_type &key) {
    m_switches.erase(key);
    m_numbers.erase(key);
    m_texts.erase(key);
  }
  
  void clear() {
    m_switches.clear();
    m_numbers.clear();
    m_texts.clear();
  }
  
  
  Property<Switch> &switch_p(const key_type &key) { return *m_switches[key]; }
  Property<Number> &number(const key_type &key) { return *m_numbers[key]; }
  Property<Text> &text(const key_type &key) { return *m_texts[key]; }
  
  bool update(const std::string &device, const std::string &name, Switch::vtype *states, char *names[], int n) const {
    return std::any_of(m_switches.begin(), m_switches.end(), [&](const std::pair<key_type, Property<Switch>::ptr> &p) { return p.second->update(device, name, states, names, n); });
  }
  bool update(const std::string &device, const std::string &name, Number::vtype *values, char *names[], int n) const {
    return std::any_of(m_numbers.begin(), m_numbers.end(), [&](const std::pair<key_type, Property<Number>::ptr> &p) { return p.second->update(device, name, values, names, n); });
  }
  bool update(const std::string &device, const std::string &name, Text::vtype *values, char *names[], int n) const {
    return std::any_of(m_texts.begin(), m_texts.end(), [&](const std::pair<key_type, Property<Text>::ptr> &p) { return p.second->update(device, name, values, names, n); });
  }

  std::unordered_map<key_type, Property<Switch>::ptr> &switches() { return m_switches; }
  std::unordered_map<key_type, Property<Number>::ptr> &numbers() { return m_numbers; }
  std::unordered_map<key_type, Property<Text>::ptr> &texts() { return m_texts; }
private:
  std::unordered_map<key_type, Property<Switch>::ptr> m_switches;
  std::unordered_map<key_type, Property<Number>::ptr> m_numbers;
  std::unordered_map<key_type, Property<Text>::ptr> m_texts;
};
}
}
#endif
