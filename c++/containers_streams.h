#pragma once
#ifndef GULINUX_COMMONS_CONTAINERS_STREAMS
#define GULINUX_COMMONS_CONTAINERS_STREAMS
/*
 * Copyright (C) 2016 Marco Gulino <marco@gulinux.net>
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
 *
 */
namespace GuLinux {
template<typename C>
class cstream {
public:
  cstream(C &c) : _container_ref{c} {}
  cstream(C &&c) : __moved{c}, _container_ref{__moved} {}
  operator C&() { return _container_ref; }
  C &ref() { return *this; }
private:
  C __moved;
  C &_container_ref;
};
}

#endif