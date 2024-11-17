#ifndef COLOR_H //Include guard, previene que el contenido sea incluido mas de una vez en el programa
#define COLOR_H

#include "vec3.h"
#include "interval.h"

using color = vec3;
//Funcion para linealizar valor gamma
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}
//Funcion para trazar color
void write_color(std::ostream& out, const color& pixel_color) {
    //Funcion para escribir colores
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Aplica funcion lineal a gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //Convierte rango [0,1] en rango [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    //Escribe color de pixel
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif //Fin include guard