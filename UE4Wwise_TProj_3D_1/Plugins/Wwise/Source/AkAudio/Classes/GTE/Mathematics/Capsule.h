/*******************************************************************************
The content of the files in this repository include portions of the
AUDIOKINETIC Wwise Technology released in source code form as part of the SDK
package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use these files in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2020
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 4.0.2019.08.13

#pragma once

#include <Mathematics/Segment.h>

// A capsule is the set of points that are equidistant from a segment, the
// common distance called the radius.

namespace gte
{
    template <int N, typename Real>
    class Capsule
    {
    public:
        // Construction and destruction.  The default constructor sets the
        // segment to have endpoints p0 = (-1,0,...,0) and p1 = (1,0,...,0),
        // and the radius is 1.
        Capsule()
            :
            radius((Real)1)
        {
        }

        Capsule(Segment<N, Real> const& inSegment, Real inRadius)
            :
            segment(inSegment),
            radius(inRadius)
        {
        }

        // Public member access.
        Segment<N, Real> segment;
        Real radius;

    public:
        // Comparisons to support sorted containers.
        bool operator==(Capsule const& capsule) const
        {
            return segment == capsule.segment && radius == capsule.radius;
        }

        bool operator!=(Capsule const& capsule) const
        {
            return !operator==(capsule);
        }

        bool operator< (Capsule const& capsule) const
        {
            if (segment < capsule.segment)
            {
                return true;
            }

            if (segment > capsule.segment)
            {
                return false;
            }

            return radius < capsule.radius;
        }

        bool operator<=(Capsule const& capsule) const
        {
            return !capsule.operator<(*this);
        }

        bool operator> (Capsule const& capsule) const
        {
            return capsule.operator<(*this);
        }

        bool operator>=(Capsule const& capsule) const
        {
            return !operator<(capsule);
        }
    };

    // Template alias for convenience.
    template <typename Real>
    using Capsule3 = Capsule<3, Real>;
}
