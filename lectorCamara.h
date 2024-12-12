#ifndef LECTORCAMARA_H
#define LECTORCAMARA_H

#include <vector>
#include <string>

class lectorCamara {
public:
    // Estructura para representar un registro del CSV
    struct Registro {
        double aspect_ratio;
        int image_width;
        int samples_per_pixel;
        int max_depth;
        double background1, background2, background3;
        int vfov;
        double lookfrom1, lookfrom2, lookfrom3;
        double lookat1, lookat2, lookat3;
        double vr1, vr2, vr3;
        double defocus_angle, focus_dist;

        void imprimir() const;
    };

    // Constructor
    explicit lectorCamara(const std::string& path);

    // Métodos públicos
    bool leerCSV();
    const std::vector<Registro>& obtenerRegistros() const;

private:
    // Métodos privados
    Registro procesarLinea(const std::string& line) const;

    // Atributos privados
    std::vector<Registro> registros;
    std::string filePath;
};

#endif // LECTORCAMARA_H
