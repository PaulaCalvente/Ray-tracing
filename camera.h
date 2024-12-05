#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <omp.h>
#include <chrono>
#include <mpi.h>

// Clase cámara, la cual incluye el renderizado, los valores de la cámara y el coloreado
class camera {
public:
    // Parámetros de la cámara
    double aspect_ratio = 1.0;          // Ratio de la imagen
    int image_width = 100;              // Longitud de imagen renderizada
    int samples_per_pixel = 10;         // Número de rayos aleatorios por pixel
    int max_depth = 10;                 // Rebotes máximos por rayo
    color background;                   // Color de fondo de la escena

    double vfov = 90;                   // Ángulo vertical (fov)
    point3 lookfrom = point3(0, 0, 0);  // Punto central de cámara
    point3 lookat = point3(0, 0, -1);   // Punto objetivo de cámara
    vec3 vr = vec3(0, 1, 0);            // Vector de referencia de la cámara

    double defocus_angle = 0;           // Ángulo de variación por pixel
    double focus_dist = 10;             // Distancia entre cámara y punto enfocado

    // Reducción de color para sumar colores
#pragma omp declare reduction(+: color : omp_out = omp_out + omp_in) initializer(omp_priv = color(0, 0, 0))

// Función de renderizado
    void render(const hittable& world) {
        initialize();

        int num_threads = omp_get_max_threads(); // Ajusta el número de hilos
        std::clog << "Numero de hilos: " << num_threads << std::endl;

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        // Variables para medir el tiempo
        auto start_time = std::chrono::high_resolution_clock::now(); // Tiempo inicial

        // Inicialización de MPI
        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int rows_per_process = image_height / size; // Filas por proceso    
        int extra_row = image_height % size; // Filas adicionales   
        int start_row = rank * rows_per_process; // Fila de inicio
        int current_rows = rows_per_process + (rank == size - 1 ? extra_row : 0); // Filas actuales: Si el nodo es el último se le asignarán las filas extras

        // Declarar buffer local a cada proceso --> Almacena los colores calculados para el bloque asignado
        std::vector<color> pixel_colors(image_width * current_rows);
        // Buffer para recibir las filas asignadas a cada proceso
        std::vector<int> recv_counts(size);
        std::vector<int> displs(size);
        // Buffer local al nodo maestro que contiene la imagen completa al final --> En MPI_Gather se copian los resultados de cada nodo a este buffer
        std::vector<color> final_image;

        for (int i = 0; i < size; ++i) {
            recv_counts[i] = (i == size - 1) ? (rows_per_process + extra_row) * image_width : rows_per_process * image_width;
            displs[i] = i * rows_per_process * image_width;
        }

        // Definir el tipo de dato MPI para color
        MPI_Datatype MPI_COLOR;
        MPI_Type_contiguous(3, MPI_DOUBLE, &MPI_COLOR);
        MPI_Type_commit(&MPI_COLOR);

        // Scatter para distribuir las filas entre los procesos
        if (rank == 0) {
            final_image.resize(image_width * image_height); // Tamaño total de la imagen
        }
        MPI_Scatterv(rank == 0 ? final_image.data() : NULL, recv_counts.data(), displs.data(), MPI_COLOR, pixel_colors.data(), recv_counts[rank], MPI_COLOR, 0, MPI_COMM_WORLD);

        // Paralelizamos el recorrido de las filas
#pragma omp parallel for schedule(dynamic) num_threads(num_threads)
        for (int j = start_row; j < start_row + current_rows; j++) {
            auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();

            int estimated_total_time = (elapsed_seconds / (j - start_row + 1)) * current_rows;
            int remaining_time = estimated_total_time - elapsed_seconds;

            // Progreso y estimación
#pragma omp critical
            {
                std::clog << "\rScanlines remaining: " << start_row + current_rows - j
                    << " | Elapsed time: " << elapsed_seconds << "s"
                    << " | Remaining time: " << remaining_time << "s"
                    << std::flush;
            }

            // Cada hilo trabaja en su propia fila
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                // Paralelizamos la generación de rayos para este píxel
#pragma omp parallel for reduction(+:pixel_color) num_threads(num_threads)
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                pixel_colors[(j - start_row) * image_width + i] = pixel_color / samples_per_pixel;
            }
        }

        // Recolectar los resultados en el nodo maestro
        if (rank == 0) {
            final_image.resize(image_width * image_height);
        }

        MPI_Gatherv(pixel_colors.data(), recv_counts[rank], MPI_COLOR, final_image.data(), recv_counts.data(), displs.data(), MPI_COLOR, 0, MPI_COMM_WORLD);

        // Escribir la imagen final en el nodo maestro
        if (rank == 0) {
            for (int j = 0; j < image_height; ++j) {
                for (int i = 0; i < image_width; ++i) {
                    write_vector_color(std::cout, final_image[j * image_width + i], i, j);
                }
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now(); // Tiempo final
        int total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
        std::clog << "\nDone. Total time: " << total_time << " seconds.\n";
    }
private:
    int    image_height;        //altura de imagen renderizada
    point3 center;              //Centro de camara
    point3 pixel00_loc;         //Localizacion de pixel (0,0) (del viewport)
    vec3   pixel_delta_u;       //Desviacion de pixel a la derecha
    vec3   pixel_delta_v;       //Desviacion de pixel hacia abajo
    vec3   u, v, w;             //Vectores base de la camara
    vec3   defocus_disk_u;      //Radio horizontal de disco de desenfoque
    vec3   defocus_disk_v;      //Radio vertical de disco de desenfoque

    //Funcion que inicializa el viewport y la camara
    //El viewport es un plano de referencia que se usa para determinar la direccion de los vectores segun el pixel
    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        //Dimensiones del viewport
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        //Vectores base unitarios de camara
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vr, w));
        v = cross(w, u);

        //Vectores de direccion de pixeles del viewport
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        //Distancia entre pixeles del viewport (no son realmente pixeles, pero para que se entienda)
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //Inicializacion del viewport
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        //Calculo de vectores base de disco de desenfoque
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    //Funcion que retorna rayo correspondiente a pixel (i,j)
    //No se ha implementado antialiasing
    ray get_ray(int i, int j) const {

        auto pixel_sample = pixel00_loc
            + (i * pixel_delta_u)
            + (j * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    //Funcion que retorna un punto aleatorio en el disco de desenfoque de la camara
    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    //Funcion que determina color de rayo mediante recursividad, de ahi que se use el parametro depth (profundidad)
    color ray_color(const ray& r, int depth, const hittable& world) const {
        //Si se excede limite de rebotes, no dar luz
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;

        //Si no se golpea nada, retornar color de fondo
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        //Valores de la  luz correspondiente al rayo
        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        //Si la luz no es dispersada se trata de un objeto emisor de luz
        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        //Color a partir de la luz dispersada
        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

        return color_from_emission + color_from_scatter;
    }
};



#endif // CAMERA_H