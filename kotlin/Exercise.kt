package com.example

class Exercise(configPath: String) {

    private var nativePtr: Long = 0

    init {
        System.loadLibrary("iq_rings")
        nativePtr = createExercise(configPath)
    }

    @Throws(Throwable::class)
    protected fun finalize() {
        destroyExercise(nativePtr)
    }
    private external fun createExercise(configStr: String): Long
    private external fun destroyExercise(ptr: Long)
    private external fun isDetailFits(ptr: Long, detailNumber: Short, row: Short, column: Short, rotation: Short, side: Boolean): Boolean
    private external fun insertDetail(ptr: Long, detailNumber: Short, row: Short, column: Short, rotation: Short, side: Boolean)
    private external fun removeDetail(ptr: Long, detailNumber: Short)
    private external fun isCompleted(ptr: Long): Boolean

    fun isDetailFits(detailNumber: Short, row: Short, column: Short, rotation: Short, side: Boolean): Boolean {
        return isDetailFits(nativePtr, detailNumber, row, column, rotation, side)
    }

    fun insertDetail(detailNumber: Short, row: Short, column: Short, rotation: Short, side: Boolean) {
        insertDetail(nativePtr, detailNumber, row, column, rotation, side)
    }

    fun removeDetail(detailNumber: Short) {
        removeDetail(nativePtr, detailNumber)
    }

    fun isCompleted(): Boolean {
        return isCompleted(nativePtr)
    }
}
