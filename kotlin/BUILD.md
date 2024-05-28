## Сборка Native модуля
1. Добавьте файл [kotlin/CMakeLists.txt](CMakeLists.txt) в папку `app/` 
2. Создайте папку `app/src/main/cpp` (рядом с папкой `java``) и перетащите туда 
содержимое папки [native_engine/src/](../native_engine/src/). 
Если основной пакет отличен от `com.katysh.iqrings`, замените в файле [native_engine/src/ExerciseJNI.cpp](../native_engine/src/ExerciseJNI.cpp) 
все вхождения `com_katysh_iqrings` на аналогичное (пакет в котором точки заменены на нижние подчеркивания и все буквы в нижнем регистре)  
3. Добавьте файл `Exercise.kt` в папку `app/src/main/java/com.{project.name}` и замените `com.katysh.iqrings` в 1 строке на актуальный пакет (если он изменился)
4. В вашем Gradle файле `app/build.gradle.kts` добавьте строки из файла [kotlin/build.gradle](../kotlin/build.gradle) и запустите синхронизацию
5. Запустите сборку проекта. Android Studio автоматически скомпилирует C++ библиотеку.
