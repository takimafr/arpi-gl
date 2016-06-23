#include <resource/TrackFactory.hpp>

namespace dma {

    TrackFactory::TrackFactory(ResourceManager& resourceManager) :
            resourceManager(resourceManager)
    {}

    std::shared_ptr<Mesh> TrackFactory::generateMesh(const std::vector<glm::vec3>& points, float thickness) const {

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> flatNormals;
        std::vector<glm::vec3> smoothNormals;
        std::vector<VertexIndices> indices;

        for (U32 i = 0; i < points.size(); ++i) {
            glm::vec3 p0 = i <= 0 ? points[0] : points[i - 1];
            glm::vec3 p1 = points[i];
            glm::vec3 p2 = (i + 1) >= points.size() ? points[points.size()-1] : points[i+1];
            glm::vec3 p3 = (i + 2) >= points.size() ? points[points.size()-1] : points[i+2];

            // skip if zero length
            if (p1 == p2) continue;

            // 1. Define the line between the two points
            glm::vec3 line = glm::normalize(p2 - p1);

            // 2. Find the normal vector of this line
            glm::vec3 normal = glm::normalize(glm::vec3(-line.z, line.y, line.x));

            // 3. Find the tangent vector at both the end points:
            //		-if there are no segments before or after this one, use the line itself
            //		-otherwise, add the two normalized lines and average them by normalizing again
            glm::vec3 tangent1 = p0 == p1 ? line : glm::normalize(glm::normalize(p1 - p0) + line);
            glm::vec3 tangent2 = p2 == p3 ? line : glm::normalize(glm::normalize(p3 - p2) + line);

            // 4. Find the miter line, which is the normal of the tangent
            glm::vec3 miter1 = glm::vec3(-tangent1.z, tangent1.y, tangent1.x);
            glm::vec3 miter2 = glm::vec3(-tangent2.z, tangent2.y, tangent2.x);

            // 5. Find length of miter by projecting the miter onto the normal,
            // take the length of the projection, invert it and multiply it by the thickness:
            // 		length = thickness * ( 1 / |normal|.|miter| )
            float length1 = thickness / glm::dot<float>(normal, miter1);
            float length2 = thickness / glm::dot<float>(normal, miter2);

            // 6. Add the four positions to the list
            positions.push_back(p1 + miter1 * length1);
            smoothNormals.push_back(miter1);

            positions.push_back(p2 + miter2 * length2);
            smoothNormals.push_back(miter2);


            positions.push_back(p1 - miter1 * length1);
            smoothNormals.push_back(-miter1);

            positions.push_back(p2 - miter2 * length2);
            smoothNormals.push_back(-miter2);

            // 7. Add the triangle indices to the list
            //TODO positions are duplicated
            U16 j = (U16) (positions.size() - 4);

            // First triangle
            VertexIndices vi1;
            vi1.p = j;
            vi1.sn = j;
            indices.push_back(vi1);

            VertexIndices vi2;
            vi2.p = j+1;
            vi2.sn = j+1;
            indices.push_back(vi2);

            VertexIndices vi3;
            vi3.p = j+3;
            vi3.sn = j+3;
            indices.push_back(vi3);

            // Second triangle
            VertexIndices vi4;
            vi4.p = j;
            vi4.sn = j;
            indices.push_back(vi1);

            VertexIndices vi5;
            vi5.p = j+3;
            vi5.sn = j+3;
            indices.push_back(vi5);

            VertexIndices vi6;
            vi6.p = j+2;
            vi6.sn = j+2;
            indices.push_back(vi6);
        }

        return resourceManager.createMesh(positions, uvs, flatNormals, smoothNormals, indices);
    }


    std::shared_ptr<Material> TrackFactory::generateMaterial(const Color &color) const {
        std::shared_ptr<Material> material = resourceManager.createMaterial("track");
        material->getPass(0).setDiffuseColor(glm::vec3(color.r, color.g, color.b));
        return material;
    }


    TrackFactory::Builder TrackFactory::builder() {
        return Builder(*this);
    }

    TrackFactory::Builder::Builder(const TrackFactory& trackFactory) :
            mTrackFactory(trackFactory)
    {}

    TrackFactory::Builder &TrackFactory::Builder::thickness(float thickness) {
        mThickness = thickness;
        return *this;
    }

    TrackFactory::Builder &TrackFactory::Builder::color(float r, float g, float b) {
        mColor = Color(r, g, b);
        return *this;
    }

    TrackFactory::PathBuilder TrackFactory::Builder::path() {
        return TrackFactory::PathBuilder(*this);
    }


    TrackFactory::PathBuilder::PathBuilder(const Builder& builder) :
            mBuilder(builder)
    {}

    TrackFactory::PathBuilder &TrackFactory::PathBuilder::point(const glm::vec3& point) {
        mPoints.push_back(point);
        return *this;
    }

    std::shared_ptr<Mesh> TrackFactory::PathBuilder::build() {
        //const Color& color = mBuilder.mColor;
        float thickness = mBuilder.mThickness;
        return mBuilder.mTrackFactory.generateMesh(mPoints, thickness);
    }


}

