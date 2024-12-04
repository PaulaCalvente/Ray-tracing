#include "header.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "bvh.h"
#include "quad.h"

//Funcion principal, donde se añaden los elementos al mundo, los volumenes de optimizacion bvh, se ajustan ciertos parametros y se ejecuta la funcion de renderizado
int main() {
    //Creacion de lista de almacenamiento (mundo)
    hittable_list world;

    //Creacion de materiales a usar
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<d_light>(color(15, 15, 15));

    //Adicion de elementos al mundo junto a sus materiales
    world.add(make_shared<quad>(vec3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(vec3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(vec3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(vec3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    //Crea volumenes de optimizacion bvh para cada objeto del mundo
    world = hittable_list(make_shared<bvh_node>(world));

    //Creacion camara
    camera cam;

    //Proporciones de la imagen
    cam.aspect_ratio      = 1.0;
    //Longitud de imagen
    cam.image_width       = 600;
    //Rayos por pixel
    cam.samples_per_pixel = 400;
    //Numero de rebotes
    cam.max_depth         = 10;
    //Color de fondo
    cam.background        = color(0,0,0);
    //Angulo de vision (vertical)
    cam.vfov     = 40;
    //Coordenadas de camara
    cam.lookfrom = vec3(278, 278, -800);
    //Coordenadas de objetivo
    cam.lookat   = vec3(278, 278, 0);
    //Vector de referencia de camara (usado para realizar ciertos giros)
    cam.vr      = vec3(0,1,0);
    //Desenfoque
    cam.defocus_angle = 0;
    //Distancia entre camara y punto enfocado
    double focus_dist = 10;

    //Funcion de renderizado
    cam.render(world);
}

//Prueba alternativa, se puede descomentar para probar
/*
//Funcion principal, donde se añaden los elementos al mundo, se ajustan ciertos parametros y se ejecuta la fuuncion de renderizado
int main() {
    //Creacion de lista de almacenamiento (mundo)
    hittable_list world;

    //Creacion de materiales a usar
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.1, 0.6, 0.2));
    auto bg = make_shared<metal>(color(0.4, 0.1, 0));
    auto mirror = make_shared<metal>(color(1, 1, 1));
    auto pirf = make_shared<lambertian>(color(0.5, 0.5, 0));
    auto pirr = make_shared<lambertian>(color(0.5, 0, 0));
    auto pirg = make_shared<lambertian>(color(0, 0.5, 0));
    auto pirb = make_shared<lambertian>(color(0, 0, 0.5));
    auto lgt = make_shared<d_light>(color(1,1,10));

    //Adicion de elementos al mundo junto a sus materiales
    
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -2.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -2.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3( 0.0,    -0.4, -0.5),   0.3, lgt));
    world.add(make_shared<sphere>(point3(-2.0,    0.0, -2.0),   1.5, material_left));
    world.add(make_shared<sphere>(point3(-1.5,    0.0, -2.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -2.0),   0.7, material_right));
    world.add(make_shared<quad>(point3(-3, -1, -4), vec3(8, 0, 0), vec3(0, 4, 3), bg));
    world.add(make_shared<quad>(point3(5, -1, -4), vec3(0, 0, 8), vec3(0, 4, 0), mirror));

    world.add(make_shared<trig>(point3(1.5, 0, -1), vec3(2.5, 0, 0.5), vec3(2, 0, -2), pirf)); //Base

    world.add(make_shared<trig>(point3(1.5, 0, -1), vec3(2.5, 0, 0.5), vec3(1.5, 2, -0.4), pirr)); //Frente
    world.add(make_shared<trig>(point3(1.5, 0, -1), vec3(2, 0, -2), vec3(1.5, 2, -0.4), pirg)); //Izda
    world.add(make_shared<trig>(point3(3.5, 0, -3), vec3(0.5, 0, 2.5), vec3(-0.5, 2, 1.6), pirb)); //Dcha

    //Aplica algoritmo de optimizacion bvh a los objetos del mundo
    world = hittable_list(make_shared<bvh_node>(world));

    //Creacion camara
    camera cam;

    //Parametros de camara
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.7,0.2,0.1);//color(0.70, 0.80, 1.00);

    cam.vfov = 90;
    cam.lookfrom = point3(1,0,2.5);
    cam.lookat   = point3(1,0,-1);
    cam.vr      = vec3(0,1,0);

    cam.defocus_angle = 0.0;
    cam.focus_dist    = 4.0;

    //Funcion de renderizado
    cam.render(world);
}
*/