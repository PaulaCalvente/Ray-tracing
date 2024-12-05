#include "header.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "bvh.h"
#include "quad.h"
#include <mpi.h>

//Funcion principal, donde se añaden los elementos al mundo, los volumenes de optimizacion bvh, se ajustan ciertos parametros y se ejecuta la funcion de renderizado
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);   // Inicialización de MPI
    clock_t start, end; // Variables para medir el tiempo que tarda en ejecutarse el programa

    // Creación de lista de almacenamiento (mundo)
    hittable_list world;

    // Creación de materiales a usar
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<d_light>(color(15, 15, 15));

    // Adición de elementos al mundo junto a sus materiales
    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    // Creación de la cámara y configuración de parámetros
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 400;
    cam.max_depth = 10;
    cam.background = color(0, 0, 0);
    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vr = vec3(0, 1, 0);
    cam.defocus_angle = 0;
    double focus_dist = 10;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el nodo que está ejecutando el proceso: Nodo master --> 0
    if (rank == 0) { // Si es el nodo master
        start = clock(); // Comienza reloj
    }

    // Función de renderizado
    cam.render(world);

    if (rank == 0) {
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::clog << "Time taken by program: " << time_taken << " sec " << std::endl // Muestra el tiempo que ha tardado el programa
            << std::flush;
    }

    MPI_Finalize(); // Finalización de MPI
    return 0;
}