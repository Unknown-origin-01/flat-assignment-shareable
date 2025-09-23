#include <fst/fstlib.h>
#include <iostream>
#include <string>

using namespace fst;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <string>\n";
        return 1;
    }

    std::string input = argv[1];

    // Create a mutable FST
    StdVectorFst fst;

    // Build states: one for each position in the string
    int start = fst.AddState();
    fst.SetStart(start);

    int current = start;
    for (char c : input) {
        int next = fst.AddState();
        fst.AddArc(current, StdArc(c, c, TropicalWeight::One(), next));
        current = next;
    }

    fst.SetFinal(current, TropicalWeight::One());

    // Reverse the FST
    StdVectorFst reversed;
    Reverse(fst, &reversed);

    // Now traverse reversed FST to collect the output string
    std::string output;
    int state = reversed.Start();
    while (reversed.Final(state) == TropicalWeight::Zero() && reversed.NumArcs(state) > 0) {
        for (fst::ArcIterator<StdVectorFst> aiter(reversed, state); !aiter.Done(); aiter.Next()) {
            const StdArc &arc = aiter.Value();
            output.push_back((char)arc.ilabel);
            state = arc.nextstate;
            break; // follow first arc
        }
    }

    std::cout << "Input: " << input << "\n";
    std::cout << "Reversed: " << output << "\n";

    return 0;
}
