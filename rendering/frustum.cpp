#include "frustum.h"

// compute frustum planes from view and projection matrices
Frustum::Frustum( const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix ) 
{
        const glm::mat4 &v = viewMatrix;
        const glm::mat4 &p = projectionMatrix;
 
        glm::mat4 clipMatrix;
 
        clipMatrix[0][0] = v[0][0]*p[0][0]+v[0][1]*p[1][0]+v[0][2]*p[2][0]+v[0][3]*p[3][0];
        clipMatrix[1][0] = v[0][0]*p[0][1]+v[0][1]*p[1][1]+v[0][2]*p[2][1]+v[0][3]*p[3][1];
        clipMatrix[2][0] = v[0][0]*p[0][2]+v[0][1]*p[1][2]+v[0][2]*p[2][2]+v[0][3]*p[3][2];
        clipMatrix[3][0] = v[0][0]*p[0][3]+v[0][1]*p[1][3]+v[0][2]*p[2][3]+v[0][3]*p[3][3];
        clipMatrix[0][1] = v[1][0]*p[0][0]+v[1][1]*p[1][0]+v[1][2]*p[2][0]+v[1][3]*p[3][0];
        clipMatrix[1][1] = v[1][0]*p[0][1]+v[1][1]*p[1][1]+v[1][2]*p[2][1]+v[1][3]*p[3][1];
        clipMatrix[2][1] = v[1][0]*p[0][2]+v[1][1]*p[1][2]+v[1][2]*p[2][2]+v[1][3]*p[3][2];
        clipMatrix[3][1] = v[1][0]*p[0][3]+v[1][1]*p[1][3]+v[1][2]*p[2][3]+v[1][3]*p[3][3];
        clipMatrix[0][2] = v[2][0]*p[0][0]+v[2][1]*p[1][0]+v[2][2]*p[2][0]+v[2][3]*p[3][0];
        clipMatrix[1][2] = v[2][0]*p[0][1]+v[2][1]*p[1][1]+v[2][2]*p[2][1]+v[2][3]*p[3][1];
        clipMatrix[2][2] = v[2][0]*p[0][2]+v[2][1]*p[1][2]+v[2][2]*p[2][2]+v[2][3]*p[3][2];
        clipMatrix[3][2] = v[2][0]*p[0][3]+v[2][1]*p[1][3]+v[2][2]*p[2][3]+v[2][3]*p[3][3];
        clipMatrix[0][3] = v[3][0]*p[0][0]+v[3][1]*p[1][0]+v[3][2]*p[2][0]+v[3][3]*p[3][0];
        clipMatrix[1][3] = v[3][0]*p[0][1]+v[3][1]*p[1][1]+v[3][2]*p[2][1]+v[3][3]*p[3][1];
        clipMatrix[2][3] = v[3][0]*p[0][2]+v[3][1]*p[1][2]+v[3][2]*p[2][2]+v[3][3]*p[3][2];
        clipMatrix[3][3] = v[3][0]*p[0][3]+v[3][1]*p[1][3]+v[3][2]*p[2][3]+v[3][3]*p[3][3];
 
        m_planes[PLANE_RIGHT].x = clipMatrix[3][0]-clipMatrix[0][0];
        m_planes[PLANE_RIGHT].y = clipMatrix[3][1]-clipMatrix[0][1];
        m_planes[PLANE_RIGHT].z = clipMatrix[3][2]-clipMatrix[0][2];
        m_planes[PLANE_RIGHT].w = clipMatrix[3][3]-clipMatrix[0][3];
 
        m_planes[PLANE_LEFT].x = clipMatrix[3][0]+clipMatrix[0][0];
        m_planes[PLANE_LEFT].y = clipMatrix[3][1]+clipMatrix[0][1];
        m_planes[PLANE_LEFT].z = clipMatrix[3][2]+clipMatrix[0][2];
        m_planes[PLANE_LEFT].w = clipMatrix[3][3]+clipMatrix[0][3];
 
        m_planes[PLANE_BOTTOM].x = clipMatrix[3][0]+clipMatrix[1][0];
        m_planes[PLANE_BOTTOM].y = clipMatrix[3][1]+clipMatrix[1][1];
        m_planes[PLANE_BOTTOM].z = clipMatrix[3][2]+clipMatrix[1][2];
        m_planes[PLANE_BOTTOM].w = clipMatrix[3][3]+clipMatrix[1][3];
 
        m_planes[PLANE_TOP].x = clipMatrix[3][0]-clipMatrix[1][0];
        m_planes[PLANE_TOP].y = clipMatrix[3][1]-clipMatrix[1][1];
        m_planes[PLANE_TOP].z = clipMatrix[3][2]-clipMatrix[1][2];
        m_planes[PLANE_TOP].w = clipMatrix[3][3]-clipMatrix[1][3];
 
        m_planes[PLANE_BACK].x = clipMatrix[3][0]-clipMatrix[2][0];
        m_planes[PLANE_BACK].y = clipMatrix[3][1]-clipMatrix[2][1];
        m_planes[PLANE_BACK].z = clipMatrix[3][2]-clipMatrix[2][2];
        m_planes[PLANE_BACK].w = clipMatrix[3][3]-clipMatrix[2][3];
 
        m_planes[PLANE_FRONT].x = clipMatrix[3][0]+clipMatrix[2][0];
        m_planes[PLANE_FRONT].y = clipMatrix[3][1]+clipMatrix[2][1];
        m_planes[PLANE_FRONT].z = clipMatrix[3][2]+clipMatrix[2][2];
        m_planes[PLANE_FRONT].w = clipMatrix[3][3]+clipMatrix[2][3];
 
        for( int i = 0; i < 6; i++ )
        {
                m_planes[i] = glm::normalize( m_planes[i] );
        }
}
 
