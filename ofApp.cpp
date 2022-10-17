#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofNoFill();
	this->random_seed = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 60 < 5) {

		this->random_seed = ofRandom(1000);
	}

	ofSeedRandom(this->random_seed);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	auto number_of_sphere = 180;
	vector<glm::vec2> location_list;
	vector<float> radius_list;

	while (location_list.size() < number_of_sphere) {

		auto location = glm::vec2(ofRandom(-300, 300), ofRandom(-300, 300));
		auto radius = ofRandom(8, 70);
		bool flag = true;
		for (int i = 0; i < location_list.size(); i++) {

			if (glm::distance(location, location_list[i]) < (radius + radius_list[i])) {

				flag = false;
				break;
			}
		}

		if (flag) {

			location_list.push_back(location);
			radius_list.push_back(radius);
		}
	}

	int count = 0;
	while (true) {

		auto all = true;

		for (int i = 0; i < location_list.size(); i++) {

			auto flag = true;
			for (int k = 0; k < location_list.size(); k++) {

				if (i == k) { continue; }

				if (glm::distance(location_list[i], location_list[k]) < (radius_list[i] + radius_list[k])) {

					flag = false;
				}
			}

			if (flag) {

				radius_list[i] += 1;
				all = false;
				break;
			}
		}

		if (all) {

			break;
		}

		count++;
	}

	ofColor color;
	int color_param = ofRandom(1000);
	int deg_span = 3;
	for (int i = 0; i < location_list.size(); i++) {

		ofMesh mesh;
		auto param = ofGetFrameNum() % 60 < 5 ? 0.2 : ofGetFrameNum() % 60 < 15 ? ofMap(ofGetFrameNum() % 60, 5, 15, 0.2, 1) : 1;
		auto len = ofMap(param, 0.2, 1, 1, 5);

		for (int deg = 0; deg < 360; deg += deg_span) {

			mesh.addVertex(glm::vec3(location_list[i], 0) + glm::vec3(radius_list[i] * param * cos(deg * DEG_TO_RAD), radius_list[i] * param * sin(deg * DEG_TO_RAD), 0));
			mesh.addVertex(glm::vec3(location_list[i], 0) + glm::vec3(radius_list[i] * param * cos((deg + deg_span) * DEG_TO_RAD), radius_list[i] * param * sin((deg + deg_span) * DEG_TO_RAD), 0));
			mesh.addVertex(glm::vec3(location_list[i], 0) + glm::vec3((radius_list[i] * param - len) * cos((deg + deg_span) * DEG_TO_RAD), (radius_list[i] * param - len) * sin((deg + deg_span) * DEG_TO_RAD), 0));
			mesh.addVertex(glm::vec3(location_list[i], 0) + glm::vec3((radius_list[i] * param - len) * cos(deg * DEG_TO_RAD), (radius_list[i] * param - len) * sin(deg * DEG_TO_RAD), 0));

			mesh.addIndex(mesh.getNumVertices() - 1); mesh.addIndex(mesh.getNumVertices() - 2); mesh.addIndex(mesh.getNumVertices() - 3);
			mesh.addIndex(mesh.getNumVertices() - 1); mesh.addIndex(mesh.getNumVertices() - 3); mesh.addIndex(mesh.getNumVertices() - 4);
		}

		for (int k = 0; k < mesh.getNumVertices(); k++) {

			auto vertex = mesh.getVertex(k);

			color.setHsb((int)ofMap(ofNoise(glm::vec4(color_param, vertex * 0.0025)), 0, 1, 180, 280) % 255, 200, 255);
			mesh.addColor(color);

		}

		mesh.draw();
	}
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}