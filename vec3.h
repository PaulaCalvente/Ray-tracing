#ifndef VEC3_H //Include guard, previene que el contenido sea incluido mas de una vez en el programa
#define VEC3_H

/*
#############################
#       Clase 3Vector       #
#############################
*/
class vec3 {
  private:
    double e[3];

  public:
    vec3() : vec3(0, 0, 0) {};
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    //Getters
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    //Operadores
    double operator[](int i) const { return e[i]; }
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    vec3& operator+=(const vec3& v) {
        e[0] += v.x();
        e[1] += v.y();
        e[2] += v.z();
        return *this;
    }

    vec3& operator*=(const vec3& v) {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    static vec3 random() {
        return random(0, 1);
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }

    //Comprueba si vector es cercano a 0 en todas las dimensiones
    bool near_zero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }
};

//Funcionalidades varias

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    vec3 uCopy = u;  // Copia el vector
    uCopy += v;      // Aplica el operador += a la copia
    return uCopy;
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    vec3 uCopy = u;  // Copia el vector
    vec3 vCopy = -v;  // Copia el vector
    return uCopy += v;
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    vec3 uCopy = u;  // Copia el vector
    uCopy *= v;      // Aplica el operador += a la copia
    return uCopy;
}

inline vec3 operator*(double t, const vec3& v) {
    vec3 vCopy = v;  // Copia el vector
    vCopy *= t;      // Aplica el operador *= a la copia
    return vCopy;
}

inline vec3 operator*(const vec3& v, double t) {
    return operator*(t, v);
}

inline vec3 operator/(const vec3& v, double t) {
    vec3 vCopy = v;  // Copia el vector
    vCopy /= t;      // Aplica el operador *= a la copia
    return vCopy;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.x() * v.x()
         + u.y() * v.y()
         + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}


///////////////////////////////////////////////////////////////////////////////
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1) //Modificado para evitar problemas con coma flotante
            return p / sqrt(lensq);
    }
}

//Funcion que determina vectores unitarios validos en superficie, no se usa porque se incorpora distribucion lambresiana
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) //Mismo hemisferio que la normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

//Funcion para reflejar, por si el nombre no es lo suficientemente autoexplicativo
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

//Funcion para refractar, por si este nombre tampoco se entiende
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

//Dispone rayos aleatoriamente en disco de desenfoque
inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

#endif 