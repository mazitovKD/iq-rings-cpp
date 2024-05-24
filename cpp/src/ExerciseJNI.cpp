#include <jni.h>
#include "details.hpp"
#include "elements.hpp"
#include "exercise.hpp"
#include "json.hpp"
#include "utils.hpp"

extern "C" {

JNIEXPORT jlong JNICALL Java_com_example_Exercise_createExercise(JNIEnv* env, jobject obj, jstring configPath) {
    const char* nativeString = env->GetStringUTFChars(configPath, 0);
    Exercise* exercise = new Exercise(nativeString);
    env->ReleaseStringUTFChars(configPath, nativeString);
    return reinterpret_cast<jlong>(exercise);
}

JNIEXPORT void JNICALL Java_com_example_Exercise_destroyExercise(JNIEnv* env, jobject obj, jlong exercisePtr) {
    Exercise* exercise = reinterpret_cast<Exercise*>(exercisePtr);
    delete exercise;
}

JNIEXPORT jboolean JNICALL Java_com_example_Exercise_isDetailFits(JNIEnv* env, jobject obj, jlong exercisePtr, jshort detailNumber, jshort row, jshort column, jshort rotation, jboolean side) {
    Exercise* exercise = reinterpret_cast<Exercise*>(exercisePtr);
    return exercise->isDetailFits(detailNumber, row, column, rotation, side);
}

JNIEXPORT void JNICALL Java_com_example_Exercise_insertDetail(JNIEnv* env, jobject obj, jlong exercisePtr, jshort detailNumber, jshort row, jshort column, jshort rotation, jboolean side) {
    Exercise* exercise = reinterpret_cast<Exercise*>(exercisePtr);
    exercise->insertDetail(detailNumber, row, column, rotation, side);
}

JNIEXPORT void JNICALL Java_com_example_Exercise_removeDetail(JNIEnv* env, jobject obj, jlong exercisePtr, jshort detailNumber) {
    Exercise* exercise = reinterpret_cast<Exercise*>(exercisePtr);
    exercise->removeDetail(detailNumber);
}

JNIEXPORT jboolean JNICALL Java_com_example_Exercise_isCompleted(JNIEnv* env, jobject obj, jlong exercisePtr) {
    Exercise* exercise = reinterpret_cast<Exercise*>(exercisePtr);
    return exercise->isComplited();
}

}
