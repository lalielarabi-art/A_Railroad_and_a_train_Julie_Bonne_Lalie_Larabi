#include "draw_scene.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {150.0};      // Distance between origin and viewpoint

GLBI_Engine myEngine;
GLBI_Convex_2D_Shape somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape grid(3);
IndexedMesh* sphere;

void drawGrid(){

	std::vector<float> gridPoints{};
	for (float k{-50}; k<=50; k+=10){
		gridPoints.insert(gridPoints.end(),{-50.0f,k,0.0f,50.0f,k,0.0f});
	}
	for (float k{-50}; k<=50; k+=10){
		gridPoints.insert(gridPoints.end(),{k,-50.0f,0.0f,k,50.0f,0.0f});
	}
	grid.initShape(gridPoints);
	grid.changeNature(GL_LINES);

}
void initScene() {
	std::vector<float> points {0.0,0.0,0.0};
	somePoints.initShape(points);

	std::vector<float> baseCarre{-50.0,-50.0,0.0,
								 50.0,-50.0,0.0,
								 50.0,50.0,0.0,
								 -50.0,50.0,0.0};
	ground.initShape(baseCarre);
	ground.changeNature(GL_TRIANGLE_FAN);

	drawGrid();
}


void drawFrame() {
	std::vector <float> origin {10.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	somePoints.initShape(origin);	
}


void drawScene() {
	glPointSize(10.0);

	myEngine.setFlatColor(0.2,0.0,0.0);
	ground.drawShape();

	myEngine.setFlatColor(0.0,1.0f,0.0);
	grid.drawShape();
	
}



