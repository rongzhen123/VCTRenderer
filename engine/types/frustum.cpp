#include <glm/detail/func_geometric.hpp>

#include "frustum.h"

#include "bounding_box.h"
#include <glm/gtc/matrix_access.hpp>

CullingFrustum::CullingFrustum()
{
}


CullingFrustum::~CullingFrustum()
{
}

void Frustum::ExtractPlanes(const glm::mat4x4 &matrix, bool normalize)
{
    // extract frustum planes from matrix
    // planes are in format: normal(xyz), offset(w)
    const glm::vec4 mRow[4] = {
        row(matrix, 0), row(matrix, 1),
        row(matrix, 2), row(matrix, 3)
    };
    planes[Left] = mRow[3] + mRow[0];
    planes[Right] = mRow[3] - mRow[0];
    planes[Bottom] = mRow[3] + mRow[1];
    planes[Top] = mRow[3] - mRow[1];
    planes[Near] = mRow[3] + mRow[2];
    planes[Far] = mRow[3] - mRow[2];

    if (!normalize) { return; }

    // Normalize them
    for (auto &p : planes) {
        p /= length(p);
    }
}

const glm::vec4 &Frustum::Plane(const PlaneFace face) const
{
    return planes[face];
}

bool CullingFrustum::InFrustum(const BoundingBox &volume) const
{
    glm::vec3 normal;

    for (auto &plane : planes) {
        normal = glm::vec3(plane);
        auto d = dot(volume.Extent(), abs(normal));
        auto r = dot(volume.Center(), normal) + plane.w;

        if (d + r > 0.0f == false) {
            return false;
        }
    }

    return true;
}


