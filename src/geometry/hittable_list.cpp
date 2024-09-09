
#include "geometry/hittable_list.hpp"

#include "math/interval.hpp"
#include "hittable_list.hpp"


using namespace std;


void HittableList::AddObject(std::shared_ptr<IHittable> object)
{
    m_objects.push_back(object);
    m_aabb = AABB(m_aabb, object->GetAABB());
}

void HittableList::AddObjects(std::vector<std::shared_ptr<IHittable>> objects)
{ 
    for(auto& object: objects){
        AddObject(object);
    }
}


bool HittableList::Hit(const Ray &ray, const Interval &interval, HitRecord &outRecord) const
{
    HitRecord tmp_record;
    bool bHit = false;

    Interval closest = interval;

    for(const shared_ptr<IHittable>& object : m_objects){
        if (object->Hit(ray, closest, tmp_record)){
            bHit = true;
            closest.SetMax(tmp_record.t);
            tmp_record.object = object;
            outRecord = tmp_record;
        }
    }

    return bHit;
}


AABB HittableList::GetAABB() const
{
    return m_aabb;
}