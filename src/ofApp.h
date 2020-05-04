#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
    void updateBackground();
    void resetStrings();
    void updateStrings(int day);
    void drawText(ofVec2f startpos);
    void drawWindVector();
    
    void getJSONData(int day);
    
    int getYears(int epoch);
    int getDays(int epoch);
    int getHours(int epoch);
    int getMinutes(int epoch);
    int getSeconds(int epoch);
    int getSecsForDays(int epoch);
    int getSecsForHours(int epoch);
    int getSecsForMinutes(int epoch);
    
    void sunriseTimeSliderChanged(int & sunriseTimeIntSlider ) ;
    void sunsetTimeSliderChanged(int & sunsetTimeIntSlider ) ;
    
    ofSoundPlayer weathermusic;
    
    ofxPanel gui;
    
    ofxToggle forceToggle;
    ofxFloatSlider windSpeedFloatSlider;
    ofxIntSlider sunriseTimeIntSlider, sunsetTimeIntSlider, windBearingIntSlider;
    
    ofxLabel screenSize;
};
