#include "draw_scene.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {50.0};      // Distance between origin and viewpoint

GLBI_Engine myEngine;
GLBI_Convex_2D_Shape somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape grid(3);
GLBI_Convex_2D_Shape curvedRailB1 (3);
GLBI_Convex_2D_Shape curvedRailT1 (3);
GLBI_Convex_2D_Shape curvedRailI1 (3);
GLBI_Convex_2D_Shape curvedRailO1 (3);
IndexedMesh* balast;
IndexedMesh* rail;


float sr{0.5};
float rr{0.3};
float sx{0.7};
float POS_X_RAIL1{3};
float POS_X_RAIL2{7};

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

	balast= basicCylinder(6.0f,rr, 100,100);
	balast->createVAO();

	rail= basicCube(1.0f);
	rail->createVAO();

	std::vector<float> curvedRailBottom1{};

	for (int k{0}; k<= 10;++k){
		curvedRailBottom1.insert(curvedRailBottom1.end(),{(POS_X_RAIL1-sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1-sr/2)*sinf(k*M_PI/20),0.0f});
		curvedRailBottom1.insert(curvedRailBottom1.end(),{(POS_X_RAIL1+sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1+sr/2)*sinf(k*M_PI/20),0.0f});
		
	}
	curvedRailB1.initShape(curvedRailBottom1);

	std::vector<float> curvedRailTop1{};

	for (int k{0}; k<= 10;++k){
		curvedRailTop1.insert(curvedRailTop1.end(),{(POS_X_RAIL1+sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1+sr/2)*sinf(k*M_PI/20),sr});
		curvedRailTop1.insert(curvedRailTop1.end(),{(POS_X_RAIL1-sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1-sr/2)*sinf(k*M_PI/20),sr});
	}
	curvedRailT1.initShape(curvedRailTop1);

	std::vector<float> curvedRailInside1{};

	for (int k{0}; k<= 10;++k){
		curvedRailInside1.insert(curvedRailInside1.end(),{(POS_X_RAIL1-sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1-sr/2)*sinf(k*M_PI/20),0.0f});
		curvedRailInside1.insert(curvedRailInside1.end(),{(POS_X_RAIL1-sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1-sr/2)*sinf(k*M_PI/20),sr});
	}
	curvedRailI1.initShape(curvedRailInside1);

	std::vector<float> curvedRailOutside1{};

	for (int k{0}; k<= 10;++k){
		curvedRailOutside1.insert(curvedRailOutside1.end(),{(POS_X_RAIL1+sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1+sr/2)*sinf(k*M_PI/20),0.0f});
		curvedRailOutside1.insert(curvedRailOutside1.end(),{(POS_X_RAIL1+sr/2)*cosf(k*M_PI/20),(POS_X_RAIL1+sr/2)*sinf(k*M_PI/20),sr});
		
	}
	curvedRailO1.initShape(curvedRailOutside1);

	
}

void drawStraightRail(){
for (int k{0}; k<=4; ++k){
myEngine.mvMatrixStack.pushMatrix();
	Vector3D tr2{8.0,(sx + rr + k*(2*rr +2*sx)),rr};
	myEngine.mvMatrixStack.addTranslation(tr2);
	Vector3D rt{0,0,1};
	myEngine.mvMatrixStack.addRotation(M_PI/2,rt);
    myEngine.setFlatColor(0.96,0.96,0.86);
    myEngine.updateMvMatrix();
	balast -> draw();
myEngine.mvMatrixStack.popMatrix();
}


myEngine.mvMatrixStack.pushMatrix();
	Vector3D tr{POS_X_RAIL1,5.0,rr+sr};
	myEngine.mvMatrixStack.addTranslation(tr);
    Vector3D hm{sr,10,sr};
	myEngine.mvMatrixStack.addHomothety(hm);
    myEngine.setFlatColor(0.8,0.8,0.8);
    myEngine.updateMvMatrix();
	rail -> draw();
myEngine.mvMatrixStack.popMatrix();

myEngine.mvMatrixStack.pushMatrix();
	Vector3D tr1{POS_X_RAIL2,5.0,2*rr +sr/2};
	myEngine.mvMatrixStack.addTranslation(tr1);
	myEngine.mvMatrixStack.addHomothety(hm);
    myEngine.setFlatColor(0.8,0.8,0.8);
    myEngine.updateMvMatrix();
	rail -> draw();
myEngine.mvMatrixStack.popMatrix();

}

void drawCurveRail(){
	myEngine.mvMatrixStack.pushMatrix();
    myEngine.setFlatColor(0.8,0.8,0.8);
    curvedRailB1.changeNature(GL_TRIANGLE_STRIP);
	curvedRailT1.changeNature(GL_TRIANGLE_STRIP);
	curvedRailI1.changeNature(GL_TRIANGLE_STRIP);
	curvedRailO1.changeNature(GL_TRIANGLE_STRIP);
    myEngine.updateMvMatrix();
    curvedRailB1.drawShape();
	curvedRailT1.drawShape();
	curvedRailI1.drawShape();
	curvedRailO1.drawShape();
	myEngine.mvMatrixStack.popMatrix();
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

	//drawStraightRail();

	drawCurveRail();
	
}



