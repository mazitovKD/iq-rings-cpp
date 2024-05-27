#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <utility>
#include <array>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <string>
#include <fstream>
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
    };

    short turn(short direction, short rotation) {
        return (6 + direction + rotation) % N_DIRECTIONS;
    }

    bool isCompatible(const Element& e1, const Element& e2)
    {
        const Element *insertElement, *holeyElement;
        if (e1.form == Form::ball && e2.form == Form::holey)
        {
            insertElement = &e1;
            holeyElement = &e2;
        }
        else if (e2.form == Form::ball && e1.form == Form::holey)
        {
            insertElement = &e2;
            holeyElement = &e1;
        }
        else
            return false;
        bool inc = std::includes(
            holeyElement->holes.begin(), holeyElement->holes.end(),
            insertElement->connections.begin(), insertElement->connections.end()
        );
        return inc;
    }
}


#endif // ELEMENTS_H
