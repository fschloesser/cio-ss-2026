#include "circle.hpp"
#include "triangle.hpp"
#include "square.hpp"

#include <memory>
#include <random>
#include <vector>

int main() {
    std::mt19937 gen(std::random_device{}()); // a random number generator, seeded once
    std::uniform_real_distribution<double> u(0.0, 1.0);

    // 1. We create a vector that will own the ten shapes
    std::vector<std::unique_ptr<Shape>> shapes;
    for (int i = 0; i < 10; ++i) {
        double x = u(gen); // uniformly distributed in [0,1)
        // 2. We create a random shape in heap memory and hand ownership over to the vector
        if (x < 1.0/3.0) {
            shapes.push_back(std::make_unique<Circle>());
        } else if (x < 2.0/3.0) {
            shapes.push_back(std::make_unique<Square>());
        } else {
            shapes.push_back(std::make_unique<Triangle>());
        }
    }

    for (const auto & shape : shapes) {
        // 3. Which draw runs is decided by the object, not by the pointer type
        shape->draw();
        // the above is equivalent to the following:
        // (*shape).draw()
    }
    return 0; // the vector goes out of scope and every shape is destroyed, no delete written anywhere
}
