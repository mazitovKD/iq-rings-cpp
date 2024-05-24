fun main() {
    val exercise = Exercise("path/to/config.json")
    val fits = exercise.isDetailFits(1, 2, 3, 0, true)
    println("Detail fits: $fits")
}