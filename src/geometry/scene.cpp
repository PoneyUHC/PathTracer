
#include "geometry/scene.hpp"

#include "interval.hpp"


using namespace std;


bool Scene::Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const
{
    HitRecord tmpRecord;
    bool bHit = false;
    double closest = interval.Max();

    for(const shared_ptr<IHittable>& object : m_objects){
        if (object->Hit(ray, Interval(interval.Min(), closest), tmpRecord)){
            bHit = true;
            closest = tmpRecord.t;
            tmpRecord.object = object;
        }
    }

    // can be outside as closest is the last modifier of record
    outRecord = tmpRecord;
    

    return bHit;
}