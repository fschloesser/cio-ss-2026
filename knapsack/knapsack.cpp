#include <algorithm>
#include <format>
#include <iostream>
#include <random>
#include <vector>

// a struct is a class whose members are public by default,
struct KnapsackItem {
    int id;
    int weight;
    int value;
};

// A generator
// a function object with state - the random number generator, the distributions and the counter.
// so every call produces the next item
// since the members are initialized at their declaration, no constructor is needed
class KnapsackItemGenerator {
    public:
        KnapsackItem operator()() {
            // fills the members in declaration order in the struct at the top (id is at the front)
            return KnapsackItem{next_id_++, weight_dist_(gen_), value_dist_(gen_)};
        }
    private:
        std::mt19937 gen_{std::random_device{}()};
        std::uniform_int_distribution<int> weight_dist_{1, 10};
        std::uniform_int_distribution<int> value_dist_{1, 100};
        int next_id_ = 0;
};

double get_value_to_weight_ratio(const KnapsackItem & item) {
    return static_cast<double>(item.value) / item.weight;
}

void print_knapsack_item(const KnapsackItem & item) {
    std::cout << std::format("item {:2}: value {:3} weight {:2}\n", item.id, item.value, item.weight);
}

// The state of the packing: what has been packed so far and the capacity that is left
struct PackingState {
    std::vector<KnapsackItem> packed;
    int remaining_capacity;
};

int main() {
    // a knapsack of capacity 25 and 10 random items with ids 0,1,2,...
    int capacity = 25;
    std::vector<KnapsackItem> items(10);
    std::ranges::generate(items, KnapsackItemGenerator{});

    // Print the items
    std::cout << "Knapsack capacity: " << capacity << '\n';
    std::cout << "generated items: \n";
    std::ranges::for_each(items, print_knapsack_item);

    // std::ranges::sort takes items, a comparator and a projection:
    // the projection computes a key for every element
    // the comparator orders the keys by comparing the projection values by the comparator function
    // this is sorted(items, key=get_value_to_weight_ratio, reverse=True)
    // the id records where each item started, since the sort shuffles them.
    // the algorithms library holds many more, std::ranges::max_element, std::ranges::count_if, std::ranges::find
    // and reaching for them before writing a loop by hand is considered good style

    // sort the items by value per weight, best first
    std::ranges::sort(items, std::ranges::greater{}, get_value_to_weight_ratio);

    // Consider the current packing state and the next item: if the item fits, pack it, otherwise do not
    auto pack_if_fit = [](PackingState state, const KnapsackItem & item) {
        if (item.weight <= state.remaining_capacity) {
            state.packed.push_back(item);
            state.remaining_capacity -= item.weight;
        }
        return state;
    };

    // the packing is functools.reduce: fold_left threads a PackingState - the packed items and the remaining capacity
    // - through pack, one call per item.
    // an item that does not fit is skipped rather than terminating the packing process, a later, lighter one may still be packed.
    // the packed weight falls out of the final state as 'capacity - final_state.remaining_capacity

    // greedily pack the knapsack by folding over the items, best first
    PackingState final_state = std::ranges::fold_left(items, PackingState{{}, capacity}, pack_if_fit);

    // note that the greedy packing is a heuristic, it is not in general the optimal solution of the knapsack problem.
    // finding that is much harder, and this course is about exactly such problems.

    std::cout << "packed items:\n";
    std::ranges::for_each(final_state.packed, print_knapsack_item);
    std::cout << std::format("packed weight {}\n", capacity - final_state.remaining_capacity);

    return 0;
}
