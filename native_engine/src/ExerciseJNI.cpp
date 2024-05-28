#include <jni.h>
#include <string>
#include "exercise.hpp"

// Helper function to convert jstring to std::string
std::string jstringToString(JNIEnv* env, jstring jStr) {
    const char* chars = env->GetStringUTFChars(jStr, nullptr);
    std::string str(chars);
    env->ReleaseStringUTFChars(jStr, chars);
    return str;
}

extern "C" {

JNIEXPORT jlong JNICALL Java_com_katysh_iqrings_Exercise_createExercise(JNIEnv* env, jobject obj, jstring configStr) {
    std::string config = jstringToString(env, configStr);
    Exercise* exercise = new Exercise(config);
    return reinterpret_cast<jlong>(exercise);
}

JNIEXPORT void JNICALL Java_com_katysh_iqrings_Exercise_destroyExercise(JNIEnv* env, jobject obj, jlong ptr) {
    Exercise* exercise = reinterpret_cast<Exercise*>(ptr);
    delete exercise;
}

JNIEXPORT jboolean JNICALL Java_com_katysh_iqrings_Exercise_isDetailFits(JNIEnv* env, jobject obj, jlong ptr, jshort detailNumber, jshort row, jshort column, jshort rotation, jboolean side) {
    Exercise* exercise = reinterpret_cast<Exercise*>(ptr);
    return exercise->isDetailFits(detailNumber, row, column, rotation, side);
}

JNIEXPORT void JNICALL Java_com_katysh_iqrings_Exercise_insertDetail(JNIEnv* env, jobject obj, jlong ptr, jshort detailNumber, jshort row, jshort column, jshort rotation, jboolean side) {
    Exercise* exercise = reinterpret_cast<Exercise*>(ptr);
    exercise->insertDetail(detailNumber, row, column, rotation, side);
}

JNIEXPORT void JNICALL Java_com_katysh_iqrings_Exercise_removeDetail(JNIEnv* env, jobject obj, jlong ptr, jshort detailNumber) {
    Exercise* exercise = reinterpret_cast<Exercise*>(ptr);
    exercise->removeDetail(detailNumber);
}

JNIEXPORT jboolean JNICALL Java_com_katysh_iqrings_Exercise_isCompleted(JNIEnv* env, jobject obj, jlong ptr) {
    Exercise* exercise = reinterpret_cast<Exercise*>(ptr);
    return exercise->isComplited();
}

}
