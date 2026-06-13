#include "draw_scene.hpp"
#include "json.hpp"
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
IndexedMesh* rail;
IndexedMesh* cube;
IndexedMesh* roue;
IndexedMesh* phare;

IndexedMesh* lightSphere;         
float lightAngle {0.0f};          
bool isLightMoving {false};       
float lightX {-20.0f};             
float lightY {-5.0f};              
float lightZ {15.0f};

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

	cube = basicCube(1.0f);
	cube->createVAO();

	roue = basicSphere(1.0f, 20, 20);
	roue->createVAO();

	lightSphere = basicSphere(0.8f, 20, 20);
    lightSphere->createVAO();

	phare = basicSphere(0.3f, 10, 10);
	phare->createVAO();

	initCurvedRail(POS_X_RAIL1, curvedRailB1, curvedRailT1, curvedRailI1, curvedRailO1);
	initCurvedRail(POS_X_RAIL2, curvedRailB2, curvedRailT2, curvedRailI2, curvedRailO2);

	
    myEngine.switchToPhongShading();
    Vector4D initialLight0Pos{lightX, lightY, lightZ, 1.0f}; 
    Vector3D initialLight0Intensity{500.0f, 500.0f, 500.0f};
    myEngine.setLightPosition(initialLight0Pos, 0);
    myEngine.setLightIntensity(initialLight0Intensity, 0);

	myEngine.addALight(
    Vector4D(0.0f, -1.0f, 2.0f, 0.0f),  // vient d'en haut
    Vector3D(0.8f, 0.8f, 0.8f)           // blanc doux
	);

    myEngine.switchToFlatShading(); 
	        
}

void updateScene(float deltaTime) {
    if (isLightMoving) {
        lightAngle += 0.5f * deltaTime; 
        lightX = -20.0f + 20.0f * cosf(lightAngle);
        lightY = -5.0f + 20.0f * sinf(lightAngle);
    }
}

void handleKeyboardInput(unsigned char key) {
    if (key == 'l' || key == 'L') {
        isLightMoving = !isLightMoving;
    }
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
		Vector3D tr{0.0f, 0.0f, 2*rr};
        myEngine.mvMatrixStack.addTranslation(tr);
        myEngine.updateMvMatrix(); 
        
        myEngine.setFlatColor(0.8, 0.8, 0.8);
        curvedRailB1.changeNature(GL_TRIANGLE_STRIP);
        curvedRailT1.changeNature(GL_TRIANGLE_STRIP);
        curvedRailI1.changeNature(GL_TRIANGLE_STRIP);
        curvedRailO1.changeNature(GL_TRIANGLE_STRIP);
        curvedRailB2.changeNature(GL_TRIANGLE_STRIP);
        curvedRailT2.changeNature(GL_TRIANGLE_STRIP);
        curvedRailI2.changeNature(GL_TRIANGLE_STRIP);
        curvedRailO2.changeNature(GL_TRIANGLE_STRIP);

        // Normales des rails
        myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, -1.0f)); 
		curvedRailB1.drawShape();
		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, 1.0f));  
		curvedRailT1.drawShape();
		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, 1.0f));  // ← était (-1,0,0)
		curvedRailI1.drawShape();
		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, 1.0f));  // ← était (1,0,0)
		curvedRailO1.drawShape();

		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, -1.0f));
		curvedRailB2.drawShape();
		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, 1.0f));
		curvedRailT2.drawShape();
		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, 1.0f));  // ← était (-1,0,0)
		curvedRailI2.drawShape();
		myEngine.setNormalForConvex2DShape(Vector3D(0.0f, 0.0f, 1.0f));  // ← était (1,0,0)
		curvedRailO2.drawShape();
		myEngine.mvMatrixStack.popMatrix();

        std::vector<float> angle {M_PI/12, M_PI/4, 5*M_PI/12};
        std::vector<float> angle2 {M_PI/8, M_PI/4, 3*M_PI/8};
        
        for(int k{0}; k<=2; ++k){
        
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

GridConfig config ;

void loadCircuitFromJSON(const std::string& filename) {
    std::ifstream fichier(filename);
    if (!fichier.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        exit(EXIT_FAILURE); 
    }

    json j;
    fichier >> j; 

    config.size_grid = j["size_grid"].get<int>();
    config.origin = { j["origin"][0].get<int>(), j["origin"][1].get<int>() };

    config.path.clear();
    for (const auto& coord : j["path"]) {
        config.path.push_back({ coord[0].get<int>(), coord[1].get<int>() });
    }

    std::cout << "Succes : Le fichier " << filename << " a ete charge !" << std::endl;
}

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
		myEngine.setFlatColor(0.8f,0.6f,0.4f);
		myEngine.updateMvMatrix();
		cube -> draw();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		Vector3D tr1{static_cast<float>(config.origin.x*10),static_cast<float>(config.origin.y*10),10.0f};
		myEngine.mvMatrixStack.addTranslation(tr1);
		Vector3D hm1{12.0f,12.0f,3.0f};
		myEngine.mvMatrixStack.addHomothety(hm1);
		myEngine.setFlatColor(0.6f,0.1f,0.1f);
		myEngine.updateMvMatrix();
		cube -> draw();
	myEngine.mvMatrixStack.popMatrix();

}

