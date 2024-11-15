#ifndef HITTABLE_H //Include guard, previene que el contenido sea incluido mas de una vez en el programa
#define HITTABLE_H

//Clase para almacenar informacion de intersección entre rayo y elemento
class hit_record {
  public:
    point3 p;
    vec3 normal;
    double t;

    bool front_face;
    //Determina si la cara impactada es frontal y almacena valor de vector normal
    //Vector normal dado debe estar normalizado
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        //Si cara es interior invierte vector normal
        normal = front_face ? outward_normal : -outward_normal;
    }
};
//Clase abstracta que indica si un elemento es atravesable por un rayo
//Varias cosas a comentar:
//      - La clase es abstracta porque incorpora métodos virtuales
//      - Un método es virtual porque tiene la tag "virtual", no tiene implementación en la clase base y obliga a implementarla en clases derivadas
class hittable {
  public:
    //Declaracion de destructor virtual, para liberar memoria y cosas de esas
    virtual ~hittable() = default;
    //Duncion que comprueba si elemento es golpeado por rayo, toma de entrada un rayo, rango de t y un objeto hit_record que almacenará información de intersección si ocurre 
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif //Fin include guard