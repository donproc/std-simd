/*  This file is part of the Vc library.

    Copyright (C) 2009-2013 Matthias Kretz <kretz@kde.org>

    Vc is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Vc is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Vc.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "unittest.h"
#include <iostream>
#include <cstring>

using namespace Vc;

template<typename Vec> void alignedStore()
{
    typedef typename Vec::EntryType T;
    enum {
        Count = 256 * 1024 / sizeof(T)
    };

    Memory<Vec, Count> array;
    // do the memset to make sure the array doesn't have the old data from a previous call which
    // would mask a real problem
    std::memset(array, 0xff, Count * sizeof(T));
    T xValue = 1;
    const Vec x(xValue);
    for (int i = 0; i < Count; i += Vec::Size) {
        x.store(&array[i]);
    }

    for (int i = 0; i < Count; ++i) {
        COMPARE(array[i], xValue);
    }

    // make sure store can be used with parameters that auto-convert to T*
    x.store(array);

    if (std::is_integral<T>::value && std::is_unsigned<T>::value) {
        // ensure that over-/underflowed values are stored correctly.
        Vec v = Vec::Zero() - Vec::One(); // underflow
        v.store(array);
        for (size_t i = 0; i < Vec::Size; ++i) {
            COMPARE(array[i], v[i]);
        }

        v = std::numeric_limits<T>::max() + Vec::One(); // overflow
        v.store(array);
        for (size_t i = 0; i < Vec::Size; ++i) {
            COMPARE(array[i], v[i]);
        }
    }
}

template<typename Vec> void unalignedStore()
{
    typedef typename Vec::EntryType T;
    enum {
        Count = 256 * 1024 / sizeof(T)
    };

    Memory<Vec, Count> array;
    // do the memset to make sure the array doesn't have the old data from a previous call which
    // would mask a real problem
    std::memset(array, 0xff, Count * sizeof(T));
    T xValue = 1;
    const Vec x(xValue);
    for (size_t i = 1; i < Count - Vec::Size + 1; i += Vec::Size) {
        x.store(&array[i], Unaligned);
    }

    for (size_t i = 1; i < Count - Vec::Size + 1; ++i) {
        COMPARE(array[i], xValue);
    }
}

template<typename Vec> void streamingAndAlignedStore()
{
    typedef typename Vec::EntryType T;
    enum {
        Count = 256 * 1024 / sizeof(T)
    };

    Memory<Vec, Count> array;
    // do the memset to make sure the array doesn't have the old data from a previous call which
    // would mask a real problem
    std::memset(array, 0xff, Count * sizeof(T));
    T xValue = 1;
    const Vec x(xValue);
    for (int i = 0; i < Count; i += Vec::Size) {
        x.store(&array[i], Streaming | Aligned);
    }

    for (int i = 0; i < Count; ++i) {
        COMPARE(array[i], xValue);
    }
}

template<typename Vec> void streamingAndUnalignedStore()
{
    typedef typename Vec::EntryType T;
    enum {
        Count = 256 * 1024 / sizeof(T)
    };

    Memory<Vec, Count> array;
    // do the memset to make sure the array doesn't have the old data from a previous call which
    // would mask a real problem
    std::memset(array, 0xff, Count * sizeof(T));
    T xValue = 1;
    const Vec x(xValue);
    for (size_t i = 1; i < Count - Vec::Size + 1; i += Vec::Size) {
        x.store(&array[i], Streaming | Unaligned);
    }

    for (size_t i = 1; i < Count - Vec::Size + 1; ++i) {
        COMPARE(array[i], xValue);
    }
}

template<typename Vec> void maskedStore()
{
    typedef typename Vec::EntryType T;
    typedef typename Vec::Mask M;
    M mask;
    {
        typedef typename Vec::IndexType I;
        const I tmp(IndexesFromZero);
        const typename I::Mask k = (tmp & I(One)) > 0;
        mask = M(k);
    }

    const int count = 256 * 1024 / sizeof(T);
    Vc::Memory<Vec> array(count);
    array.setZero();
    const T nullValue = 0;
    const T setValue = 170;
    const Vec x(setValue);
    for (int i = 0; i < count; i += Vec::Size) {
        x.store(&array[i], mask, Vc::Aligned);
    }

    for (int i = 1; i < count; i += 2) {
        COMPARE(array[i], setValue) << ", i: " << i << ", count: " << count << ", mask: " << mask << ", array:\n" << array;
    }
    for (int i = 0; i < count; i += 2) {
        COMPARE(array[i], nullValue) << ", i: " << i << ", count: " << count << ", mask: " << mask << ", array:\n" << array;
    }
}

void testmain()
{
    testAllTypes(alignedStore);
    testAllTypes(unalignedStore);
    testAllTypes(streamingAndAlignedStore);
    testAllTypes(streamingAndUnalignedStore);

    if (float_v::Size > 1) {
        runTest(maskedStore<int_v>);
        runTest(maskedStore<uint_v>);
        runTest(maskedStore<float_v>);
        runTest(maskedStore<double_v>);
        runTest(maskedStore<short_v>);
        runTest(maskedStore<ushort_v>);
    }
}
