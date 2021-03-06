/***********************************
 * iou.h
 *
 * Calculate the iou (Intersection over Union) ratio.
 * For ONLY Convex Polygons.
 *
 * Author: WeiQM (weiquanmao@hotmail.com)
 * Github: https://github.com/CheckBoxStudio/IoU
 *
 * 2018
 ***********************************/

#ifndef _IOU_H_FILE_
#define _IOU_H_FILE_

#include "math.h"
#include <assert.h>
#include <vector>

namespace IOU
{
    const double _ZERO_ = 1e-6;

    enum WiseType
    {
        NoneWise,
        ClockWise,
        AntiClockWise
    };
    enum LocPosition
    {
        OutSide,
        OnLine,
        InSide
    };


    template <typename T>
    struct Vec2 {
        // Members.
        union {
            struct 
            {
                T x;
                T y;
            };
            T D[2];
        };

        inline bool isZero() { return (abs(x) <= _ZERO_ && abs(y) <= _ZERO_); }
        inline bool nonZero() { return !isZero(); }

        // Constructors.
        Vec2() : x(0), y(0) {}
        Vec2(T _x, T _y) : x(_x), y(_y) {}
        Vec2(const Vec2& p) : x(p.x), y(p.y) {}

        // Access component.
        inline T& operator[](unsigned int i) { assert(i < 2); return D[i]; }
        inline const T& operator[](unsigned int i) const { assert(i < 2); return D[i]; }

        // Operations.
        inline Vec2& operator=(const Vec2 &p) { x = p.x; y = p.y; return *this; }
        inline bool operator==(const Vec2 &p) const {
            return (abs(x - p.x) <= _ZERO_ && abs(y - p.y) <= _ZERO_);
        }
        inline Vec2 operator*(T t) const { return Vec2(x * t, y * t); }
        inline Vec2 operator/(T t) const { return Vec2(x / t, y / t); }       
        inline Vec2& operator*=(T t) { x *= t; y *= t; return *this; }
        inline Vec2& operator/=(T t) { x /= t; y /= t; return *this; }

        inline Vec2 operator+(const Vec2 &p) const { return Vec2(x + p.x, y + p.y); }
        inline Vec2 operator-(const Vec2 &p) const { return Vec2(x - p.x, y - p.y); }
        inline Vec2& operator+=(const Vec2 &p) { x += p.x; y += p.y; return *this; }
        inline Vec2& operator-=(const Vec2 &p) { x -= p.x; y -= p.y; return *this; }

        inline Vec2 dmul(const Vec2 &p) const { return Vec2(x * p.x, y * p.y); }
        inline Vec2 ddiv(const Vec2 &p) const { return Vec2(x / p.x, y / p.y); }
        
        inline T dot(const Vec2 &p) const { return x * p.x + y * p.y; }
        inline T operator*(const Vec2 &p) const { return x * p.x + y * p.y; }

        inline T cmul(const Vec2 &p) const { return x * p.y - y * p.x; }
        inline T operator^(const Vec2 &p) const { return x * p.y - y * p.x; }

        inline T norm() const { return std::sqrt(x*x + y*y); }
        inline T normSquared() const { return x*x + y*y; }

        void normalize() { *this /= norm(); }
        Vec2 normalized() const { return *this / norm(); }

        T distance(const Vec2 &p) const {
            return (*this - p).norm();
        }
        T squareDistance(const Vec2 &p) const {
            return (*this - p).normSquared();
        }
        double angle(const Vec2 &r) const {
            return acos( dot(r) / (norm() * r.norm()) ); }
        double theta() const {
            Vec2 ax(1.0, 0.0);
            double a = angle(ax);
            if (cmul(ax) > 0.0)
                a = 3.141592653 * 2 - a;
            return a;
        }
        void swap(Vec2<T> &p) {
            Vec2<T> temp = *this;
            *this = p;
            p = temp;
        }
    };
    template <typename T>
    inline Vec2<T> operator*(T t, const Vec2<T>& v) { return Vec2<T>(v.x * t, v.y * t); }
    template <typename T>
    inline T distance(const Vec2<T> &p1, const Vec2<T> &p2) { return p1.distance(p2); }
    template <typename T>
    inline T squareDistance(const Vec2<T> &p1, const Vec2<T> &p2) { return p1.squareDistance(p2); }
    template <typename T>
    inline double angle(const Vec2<T> &p1, const Vec2<T> &p2) { return p1.angle(p2); }
    template <typename T>
    inline void swap(Vec2<T> &p1, Vec2<T> &p2) { return p1.swap(p2); }


