
#include "geometry/scene.hpp"


using namespace std;


bool Scene::Hit(const Ray& ray, double tmin, double tmax, HitRecord& outRecord) const
{
    HitRecord tmpRecord;
    bool bHit;
    double closest = tmax;

    for(const shared_ptr<IHittable>& object : m_objects){
        if (object->Hit(ray, tmin, closest, tmpRecord)){
            bHit = true;
            closest = tmpRecord.t;
        }
    }

    // can be outside as closest is the last modifier of record
    outRecord = tmpRecord;
    

    return bHit;
}