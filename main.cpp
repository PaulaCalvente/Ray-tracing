#include "header.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "bvh.h"
#include "quad.h"

// Importaciones para la lectura de datos
#include "lector.h"
#include "lectorCamara.h"

// Función principal
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);   // Inicialización de MPI
    clock_t start, end; // Variables para medir el tiempo que tarda en ejecutarse el programa

    CSVReader lector("ruta");
    lectorCamara lectorCam("ruta_a_camara.csv");

    // Creación de lista de almacenamiento (mundo)
    hittable_list world;

    /////////////////////////////////////////////
    // Procesar los registros de objetos
    if (lector.leerCSV()) {
        const auto& registros = lector.obtenerRegistros();

        for (const auto& registro : registros) {
            std::shared_ptr<material> objMaterial;

            // Crear el material correspondiente
            if (registro.material == "lambertian") {
                objMaterial = std::make_shared<lambertian>(color(registro.r, registro.g, registro.b));
            }
            else if (registro.material == "metal") {
                objMaterial = std::make_shared<metal>(color(registro.r, registro.g, registro.b));
            }
            else if (registro.material == "d_light") {
                objMaterial = std::make_shared<diffuse_light>(color(registro.r, registro.g, registro.b));
            }
            else if (registro.material == "dielectric") {
                objMaterial = std::make_shared<dielectric>(registro.d);
            }
            else {
                std::cout << "Material no válido: " << registro.material << std::endl;
                continue;
            }

            // Crear la figura correspondiente
            if (registro.figura == "quad") {
                world.add(std::make_shared<quad>(
                    point3(registro.v1x, registro.v1y, registro.v1z),
                    vec3(registro.v2x, registro.v2y, registro.v2z),
                    vec3(registro.v3x, registro.v3y, registro.v3z),
                    objMaterial
                ));
            }
            else if (registro.figura == "trig") {
                world.add(std::make_shared<trig>(
                    point3(registro.v1x, registro.v1y, registro.v1z),
                    vec3(registro.v2x, registro.v2y, registro.v2z),
                    vec3(registro.v3x, registro.v3y, registro.v3z),
                    objMaterial
                ));
            }
            else if (registro.figura == "sphere") {
                world.add(std::make_shared<sphere>(
                    point3(registro.v1x, registro.v1y, registro.v1z),
                    registro.d,
                    objMaterial
                ));
            }
            else {
                std::cout << "Figura no válida: " << registro.figura << std::endl;
                continue;
            }
        }
    }

    /////////////////////////////////////////////
    // Configuración de la cámara
    if (lectorCam.leerCSV()) {
        const auto& registrosCam = lectorCam.obtenerRegistros();

        if (!registrosCam.empty()) {
            const auto& registroCamara = registrosCam[0]; // Suponiendo que solo hay un registro para la cámara

            // Asignar valores del registro al objeto `camera`
            camera cam;

            cam.aspect_ratio = registroCamara.aspect_ratio;
            cam.image_width = registroCamara.image_width; // Si necesitas calcular a partir de aspect_ratio
            cam.samples_per_pixel = registroCamara.samples_per_pixel;
            cam.max_depth = registroCamara.max_depth;
            cam.background = color(
                registroCamara.background1,
                registroCamara.background2,
                registroCamara.background3
            );
            cam.vfov = registroCamara.vfov;
            cam.lookfrom = point3(
                registroCamara.lookfrom1,
                registroCamara.lookfrom2,
                registroCamara.lookfrom3
            );
            cam.lookat = point3(
                registroCamara.lookat1,
                registroCamara.lookat2,
                registroCamara.lookat3
            );
            cam.vr = vec3(
                registroCamara.vr1,
                registroCamara.vr2,
                registroCamara.vr3
            );
            cam.defocus_angle = registroCamara.defocus_angle;
            double focus_dist = registroCamara.focus_dist;

            // Confirmar los valores asignados
            std::cout << "Cámara configurada con éxito:" << std::endl;
            std::cout << "Aspect Ratio: " << cam.aspect_ratio << ", VFOV: " << cam.vfov << std::endl;

            /////////////////////////////////////////////
            // Renderizar la escena
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el nodo que está ejecutando el proceso: Nodo master --> 0
            if (rank == 0) { // Si es el nodo master
                start = clock(); // Comienza reloj
            }

            cam.render(world);

            if (rank == 0) {
                end = clock();
                double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
                std::clog << "Time taken by program: " << time_taken << " sec " << std::endl // Muestra el tiempo que ha tardado el programa
                    << std::flush;
            }
        }
        else {
            std::cerr << "El archivo de configuración de la cámara está vacío o no contiene registros válidos." << std::endl;
        }
    }
    else {
        std::cerr << "No se pudo leer el archivo de configuración de la cámara." << std::endl;
    }

    MPI_Finalize(); // Finalización de MPI
    return 0;
}
