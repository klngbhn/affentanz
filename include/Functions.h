#pragma once

#include <string>
#include <sstream>

/*
 * Contains some helper functions and definitions.
 */
namespace engine_functions {

/*
 * Converts int, float, double etc to string. Is equal to the to_string
 * function of the c++11 standard library, that isn't supported by
 * android ndk.
 */
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

/**
 * Frees memory of objects in a safe way.
 */
#define SAFE_DELETE(X) {if (X != NULL) {delete (X); X=NULL;}}


/**
 * Norms a value to an intervall
 */
#define NORM(value, valueMin, valueMax, resultMin, resultMax) (((value) - (valueMin)) * (((resultMax) - (resultMin)) / ((valueMax) - (valueMin))) + (resultMin))


/**
 * Clips a value to an intervall (if it's greater than min or max it will be min or max)
 */
#define CLIP(value, resultMin, resultMax) (((value) < (resultMin)) ? (resultMin) : ((value) > (resultMax)) ? (resultMax) : (value))

}
