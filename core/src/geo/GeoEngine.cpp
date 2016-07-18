/*
 * Copyright (C) 2015  eBusiness Information
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <utils/GeoUtils.hpp>
#include <rapidjson.h>
#include <rapidjson/document.h>
#include <animation/GeoPathAnimation.hpp>
#include "geo/GeoEngine.hpp"
#include <utils/triangulate/Geometry.hpp>
#include <shape/ShapeFactory.hpp>
#include <geo/tile/mvt/GeometryMapper.hpp>
#include <shape/GeometryUtils.hpp>
#include <geo/VectorTile.hpp>
#include <geo/tile/StyleParser.hpp>

constexpr char TAG[] = "GeoEngine";

namespace dma {
    GeoEngine::GeoEngine(const std::string &resourceDir) :
            mRootDir(resourceDir),
            mEngine(resourceDir),
            mPoiFactory(mGeoSceneManager, mEngine.getResourceManager()),
            mGeoSceneManager(mEngine.getScene(), mEngine.getResourceManager()),
            mDefaultCallbacks(new GeoEngineCallbacks()),
            mCallbacks(mDefaultCallbacks)
    {}

    GeoEngine::~GeoEngine() {

    }

    bool GeoEngine::init() {
        bool res = mEngine.init();
        mGeoSceneManager.init();

        std::shared_ptr<HUDElement> watermark = std::make_shared<HUDElement>();
        watermark->x = 20;
        watermark->y = 200 + 20;
        watermark->width = 200;
        watermark->height = 200;
        watermark->textureSID = "watermark";
        mEngine.addHUDElement(watermark);

        //TODO remove: used for buildings/tracks demo
        mGeoSceneManager.placeCamera(LatLngAlt(48.8708735, 2.3036656, 5.0));

        // 1. Stringify the file
        std::string json;
        std::string path = mRootDir + "/buildings.json";
        Utils::bufferize(path, json);

        rapidjson::Document document;
        // 2. Create the DOM
//            if (document.Parse(json.c_str()).HasParseError()) {
        if (document.Parse(json.c_str()).HasParseError()) {
            std::string error = "Unable to parse buildings " + path;
            Log::error(TAG, error);
            throw std::runtime_error(error);
        }
        for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
            double lat = itr->value["lat"].GetDouble();
            double lng = itr->value["lng"].GetDouble();
            std::string id = itr->name.GetString();

            std::string sid = "building/" + id;
            std::shared_ptr<GeoEntity> building = mGeoSceneManager.createGeoEntity(sid, "building");
            building->setCoords(LatLngAlt(lat, lng, 0.1));
            mGeoSceneManager.addGeoEntity(sid, building);
        }

        float height = 1.0f;

        LatLngAlt origin = LatLngAlt(48.870690, 2.303191, height);
        std::shared_ptr<Mesh> trackMesh = mEngine.getTrackFactory().builder()
                .thickness(3.0f)
                .color(0.1098, 0.6, 1.0)
                .path()
                .point(GeoUtils::vector(origin, origin))
                .point(GeoUtils::vector(origin, LatLngAlt(48.870307 , 2.302872, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.869945 , 2.302545, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.869571 , 2.302226, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.869250 , 2.302792, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.869504 , 2.303315, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.869871 , 2.304026, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.870169 , 2.304634, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.870515 , 2.305281, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.870706 , 2.304672, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.870926 , 2.303964, height)))
                .point(GeoUtils::vector(origin, LatLngAlt(48.871069 , 2.303543, height)))
                .build();

        std::shared_ptr<Material> trackMaterial = mEngine.getTrackFactory().generateMaterial(Color(0.1098f, 0.6f, 1.0f));

        auto track = mGeoSceneManager.createGeoEntity(trackMesh, trackMaterial);
        track->setCoords(origin);
        track->addAnimationComponent();
        AnimationComponent* animationComponent = track->getAnimationComponent();
        TransformComponent& transformComponent = track->getTransformComponent();
        animationComponent->add(std::make_shared<TranslationAnimation>(transformComponent, //TODO remove: that is a leak
                                                                       transformComponent.getPosition(),
                                                                       transformComponent.getPosition() + glm::vec3(0.0f, 2.0f, 0.0f),
                                                                       6.0f, TranslationAnimation::Function::EASE, true, true));
        mGeoSceneManager.addGeoEntity("track0", track);

        std::vector<LatLngAlt> points;
        points.push_back(LatLngAlt(48.871069 , 2.303543, 15.0));
        points.push_back(LatLngAlt(48.870690 , 2.303191, 15.0));
        points.push_back(LatLngAlt(48.870307 , 2.302872, 15.0));
        points.push_back(LatLngAlt(48.869945 , 2.302545, 15.0));
        points.push_back(LatLngAlt(48.869571 , 2.302226, 15.0));
        points.push_back(LatLngAlt(48.869250 , 2.302792, 15.0));
        points.push_back(LatLngAlt(48.869504 , 2.303315, 15.0));
        points.push_back(LatLngAlt(48.869871 , 2.304026, 15.0));
        points.push_back(LatLngAlt(48.870169 , 2.304634, 15.0));
        points.push_back(LatLngAlt(48.870515 , 2.305281, 15.0));
        points.push_back(LatLngAlt(48.870706 , 2.304672, 15.0));
        points.push_back(LatLngAlt(48.870926 , 2.303964, 15.0));

//            Camera& camera = mGeoSceneManager.getScene().getCamera();
//            camera.getAnimationComponent().add(std::make_shared<GeoPathAnimation>(camera.getTransformComponent(), points, 25.0f, true, mGeoSceneManager));

        auto flag = mPoiFactory.builder()
                .icon("flag")
                .shape("flag")
                .sid("flag")
                .color(Color(1.0f, 1.0f, 1.0f))
                .animation(false)
                .build();
        flag->setCoords(LatLngAlt(48.871069 , 2.303543, 0.0));
        mGeoSceneManager.addGeoEntity("flag", flag);

        GOOGLE_PROTOBUF_VERIFY_VERSION;

//            std::string mvt;
//            Utils::bufferize(mRootDir + "/mvt/b.mvt", mvt);
        vector_tile::Tile tile;
//            tile.ParseFromString(mvt);
        std::fstream input(mRootDir + "/mvt/d.mvt", std::ios::in | std::ios::binary);
        tile.ParseFromIstream(&input);
        VectorTile t(tile);

//        for (auto f : tile.layers(0).features()) {
        auto f = tile.layers(0).features(3);
        std::vector<Polygon> polygons = GeometryMapper::polygons(f.geometry());

        float scaleX = 20.0f / tile.layers(0).extent();
        float scaleY = 20.0f / tile.layers(0).extent();
        for (auto& p : polygons) {
            GeometryUtils::scale(p, scaleX, scaleY);
        }

        ShapeFactory shapeFactory(mEngine.getResourceManager());

//                std::vector<glm::vec2> poly;
//                poly.push_back(glm::vec2(0, 0));
//                poly.push_back(glm::vec2(10, 0));
//                poly.push_back(glm::vec2(10, 10));
//                poly.push_back(glm::vec2(0, 10));
//                Polygon polygon;
//                polygon.push_back(poly);

        auto mesh = shapeFactory.polygon(polygons[0]);
        auto material = mEngine.getResourceManager().acquireMaterial("building");
        auto building = std::make_shared<Entity>(mesh, material, 0.0f, 25.0f, 0.0f);
//            building->setScale(glm::vec3(0.05));
        mEngine.getScene().addEntity(building);
//        }
        return res;
    }

    void GeoEngine::refresh() {
        mEngine.refresh();
    }

    void GeoEngine::reload() {
        mEngine.reload();
    }

    void GeoEngine::unload() {
        mGeoSceneManager.unload();
        mEngine.unload();
    }

    void GeoEngine::wipe() {
        mEngine.wipe();
    }

    void GeoEngine::step() {
        mMessageQueue.flush();
        mEngine.step();
    }

    void GeoEngine::post(std::function<void()> message) {
        mMessageQueue << message;
    }


    void GeoEngine::setCallback(GeoEngineCallbacks* callbacks) {
        if (!callbacks) {
            mCallbacks = mDefaultCallbacks;
        } else {
            mCallbacks = callbacks;
        }
        mGeoSceneManager.setCallbacks(callbacks);
    }
} /* namespace dma */

