// lector.h
#ifndef LECTOR_H
#define LECTOR_H

#include <string>
#include <vector>

class CSVReader {
public:
    // La definición de la clase como la tienes.
    struct Registro {
        std::string figura;
        std::string material;
        double r, g, b;
        double radio, d, v1x, v1y, v1z;
        double v2x, v2y, v2z;
        double v3x, v3y, v3z;

        void imprimir() const;
    };

    explicit CSVReader(const std::string& path);
    bool leerCSV();
    void imprimirRegistros() const;
    const std::vector<Registro>& obtenerRegistros() const;

private:
    std::vector<Registro> registros;
    std::string filePath;

    Registro procesarLinea(const std::string& line) const;
};

#endif // LECTOR_H
