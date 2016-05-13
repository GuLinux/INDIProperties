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

#include "switch.h"
#include <map>
#include <indiproperty.h>

using namespace std;
using namespace INDI::Properties;

Switch::Switch(Property<Switch>& main, ISRule rule) : main{main}, m_rule{rule}
{
}



Switch::single_property Switch::new_property(const string& name, const string& label, ISState state)
{
  single_property s;
  IUFillSwitch(&s, name.c_str(), label.c_str(), state);
  return s;
}


void Switch::fill_vector()
{
  IUFillSwitchVector(&main.m_vector_property, main.m_properties.data(), main.m_properties.size(), 
		     main.m_device.c_str(), main.m_name.c_str(), main.m_label.c_str(),
		     main.m_group.c_str(), IP_RO, m_rule, 60, IPS_OK);
}
