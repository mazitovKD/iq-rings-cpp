#include <iostream>
#include <string>

#include <exercise.hpp>

void paste(Exercise &e, short n, short y, short x, short r, bool s) {
    bool fits = e.isDetailFits(n, y, x, r, s);
    std::cout << n << " " << y << " " << x << " " << r << " " << s << "";
    std::cout << n << ":\t" << ((fits) ? "" : " NO") << " FITS ";
    if (fits) {
        e.insertDetail(n, y, x, r, s);
        bool comp = e.isComplited();
        std::cout << "\t" << ((comp)? "": "NO ") << "COMPLITED\n";
    }
    else {
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "--------------------------------------\n";
}

void remove(Exercise &e, short n) {
    std::cout << n << ": removing\n";
    e.removeDetail(n);
}


int main() {
    std::string cfgPath = "../configs/exercise_junior25.json";
    Exercise exercise(cfgPath);

    std::cout << "--------------------------------------\n";

    paste(exercise, 1, 0, 4, 3, false);
    paste(exercise, 8, 3, 4, 4, true); // false
    paste(exercise, 1, 2, 5, 1, true); // false
    paste(exercise, 3, 0, 5, 3, true); // false
    paste(exercise, 3, 2, 5, 3, false);
    paste(exercise, 6, 1, 2, 0, true);
    paste(exercise, 8, 0, 1, 0, true);

    remove(exercise, 1);
    paste(exercise, 1, 2, 5, 1, true); // false
    paste(exercise, 3, 0, 5, 3, true); // false

    paste(exercise, 1, 0, 4, 3, false);
    return 0;
};