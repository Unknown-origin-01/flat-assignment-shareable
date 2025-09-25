#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace fst;

int main() {
    StdVectorFst fst;

    int start = fst.AddState();
    fst.SetStart(start);

    int accept = fst.AddState();
    fst.SetFinal(accept, TropicalWeight::One());

    // Digit to word map
    std::unordered_map<char, std::string> digitWords = {
        {'0', "ZERO"}, {'1', "ONE"}, {'2', "TWO"}, {'3', "THREE"}, {'4', "FOUR"},
        {'5', "FIVE"}, {'6', "SIX"}, {'7', "SEVEN"}, {'8', "EIGHT"}, {'9', "NINE"}
    };

    // Add arcs (digit → word’s first letter, rest handled manually in output step)
    for (auto &p : digitWords) {
        fst.AddArc(start, StdArc(p.first, p.second[0], TropicalWeight::One(), accept));
    }

    // Identity arcs for non-digits
    for (char c = 'A'; c <= 'Z'; ++c)
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    for (char c = 'a'; c <= 'z'; ++c)
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    fst.AddArc(start, StdArc(' ', ' ', TropicalWeight::One(), accept));

    std::string input;
    std::cout << "Enter a string with digits: ";
    std::getline(std::cin, input);

    std::string output;
    for (char c : input) {
        bool matched = false;
        for (ArcIterator<StdVectorFst> aiter(fst, start); !aiter.Done(); aiter.Next()) {
            const StdArc &arc = aiter.Value();
            if (arc.ilabel == c) {
                if (digitWords.count(c)) {
                    output += digitWords[c];  // expand full word
                } else {
                    output.push_back(static_cast<char>(arc.olabel));
                }
                matched = true;
                break;
            }
        }
        if (!matched) output.push_back(c);
    }

    std::cout << "Converted string: " << output << std::endl;
    return 0;
}