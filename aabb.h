#ifndef AABB_H
#define AABB_H

//Clase que genera volumenes de optimizacion bvh, los cuales suelen ser denominados como AABB
//Técnicamente no es una figura volumetrica, sino que consiste en una serie de planos que delimitan un espacio, los cuales sirven para detectar si un rayo los ha atravesado
class aabb {
  public:
    interval x, y, z;
    //AABB por defecto esta vacio
    aabb() {}

    //Constructor dado tres intervalos los cuales definen los planos
    aabb(const interval& x, const interval& y, const interval& z)
      : x(x), y(y), z(z)
    {
        pad_to_minimums();
    }

    //Constructor dados dos extremos del "cubo"
    aabb(const vec3& a, const vec3& b) {
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

        pad_to_minimums();

    }

    //Constructor dados dos volumenes AABB
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    //Intervalos de planos
    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    //Comprueba si un rayo golpea el volumen
    bool hit(const ray& r, interval ray_t) const {
        const vec3& ray_orig = r.origin();
        const vec3& ray_dir  = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval& ax = axis_interval(axis);
            const double adinv = 1.0 / ray_dir[axis];

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }

    //Indice de eje mas largo de AABB
    int longest_axis() const {
        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else
            return y.size() > z.size() ? 1 : 2;
    }

    static const aabb empty, universe;

  private:
    //Ajuste de AABB por problemas de coma flotante
    void pad_to_minimums() {
        double delta = 0.0001;
        if (x.size() < delta) x = x.expand(delta);
        if (y.size() < delta) y = y.expand(delta);
        if (z.size() < delta) z = z.expand(delta);
    }
};

const aabb aabb::empty    = aabb(interval::empty,    interval::empty,    interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif