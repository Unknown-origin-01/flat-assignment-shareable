#include <fst/fstlib.h>
#include <iostream>
#include <string>

using namespace fst;

// Function to build FST for binary complement
StdVectorFst BuildBinaryComplementFST() {
    StdVectorFst fst;

    int start = fst.AddState();
    fst.SetStart(start);

    int accept = fst.AddState();
    fst.SetFinal(accept, TropicalWeight::One());

    // Complement rules: 0 -> 1, 1 -> 0
    fst.AddArc(start, StdArc('0', '1', TropicalWeight::One(), accept));
    fst.AddArc(start, StdArc('1', '0', TropicalWeight::One(), accept));

    // Identity arcs for other characters (unchanged)
    for (char c = 'a'; c <= 'z'; ++c)
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    for (char c = 'A'; c <= 'Z'; ++c)
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    fst.AddArc(start, StdArc(' ', ' ', TropicalWeight::One(), accept));

    return fst;
}

// Apply FST to a string
std::string ApplyFST(const StdVectorFst &fst, const std::string &input) {
    std::string output;

    int start = fst.Start();
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
            // If no rule found, keep as-is
            output.push_back(c);
        }
    }

    return output;
}

int main() {
    StdVectorFst fst = BuildBinaryComplementFST();

    std::string input;
    std::cout << "Enter a binary string: ";
    std::getline(std::cin, input);

    std::string output = ApplyFST(fst, input);

    std::cout << "Binary complement: " << output << std::endl;
    return 0;
}