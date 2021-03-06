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

#include <Mathematics/IntrIntervals.h>
#include <Mathematics/IntrLine2Circle2.h>
#include <Mathematics/Ray.h>

// The queries consider the circle to be a solid (disk).

namespace gte
{
    template <typename Real>
    class TIQuery<Real, Ray2<Real>, Circle2<Real>>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        Result operator()(Ray2<Real> const& ray, Circle2<Real> const& circle)
        {
            Result result;
            FIQuery<Real, Ray2<Real>, Circle2<Real>> rcQuery;
            result.intersect = rcQuery(ray, circle).intersect;
            return result;
        }
    };

    template <typename Real>
    class FIQuery<Real, Ray2<Real>, Circle2<Real>>
        :
        public FIQuery<Real, Line2<Real>, Circle2<Real>>
    {
    public:
        struct Result
            :
            public FIQuery<Real, Line2<Real>, Circle2<Real>>::Result
        {
            // No additional information to compute.
        };

        Result operator()(Ray2<Real> const& ray, Circle2<Real> const& circle)
        {
            Result result;
            DoQuery(ray.origin, ray.direction, circle, result);
            for (int i = 0; i < result.numIntersections; ++i)
            {
                result.point[i] = ray.origin + result.parameter[i] * ray.direction;
            }
            return result;
        }

    protected:
        void DoQuery(Vector2<Real> const& rayOrigin,
            Vector2<Real> const& rayDirection, Circle2<Real> const& circle,
            Result& result)
        {
            FIQuery<Real, Line2<Real>, Circle2<Real>>::DoQuery(rayOrigin,
                rayDirection, circle, result);

            if (result.intersect)
            {
                // The line containing the ray intersects the disk; the
                // t-interval is [t0,t1].  The ray intersects the disk as long
                // as [t0,t1] overlaps the ray t-interval [0,+infinity).
                std::array<Real, 2> rayInterval = { (Real)0, std::numeric_limits<Real>::max() };
                FIQuery<Real, std::array<Real, 2>, std::array<Real, 2>> iiQuery;
                auto iiResult = iiQuery(result.parameter, rayInterval);
                result.intersect = iiResult.intersect;
                result.numIntersections = iiResult.numIntersections;
                result.parameter = iiResult.overlap;
            }
        }
    };
}
