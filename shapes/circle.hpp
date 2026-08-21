// circle.hpp
#pragma once
#include "shape.hpp"

class Circle : public Shape {
    public:
        void draw() const override;
};
