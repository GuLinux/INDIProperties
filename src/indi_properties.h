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
#include <map>
#include <string>
#include <functional>
#include "indi_property.h"
#include "switch.h"
#include "number.h"
#include "text.h"
#include "blob.h"
#include "light.h"

namespace INDI {
namespace Properties {

template<typename key_type = std::string>
class Properties {
public:
  Property<Switch> &add_switch(const key_type &key, DefaultDevice *device, const Identity &options, ISRule rule, Switch::OnUpdate on_update) {
    return *(m_switches[key] = std::make_shared<Property<Switch>>(device, options, rule, on_update));
  }
  Property<Number> &add_number(const key_type &key, DefaultDevice *device, const Identity &options, Number::OnUpdate on_update) {
    return *(m_numbers[key] = std::make_shared<Property<Number>>(device, options, on_update));
  }
  Property<Text> &add_text(const key_type &key, DefaultDevice *device, const Identity &options, Text::OnUpdate on_update) {
    return *(m_texts[key] = std::make_shared<Property<Text>>(device, options, on_update));
  }
  Property<Text> &add_blob(const key_type &key, DefaultDevice *device, const Identity &options, Blob::OnUpdate on_update) {
    return *(m_blobs[key] = std::make_shared<Property<Blob>>(device, options, on_update));
  }
  Property<Text> &add_light(const key_type &key, DefaultDevice *device, const Identity &options) {
    return *(m_lights[key] = std::make_shared<Property<Light>>(device, options));
  }
  
  void remove(const key_type &key) {
    m_switches.erase(key);
    m_numbers.erase(key);
    m_texts.erase(key);
    m_blobs.erase(key);
    m_lights.erase(key);
  }
  
  void clear() {
    m_switches.clear();
    m_numbers.clear();
    m_texts.clear();
    m_blobs.clear();
    m_lights.clear();
  }
  
  
  Property<Switch> &switch_p(const key_type &key) { return *m_switches[key]; }
  Property<Number> &number(const key_type &key) { return *m_numbers[key]; }
  Property<Text> &text(const key_type &key) { return *m_texts[key]; }
  Property<Blob> &blob(const key_type &key) { return *m_blobs[key]; }
  Property<Blob> &light(const key_type &key) { return *m_lights[key]; }
  
  bool update(const std::string &device, const std::string &name, Switch::vtype *states, char *names[], int n) const {
    return std::any_of(m_switches.begin(), m_switches.end(), [&](const std::pair<key_type, Property<Switch>::ptr> &p) { return p.second->update(device, name, states, names, n); });
  }
  bool update(const std::string &device, const std::string &name, Number::vtype *values, char *names[], int n) const {
    return std::any_of(m_numbers.begin(), m_numbers.end(), [&](const std::pair<key_type, Property<Number>::ptr> &p) { return p.second->update(device, name, values, names, n); });
  }
  bool update(const std::string &device, const std::string &name, Text::vtype *values, char *names[], int n) const {
    return std::any_of(m_texts.begin(), m_texts.end(), [&](const std::pair<key_type, Property<Text>::ptr> &p) { return p.second->update(device, name, values, names, n); });
  }
  bool update(const std::string &device, const std::string &name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n) const {
    return std::any_of(m_blobs.begin(), m_blobs.end(), [&](const std::pair<key_type, Property<Blob>::ptr> &p) { return p.second->update(device, name, sizes, blobsizes, blobs, formats, names, n); });
  }

  std::map<key_type, Property<Switch>::ptr> &switches() { return m_switches; }
  std::map<key_type, Property<Number>::ptr> &numbers() { return m_numbers; }
  std::map<key_type, Property<Text>::ptr> &texts() { return m_texts; }
  std::map<key_type, Property<Light>::ptr> &lights() { return m_lights; }
  
  void register_unregistered_properties() {
    autoregister(m_switches);
    autoregister(m_numbers);
    autoregister(m_texts);
    autoregister(m_blobs);
    autoregister(m_lights);
  }
  
  void save_config(FILE *fp) const {
    save_config_to(m_switches, fp);
    save_config_to(m_numbers, fp);
    save_config_to(m_texts, fp);
    save_config_to(m_blobs, fp);
  }
private:
  template<typename T> void autoregister(T &m_map) {
    typedef typename T::mapped_type V;
    typedef typename std::pair<key_type, V> P;
    GuLinux::make_stream(m_map).cp_filter([](const P &p){ return !p.second->is_property_registered(); }).for_each([](const P &p){ p.second->do_register(); });
  }
  template<typename T> void save_config_to(const T &t, FILE *fp) const {
    GuLinux::make_stream(t).for_each([&](const std::pair<key_type, typename T::mapped_type> &p){ p.second->get().save_config(fp); });
  }
  std::map<key_type, Property<Switch>::ptr> m_switches;
  std::map<key_type, Property<Number>::ptr> m_numbers;
  std::map<key_type, Property<Text>::ptr> m_texts;
  std::map<key_type, Property<Blob>::ptr> m_blobs;
  std::map<key_type, Property<Light>::ptr> m_lights;
};
}
}
#endif
