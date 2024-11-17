#ifndef HEADER_H
#define HEADER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

//Cosas de C++ (diria que se usan mayormente en hittable_list.h)

using std::make_shared;
using std::shared_ptr;

//Constantes

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Funciones varias

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

//Retorna real aleatorio entre [0,1) (segunda implementacion del libro)
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

//Retorna real aleatorio entre [min,max)
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

//Cabeceras comunes

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif