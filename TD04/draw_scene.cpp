#include "draw_scene.hpp"
#include "json.hpp"
#include "tools/texture.hpp"
using json = nlohmann::json;
#include <fstream>

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
GLBI_Convex_2D_Shape curvedRailB2 (3);
GLBI_Convex_2D_Shape curvedRailT2 (3);
GLBI_Convex_2D_Shape curvedRailI2 (3);
GLBI_Convex_2D_Shape curvedRailO2 (3);
IndexedMesh* balast;
IndexedMesh* cube;
IndexedMesh* roue;

Texture2D texSol;

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

void initCurvedRail(float r, GLBI_Convex_2D_Shape& bottom, GLBI_Convex_2D_Shape& top, GLBI_Convex_2D_Shape& inside, GLBI_Convex_2D_Shape& outside){
	std::vector<float> curvedRailBottom1{};

	for (int k{0}; k<= 10;++k){
		curvedRailBottom1.insert(curvedRailBottom1.end(),{(r-sr/2)*cosf(k*M_PI/20),(r-sr/2)*sinf(k*M_PI/20),0.0f});
		curvedRailBottom1.insert(curvedRailBottom1.end(),{(r+sr/2)*cosf(k*M_PI/20),(r+sr/2)*sinf(k*M_PI/20),0.0f});
		
	}
	bottom.initShape(curvedRailBottom1);

	std::vector<float> curvedRailTop1{};

	for (int k{0}; k<= 10;++k){
		curvedRailTop1.insert(curvedRailTop1.end(),{(r+sr/2)*cosf(k*M_PI/20),(r+sr/2)*sinf(k*M_PI/20),sr});
		curvedRailTop1.insert(curvedRailTop1.end(),{(r-sr/2)*cosf(k*M_PI/20),(r-sr/2)*sinf(k*M_PI/20),sr});
	}
	top.initShape(curvedRailTop1);

	std::vector<float> curvedRailInside1{};

	for (int k{0}; k<= 10;++k){
		curvedRailInside1.insert(curvedRailInside1.end(),{(r-sr/2)*cosf(k*M_PI/20),(r-sr/2)*sinf(k*M_PI/20),0.0f});
		curvedRailInside1.insert(curvedRailInside1.end(),{(r-sr/2)*cosf(k*M_PI/20),(r-sr/2)*sinf(k*M_PI/20),sr});
	}
	inside.initShape(curvedRailInside1);

	std::vector<float> curvedRailOutside1{};

	for (int k{0}; k<= 10;++k){
		curvedRailOutside1.insert(curvedRailOutside1.end(),{(r+sr/2)*cosf(k*M_PI/20),(r+sr/2)*sinf(k*M_PI/20),0.0f});
		curvedRailOutside1.insert(curvedRailOutside1.end(),{(r+sr/2)*cosf(k*M_PI/20),(r+sr/2)*sinf(k*M_PI/20),sr});
		
	}
	outside.initShape(curvedRailOutside1);
}

void initScene() {

	Texture2D texSol("herbe.tga");
	texSol.initTexture();

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

	cube = basicCube(1.0f);
	cube->createVAO();

	roue=basicCylinder(0.5f,1.0f,20.0f,1.0f);
	roue -> createVAO();

	initCurvedRail(POS_X_RAIL1, curvedRailB1, curvedRailT1, curvedRailI1, curvedRailO1);
	initCurvedRail(POS_X_RAIL2, curvedRailB2, curvedRailT2, curvedRailI2, curvedRailO2);


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
	Vector3D tr{POS_X_RAIL1,5.0,2*rr +sr/2};
	myEngine.mvMatrixStack.addTranslation(tr);
    Vector3D hm{sr,10,sr};
	myEngine.mvMatrixStack.addHomothety(hm);
    myEngine.setFlatColor(0.8,0.8,0.8);
    myEngine.updateMvMatrix();
	cube -> draw();
myEngine.mvMatrixStack.popMatrix();

myEngine.mvMatrixStack.pushMatrix();
	Vector3D tr1{POS_X_RAIL2,5.0,2*rr +sr/2};
	myEngine.mvMatrixStack.addTranslation(tr1);
	myEngine.mvMatrixStack.addHomothety(hm);
    myEngine.setFlatColor(0.8,0.8,0.8);
    myEngine.updateMvMatrix();
	cube -> draw();
myEngine.mvMatrixStack.popMatrix();

}

void drawCurveRail(){
	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr{0.0f,0.0f,2*rr };
		myEngine.mvMatrixStack.addTranslation(tr);
		myEngine.updateMvMatrix(); 
		myEngine.setFlatColor(0.8,0.8,0.8);
		curvedRailB1.changeNature(GL_TRIANGLE_STRIP);
		curvedRailT1.changeNature(GL_TRIANGLE_STRIP);
		curvedRailI1.changeNature(GL_TRIANGLE_STRIP);
		curvedRailO1.changeNature(GL_TRIANGLE_STRIP);
		curvedRailB2.changeNature(GL_TRIANGLE_STRIP);
		curvedRailT2.changeNature(GL_TRIANGLE_STRIP);
		curvedRailI2.changeNature(GL_TRIANGLE_STRIP);
		curvedRailO2.changeNature(GL_TRIANGLE_STRIP);
		curvedRailB1.drawShape();
		curvedRailT1.drawShape();
		curvedRailI1.drawShape();
		curvedRailO1.drawShape();
		curvedRailB2.drawShape();
		curvedRailT2.drawShape();
		curvedRailI2.drawShape();
		curvedRailO2.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	std::vector<float> angle {M_PI/12,M_PI/4,5*M_PI/12};
	std::vector<float> angle2 {M_PI/8,M_PI/4,3*M_PI/8};
	for(int k{0}; k<=2;++k){
		myEngine.mvMatrixStack.pushMatrix();
			Vector3D tr{2*cos(angle2[k]),2*sin(angle2[k]) ,rr};
			myEngine.mvMatrixStack.addTranslation(tr);
			Vector3D rt{0,0,1};
			myEngine.mvMatrixStack.addRotation(-M_PI/2+angle[k],rt);
			myEngine.setFlatColor(0.96,0.96,0.86);
			myEngine.updateMvMatrix();
			balast -> draw();
		myEngine.mvMatrixStack.popMatrix();
	}
}

GridConfig config {
    .size_grid = 10,
    .origin    = {-1, 0},
    .path      = {
        { 0,  0}, 
		{ 0,  1}, 
		{-1,  1},
        {-2,  1}, 
		{-3,  1},
		{-3,  0},
		{-3, -1},
        {-4, -1}, 
		{-4, -2}, 
		{-3, -2},
        {-2, -2}, 
		{-1, -2}, 
		{ 0, -2},
        { 0, -1}
    }
};

void drawCircuit() {
	int size = config.path.size();

	for(int i{0}; i<size; i++){
		int dx_entree = config.path[i].x - config.path[(i-1+size)%size].x;  //modulos pour boucler entre la premiere et la derniere case
		int dy_entree = config.path[i].y - config.path[(i-1+size)%size].y;
		int dx_sortie = config.path[(i+1)%size].x - config.path[i].x;
		int dy_sortie = config.path[(i+1)%size].y - config.path[i].y;

		if (dx_entree==dx_sortie and dy_entree==dy_sortie ){
			if(dx_entree==1 or dx_entree==-1){
				myEngine.mvMatrixStack.pushMatrix();
				Vector3D tr {static_cast<float>(config.path[i].x*10),static_cast<float>(config.path[i].y*10),0.0f};
				myEngine.mvMatrixStack.addTranslation(tr);
				Vector3D rt(0,0,1);
				myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
				Vector3D tr1 {0.0f,-10.0f,0.0f};
				myEngine.mvMatrixStack.addTranslation(tr1);
				drawStraightRail();
				myEngine.mvMatrixStack.popMatrix();

			}
			else{
				myEngine.mvMatrixStack.pushMatrix();
				Vector3D tr {static_cast<float>(config.path[i].x*10),static_cast<float>(config.path[i].y*10),0.0f};
				myEngine.mvMatrixStack.addTranslation(tr);
				drawStraightRail();
				myEngine.mvMatrixStack.popMatrix();
			}

		}
	else{
	// vient du bas, va à droite
	if(dx_entree==0 and dy_entree==1 and dx_sortie==1 and dy_sortie==0){
	
	}
	// vient du bas, va à gauche
	if(dx_entree==0 and dy_entree==1 and dx_sortie==-1 and dy_sortie==0){
		myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr {static_cast<float>(config.path[i].x*10),static_cast<float>(config.path[i].y*10),0.0f};
		myEngine.mvMatrixStack.addTranslation(tr);
		drawCurveRail();
		myEngine.mvMatrixStack.popMatrix();
	}
	// vient du haut, va à droite
	if(dx_entree==0 and dy_entree==-1 and dx_sortie==1 and dy_sortie==0){
		myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr {static_cast<float>(config.path[i].x*10)+10.0f,static_cast<float>(config.path[i].y*10)+10.0f,0.0f};
		myEngine.mvMatrixStack.addTranslation(tr);
		Vector3D rt(0,0,1);
		myEngine.mvMatrixStack.addRotation(M_PI, rt);
		drawCurveRail();
		myEngine.mvMatrixStack.popMatrix();
	}
	// vient du haut, va à gauche
	if(dx_entree==0 and dy_entree==-1 and dx_sortie==-1 and dy_sortie==0){
		myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr {static_cast<float>(config.path[i].x*10),static_cast<float>(config.path[i].y*10) +10.0f,0.0f};
		myEngine.mvMatrixStack.addTranslation(tr);
		Vector3D rt(0,0,1);
		myEngine.mvMatrixStack.addRotation(-M_PI/2, rt);
		drawCurveRail();
		myEngine.mvMatrixStack.popMatrix();
	}
	// vient de gauche, va en haut
	if(dx_entree==1 and dy_entree==0 and dx_sortie==0 and dy_sortie==1){
			myEngine.mvMatrixStack.pushMatrix();
			Vector3D tr {static_cast<float>(config.path[i].x*10),static_cast<float>(config.path[i].y*10)+10.0f,0.0f};
			myEngine.mvMatrixStack.addTranslation(tr);
			Vector3D rt(0,0,1);
			myEngine.mvMatrixStack.addRotation(-M_PI/2, rt);
			drawCurveRail();
			myEngine.mvMatrixStack.popMatrix();
	}
	// vient de gauche, va en bas
	if(dx_entree==1 and dy_entree==0 and dx_sortie==0 and dy_sortie==-1){

	}
	// vient de droite, va en haut
	if(dx_entree==-1 and dy_entree==0 and dx_sortie==0 and dy_sortie==1){

	}
	// vient de droite, va en bas
	if(dx_entree==-1 and dy_entree==0 and dx_sortie==0 and dy_sortie==-1){
		myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr {static_cast<float>(config.path[i].x*10)+10.0f,static_cast<float>(config.path[i].y*10),0.0f};
		myEngine.mvMatrixStack.addTranslation(tr);
		Vector3D rt(0,0,1);
		myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
		drawCurveRail();
		myEngine.mvMatrixStack.popMatrix();
	}
		
	
	}
	}

}

void drawGare(){
	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr{static_cast<float>(config.origin.x*10),static_cast<float>(config.origin.y*10),5.0f};
		myEngine.mvMatrixStack.addTranslation(tr);
		Vector3D hm{10.0f,10.0f,10.0f};
		myEngine.mvMatrixStack.addHomothety(hm);
		myEngine.setFlatColor(0.96,0.96,0.86);
		myEngine.updateMvMatrix();
		cube -> draw();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr1{static_cast<float>(config.origin.x*10),static_cast<float>(config.origin.y*10),10.0f};
		myEngine.mvMatrixStack.addTranslation(tr1);
		Vector3D hm1{12.0f,12.0f,3.0f};
		myEngine.mvMatrixStack.addHomothety(hm1);
		myEngine.setFlatColor(0.0f,0.0f,0.0f);
		myEngine.updateMvMatrix();
		cube -> draw();
	myEngine.mvMatrixStack.popMatrix();

}
void drawTrain(){
	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr1{static_cast<float>(config.path[0].x*10+5.0f),static_cast<float>(config.path[0].y*10+5.0f),3.0f};
		myEngine.mvMatrixStack.addTranslation(tr1);
		Vector3D hm1{4.0f,10.0f,5.0f};
		myEngine.mvMatrixStack.addHomothety(hm1);
		myEngine.setFlatColor(0.9f,0.9f,0.9f);
		myEngine.updateMvMatrix();
		cube -> draw();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr2{static_cast<float>(config.path[0].x*10+5.0f),static_cast<float>(config.path[0].y*10+5.0f),3.0f};
		myEngine.mvMatrixStack.addTranslation(tr2);
		Vector3D hm2{4.0f,10.0f,5.0f};
		myEngine.mvMatrixStack.addHomothety(hm2);
		myEngine.setFlatColor(0.9f,0.9f,0.9f);
		myEngine.updateMvMatrix();
		cube -> draw();
	myEngine.mvMatrixStack.popMatrix();

	float cx = config.path[0].x*10+5.0f;
	float cy = config.path[0].y*10+5.0f;


	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr3{cx-2.0f, cy+3.0f, 1.0f+2*rr+sr};
		myEngine.mvMatrixStack.addTranslation(tr3);
		Vector3D rt{0,0,1};
		myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
		myEngine.setFlatColor(0.2f,0.2f,0.2f);
		myEngine.updateMvMatrix();
		roue->draw();
	myEngine.mvMatrixStack.popMatrix();


	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr4{cx+2.5f, cy+3.0f, 1.0f+2*rr+sr};
		myEngine.mvMatrixStack.addTranslation(tr4);
		myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
		myEngine.setFlatColor(0.2f,0.2f,0.2f);
		myEngine.updateMvMatrix();
		roue->draw();
	myEngine.mvMatrixStack.popMatrix();


	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr5{cx-2.0f, cy-3.0f, 1.0f+2*rr+sr};
		myEngine.mvMatrixStack.addTranslation(tr5);
		myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
		myEngine.setFlatColor(0.2f,0.2f,0.2f);
		myEngine.updateMvMatrix();
		roue->draw();
	myEngine.mvMatrixStack.popMatrix();

	
	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr6{cx+2.5f, cy-3.0f, 1.0f+2*rr+sr};
		myEngine.mvMatrixStack.addTranslation(tr6);
		myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
		myEngine.setFlatColor(0.2f,0.2f,0.2f);
		myEngine.updateMvMatrix();
		roue->draw();
	myEngine.mvMatrixStack.popMatrix();
}

void drawFrame() {
	std::vector <float> origin {10.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	somePoints.initShape(origin);	
}


void drawScene() {
	/*/glPointSize(10.0);
	texSol.loadTexture();
	ground.drawShape();
	texSol.unloadTexture();/*/
	myEngine.activateTexturing(true);
		texSol.loadTexture();
		ground.drawShape();
		texSol.unloadTexture();
	myEngine.activateTexturing(false);

	myEngine.setFlatColor(0.0,1.0f,0.0);
	grid.drawShape();

	//drawStraightRail();

	//drawCurveRail();
	drawCircuit();
	drawGare();
	drawTrain();
}



