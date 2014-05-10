#include "ofxSurfaceGui.h"

ofxSurfaceGui::ofxSurfaceGui()
{
    surface = NULL;
    mode = NONE;
    bProjectionMappingJointSelected = false;
    bTextureMappingJointSelected = false;
}

ofxSurfaceGui::~ofxSurfaceGui()
{
    
}

void ofxSurfaceGui::setup(ofxTriangleSurface& surfaceForGui)
{
    surface = &surfaceForGui;
    addNumProjectionMappingJoints(3);
    addNumTextureMappingJoints(3);
}

void ofxSurfaceGui::update()
{
    if (surface == NULL) return;
    if (mode == NONE) return;
    
    if (mode == PROJECTION_MAPPING) {
        for ( int i=0; i<projectionMappingJoints.size(); i++ ) {
            projectionMappingJoints[i].update();
        }
    } else if (mode == TEXTURE_MAPPING) {
        for ( int i=0; i<textureMappingJoints.size(); i++ ) {
            textureMappingJoints[i].update();
        }
    }
}

void ofxSurfaceGui::draw()
{
    if (surface == NULL) return;
    if (mode == NONE) return;
    
    if (mode == PROJECTION_MAPPING) {
        ofPolyline line;
        for ( int i=0; i<projectionMappingJoints.size(); i++ ) {
            line.addVertex( ofPoint(projectionMappingJoints[i].position.x,
                                    projectionMappingJoints[i].position.y) );
        }
        line.close();
        line.draw();
        
        for ( int i=0; i<projectionMappingJoints.size(); i++ ) {
            projectionMappingJoints[i].draw();
        }
        
    } else if (mode == TEXTURE_MAPPING) {
        ofPolyline line;
        for ( int i=0; i<textureMappingJoints.size(); i++ ) {
            line.addVertex( ofPoint(textureMappingJoints[i].position.x,
                                    textureMappingJoints[i].position.y) );
        }
        line.close();
        line.draw();
        
        for ( int i=0; i<textureMappingJoints.size(); i++ ) {
            textureMappingJoints[i].draw();
        }
    }
}

void ofxSurfaceGui::mousePressed(int x, int y, int button)
{
    if (surface == NULL) return;
    if (mode == NONE) return;
    
    if (mode == PROJECTION_MAPPING) {
        bProjectionMappingJointSelected = false;
        for ( int i=0; i<projectionMappingJoints.size(); i++ ) {
            projectionMappingJoints[i].selected = false;
            projectionMappingJoints[i].mousePressed(x, y, button);
            if ( !isProjectionMappingJointSelected() && projectionMappingJoints[i].hitTest(ofVec2f(x, y)) ) {
                projectionMappingJoints[i].selected = true;
                projectionMappingJoints[i].startDrag();
                bProjectionMappingJointSelected = true;
            }
        }
    } else if (mode == TEXTURE_MAPPING) {
        bTextureMappingJointSelected = false;
        for ( int i=0; i<textureMappingJoints.size(); i++ ) {
            textureMappingJoints[i].selected = false;
            textureMappingJoints[i].mousePressed(x, y, button);
            if ( !isTextureMappingJointSelected() && textureMappingJoints[i].hitTest(ofVec2f(x, y)) ) {
                textureMappingJoints[i].selected = true;
                textureMappingJoints[i].startDrag();
                bTextureMappingJointSelected = true;
            }
        }
    }
}

void ofxSurfaceGui::mouseReleased(int x, int y, int button)
{
    if (surface == NULL) return;
    if (mode == NONE) return;
    
    if (mode == PROJECTION_MAPPING) {
        for ( int i=0; i<projectionMappingJoints.size(); i++ ) {
            projectionMappingJoints[i].mouseReleased(x, y, button);
        }
    } else if (mode == TEXTURE_MAPPING) {
        for ( int i=0; i<textureMappingJoints.size(); i++ ) {
            textureMappingJoints[i].mouseReleased(x, y, button);
        }
    }
}

void ofxSurfaceGui::mouseDragged(int x, int y, int button)
{
    if (surface == NULL) return;
    if (mode == NONE) return;
    
    if (mode == PROJECTION_MAPPING) {
        for ( int i=0; i<projectionMappingJoints.size(); i++ ) {
            projectionMappingJoints[i].mouseDragged(x, y, button);
            if ( projectionMappingJoints[i].isDragged() ) {
                surface->setVertex(i, projectionMappingJoints[i].position);
            }
        }
    } else if (mode == TEXTURE_MAPPING) {
        ofVec2f textureSize = ofVec2f( surface->getTexture()->getWidth(), surface->getTexture()->getHeight() );
        for ( int i=0; i<textureMappingJoints.size(); i++ ) {
            textureMappingJoints[i].mouseDragged(x, y, button);
            if ( textureMappingJoints[i].isDragged() ) {
                surface->setTexCoord(i, textureMappingJoints[i].position/textureSize);
            }
        }
    }
}

void ofxSurfaceGui::setMode(ofxSurfaceGui::editMode newMode)
{
    if (mode != NONE &&
        mode != PROJECTION_MAPPING &&
        mode != TEXTURE_MAPPING) {
        throw std::runtime_error("Trying to set invalid mode.");
    };
    
    mode = newMode;
}

ofxSurfaceGui::editMode ofxSurfaceGui::getMode()
{
    return mode;
}

void ofxSurfaceGui::addProjectionMappingJoint()
{
    projectionMappingJoints.push_back(ofxCircleJoint());
    projectionMappingJoints.back().position = surface->getVertex(projectionMappingJoints.size()-1);
}

void ofxSurfaceGui::addNumProjectionMappingJoints(int num)
{
    for ( int i=0; i<num; i++ ) {
        addProjectionMappingJoint();
    }
}

void ofxSurfaceGui::addTextureMappingJoint()
{
    textureMappingJoints.push_back(ofxCircleJoint());
    ofVec2f textureSize = ofVec2f(surface->getTexture()->getWidth(), surface->getTexture()->getHeight());
    textureMappingJoints.back().position = surface->getTexCoord(textureMappingJoints.size()-1) * textureSize;
}

void ofxSurfaceGui::addNumTextureMappingJoints(int num)
{
    for ( int i=0; i<num; i++ ) {
        addTextureMappingJoint();
    }
}

bool ofxSurfaceGui::isProjectionMappingJointSelected()
{
    return bProjectionMappingJointSelected;
}

bool ofxSurfaceGui::isTextureMappingJointSelected()
{
    return bTextureMappingJointSelected;
}