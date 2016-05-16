/*
 * Copyright (C) 2016 Marco Gulino (marco AT gulinux.net)
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include "number.h"
using namespace std;
using namespace INDI::Properties;

Number::Number(Property< Number >& main, OnUpdate on_update) : main {main}, on_update {on_update}
{
}


Number::single_property Number::new_property(const string& name, const string& label, vtype min, vtype max, vtype step, vtype val, const string format)
{
    single_property s;
    IUFillNumber(&s, name.c_str(), label.c_str(), format.c_str(), min, max, step, val);
    return s;
}

void Number::fill_vector()
{
    IUFillNumberVector(&main.m_vector_property, main.m_properties.data(), main.m_properties.size(),
                       UNPACK_IDENTITY(main), main.m_base_options.permissions, main.m_base_options.timeout, main.m_base_options.state);
    send();
}

void Number::send(const string& message)
{
    IDSetNumber(&main.m_vector_property, message.empty() ? message.c_str() : nullptr);
}

void Number::do_register() const
{
    main.m_device->defineNumber(&main.m_vector_property);
}

bool Number::update(vtype* values, char* names[], int n)
{
    if(! on_update(values, names, n ))
        return false;
    return IUUpdateNumber(&main.m_vector_property, values, names, n) == 0;
}