void drawTrain(){
    Vector3D rt{0, 0, 1};
    Vector3D rx{1, 0, 0};
    float cx = config.path[0].x*10 + 5.0f;
    float cy = config.path[0].y*10 + 5.0f;
    float rayonRoue = 0.8f;
    float zRoue = 2*rr + sr + rayonRoue;
    float hauteurCorps = zRoue + rayonRoue;

    // === CORPS PRINCIPAL (long et bas) ===
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy, hauteurCorps + 1.0f});
        myEngine.mvMatrixStack.addHomothety(Vector3D{4.0f, 8.0f, 3.5f});
        myEngine.setFlatColor(0.1f, 0.3f, 0.7f); // bleu
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // === CABINE (carrée, à l'arrière Y+) ===
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy + 2.f, hauteurCorps + 3.5f});
        myEngine.mvMatrixStack.addHomothety(Vector3D{3.8f, 3.5f, 4.0f});
        myEngine.setFlatColor(0.1f, 0.25f, 0.6f); // bleu foncé
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // Toit cabine
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy + 2.f, hauteurCorps + 5.5f});
        myEngine.mvMatrixStack.addHomothety(Vector3D{4.2f, 4.0f, 0.4f});
        myEngine.setFlatColor(0.05f, 0.15f, 0.4f); // bleu très foncé
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // === CHAUDIÈRE (cylindre horizontal à l'avant Y-) ===
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy - 1.5f, hauteurCorps + 1.5f});
        myEngine.mvMatrixStack.addHomothety(Vector3D{0.6f, 0.6f, 1.f});
        myEngine.setFlatColor(0.15f, 0.3f, 0.65f);
        myEngine.updateMvMatrix();
        roue->draw(); // cylindre horizontal
    myEngine.mvMatrixStack.popMatrix();

    // === CHEMINÉE VERTICALE ===
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy - 1.0f, hauteurCorps + 2.5f});
        myEngine.mvMatrixStack.addRotation(M_PI/2, rx); // vertical
        myEngine.mvMatrixStack.addHomothety(Vector3D{0.5f, 0.5f, 0.5f});
        myEngine.setFlatColor(0.1f, 0.1f, 0.1f); // noir
        myEngine.updateMvMatrix();
        balast->draw();
    myEngine.mvMatrixStack.popMatrix();

    // Chapeau cheminée
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy - 1.0f, hauteurCorps + 5.5f});
        myEngine.mvMatrixStack.addHomothety(Vector3D{0.8f, 0.8f, 0.3f});
        myEngine.setFlatColor(0.1f, 0.1f, 0.1f);
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // === PARE-CHOC AVANT ===
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy - 4.f, hauteurCorps + 0.8f});
        myEngine.mvMatrixStack.addHomothety(Vector3D{4.2f, 0.2f, 1.5f});
        myEngine.setFlatColor(0.3f, 0.3f, 0.3f); // gris
        myEngine.updateMvMatrix();
        cube->draw();
    myEngine.mvMatrixStack.popMatrix();

    // === 4 ROUES ===
    std::vector<std::pair<float,float>> posRoues = {
        {cx - 2.0f, cy + 2.5f},  // arrière gauche
        {cx + 2.0f, cy + 2.5f},  // arrière droit
        {cx - 2.0f, cy - 2.5f},  // avant gauche
        {cx + 2.0f, cy - 2.5f}   // avant droit
    };
    for(auto& pos : posRoues){
        myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTranslation(Vector3D{pos.first, pos.second, zRoue});
            myEngine.mvMatrixStack.addRotation(M_PI/2, rt);
            myEngine.mvMatrixStack.addHomothety(Vector3D{rayonRoue, 0.3f, rayonRoue});
            myEngine.setFlatColor(0.1f, 0.1f, 0.1f);
            myEngine.updateMvMatrix();
            roue->draw();
        myEngine.mvMatrixStack.popMatrix();
    }

    // === PHARE AVANT ===
    myEngine.switchToFlatShading();
    myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation(Vector3D{cx, cy - 4.2f, hauteurCorps + 1.5f});
		myEngine.mvMatrixStack.addHomothety(Vector3D{3.f, 1.f, 3.f});
        myEngine.setFlatColor(1.0f, 1.0f, 0.8f);
        myEngine.updateMvMatrix();
        phare->draw();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.switchToPhongShading();
    Vector4D currentLight0Pos{lightX, lightY, lightZ, 1.0f};
    myEngine.setLightPosition(currentLight0Pos, 0);
    myEngine.setLightIntensity(Vector3D(500.0f, 500.0f, 500.0f), 0);

myEngine.mvMatrixStack.popMatrix();
}

void drawFrame() {
	std::vector <float> origin {10.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	somePoints.initShape(origin);	
}


void drawScene() {
	glPointSize(10.0);

	myEngine.switchToFlatShading();
    myEngine.setFlatColor(0.2, 0.0, 0.0);
    ground.drawShape();
    myEngine.setFlatColor(0.0, 1.0f, 0.0);
    grid.drawShape();

    myEngine.mvMatrixStack.pushMatrix();
        Vector3D trLight{lightX, lightY, lightZ};
        myEngine.mvMatrixStack.addTranslation(trLight);
        myEngine.setFlatColor(1.0f, 1.0f, 0.0f);
        myEngine.updateMvMatrix();
        lightSphere->draw();
    myEngine.mvMatrixStack.popMatrix();

	myEngine.switchToPhongShading();
	myEngine.activateTexturing(false);

    Vector4D currentLight0Pos{lightX, lightY, lightZ, 1.0f}; 
    myEngine.setLightPosition(currentLight0Pos, 0);
	myEngine.setLightIntensity(Vector3D(200.0f, 200.0f, 200.0f), 0);
    myEngine.setShininess(32.0f);
    myEngine.setSpecularColor(Vector3D(0.0f, 0.0f, 0.0f));
    drawCircuit();
	myEngine.setFlatColor(0.8f, 0.7f, 0.5f);
	drawGare();
	myEngine.setFlatColor(0.2f, 0.2f, 0.8f);
	drawTrain();

    myEngine.switchToFlatShading();
	//drawStraightRail();

	//drawCurveRail();
	
}



