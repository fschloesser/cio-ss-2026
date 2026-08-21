// triangle.hpp
#pragma once
#include "shape.hpp"

class Triangle : public Shape {
    public:
        void draw() const override;
};
