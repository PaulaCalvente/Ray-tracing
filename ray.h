#ifndef RAY_H //Include guard, previene que el contenido sea incluido mas de una vez en el programa
#define RAY_H

#include "vec3.h"
//Clase del rayo, para trazar valores de pixeles
class ray {
  public:
    ray() {}
    //Constructor de rayo, recibe punto de origen y direccion
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const point3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    //Funcion que define rayo
    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;
    vec3 dir;
};

#endif //Fin include guard