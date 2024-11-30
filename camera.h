#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <omp.h>
#include <chrono>

//Clase camara, la cual incluye el renderizado, los valores de la camara y el coloreado
class camera {
  public:

    //Parametros de la camara
    double aspect_ratio = 1.0;          //Ratio de la imagen
    int    image_width  = 100;          //Longitud de imagen renderizada
    int    samples_per_pixel = 10;      //Numero de rayos aleatorios por pixel
    int    max_depth         = 10;      //Rebotes maximos por rayo
    color  background;                  // Scene background color

    double vfov = 90;                   //Angulo vertical (fov)
    point3 lookfrom = point3(0,0,0);    //Punto central de camara
    point3 lookat   = point3(0,0,-1);   //Punto objetivo de camara
    vec3   vr      = vec3(0,1,0);       //Vector de referencia de la camara

    double defocus_angle = 0;           //Angulo de variacion por pixel
    double focus_dist = 10;             //Distancia entre camara y punto enfocado

    // Reducción de color para sumar colores
    #pragma omp declare reduction(+: color : omp_out = omp_out + omp_in) initializer(omp_priv = color(0,0,0))
    
    //Funcion de renderizado
    void render(const hittable& world) {
        initialize();

        int num_threads = omp_get_max_threads(); // Ajusta el número de hilos
        // int num_threads = 8; //Para ajustar manualmente hilos, útil para hacer pruebas
        std::clog << "Numero de hilos: " << num_threads << std::endl;

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        // Variables para medir el tiempo
        auto start_time = std::chrono::high_resolution_clock::now(); // Tiempo inicial

        // Paralelizamos el recorrido de las filas
        #pragma omp parallel for schedule(dynamic) num_threads(num_threads)
        for (int j = 0; j < image_height; j++) {
            auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
            
            int estimated_total_time = (elapsed_seconds / (j)) * image_height;
            int remaining_time = estimated_total_time - elapsed_seconds;

            // Progreso y estimación
            #pragma omp critical
            {
                std::clog << "\rScanlines remaining: " << image_height-j 
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

                #pragma omp critical
                write_vector_color(std::cout, pixel_color / samples_per_pixel, i, j);
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now(); // Tiempo final
        int total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

        std::clog << "\nDone. Total time: " << total_time << " seconds.\n";
    }

  //Parametros privados de la camara
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
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

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
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
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
            return color(0,0,0);
        
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
        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

        return color_from_emission + color_from_scatter;
    }
};

#endif