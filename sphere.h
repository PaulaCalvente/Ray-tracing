#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

//Clase esfera, implementa clase abstracta hittable
class sphere : public hittable {
  public:
    //Constructor de esfera, recibe centro y radio
    sphere(const vec3& center, double radius, shared_ptr<material> mat)
      : center(center), radius(std::fmax(0,radius)), mat(mat) {
        auto rvec = vec3(radius, radius, radius);
        //Creacion de volumen de limitacion AABB
        bbox = aabb(center - rvec, center + rvec);

      }

    //Comprueba si esfera es atravesada por rayo
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        //Vector inicial
        vec3 oc = center - r.origin();
        //Coeficientes ecuación cuadrática (a*t^2 + 2*h*t + c = 0)
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        //Determina si hay interseccion, si no retorna false directamente
        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        //Raiz mas cercana a cámara perteneciente a rango t dado
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }
        //Actualiza hit_record
        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;
        //Vector normal saliente
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        //Material
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }
    
  private:
    vec3 center;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif