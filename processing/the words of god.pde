
import processing.sound.*;
SoundFile song;
FFT fft;

void setup() {
  size(1280, 720, P3D);

  song = new SoundFile(this, "The Words of God.mp3");
  song.play();

  fft = new FFT(this, 1024);
  fft.input(song);
  
  // brownian
  for(int n = 0; n < 3; n++) {
    for(int i = 0; i < num; i++) {
      ax[n][i] = width/2;
      ay[n][i] = height/2;
    }
  }
}

boolean SW_SOUND = false,
    SW_SYNTH = false,
    SW_ROUD = false,
    SW_ROTATE = false,
    SW_3D = false;
int TIME_INTRO_END = 26 * 1000,
    TIME_SNAKE = 44 * 1000,
    TIME_VERSE1_END = 61 * 1000,
    TIME_ROTATE = 83 * 1000,
    TIME_3D = 119 * 1000,
    TIME_OUTRO = 151 * 1000;
    
float angle;
float angle2;

void draw() {
  // 
  
  // remove background
  background( map(mouseY, 0, height, 0, 255) );
  
  // set center
  translate(width/2, height/2);
  rotate(PI/2);
  
  // rotate time
  if( millis() > TIME_ROTATE ){
    angle += 0.005;
    rotate(angle);
  }
  // 3d time
  if( millis() > TIME_3D ){
    angle2 += 0.01;
    rotateY(angle2);
  } 
  
  // ffts
  fft.analyze();
  float w = height / (fft.size()/10);
  float r = 0;
  
  // fft
  float angle2 = 0.0615;
  for (int i = 0; i < fft.size()/10; i++) {
    float h = map(fft.spectrum[i], 0, 1, 0, height);
    
    // if roud
    if(h > height/5 && millis() > TIME_INTRO_END){
      SW_ROUD = true;
      r += 10;
    }
    
    // is sound
    if(h > height/30){
      SW_SOUND = true;
    }
    
    // if synth
    if(i>50 && h>height/8){
      SW_SYNTH = true;
    }
    
    pushMatrix();
    rotate(angle2*i);
    drawAnalyzer(i, height/5 +r, w, h);
    popMatrix();
  }
  
  // snake time
  if( millis() > TIME_SNAKE){
    drawBrownian(0, color(255,100,0), TIME_ROTATE-5000);
  }
  if( millis() > TIME_VERSE1_END){
    drawBrownian(1, color(100,100,0), TIME_ROTATE-5000);
  }
  // 3d time
  if( millis() > TIME_3D){
    drawBrownian(0, color(255,100,0), TIME_OUTRO);
    drawBrownian(1, color(100,100,0), TIME_OUTRO);
    drawBrownian(2, color(200,100,100), TIME_OUTRO);
  } 
  
  if(SW_SYNTH) createWave();
  drawWave();
  
  // switch init
  SW_SOUND = false;
  SW_ROUD = false;
  SW_SYNTH = false;
}

void drawAnalyzer(int i, float r, float w, float h){
  color c = color(150+i, 250-i, i);
  stroke(c);
  fill(c);
  ellipse(r,w,h,w);
}

// wave
int waveNum = 20;
boolean[] waveNow = new boolean[waveNum];
float[] waveX = new float[waveNum];
float[] waveY = new float[waveNum];
int[] waveFrame = new int[waveNum];
color[] waveColor = new color[waveNum];

void createWave(){
  boolean isClose = false;
  int no = -1;
  
  for(int i=0; i<waveNum; i++){
    if(!isClose && waveFrame[i] > 0 && waveFrame[i] < 5){
      isClose = true;
    }
    if(waveNow[i] == false){
      no = i;
    }
  }
  
  if(!isClose && no>-1){
    waveNow[no] = true;
    waveFrame[no] = 0;
    waveX[no] = random(width)-width/2;
    waveY[no] = random(height)-height/2;
    waveColor[no] = color(random(250), random(250), 100); 
  }
}
void drawWave(){
  pushStyle();
  for(int i=0; i<waveNum; i++){
    if(waveNow[i]){
      waveFrame[i]++;
      color c = waveColor[i];
      stroke(c, 255-waveFrame[i]*3);
      strokeWeight(5/log(waveFrame[i]));
      noFill();
      float r = log(waveFrame[i]) * (height/4);
      ellipse(waveX[i], waveY[i], r, r);
    }
    if(waveFrame[i]*3>255){
      waveNow[i] = false; 
    }
  }
  popStyle();
}


// Brownian
int num = 50;
int range = 100;

float[][] ax = new float[3][num];
float[][] ay = new float[3][num];
float[][] az = new float[3][num];

void drawBrownian(int n, color c, int end){
  if(millis()>end){
    for(int i = 1; i < num; i++) {
      ax[n][i-1] = ax[n][i];
      ay[n][i-1] = ay[n][i];
      az[n][i-1] = az[n][i];
    }
  }
  else if(SW_SOUND){
    for(int i = 1; i < num; i++) {
      ax[n][i-1] = ax[n][i];
      ay[n][i-1] = ay[n][i];
      az[n][i-1] = az[n][i];
    }
    // Put a new value at the end of the array
    ax[n][num-1] += random(-range, range);
    ay[n][num-1] += random(-range, range);
    az[n][num-1] += random(-range, range);
  
    // Constrain all points to the screen
    ax[n][num-1] = constrain(ax[n][num-1], 0-width/2, width/2);
    ay[n][num-1] = constrain(ay[n][num-1], 0-height/2, height/2);
    az[n][num-1] = constrain(az[n][num-1], 0-height/2, height/2);
  }
  
  pushStyle();
  strokeWeight(2);
  // Draw a line connecting the points
  for(int i=1; i<num; i++) {
    float val = float(i)/num * 204.0;
    stroke(c, val);
    line(ax[n][i-1], ay[n][i-1], az[n][i-1], ax[n][i], ay[n][i], az[n][i]);
  }
  popStyle();
}