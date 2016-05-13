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
#include "indi_property.h"
namespace INDI {
namespace Properties {

class Switch
{
public:
  typedef ISwitchVectorProperty vector_property;
  typedef ISwitch single_property;
  Switch(Property<Switch> &main, ISRule rule);
  single_property new_property(const std::string& name, const std::string& label, ISState state);
  void fill_vector();
private:
  Property<Switch> &main;
  ISRule m_rule;
};
}
}

#endif // INDI_PROPERTIES_SWITCH_H