// check whether an AABB intersects the frustum
bool Frustum::boxCollide( const BB &bounding_box ) const
{
		//INSIDE
        //bool result = true;
 
        for( uint i = 0; i < 6; i++ )
        {
                const float pos = m_planes[i].w;
                const glm::vec3 normal = glm::vec3(m_planes[i]);
 
 				/*
                if( glm::dot(normal, getPositiveVertex(normal, bounding_box))+pos < 0.0f )
                {
                		//OUTSIDE
                        return false;
                }
 
                if( glm::dot(normal, getNegativeVertex(normal, bounding_box))+pos < 0.0f )
                {
                		//INTERSECT
                        result = true; //As of noow, for visual testing
                }
                */
                glm::vec3 axisVert;
                if(m_planes[i].x < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the x axis
	                axisVert.x = bounding_box.min.x; // min x plus tree positions x
	            else
	                axisVert.x = bounding_box.max.x; // max x plus tree positions x

	            // y-axis
	            if(m_planes[i].y < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the y axis
	                axisVert.y = bounding_box.min.y; // min y plus tree positions y
	            else
	                axisVert.y = bounding_box.max.y; // max y plus tree positions y

	            // z-axis
	            if(m_planes[i].z < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the z axis
	                axisVert.z = bounding_box.min.z; // min z plus tree positions z
	            else
	                axisVert.z = bounding_box.max.z; // max z plus tree positions z

	            if(glm::dot(normal, axisVert) + pos < 0.0f)
	            	return false;
        }
 
        return true;
}
 
glm::vec3 Frustum::getPositiveVertex( const glm::vec3 &normal, const BB &bounding_box ) const
{
        glm::vec3 positiveVertex = bounding_box.min;
 
        if( normal.x >= 0.0f ) positiveVertex.x = bounding_box.max.x;
        if( normal.y >= 0.0f ) positiveVertex.y = bounding_box.max.y;
        if( normal.z >= 0.0f ) positiveVertex.z = bounding_box.max.z;
 
        return positiveVertex;
}
 
glm::vec3 Frustum::getNegativeVertex( const glm::vec3 &normal, const BB &bounding_box ) const
{
        glm::vec3 negativeVertex = bounding_box.max;
 
        if( normal.x >= 0.0f ) negativeVertex.x = bounding_box.min.x;
        if( normal.y >= 0.0f ) negativeVertex.y = bounding_box.min.y;
        if( normal.z >= 0.0f ) negativeVertex.z = bounding_box.min.z;
 
        return negativeVertex;
}
 