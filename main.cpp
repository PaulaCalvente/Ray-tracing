#include "header.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
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

    //Adicion de elementos al mundo junto a sus materiales
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -2.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -2.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-2.0,    0.0, -2.0),   1.5, material_left));
    world.add(make_shared<sphere>(point3(-1.5,    0.0, -2.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -2.0),   0.7, material_right));

    //Creacion camara
    camera cam;

    //Parametros de camara
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 50;
    cam.lookfrom = point3(-2,2,1);
    cam.lookat   = point3(-1,0,-1);
    cam.vr      = vec3(0,1,0);

    cam.defocus_angle = 10.0;
    cam.focus_dist    = 4.0;

    //Funcion de renderizado
    cam.render(world);
}