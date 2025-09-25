#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace fst;

int main() {
    StdVectorFst fst;

    int start = fst.AddState();
    fst.SetStart(start);

    int accept = fst.AddState();
    fst.SetFinal(accept, TropicalWeight::One());

    // Vowel set
    std::unordered_set<char> vowels = {'A','E','I','O','U','a','e','i','o','u'};

    // Add arcs: vowels → 'V', consonants → 'C'
    for (char c = 'A'; c <= 'Z'; ++c) {
        if (vowels.count(c)) {
            fst.AddArc(start, StdArc(c, 'V', TropicalWeight::One(), accept));
        } else {
            fst.AddArc(start, StdArc(c, 'C', TropicalWeight::One(), accept));
        }
    }
    for (char c = 'a'; c <= 'z'; ++c) {
        if (vowels.count(c)) {
            fst.AddArc(start, StdArc(c, 'V', TropicalWeight::One(), accept));
        } else {
            fst.AddArc(start, StdArc(c, 'C', TropicalWeight::One(), accept));
        }
    }

    // Identity for non-alphabet
    for (char c = '0'; c <= '9'; ++c)
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    fst.AddArc(start, StdArc(' ', ' ', TropicalWeight::One(), accept));

    std::string input;
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);

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
        if (!matched) output.push_back(c);
    }

    std::cout << "Vowel/Consonant string: " << output << std::endl;
    return 0;
}