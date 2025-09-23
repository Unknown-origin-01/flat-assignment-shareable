#include <fst/fstlib.h>
#include <iostream>
#include <string>

using namespace fst;

int main() {
    // Build the FST
    StdVectorFst fst;

    int start = fst.AddState();
    fst.SetStart(start);

    int accept = fst.AddState();
    fst.SetFinal(accept, TropicalWeight::One());

    // Lowercase -> Uppercase
    for (char c = 'a'; c <= 'z'; ++c) {
        char upper = c - 'a' + 'A';
        fst.AddArc(start, StdArc(c, upper, TropicalWeight::One(), accept));
    }

    // Identity arcs for other characters (keep them unchanged)
    for (char c = 'A'; c <= 'Z'; ++c) {
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    }
    for (char c = '0'; c <= '9'; ++c) {
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    }
    fst.AddArc(start, StdArc(' ', ' ', TropicalWeight::One(), accept));

    // Take user input
    std::string input;
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);

    // Process input string through the FST manually
    std::string output;
    for (char c : input) {
        bool matched = false;
        for (ArcIterator<StdVectorFst> aiter(fst, start); !aiter.Done(); aiter.Next()) {
            const StdArc &arc = aiter.Value();
            if (arc.ilabel == c) {
                output.push_back(static_cast<char>(arc.olabel));
                matched = true;
                break;
            }
        }
        if (!matched) {
            // If no matching arc, keep character unchanged
            output.push_back(c);
        }
    }

    // Show result
    std::cout << "Converted string: " << output << std::endl;

    return 0;
}
