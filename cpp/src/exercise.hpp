#ifndef EXERCISE_H
#define EXERCISE_H

#include <utility>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>

#include <details.hpp>
#include <elements.hpp>
#include <utils.hpp>
#include <json.hpp>
using json = nlohmann::json;


class MainField
{
private:
    short rowsNumber;
    short rowLenght;
    std::set<Cell> disabledCells;
    std::map<Cell, std::set<elements::Element>> occupiedCells;

    bool cellExists(const Cell& cell) const {
        return (
            !disabledCells.count(cell) &&
            0 <= cell.first && cell.first < rowsNumber &&
            0 <= cell.second && cell.second < rowLenght
        );
    };

public:
    MainField(short rowsNumber, short rowLenght, std::set<Cell> disabledCells)
        : rowsNumber(rowsNumber), rowLenght(rowLenght),
        disabledCells(disabledCells), occupiedCells() {};
    
    MainField(short rowsNumber, short rowLenght)
        : MainField(rowsNumber, rowLenght, {}) {};
    
    MainField(json config) {
        rowsNumber = config["rows"].get<short>();
        rowLenght = config["columns"].get<short>();
        disabledCells = config["disabled"];
        occupiedCells = {};
    };
    
    void insertValidDetail(details::Detail* detail, details::State state)
    {
        detail->setState(state);
        auto positions = detail->getCells();

        for (const auto& [part, cell]: positions)
        {
            if (!occupiedCells.count(cell))
                occupiedCells[cell] = {};
            occupiedCells[cell].insert(detail->getElement(part));
        };
    };

    bool isDetailFits(const details::Detail& detail, details::State state) const
    {
        auto positions = detail.getCells(state);
        for (const auto& [part, cell]: positions)
            if (!cellExists(cell))
                return false;
        for (const auto& [part, cell]: positions) {
            if (!occupiedCells.count(cell))
                continue;
            const auto *cellElements = &occupiedCells.at(cell);
            if (cellElements->size() > 1)
                return false;
            
            if (!elements::isCompatible(
                detail.getElement(part, state), *cellElements->begin()
            ))
                return false;
        }
        return true;
    };

    void removeDetail(details::Detail& detail)
    {
        if (!detail.isActive()) {
            return;
        }
        auto positions = detail.getCells();
        for (const auto& [part, cell] : positions)
        {
            assert (occupiedCells.count(cell) > 0);
            auto & cellElements = occupiedCells[cell];
            if (cellElements.size() == 1) {
                occupiedCells.erase(cell);
                continue;
            }
            for (auto & elem: cellElements)
                if (elem.form == detail.getElementForm(part)) {
                    cellElements.erase(elem);
                    break;
                }
        }
        detail.setFree();
    };
};


class Exercise
{
private:
    MainField mainField;
    std::vector<details::Detail> details;
    std::vector<short> startDetails;
    std::vector<details::State> solution;

public:
    Exercise(json config): mainField(config["field"]), startDetails(config["initial"])
    {
        json detailsConfigs = config["details"];
        std::vector<details::Detail> details1{};
        std::vector<details::State> solution1{};
        for (int i = 0; i < detailsConfigs.size(); ++i) {
            details1.emplace_back(details::Detail(detailsConfigs[i]["form"]));
            solution1.emplace_back(details::State(detailsConfigs[i]["state"]));
        }
        details = std::move(details1);
        solution = std::move(solution1);
        for (short i: startDetails)
            mainField.insertValidDetail(&details[i], solution[i]);
    };

    bool isDetailFits(short detailNumber, short row, short column, short rotation, bool side) {
        details::State state({row, column}, rotation, side);
        return mainField.isDetailFits(details[detailNumber], state);
    };

    void insertDetail(short detailNumber, short row, short column, short rotation, bool side) {
        details::State state({row, column}, rotation, side);
        mainField.insertValidDetail(&details[detailNumber], state);
    }

    void removeDetail(short detailNumber) {
        mainField.removeDetail(details[detailNumber]);
    }

    bool isComplited() const {
        for (const auto& d: details)
            if (!d.isActive())
                return false;
        for (int i = 0; i < details.size(); ++i)
            if (!details[i].isIn(solution[i]))
                return false;
        return true;
    };
};


Exercise makeExercise(const std::string& configPath) {
    return Exercise(json::parse(configPath));
}

#endif // EXERCISE_H
