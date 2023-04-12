#ifndef XNA_ENUMERATIONS_HPP
#define XNA_ENUMERATIONS_HPP

namespace xna {
    enum class ContainmentType {
        Disjoint,
        Contains,
        Intersects,
    };

    enum class PlaneIntersectionType {
        Front,
        Back,
        Intersecting,
    };

    enum class PlayerIndex {
        One,
        Two,
        Three,
        Four,
    };

    enum class CurveContinuity {
        Smooth,
        Step,
    };

    enum class CurveLoopType {
        Constant,
        Cycle,
        CycleOffset,
        Oscillate,
        Linear,
    };

    enum class CurveTangent {
        Flat,
        Linear,
        Smooth,
    };

    enum class DisplayOrientation {
        Default = 0,
        LandscapeLeft = 1,
        LandscapeRight = 2,
        Portrait = 4,
    };
}

#endif