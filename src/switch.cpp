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
#include <algorithm>
#include "c++/containers_streams.h"

using namespace std;
using namespace INDI::Properties;

Switch::Switch(Property<Switch>& main, ISRule rule, OnUpdate on_update) : main {main}, m_rule {rule}, on_update {on_update}
{
}



Switch::single_property Switch::new_property(const string& name, const string& label, vtype state)
{
    single_property s;
    IUFillSwitch(&s, name.c_str(), label.c_str(), state);
    return s;
}


void Switch::fill_vector()
{
    IUFillSwitchVector(&main.m_vector_property, main.m_properties.data(), main.m_properties.size(),
                       UNPACK_IDENTITY(main), main.m_base_options.permissions, m_rule, main.m_base_options.timeout, main.m_base_options.state);
    send();
}

void Switch::do_register() const
{
    main.m_device->defineSwitch(&main.m_vector_property);
}
bool Switch::update(Switch::vtype* states, char* names[], int n)
{
    if(! on_update(states, names, n ))
        return false;
    return IUUpdateSwitch(&main.m_vector_property, states, names, n) == 0;
}


void Switch::send(const string& message)
{
    IDSetSwitch(&main.m_vector_property, message.empty() ? message.c_str() : nullptr);
}

bool filter_on(Switch::Entry s) {
    return s.iswitch.s == ISS_ON;
}
struct SwitchToEntry {
    long index = 0;
    Switch::Entry operator()(ISwitch s) {
        return {index++, s};
    }
};

void Switch::first_on_switch(RunOnSwitch run_on_switch)
{
    SwitchToEntry s2e;
    auto filtered = GuLinux::make_stream(main.m_properties).transform<vector<Entry>>(s2e).filter(filter_on).get();
    if(filtered.size() >= 1)
        run_on_switch(filtered[0]);
}


void Switch::on_switches(RunOnSwitch run_on_switches)
{
    SwitchToEntry s2e;
    GuLinux::make_stream(main.m_properties).transform<vector<Entry>>(s2e).filter(filter_on).for_each(run_on_switches);
}

vector< Switch::Entry > Switch::on_switches() const
{
    SwitchToEntry s2e;
    return GuLinux::make_stream(main.m_properties).transform<vector<Entry>>(s2e).filter(filter_on).get();

}