    typedef Vec2<int> Vec2i;
    typedef Vec2<float> Vec2f;
    typedef Vec2<double> Vec2d;
    typedef Vec2d Point;
    typedef std::vector<Point> Vertexes;


    class Line {
    public:
        union {
            struct
            {
                Point p1;
                Point p2;
            };
            Point vert[2];
        };
        // Constructors.
        Line() : p1(Point()), p2(Point()) {}
        Line(const Point &_p1, const Point &_p2) : p1(_p1), p2(_p2) {}
        Line(const Point _vert[2]) : p1(_vert[0]), p2(_vert[1]) {}
        Line(const Line &line) : p1(line.p1), p2(line.p2) {}

        // Operations.
        Line& operator=(const Line &line) { p1 = line.p1; p2 = line.p2; return *this; }

        // Methods
        double length() const {return p1.distance(p2); }
        bool isOnLine(const Point &p) const;
        Point intersection(const Line &line, bool *bOnline = 0) const;
    };
    inline bool isOnLine(const Line &line, const Point &p) {
        return line.isOnLine(p); }
    inline bool isOnLine(const Point &p, const Line &line) {
        return line.isOnLine(p); }
    inline Point intersection(const Line &line1, const Line &line2, bool *bOnline = 0) {
        return line1.intersection(line2,bOnline); }


    class Quad {
    public:
        // in clockwise
        union {
            struct
            {
                Point p1;
                Point p2;
                Point p3;
                Point p4;
            };
            Point vert[4];
        };
        // Constructors.
        Quad() : p1(Point()), p2(Point()), p3(Point()), p4(Point()) {}
        Quad(const Point &_p1, const Point &_p2, const Point &_p3, const Point &_p4)
            : p1(_p1), p2(_p2), p3(_p3), p4(_p4) {}
        Quad(const Point _vert[4])
            : p1(_vert[0]), p2(_vert[1]), p3(_vert[2]), p4(_vert[3]) {}
        Quad(const Quad &quad)
            : p1(quad.p1), p2(quad.p2), p3(quad.p3), p4(quad.p4) {}

        // Operations.
        Quad& operator=(const Quad &quad) {
            p1 = quad.p1; p2 = quad.p2; p3 = quad.p3; p4 = quad.p4; return *this; }

        // Methods.
        void flip() { swap(p2, p4); }
        void getVertList(Vertexes &_vert) const;
        bool haveRepeatVert() const;

        double area() const;
        WiseType whichWise() const;
        bool isInClockWise() const { return ClockWise == whichWise(); }
        bool isInAntiClockWise() const { return AntiClockWise == whichWise(); }
        void beInSomeWise(const WiseType wiseType);
        void beInClockWise() { beInSomeWise(ClockWise); }
        void beInAntiClockWise() { beInSomeWise(AntiClockWise); }

        LocPosition location(const Point &p) const;
        int interPts(const Line &line, Vertexes &pts) const;
    };
    inline LocPosition location(const Quad &quad, const Point &p) {
        return quad.location(p); }
    inline int interPts(const Quad &quad, const Line &line, Vertexes &pts) {
        return quad.interPts(line,pts); }


    // For any convex polygon
    double areaEx(const Vertexes &C);
    WiseType whichWiseEx(const Vertexes &C);
    void beInSomeWiseEx(Vertexes &C, const WiseType wiseType);
    LocPosition locationEx(const Vertexes &C, const Point &p);
    int interPtsEx(const Vertexes &C, const Line &line, Vertexes &pts);


    // For any convex polygon
    int findInterPointsEx(const Vertexes &C1, const Vertexes &C2, Vertexes &vert);
    int findInnerPointsEx(const Vertexes &C1, const Vertexes &C2, Vertexes &vert);
    double areaIntersectionEx(const Vertexes &C1, const Vertexes &C2);
    double areaUnionEx(const Vertexes &C1, const Vertexes &C2);
    double iouEx(const Vertexes &C1, const Vertexes &C2);


    // For convex quadrilateral
    int findInterPoints(const Quad &Q1, const Quad &Q2, Vertexes &vert);
    int findInnerPoints(const Quad &Q1, const Quad &Q2, Vertexes &vert);
    double areaIntersection(const Quad&Q1, const Quad &Q2);
    double areaUnion(const Quad &Q1, const Quad &Q2);
    double iou(const Quad &Q1, const Quad &Q2);
}
#endif // !_IOU_H_FILE_
