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
#include <queue>
#include <iostream>
#include <fstream>


#include <details.hpp>
#include <elements.hpp>
#include <utils.hpp>
#include <json.hpp>
using json = nlohmann::json;

// #define GENERATION 1


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

    short sise() const {
        return rowsNumber * rowLenght - disabledCells.size();
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

#ifdef GENERATION
    std::set<Cell> getOccupiedCells() const {
        std::set<Cell> result{};
        for (const auto& [cell, elems] : occupiedCells) {
            if (elems.size() == 2)
            {
                result.insert(cell);
                continue;
            }
            const auto& elem = elems.begin();
            if (elem->form == elements::Form::solid)
            {
                result.insert(cell);
                continue;
            }
            if ((elem->form == elements::Form::holey) && (elem->holes.size() == 1)) {
                short h = *elem->holes.begin(), y = cell.first, x = cell.second;
                if (
                    ((y == 0) && (h > 3)) ||
                    ((y == rowsNumber - 1) && (turn(h, -1) < 2))
                )
                {
                    result.insert(cell);
                    continue;
                }
                if (
                    ((x == 0) && !(y % 2) && (turn(h, 1) > 2)) ||
                    ((x == 0) &&  (y % 2) && (h == 3)) ||
                    ((x == rowLenght - 1) &&  (y % 2) && (turn(h, 1) < 3)) ||
                    ((x == rowLenght - 1) && !(y % 2) && (h == 0))
                )
                {
                    result.insert(cell);
                    continue;
                }
            }
        }
        return result;
    };

    std::set<Cell> getVacantCells() const {
        std::set<Cell> result{};
        for (const auto& [cell, elems] : occupiedCells) {
            if (elems.size() == 2)
                continue;
            
            const auto& elem = elems.begin();
            if (elem->form == elements::Form::solid)
                continue;
            if ((elem->form == elements::Form::holey) && (elem->holes.size() == 1)) {
                short h = *elem->holes.begin(), y = cell.first, x = cell.second;
                if (
                    ((y == 0) && (h > 3)) ||
                    ((y == rowsNumber - 1) && (turn(h, -1) < 2))
                )
                    continue;
                if (
                    ((x == 0) && !(y % 2) && (turn(h, 1) > 2)) ||
                    ((x == 0) &&  (y % 2) && (h == 3)) ||
                    ((x == rowLenght - 1) &&  (y % 2) && (turn(h, 1) < 3)) ||
                    ((x == rowLenght - 1) && !(y % 2) && (h == 0))
                )
                    continue;
            }
            result.insert(cell);
        }
        return result;
    };

    std::set<Cell> tryInsert(details::Detail &detail, details::State state) {
        auto positions = detail.getCells(state);
        // check that is posible
        for (const auto& [part, cell]: positions)
            if (!cellExists(cell))
                return {};
        for (const auto& [part, cell]: positions) {
            if (!occupiedCells.count(cell))
                continue;
            const auto *cellElements = &occupiedCells.at(cell);
            if (cellElements->size() > 1)
                return {};
            
            if (!elements::isCompatible(
                detail.getElement(part, state), *cellElements->begin()
            ))
                return {};
        }

        // update Field
        detail.setState(state);
        for (const auto& [part, cell]: positions)
            occupiedCells[cell].insert(detail.getElement(part));
        return {
            positions[details::Part::left],
            positions[details::Part::center],
            positions[details::Part::right]
        };        
    }

    std::set<Cell> tryRemove(details::Detail& detail)
    {
        if (!detail.isActive()) {
            return {};
        }
        auto positions = detail.getCells();
        for (const auto& [part, cell] : positions)
        {
            auto & cellElements = occupiedCells[cell];
            for (auto & elem: cellElements)
                if (elem.form == detail.getElementForm(part)) {
                    cellElements.erase(elem);
                    break;
                }
            if (cellElements.empty())
                occupiedCells.erase(cell);
        }
        detail.setFree();
        return {
            positions[details::Part::left],
            positions[details::Part::center],
            positions[details::Part::right]
        };
    };
#else
    
    void insertValidDetail(details::Detail* detail, details::State state)
    {
        detail->setState(state);
        auto positions = detail->getCells();

        for (const auto& [part, cell]: positions)
            occupiedCells[cell].insert(detail->getElement(part));
    };

    void removeDetail(details::Detail& detail)
    {
        if (!detail.isActive()) {
            return;
        }
        auto positions = detail.getCells();
        for (const auto& [part, cell] : positions)
        {
            auto & cellElements = occupiedCells[cell];
            for (auto & elem: cellElements)
                if (elem.form == detail.getElementForm(part)) {
                    cellElements.erase(elem);
                    break;
                }
            if (cellElements.empty())
                occupiedCells.erase(cell);
        }
        detail.setFree();
    };
#endif
};


