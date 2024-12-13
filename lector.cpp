// lector.cpp
#include "lector.h"
#include <fstream>
#include <sstream>
#include <iostream>

CSVReader::CSVReader(const std::string& path) : filePath(path) {}

bool CSVReader::leerCSV() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return false;
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        registros.push_back(procesarLinea(line)); // Procesar y almacenar la línea
    }

    file.close();
    return true;
}


void CSVReader::Registro::imprimir() const {
    std::cout << "Figura: " << figura << ", Material: " << material
        << ", R: " << r << ", G: " << g << ", B: " << b << ", D: " << d
        << ", V1(" << v1x << ", " << v1y << ", " << v1z << ")"
        << ", V2(" << v2x << ", " << v2y << ", " << v2z << ")"
        << ", V3(" << v3x << ", " << v3y << ", " << v3z << ")"
        << std::endl;
}

void CSVReader::imprimirRegistros() const {
    for (const auto& registro : registros) {
        registro.imprimir();
    }
}

const std::vector<CSVReader::Registro>& CSVReader::obtenerRegistros() const {
    return registros;
}

CSVReader::Registro CSVReader::procesarLinea(const std::string& line) const {
    Registro registro;
    std::istringstream lineStream(line);
    std::string temp;

    std::getline(lineStream, registro.figura, ',');
    std::getline(lineStream, registro.material, ',');
    std::getline(lineStream, temp, ','); registro.r = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.g = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.b = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.d = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v1x = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v1y = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v1z = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v2x = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v2y = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v2z = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v3x = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v3y = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.v3z = std::stod(temp);

    return registro;
}
