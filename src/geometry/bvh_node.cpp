
#include "bvh_node.hpp"

#include <algorithm>


using namespace std;


bool BVHNode::CompareAABB(shared_ptr<IHittable> obj1, shared_ptr<IHittable> obj2, int axis_index)
{
    const Interval& i1 = obj1->GetAABB().IndexToInterval(axis_index);
    const Interval& i2 = obj2->GetAABB().IndexToInterval(axis_index);
    return i1.Min() < i2.Min();
}


BVHNode::BVHNode(vector<shared_ptr<IHittable>>& objects, size_t start, size_t end)
{
    size_t span = end - start;

    if (span == 1){
        m_left = objects[start];
        m_right = objects[start];
        m_aabb = m_left->GetAABB();
        return;
    }

    if(span == 2) {
        m_left = objects[start];
        m_right = objects[start+1];
        m_aabb = AABB(m_left->GetAABB(), m_right->GetAABB());
        return;
    }

    m_aabb = AABB::Empty;
    for (size_t i=start; i<end; ++i){
        m_aabb = AABB(m_aabb, objects[i]->GetAABB());
    }

    int axis_index = m_aabb.LongestAxis();
    auto comparator = [axis_index](shared_ptr<IHittable> obj1, shared_ptr<IHittable> obj2){
        return BVHNode::CompareAABB(obj1, obj2, axis_index);
    };

    sort(objects.begin() + start, objects.begin() + end, comparator);

    size_t mid = start + span/2;
    m_left = make_shared<BVHNode>(objects, start, mid);
    m_right = make_shared<BVHNode>(objects, mid, end);
}


bool BVHNode::Hit(const Ray &ray, Interval &interval, HitRecord &outRecord) const
{
    if (!m_aabb.Hit(ray, interval)){
        return false;
    }

    Interval left_interval = interval;
    bool hit_left = m_left->Hit(ray, left_interval, outRecord);
    if (m_left == m_right){
        return hit_left;
    }

    Interval right_interval = interval;
    if(hit_left){
        right_interval.SetMax(outRecord.t);
    }
    bool hit_right = m_right->Hit(ray, right_interval, outRecord);

    return hit_left || hit_right;
}