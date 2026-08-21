// square.hpp
#pragma once
#include "shape.hpp"

class Square : public Shape {
    public:
        void draw() const override;
};
