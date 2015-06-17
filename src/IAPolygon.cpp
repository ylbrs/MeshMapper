//
//  IAPolygon.cpp
//  MeshMapper
//
//  Created by plutots on 17/06/15.
//
//

#include "IAPolygon.h"
#include "constants.h"

IAPolygon::IAPolygon(){
    
    enableAllEvents();
    polygon = new ofPolyline;
}

IAPolygon::~IAPolygon(){
    
    disableAllEvents();
    polygon->clear();
    delete polygon;
    while(!iaverts.empty()) delete iaverts.back(), iaverts.pop_back();
    iaverts.clear();
}

void IAPolygon::addVertex(float x, float y){
    polygon->addVertex(x, y);
    IAVertex* v = new IAVertex;
    v->set(x, y);
    iaverts.push_back(v);
}

void IAPolygon::setup() {

}

void IAPolygon::update() {
    polygon->clear();
    list<IAVertex*>::iterator it;
    for (it=iaverts.begin(); it!=iaverts.end(); it++) {
        IAVertex* v = *it;
        polygon->addVertex(v->x,v->y);
    }
    
    if(drag){
        translate(getMouseX()-mouse_anchorX,getMouseY()-mouse_anchorY);
        mouse_anchorX = getMouseX();
        mouse_anchorY = getMouseY();
    }
    
}

void IAPolygon::draw() {
    polygon->close();
    ofPushStyle();
    ofSetLineWidth(2);
    ofNoFill();
    if(active){
        ofSetHexColor(DOWN_COLOR);
        ofLine(x, 0, x, ofGetHeight());
        ofLine(0, y, ofGetWidth(), y);
    }
    else if(isMouseOver()) ofSetHexColor(OVER_COLOR);
    else ofSetHexColor(IDLE_COLOR);
    polygon->draw();
    
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(42, 161, 152, 30);
    ofBeginShape();
    for( int i = 0; i < polygon->getVertices().size(); i++) {
        ofVertex(polygon->getVertices().at(i).x, polygon->getVertices().at(i).y);
    }
    ofEndShape();
    
    ofPopStyle();
}

void IAPolygon::translate(float dx, float dy){
    list<IAVertex*>::iterator it;
    for (it=iaverts.begin(); it!=iaverts.end(); it++) {
        IAVertex* v = *it;
        v->x+=dx;
        v->y+=dy;
    }
}

bool IAPolygon::hitTest(int tx, int ty) const {
    return (polygon->inside(tx, ty));
}

void IAPolygon::onDragOver(int x, int y, int button) {
    if (active) drag = true;
}

void IAPolygon::onPress(int x, int y, int button) {
    active = true;
    mouse_anchorX = x;
    mouse_anchorY = y;
}

void IAPolygon::onPressOutside(int x, int y, int button) {
    active = false;
}

void IAPolygon::onRelease(int x, int y, int button) {
    if (drag) drag = false;
}