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

#include <Mathematics/DistLine3AlignedBox3.h>
#include <Mathematics/Line.h>
#include <Mathematics/OrientedBox.h>

namespace gte
{
    template <typename Real>
    class DCPQuery<Real, Line3<Real>, OrientedBox3<Real>>
        :
        public DCPQuery<Real, Line3<Real>, AlignedBox3<Real>>
    {
    public:
        struct Result
            :
            public DCPQuery<Real, Line3<Real>, AlignedBox3<Real>>::Result
        {
            // No additional information to compute.
        };

        Result operator()(Line3<Real> const& line, OrientedBox3<Real> const& box)
        {
            // Transform the line to the coordinate system of the oriented
            // box.  In this system, the box is axis-aligned with center at
            // the origin.
            Vector3<Real> diff = line.origin - box.center;
            Vector3<Real> point, direction;
            for (int i = 0; i < 3; ++i)
            {
                point[i] = Dot(diff, box.axis[i]);
                direction[i] = Dot(line.direction, box.axis[i]);
            }

            Result result;
            this->DoQuery(point, direction, box.extent, result);

            // Compute the closest point on the line.
            result.closestPoint[0] = line.origin + result.lineParameter * line.direction;

            // Compute the closest point on the box.
            result.closestPoint[1] = box.center;
            for (int i = 0; i < 3; ++i)
            {
                result.closestPoint[1] += point[i] * box.axis[i];
            }
            return result;
        }
    };
}
