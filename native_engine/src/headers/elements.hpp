#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <utility>
#include <array>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <set>

namespace elements
{
    const short N_DIRECTIONS = 6;
    enum class Form {
        ball, holey, solid
    };
    const std::map<std::string, Form> formsMap{
        {"ball", Form::ball},
        {"holey", Form::holey},
        {"solid", Form::solid}
    };

    struct Element {
        Form form;
        std::set<short> connections;
        std::set<short> holes;
        Element(
            Form form,
            const std::set<short>& connections,
            const std::set<short>& holes
        ) : form(form), connections(connections), holes(holes) {};
        bool operator<(Element const& rhs) const {
            return form < rhs.form;
        };

        void print() const {
            std::cout << int(form);
            if (form == Form::holey) {
                std::cout << " h: ";
                for (auto h: holes)
                    std::cout << h << ", ";
            }
            else {
                std::cout << " c: ";
                for (auto h: connections)
                    std::cout << h << ", ";
            }
            std::cout << '\n';
        };
    };

    short turn(short direction, short rotation) {
        return (6 + direction + rotation) % N_DIRECTIONS;
    }

    bool isCompatibleHB(const Element& h, const Element& b)
    {
        return std::includes(
            h.holes.begin(), h.holes.end(),
            b.connections.begin(), b.connections.end()
        );
    }

    bool isCompatible(const Element& e1, const Element& e2)
    {
        if (e1.form == Form::ball && e2.form == Form::holey)
            return isCompatibleHB(e2, e1);
        if (e2.form == Form::ball && e1.form == Form::holey)
            return isCompatibleHB(e1, e2);
        return false;
    }
}


#endif // ELEMENTS_H
