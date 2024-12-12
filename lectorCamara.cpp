#include "lectorCamara.h"
#include <iostream>
#include <fstream>
#include <sstream>

lectorCamara::lectorCamara(const std::string& path) : filePath(path) {}

bool lectorCamara::leerCSV() {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return false;
    }

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) { // Saltar la primera línea
            firstLine = false;
            continue;
        }

        registros.push_back(procesarLinea(line));
    }

    file.close();
    return true;
}

lectorCamara::Registro lectorCamara::procesarLinea(const std::string& line) const {
    Registro registro;
    std::istringstream lineStream(line);
    std::string temp;

    std::getline(lineStream, temp, ','); registro.aspect_ratio = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.image_width = std::stoi(temp);
    std::getline(lineStream, temp, ','); registro.samples_per_pixel = std::stoi(temp);
    std::getline(lineStream, temp, ','); registro.max_depth = std::stoi(temp);

    std::getline(lineStream, temp, ','); registro.background1 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.background2 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.background3 = std::stod(temp);

    std::getline(lineStream, temp, ','); registro.vfov = std::stoi(temp);

    std::getline(lineStream, temp, ','); registro.lookfrom1 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.lookfrom2 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.lookfrom3 = std::stod(temp);

    std::getline(lineStream, temp, ','); registro.lookat1 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.lookat2 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.lookat3 = std::stod(temp);

    std::getline(lineStream, temp, ','); registro.vr1 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.vr2 = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.vr3 = std::stod(temp);

    std::getline(lineStream, temp, ','); registro.defocus_angle = std::stod(temp);
    std::getline(lineStream, temp, ','); registro.focus_dist = std::stod(temp);

    return registro;
}

const std::vector<lectorCamara::Registro>& lectorCamara::obtenerRegistros() const {
    return registros;
}

void lectorCamara::Registro::imprimir() const {
    std::cout << "Aspect Ratio: " << aspect_ratio
        << ", Image Width: " << image_width
        << ", Samples per Pixel: " << samples_per_pixel
        << ", Max Depth: " << max_depth
        << ", Background: (" << background1 << ", " << background2 << ", " << background3 << ")"
        << ", VFOV: " << vfov
        << ", LookFrom: (" << lookfrom1 << ", " << lookfrom2 << ", " << lookfrom3 << ")"
        << ", LookAt: (" << lookat1 << ", " << lookat2 << ", " << lookat3 << ")"
        << ", VR: (" << vr1 << ", " << vr2 << ", " << vr3 << ")"
        << ", Defocus Angle: " << defocus_angle
        << ", Focus Distance: " << focus_dist
        << std::endl;
}
