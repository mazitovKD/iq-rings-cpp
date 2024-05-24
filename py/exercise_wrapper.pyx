# distutils: language = c++
# distutils: sources = cpp/src/details.cpp cpp/src/elements.cpp cpp/src/exercise.hpp cpp/src/json.cpp cpp/src/utils.cpp

cdef extern from "exercise.hpp":
    cdef cppclass Exercise:
        Exercise(string configPath)
        bool isDetailFits(short detailNumber, short row, short column, short rotation, bool side)
        void insertDetail(short detailNumber, short row, short column, short rotation, bool side)
        void removeDetail(short detailNumber)
        bool isComplited()

cdef class PyExercise:
    cdef Exercise* thisptr      # Hold a C++ instance

    def __cinit__(self, configPath):
        self.thisptr = new Exercise(configPath.encode('utf-8'))

    def __dealloc__(self):
        del self.thisptr

    def is_detail_fits(self, detailNumber, row, column, rotation, side):
        return self.thisptr.isDetailFits(detailNumber, row, column, rotation, side)

    def insert_detail(self, detailNumber, row, column, rotation, side):
        self.thisptr.insertDetail(detailNumber, row, column, rotation, side)

    def remove_detail(self, detailNumber):
        self.thisptr.removeDetail(detailNumber)

    def is_completed(self):
        return self.thisptr.isComplited()
