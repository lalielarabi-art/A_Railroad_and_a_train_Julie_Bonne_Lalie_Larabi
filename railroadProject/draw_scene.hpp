#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include <json.hpp>

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};
extern float angle_theta;      // Angle between x axis and viewpoint
extern float angle_phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom  ;      // Distance between origin and viewpoint
extern bool isTrainMoving;
/* OpenGL Engine */
extern GLBI_Engine myEngine;

void initScene();

void drawFrame();

void drawBase();

void drawArm();

void drawPan();

void drawScene();

void loadCircuitFromJSON(const std::string& filename);

void updateScene(float deltaTime);

void handleKeyboardInput(unsigned char key);
struct Position
{
    int x {0};
    int y {0};

    // This macro generates the to_json and from_json functions for the Position struct
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y)
};

struct GridConfig
{
    int size_grid {0};
    Position origin {};
    std::vector<Position> path {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GridConfig, size_grid, origin, path)
};
