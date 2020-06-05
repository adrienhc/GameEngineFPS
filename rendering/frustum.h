#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../pch.h"
#include "../geometry/bounding_box.h"

class Frustum
{
        public:
 
                enum Plane
                {
                        PLANE_BACK,
                        PLANE_FRONT,
                        PLANE_RIGHT,
                        PLANE_LEFT,
                        PLANE_TOP,
                        PLANE_BOTTOM
                };
 
        public:
                Frustum() {};
                Frustum( const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix );
 
                const glm::vec4 &getPlane( const int plane ) const;
               
                //bool testIntersection( const glm::vec3 &point ) const;
                bool boxCollide( const BB &bounding_box ) const;
                //bool testIntersection( shared_ptr<const BoundingSphere> sphere ) const;
 
        protected:

                glm::vec4 m_planes[6];
                glm::vec3 getPositiveVertex( const glm::vec3 &normal, const BB &bounding_box ) const;
                glm::vec3 getNegativeVertex( const glm::vec3 &normal, const BB &bounding_box ) const;
};

#endif //FRUSTUM_H