class Exercise
{
private:
    MainField mainField;
    std::vector<details::Detail> details;
#ifndef GENERATION
    std::vector<short> startDetails;
    std::vector<details::State> solution;
#else
    // `Cell` -> `Detail` number
    std::map<Cell, std::set<short>> occupiedCells;

    std::set<short> getConnectedDetails(short detailNumber) const {
        if (!details.at(detailNumber).isActive())
            return {};
        std::map<details::Part, Cell> cells = details.at(detailNumber).getCells();
        std::set<short> connectedDetails{};
        for (const auto& [part, cell] : cells) 
            for (auto n: occupiedCells.at(cell))
                connectedDetails.insert(n);
        connectedDetails.erase(detailNumber);
        return connectedDetails;
    };

    std::set<short> getComponent(short detailNumber) const {
        std::set<short> component;  // множество для хранения номеров деталей в одной компоненте связности
        std::queue<short> toVisit;  // очередь для BFS
        std::set<short> visited;    // множество для отслеживания посещенных деталей

        toVisit.push(detailNumber);  // начинаем с переданной детали
        visited.insert(detailNumber); // и сразу же ее помечаем как посещенную

        while (!toVisit.empty()) {
            short current = toVisit.front();
            toVisit.pop();
            component.insert(current);

            std::set<short> connectedDetails = getConnectedDetails(current);

            for (short neighbor : connectedDetails) {
                if (visited.find(neighbor) == visited.end()) { // если еще не посещали эту деталь
                    toVisit.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }
        return component;
    }
#endif

public:

    short activeCount() const {
        short result = 0; 
        for (const auto& d: details)
            result += d.isActive();
        return result;
    };

#ifdef GENERATION
    Exercise(const std::string& configStr)
        : mainField(json::parse(configStr)["field"]), details()
    {
        json detailsConfigs = json::parse(configStr)["details"];
        for (int i = 0; i < detailsConfigs.size(); ++i) 
            details.emplace_back(details::Detail(detailsConfigs[i]["form"]));
    };

    bool tryInsert(short detailNumber, short row, short column, short rotation, bool side) {
        details::State state({row, column}, rotation, side);
        auto cells = mainField.tryInsert(details[detailNumber], state);
        if (!cells.size())
            return false;
        for (const auto& cell: cells)
            occupiedCells[cell].insert(detailNumber);
        return true;
    };
#else

    Exercise(const std::string& configStr)
        : mainField(json::parse(configStr)["field"]), details(),
        startDetails(json::parse(configStr)["initial"]), solution()
    {
        json detailsConfigs = json::parse(configStr)["details"];
        for (int i = 0; i < detailsConfigs.size(); ++i) {
            details.emplace_back(details::Detail(detailsConfigs[i]["form"]));

            solution.emplace_back(details::State(detailsConfigs[i]["state"]));
        }
        for (short i: startDetails)
            mainField.insertValidDetail(&details[i], solution[i]);
    };

    Exercise(json cfg) : mainField(cfg["field"]), details(),
        startDetails(cfg["initial"]), solution() 
    {
        json detailsConfigs = cfg["details"];
        for (int i = 0; i < detailsConfigs.size(); ++i) {
            details.emplace_back(details::Detail(detailsConfigs[i]["form"]));

            solution.emplace_back(details::State(detailsConfigs[i]["state"]));
        }
        for (short i: startDetails)
            mainField.insertValidDetail(&details[i], solution[i]);
    };

    void insertDetail(short detailNumber, short row, short column, short rotation, bool side) {
        details::State state({row, column}, rotation, side);
        mainField.insertValidDetail(&details[detailNumber], state);
    };
#endif

    bool isDetailFits(short detailNumber, short row, short column, short rotation, bool side) const {
        details::State state({row, column}, rotation, side);
        return mainField.isDetailFits(details[detailNumber], state);
    };

    void removeDetail(short detailNumber) {
#ifdef GENERATION
        auto cells = mainField.tryRemove(details[detailNumber]);
        if (!cells.size())
            return;
        for (const auto& cell: cells) {
            occupiedCells[cell].erase(detailNumber);
            if (occupiedCells[cell].empty())
                occupiedCells.erase(cell);
        }
#else
        mainField.removeDetail(details[detailNumber]);
#endif
    };

    bool allActive() const {
        return activeCount() == details.size();
    };

#ifdef GENERATION

    std::set<Cell> getOccupiedCells() const {
        return mainField.getOccupiedCells();
    };

    bool isConnected() const {
        if (!allActive())
            return false;
        return isDetailConnected(0);
    };

    bool isDetailConnected(short detailNumber) const {
        return getComponent(detailNumber).size() == activeCount();
    };

    std::set<Cell> getVacantCells() const {
        return mainField.getVacantCells();
    };
#else

    bool isComplited() const {
        if (!allActive())
            return false;
        for (int i = 0; i < details.size(); ++i)
            if (!details[i].isIn(solution[i]))
                return false;
        return true;
    };
#endif
};

#endif // EXERCISE_H
