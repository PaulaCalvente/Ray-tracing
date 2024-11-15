#ifndef HEADER_H
#define HEADER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


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

//Cabeceras comunes

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif