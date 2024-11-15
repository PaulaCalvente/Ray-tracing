#ifndef COLOR_H //Include guard, previene que el contenido sea incluido mas de una vez en el programa
#define COLOR_H

#include "vec3.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    //Funcion para escribir colores
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //Convierte rango [0,1] en rango [0,255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    //Escribe en pixel color de componentes
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif //Fin include guard