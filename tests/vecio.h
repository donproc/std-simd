/*
    Copyright (C) 2009 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) version 3.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.

*/

#ifndef VECIO_H
#define VECIO_H

#include "../vector.h"
#include <iostream>

namespace AnsiColor
{
  static const char *const green  = "\033[1;40;32m";
  static const char *const yellow = "\033[1;40;33m";
  static const char *const blue   = "\033[1;40;34m";
  static const char *const normal = "\033[0m";
} // namespace AnsiColor

template<typename T>
std::ostream &operator<<(std::ostream &out, const Vc::Vector<T> &v)
{
    using namespace AnsiColor;
    out << green << "[";
    for (int i = 0; i < v.Size; ++i) {
        if (i > 0) {
            out << ", ";
        }
        out << v[i];
    }
    out << "]" << normal;
    return out;
}

#ifdef ENABLE_LARRABEE
template<typename T>
std::ostream &operator<<(std::ostream &out, const Larrabee::VectorMultiplication<T> &v)
{
    return out << Larrabee::Vector<T>(v);
}
#endif

#ifdef USE_SSE
std::ostream &operator<<(std::ostream &out, const SSE::Float8Mask &m)
{
    using namespace AnsiColor;
    out << yellow << "m[";
    for (unsigned int i = 0; i < 8; ++i) {
        if (i > 0 && (i % 4) == 0) {
            out << " ";
        }
        out << m[i];
    }
    out << "]" << normal;
    return out;
}
#endif

template<unsigned int VectorSize>
std::ostream &operator<<(std::ostream &out, const Vc::Mask<VectorSize> &m)
{
    using namespace AnsiColor;
    out << yellow << "m[";
    for (unsigned int i = 0; i < VectorSize; ++i) {
        if (i > 0 && (i % 4) == 0) {
            out << " ";
        }
        out << m[i];
    }
    out << "]" << normal;
    return out;
}

#endif // VECIO_H