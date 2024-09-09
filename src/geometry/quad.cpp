
#include "quad.hpp"

#include "math/ray.hpp"


using namespace std;


Quad::Quad(const Point3 &q, const Vec3 &u, const Vec3 &v, shared_ptr<IMaterial> material)
    : m_q{q}, m_u{u}, m_v{v}, m_material{material}
{
    Vec3 n = m_u.Cross(m_v);
    m_normal = n.Normalized();
    m_d = m_normal.Dot(m_q);
    m_w = n / n.Dot(n);

    AABB diag1_aabb = AABB(m_q, m_q + m_u + m_v);
    AABB diag2_aabb = AABB(m_q + m_u, m_q + m_v);
    m_aabb = AABB(diag1_aabb, diag2_aabb);
}


bool Quad::Hit(const Ray &ray, const Interval &interval, HitRecord &outRecord) const
{
    double denominator = m_normal.Dot(ray.Direction());
    if(std::fabs(denominator) < 1e-8){
        return false;
    }

    double t = (m_d - m_normal.Dot(ray.Origin())) / denominator;

    if(! interval.Contains(t)){
        return false;
    }

    Vec3 intersection = ray.At(t);

    Vec3 plan_centered_hitpoint = intersection - m_q;
    double alpha = m_w.Dot(plan_centered_hitpoint.Cross(m_v));
    double beta = m_w.Dot(m_u.Cross(plan_centered_hitpoint));

    if(! IsInQuad(alpha, beta, outRecord)){
        return false;
    }

    outRecord.t = t;
    outRecord.hit_point = intersection;
    outRecord.material = m_material;
    outRecord.SetFaceNormal(ray, m_normal);

    return true;
}


bool Quad::IsInQuad(double alpha, double beta, HitRecord& hit_record) const {
    static const Interval unit_interval = Interval(0.0, 1.0);
    
    // set it anyway to make code branchless
    hit_record.u = alpha;
    hit_record.v = beta;

    return unit_interval.Contains(alpha) && unit_interval.Contains(beta);
}


shared_ptr<HittableList> Quad::Box(const Point3& a, const Point3& b, shared_ptr<IMaterial> mat)
{
    auto sides = make_shared<HittableList>();

    auto min = Point3(std::fmin(a[0],b[0]), std::fmin(a[1],b[1]), std::fmin(a[2],b[2]));
    auto max = Point3(std::fmax(a[0],b[0]), std::fmax(a[1],b[1]), std::fmax(a[2],b[2]));

    auto dx = Vec3(max[0] - min[0], 0, 0);
    auto dy = Vec3(0, max[1] - min[1], 0);
    auto dz = Vec3(0, 0, max[2] - min[2]);

    sides->AddObjects({
        make_shared<Quad>(Point3(min[0], min[1], max[2]),  dx,  dy, mat), // front
        make_shared<Quad>(Point3(max[0], min[1], max[2]), -dz,  dy, mat), // right
        make_shared<Quad>(Point3(max[0], min[1], min[2]), -dx,  dy, mat), // back
        make_shared<Quad>(Point3(min[0], min[1], min[2]),  dz,  dy, mat), // left
        make_shared<Quad>(Point3(min[0], max[1], max[2]),  dx, -dz, mat), // top
        make_shared<Quad>(Point3(min[0], min[1], min[2]),  dx,  dz, mat)  // bottom
    });

    return sides;
}