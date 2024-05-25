#ifndef DETAILS_H
#define DETAILS_H

#include <string>
#include <map>

#include <elements.hpp>
#include <utils.hpp>
#include <json.hpp>
using json = nlohmann::json;


namespace details
{
    enum class Form { acute = 1, obtuse, straight };

    const std::map<std::string, Form> FormsMap{
        {"acute", Form::acute},
        {"obtuse", Form::obtuse},
        {"straight", Form::straight}
    };

    enum class Part { left, center, right };

    const std::map<std::string, Part> PartMap{
        {"left", Part::left},
        {"center", Part::center},
        {"right", Part::right}
    };


    class State
    {
    public:
        Cell position;
        short rotation;
        bool isFrontSide;
        State(Cell position, short rotation, bool isFrontSide)
            : position(position), rotation(rotation), isFrontSide(isFrontSide) {};
        State() : State(OUT_OF_BOUNDS, 0, true) {};
        State(json config)
            : State(config["position"], config["rotation"], config["isFrontSide"]) {};
        
        bool operator==(State const& rhs) const {
            return (
                (position == rhs.position) &&
                (rotation == rhs.rotation) &&
                (isFrontSide == rhs.isFrontSide)
            );
        };
    };


    class Detail
    {
    protected:
        std::map<Part, elements::Element> elementsMap;
        Form form;
    public:
        State state;

        Detail(json config) : state()
        {
            form = FormsMap.at(config["angle"]);
            std::map<Part, std::set<short>> connectionsMap{
                {Part::left, {3}}
            };
            switch (form)
            {
            case Form::acute:
                connectionsMap.insert({Part::center, {0, 1}});
                connectionsMap.insert({Part::right, {4}});
                break;
            case Form::obtuse:
                connectionsMap.insert({Part::center, {0, 2}});
                connectionsMap.insert({Part::right, {5}});
                break;
            case Form::straight:
                connectionsMap.insert({Part::center, {0, 3}});
                connectionsMap.insert({Part::right, {0}});
                break;
            
            }
            for (const auto& item : config["elements"].items())
            {
                elements::Form type = elements::formsMap.at(item.value()["type"]);
                Part part = PartMap.at(item.key());
                std::set<short> holes{};
                if (type == elements::Form::holey)
                    holes = item.value()["holes"].get<std::set<short>>();            
                elementsMap.insert({part, elements::Element(
                    type, connectionsMap[part], holes
                )}); 
            }
        };

        bool isIn(State s) const {
            return state == s;
        }

        bool isActive() const {
            return state.position != OUT_OF_BOUNDS;
        };

        bool setFree() {
            state.position = OUT_OF_BOUNDS;
            return false;
        };

        bool setState(State newState) {
            state = newState;
            return true;
        };

        std::map<Part, Cell> getCells(State newState) const {
            std::map<Part, Cell> positions{
                {Part::center, newState.position}};
            positions.insert({Part::left, turn(newState.position, newState.rotation)});
            positions.insert({Part::right, turn(newState.position, turn(
                newState.rotation,
                sgn(newState.isFrontSide - 0.5) * (int)form
            ))});
            return positions;
        };

        std::map<Part, Cell> getCells() const {
            return getCells(state);
        }

        elements::Element getElement(Part part, State newState) const {
            std::set<short> connections{};
            for (auto c: elementsMap.at(part).connections)
                connections.insert(turn(sgn(newState.isFrontSide - 0.5) * c, newState.rotation));

            std::set<short> holes{};
            for (auto c: elementsMap.at(part).holes)
                holes.insert(turn(sgn(newState.isFrontSide - 0.5) * c, newState.rotation));
            return elements::Element(elementsMap.at(part).form, connections, holes);
        };

        elements::Element getElement(Part part) const {
            return getElement(part, state);
        };

        elements::Form getElementForm(Part part) const {
            return elementsMap.at(part).form;
        };
    };
}

#endif // DETAILS_H
