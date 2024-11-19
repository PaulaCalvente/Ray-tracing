#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

//Clase abstracta para materiales
class material {
  public:
    virtual ~material() = default;

    //Funcion a implementar que determina cantidad de luz emitida por un elemento (por defecto es 0)
    virtual color emitted(double u, double v, const point3& p) const {
        return color(0,0,0);
    }
    
    //Funcion a implementar que determina cantidad, intensidad y direccion de iluminacion reflejada
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
        return false;
    }
};

//Clase de difusion lambertiana, la cual implementa refraccion lambertiana
class lambertian : public material {
  public:
    lambertian(const color& albedo) : albedo(albedo) {}

    //La luz se dispersa semialeatoriamente
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

  private:
    color albedo;
};
//Clase metal, que implementa reflexion
class metal : public material {
  public:
    metal(const color& albedo) : albedo(albedo) {}

    //La luz se refleja
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }

  private:
    color albedo;
};
//Clase dielectrico, que implementa refraccion
class dielectric : public material {
  public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    //Luz en dielectricos, es bastante dificil pero viene en el libro
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());

        //Hay casos en los que la ley de Snell (refraccion) no puede ser resuelta, de tal forma que deben reflejar

        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction);
        return true;
    }

  private:
    //Ratio de refraccion de elemento respecto de entorno
    double refraction_index;

    //Aproximacion de Schlick para reflectancia (ni yo lo acabo de entender, no te preocupes)
    static double reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};
//Clase luz, que implementa iluminacion a los objetos
class d_light : public material {
  public:
    d_light(const color& emission) : emit(emission) {}

    //Material emisivo no dispersa rayos.
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        return false;
    }

    //Luz emitida
    color emitted(double u, double v, const point3& p) const override {
        return emit;
    }

  private:
    color emit;
};
#endif