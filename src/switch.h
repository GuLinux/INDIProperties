/*
 *     Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)
 *
 *     This library is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published
 *     by the Free Software Foundation; either version 2.1 of the License, or
 *     (at your option) any later version.
 *
 *     This library is distributed in the hope that it will be useful, but
 *     WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *     or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *     License for more details.
 *
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this library; if not, write to the Free Software Foundation,
 *     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
*/

#ifndef INDI_PROPERTIES_SWITCH_H
#define INDI_PROPERTIES_SWITCH_H

#include <indiapi.h>
#include <string>
#include <functional>
#include "indi_property.h"
namespace INDI {
namespace Properties {

class Switch
{
public:
  typedef ISwitchVectorProperty vector_property;
  typedef ISwitch single_property;
  typedef ISState value_type;
  typedef std::function<bool(ISState *, char **, int)> OnUpdate;
  
  Switch(Property<Switch> &main, ISRule rule, OnUpdate on_update);
  single_property new_property(const std::string& name, const std::string& label, ISState state);
  void fill_vector();
  void do_register() const;
  bool update(ISState *states, char *names[], int n);
  void send(const std::string &message = {});
  
  struct Entry {long index; single_property iswitch; };
  typedef std::function<void(const Entry &)> RunOnSwitch;
  
  void first_on_switch(RunOnSwitch run_on_switch);
  void on_switches(RunOnSwitch run_on_switches);
  std::vector<Entry> on_switches() const;
private:
  Property<Switch> &main;
  ISRule m_rule;
  OnUpdate on_update;
};
}
}

#endif // INDI_PROPERTIES_SWITCH_H
