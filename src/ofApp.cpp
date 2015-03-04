#include "ofApp.h"

void ofApp::setup() {
  screenWidth = ofGetWindowWidth();
  screenHeight = ofGetWindowHeight();

  inputPixels = NULL;
  maskPixels = NULL;
  outputPixels = NULL;

  loadFrames("adam_magyar_stainless01");
}

void ofApp::update() {
  loadMask();

  for (int i = 0; i < frameWidth * frameHeight; i++) {
    int frameIndex = maskPixels[i] * frameCount / 256;
    for (int c = 0; c < 3; c++) {
      outputPixels[i * 3 + c] = inputPixels[frameIndex * frameWidth * frameHeight * 3 + i * 3 + c];
    }
  }

  ofSleepMillis(100);
}

void ofApp::draw() {
  ofBackground(0);

  if (frameWidth > 0) {
    ofSetColor(255);
    distorted.setFromPixels(outputPixels, frameWidth, frameHeight, OF_IMAGE_COLOR);
    distorted.draw(0, 0);
  }
}

void ofApp::exit() {
  clearFrames();
}

void ofApp::clearFrames() {
  if (inputPixels != NULL) delete[] inputPixels;
  if (maskPixels != NULL) delete[] maskPixels;
  if (outputPixels != NULL) delete[] outputPixels;

  inputPixels = NULL;
  maskPixels = NULL;
  outputPixels = NULL;

  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;
}

void ofApp::loadFrames(string path) {
  ofImage image;

  clearFrames();

  image.loadImage(path + "/frame0001.png");
  frameWidth = image.width;
  frameHeight = image.height;
  frameCount = countFrames(path);

  cout << "Loading " << frameCount << " frames " << endl
    << "\tPath: " << path << endl
    << "\tDimensions: " << frameWidth << "x" << frameHeight << endl
    << "\tSize: " << floor(frameCount * frameWidth * frameHeight * 3 / 1024 / 1024) << " MB" << endl;

  inputPixels = new unsigned char[frameCount * frameWidth * frameHeight * 3];
  for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    image.loadImage(path + "/frame" + ofToString(frameIndex + 1, 0, 4, '0') + ".png");
    image.setImageType(OF_IMAGE_COLOR);

    for (int i = 0; i < frameWidth * frameHeight * 3; i++) {
      inputPixels[frameIndex * frameWidth * frameHeight * 3 + i] = image.getPixels()[i];
    }
  }

  maskPixels = new unsigned char[frameWidth * frameHeight * 1];
  outputPixels = new unsigned char[frameWidth * frameHeight * 3];

  cout << "Loading complete." << endl;
}

void ofApp::loadMask() {
  if (mask.loadImage("mask.png")) {
    unsigned char* loadMaskPixels = mask.getPixels();
    for (int i = 0; i < frameWidth * frameHeight; i++) {
      maskPixels[i] = loadMaskPixels[i];
    }
  }
  else {
    cout << "Warning: problem loading mask." << endl;
  }
}

void ofApp::saveDistorted() {
  distorted.setFromPixels(outputPixels, frameWidth, frameHeight, OF_IMAGE_COLOR);
  distorted.saveImage("render.jpg", OF_IMAGE_QUALITY_BEST);
}

int ofApp::countFrames(string path) {
  int n = 0;
  ofFile file;
  while (file.doesFileExist(path + "/frame" + ofToString(n + 1, 0, 4, '0') + ".png")) n++;
  return n;
}

void ofApp::keyPressed(int key) {
}

void ofApp::keyReleased(int key) {
  switch (key) {
    case ' ':
      saveDistorted();
      break;
  }
}

void ofApp::mouseMoved(int x, int y) {
}

void ofApp::mouseDragged(int x, int y, int button) {
}

void ofApp::mousePressed(int x, int y, int button) {
}

void ofApp::mouseReleased(int x, int y, int button) {
}

void ofApp::windowResized(int w, int h) {
}

void ofApp::gotMessage(ofMessage msg) {
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
}
