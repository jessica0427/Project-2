#include "ofApp.h"
#include "ofxJSON.h"

#include <ctime>

using namespace std;

ofColor skyblue, pink, purple, darkblue;

const int SECS_PER_MIN = 70;
const int SECS_PER_HR = 3600;
const int SECS_PER_DAY = 86400;
const int EPOCH_SECS_YEAR = 31556926;

int days = 0;
int fontSize = 5;
int currentEpoch, currentYears, currentDays, currentHour, currentMinute, currentSecond;

ostringstream dayText, timeText,
                epochTimeText, sunriseText, sunsetText, tempText,
                windSpeedText, windBearingText;
int day, hour, minute;
int sunriseEpoch, sunsetEpoch, windBearing;
float tempHigh, tempLow, temp, windSpeed;
time_t tim;
struct tm *currTime;

ofxJSONElement root;

ofTrueTypeFont helvetica, windFont;
ofVec2f textPos;

//--------------------------------------------------------------
void ofApp::setup(){
    
    weathermusic.load("weathermusic.mp3");
    weathermusic.setVolume(0.5);
    weathermusic.play();
    
    
    fontSize = ofGetHeight()/15;
    helvetica.load("HelveticaNeue.ttc", fontSize);
    textPos.set(30, 120);
    
    windFont.load("HelveticaNeue.ttc",20);
    
    skyblue.setHex(0x87CEEB);
    pink.setHex(0xf67280);
    purple.setHex(0x6c567b);
    darkblue.setHex(0x003366);
    
    string url = "https://api.darksky.net/forecast/5c68178780abb713f03251f8fb748e31/1.3521, 103.8198"; //New York
    
    bool parsingSuccessful = root.open(url);
    if (parsingSuccessful)
    {
        cout << root.getRawString(true) << endl;
    } else {
        cout << "Failed to parse JSON." << endl;
    }
    getJSONData(day);
    
    sunriseTimeIntSlider.addListener(this, &ofApp::sunriseTimeSliderChanged);
    sunsetTimeIntSlider.addListener(this, &ofApp::sunsetTimeSliderChanged);
    
    gui.setup();
    gui.add(forceToggle.setup("force values",false));
    gui.add(sunriseTimeIntSlider.setup("time of sunrise", 5, 1, 10));
    gui.add(sunsetTimeIntSlider.setup("time of sunset", 5, 1, 10));
    gui.add(windBearingIntSlider.setup("wind bearing", 45, 0, 360));
    gui.add(windSpeedFloatSlider.setup("wind speed", 5,  0.0f, 10.0f));
}


//--------------------------------------------------------------
void ofApp::update(){
    currentEpoch = time(NULL);  // get current epoch
    currentYears = getYears(currentEpoch);
    currentDays = getDays(currentEpoch) + 1;
    currentHour = getHours(currentEpoch);
    currentMinute = getMinutes(currentEpoch);
    currentSecond = getSeconds(currentEpoch);
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    resetStrings();
    
    if (!forceToggle) {
        getJSONData(day);
    }
    updateStrings(day);
    updateBackground();
    drawWindVector();
    drawText(textPos);
    gui.draw();
}

