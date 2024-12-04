#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
//Clase paralelogramos, la cual sirve para crear paralelogramos además de implementar varias funciones que pueden ser de utilidad para otras figuras planas
class quad : public hittable {
  public:
    //Constructor de quad
    quad(const vec3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
      : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n,n);

        set_bounding_box();
    }

    //Crea volumen de limitacion AABB
    virtual void set_bounding_box() {
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }

    aabb bounding_box() const override { return bbox; }

    //Comprueba si la figura fue atravesada por un rayo, para lo cual hace uso de un plano el cual contiene a la propia figura (el cual tiene su propio sistema de coordenadas)
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        //No se atraviesa si el rayo es paralelo al plano (con un ajuste de coma flotante por supuesto)
        if (std::fabs(denom) < 1e-8)
            return false;

        //No se atraviesa si el punto de impacto esta fuera del intervalo del rayo
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        //Determina el punto de interseccion con el plano mediante las coordenadas del propio plano
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        //Si el rayo atraviesa la figura retorna true y actualiza valores
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }
    //Mediante coordenadas del plano, determina si un rayo atraviesa una zona interior o no
    //Se considera interior aquel perimetro del plano que contiene la figura creada
    //Esta funcion es extremadamente util para crear nuevas figuras planas
    virtual bool is_interior(double a, double b, hit_record& rec) const {
        interval unit_interval = interval(0, 1);

        if (!unit_interval.contains(a) || !unit_interval.contains(b))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

  private:
    vec3 Q;
    vec3 u, v;
    vec3 w;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

//Clase triangulos, cuya unica modificacion respecto a quad es como se determina si una zona es interior o no
class trig : public quad {
  public:
    trig(const vec3& o, const vec3& aa, const vec3& ab, shared_ptr<material> m)
      : quad(o, aa, ab, m)
    {}
    //Funcion que determina si un rayo atraviesa zona interior
    virtual bool is_interior(double a, double b, hit_record& rec) const {

        if (a < 0 || b < 0 || a + b > 1)
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }
};

#endif