
#include "geometry/scene.hpp"

#include "interval.hpp"


using namespace std;


bool Scene::Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const
{
    HitRecord tmp_record;
    bool bHit = false;

    for(const shared_ptr<IHittable>& object : m_objects){
        if (object->Hit(ray, interval, tmp_record)){
            bHit = true;
            interval.SetMax(tmp_record.t);
            tmp_record.object = object;
        }
    }

    // can be outside as closest is the last modifier of record
    outRecord = tmp_record;
    

    return bHit;
}