void ofApp::sunriseTimeSliderChanged(int &sunriseTimeIntSlider ) {
    sunriseEpoch = sunriseEpoch - ofMap(sunriseTimeIntSlider, 1, 10, 0, 3600); //vary by an hour
    cout << sunriseEpoch <<endl;
}
void ofApp::sunsetTimeSliderChanged(int &sunsetTimeIntSlider ) {
    sunsetEpoch = sunsetEpoch - ofMap(sunsetTimeIntSlider, 1, 10, 0, 3600); //vary by an hour
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // for keys 1-7
    if (key > '0' && key < '8') {
        resetStrings();
        day = key - 48;
        getJSONData(day);
        updateStrings(day);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
void ofApp::resetStrings() {
    dayText.str("");
    timeText.str("");
    epochTimeText.str("");
    sunriseText.str("");
    sunsetText.str("");
    tempText.str("");
    windBearingText.str("");
    windSpeedText.str("");
}
void ofApp::updateBackground() {
    ofColor dayNight = skyblue;
    int dayNightPct;
    dayNightPct = abs((sunsetEpoch - currentEpoch) / (sunsetEpoch - sunriseEpoch));
    dayNight.lerp(purple, dayNightPct);
    
    ofBackground(dayNight);
}
void ofApp::drawWindVector() {
    ofSetColor(purple);
    ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        if (forceToggle == false) {
            ofRotateDeg(-90 + windBearing);
            ofDrawRectangle(0, 0, ofMap(windSpeed, 0, 10, 50, 300), 20);
            
        } else {
            ofRotateDeg(-90 + windBearingIntSlider);
            ofDrawRectangle(0, 0, ofMap(windSpeedFloatSlider, 0, 10, 50, 300), 20);
            windBearingText.str("");
            windSpeedText.str("");
            windBearingText << "Wind Bearing: " << windBearingIntSlider ;
            windSpeedText << "Wind Speed: "<< windSpeedFloatSlider ;
        }
    windFont.drawString(windBearingText.str(), 0, - 2 * windFont.getLineHeight());
    windFont.drawString(windSpeedText.str(), 0, - windFont.getLineHeight());
        
    ofPopMatrix();
}
void ofApp::updateStrings(int day) {
    dayText << "Day "<< currentDays + day << " of 366";
    if (currentMinute < 10) {
        timeText << "Time: " << currentHour << ":0" << currentMinute;
    } else {
        timeText << "Time: " << currentHour << ":" << currentMinute;
    }
    epochTimeText << "Epoch: " << currentEpoch;
    sunriseText << "Sunrise: " << sunriseEpoch;
    sunsetText << "Sunset: " << sunsetEpoch;
    tempText << "Temperature: " << temp <<  "F";
    windBearingText << "Wind Bearing: " << windBearing ;
    windSpeedText << "Wind Speed: "<< windSpeed ;
}
void ofApp::drawText(ofVec2f startpos) {
    ofSetColor(skyblue);
    int lineHeight = helvetica.getLineHeight();
    helvetica.drawString(dayText.str(), startpos.x, startpos.y);
    helvetica.drawString(timeText.str(), startpos.x, startpos.y + 1 * lineHeight );
    helvetica.drawString(epochTimeText.str(),startpos.x,startpos.y + 2 * lineHeight);
    helvetica.drawString(sunriseText.str(), startpos.x, startpos.y + 3 * lineHeight);
    helvetica.drawString(sunsetText.str(), startpos.x, startpos.y + 4 * lineHeight);
    helvetica.drawString(tempText.str(), startpos.x, startpos.y + 5 * lineHeight);
}
void ofApp::getJSONData(int day) {
    sunriseEpoch = root["daily"]["data"][day]["sunriseTime"].asInt();
    sunsetEpoch = root["daily"]["data"][day]["sunsetTime"].asInt();
    tempHigh = root["daily"]["data"][day]["temperatureHigh"].asFloat();
    tempLow = root["daily"]["data"][day]["temperatureLow"].asFloat();
    temp = (tempHigh + tempLow) / 2 ;
    windBearing = root["daily"]["data"][day]["windSpeed"].asInt();
    windSpeed = root["daily"]["data"][day]["windSpeed"].asFloat();
}

int ofApp::getYears(int epoch) {
    return epoch / EPOCH_SECS_YEAR; //find years
}
int ofApp::getSecsForDays(int epoch) {
    return epoch % EPOCH_SECS_YEAR; //remainder after finding years
}

int ofApp::getDays(int epoch) {
    return getSecsForDays(epoch) / SECS_PER_DAY; //find days
}

int ofApp::getSecsForHours(int epoch) {
    return getSecsForDays(epoch) % SECS_PER_DAY; //remainder after finding days
}

int ofApp::getHours(int epoch) {
   return getSecsForHours(epoch) / SECS_PER_HR ; //find hours
}
int ofApp::getSecsForMinutes(int epoch) {
   return getSecsForHours(epoch) % SECS_PER_HR ; //remainder after finding hours
}

int ofApp::getMinutes(int epoch) {
   return getSecsForMinutes(epoch) / SECS_PER_MIN ; //find minutes
}

int ofApp::getSeconds(int epoch) {
    return getSecsForMinutes(epoch) % SECS_PER_MIN ; //remainder after finding minutes
}
