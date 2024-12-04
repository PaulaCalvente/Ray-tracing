#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "aabb.h"
#include "hittable.h"

#include <vector>

//Clase de lista de elementos atravesables por rayo 
class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    //Constructores
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    //Funcion que elimina objeto
    void clear() { objects.clear(); }

    //Funcion que añade objeto
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    //Funcion que comprueba si algun elemento del mundo es golpeado
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    aabb bounding_box() const override { return bbox; }

  private:
    aabb bbox;
};

#endif