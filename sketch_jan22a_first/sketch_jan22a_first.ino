#include<string.h>
#include<stdio.h>
#include<SD.h>
#include<SPI.h>
#include<LiquidCrystal.h>
#include<TMRpcm.h>
#define SPEED_LOW 7
#define SPEED_HIGH 2
#define LEVEL_1_SCORE 20
#define LEVEL_2_SCORE 40
#define LEVEL_3_SCORE 55
#define LEVEL_4_SCORE 70
#define LEVEL_5_SCORE 80
#define RS 24
#define EN 25
#define D4 26
#define D5 27
#define D6 28
#define D7 29
#define SD_CARD_CS 53
#define ROW_HIGH 49
#define ROW_LOW 45
#define COL_HIGH 37
#define COL_LOW 30
#define ROW_ENABLE 44
#define X_THUMB A0
#define Y_THUMB A1
#define THUMB_PRESS 21
void setPinMode(int highPin, int lowPin, int MODE);
void digitalWriteByte(int highPin, int lowPin, unsigned char byte, int highBit = 7, int lowBit = 0);
class LEDMatrix;

LEDMatrix* matrix;
File file;
LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);
TMRpcm tmrpcm;

void setPinMode(int highPin, int lowPin, int MODE) {
  for (int i=lowPin;i<=highPin;i++) {
    pinMode(i,MODE);
  }
}
void digitalWriteByte(int highPin, int lowPin, unsigned char byte, int highBit = 7, int lowBit = 0) {
    if (highPin-lowPin+1!=highBit-lowBit+1) return;
    unsigned char ch = 1;
    for (int i=lowPin;i<=highPin;i++) {
      // assigning output bit
      digitalWrite(i,(byte&ch?HIGH:LOW));
      ch = ch<<1;
    }
}

//new icons
byte newgame_Left[8] = {
  B00001,
  B00011,
  B10111,
  B10011,
  B10001,
  B10000,
  B10000,
  B11111
};

byte newgame_Right[8] = {
  B00000,
  B00000,
  B11111,
  B00001,
  B00001,
  B00001,
  B00001,
  B11111
};


byte continue_Left[8] = {
  B01100,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01100
};


byte continue_Right[8] = {
  B00000,
  B00000,
  B11000,
  B11110,
  B11110,
  B11000,
  B00000,
  B00000
};


byte highscore_Left[8] = {
  B00000,
  B00001,
  B11100,
  B10100,
  B00100,
  B11101,
  B10000,
  B11100
};


byte highscore_Right[8] = {
  B10000,
  B10000,
  B10000,
  B10111,
  B10001,
  B10111,
  B00001,
  B00111
};


byte highscore_Left1[8] = {
  B11110,
  B11110,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};


byte highscore_Right1[8] = {
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B00001,
  B00001,
  B00001
};


byte soundsign[8]={

  B00001,
  B00011,
  B00111,
  B11111,
  B11111,
  B00111,
  B00011,
  B00001
  
};


byte soundoffsign[8]={

  B00000,
  B10001,
  B01010,
  B00100,
  B00100,
  B01010,
  B10001,
  B00000
  
};

byte soundonsign[8]={

  B01000,
  B00100,
  B10010,
  B01001,
  B01001,
  B10010,
  B00100,
  B01000,

};


byte entersign[8] = {
  B00000,
  B00001,
  B00101,
  B01101,
  B11111,
  B01100,
  B00100,
  B00000
};


byte backspacesign[8] = {
  B00000,
  B00000,
  B00100,
  B01100,
  B11111,
  B01100,
  B00100,
  B00000
};

class Piece {
public:
  unsigned char* pieceBoard;
  int rowSize;
  int colSize;
  int originR;
  int originC;
  int id;
  String name;
  int rotateOrientation;
  Piece() {
    rotateOrientation = 0;
    rowSize = 4;
    colSize = 4;
    originR = 0;
    originC = 5;
    pieceBoard = new unsigned char[rowSize];
  }
  virtual ~Piece() {
    delete [] pieceBoard;
  }
  unsigned char* getPieceBoard() {
    return pieceBoard;
  }
  int getId() {
    return id;
  }
  String getName() {
    return name;
  }
  int getRowSize() {
    return rowSize;
  }
  int getColSize() {
    return colSize;
  }
  int getOriginR() {
    return originR;
  }
  int getOriginC() {
    return originC;
  }
  void setOriginR(int value) {
    originR = value;
  }
  void setOriginC(int value) {
    originC = value;
  }
  void rotatePieceBoard() {
    // we always rotate to the left
    rotateOrientation = (rotateOrientation+1)%4;
    unsigned char* temp = new unsigned char[rowSize];
    for (int i=0;i<rowSize;i++) {
      temp[i] = 0b00000000;
      for (int j=0;j<colSize;j++) {
        if (pieceBoard[j]&(1<<(rowSize-i-1))) {
          temp[i] |= (1<<j);
        }
      }
    }
    delete [] pieceBoard;
    pieceBoard = temp;
  }
  unsigned char* getOneStepRotatedPieceBoard() {
    // we always rotate to the left
    unsigned char* temp = new unsigned char[rowSize];
    for (int i=0;i<rowSize;i++) {
      temp[i] = 0b00000000;
      for (int j=0;j<colSize;j++) {
        if (pieceBoard[j]&(1<<(rowSize-i-1))) {
          temp[i] |= (1<<j);
        }
      }
    }
    return temp;
  }
};
class ZPiece: public Piece {
private:
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00001100;
    pieceBoard[2] = 0b00000110;
    pieceBoard[3] = 0b00000000;
  }
public:
  ZPiece(int randomNumber) {
    id = 0;
    name = "Z-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~ZPiece() {

  }
  
};
class SPiece: public Piece {
private:
  void setInitialPieceBoard() {
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000110;
    pieceBoard[2] = 0b00001100;
    pieceBoard[3] = 0b00000000;
  }
public:
  SPiece(int randomNumber) {
    id = 1;
    name = "S-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~SPiece() {

  }
  
};
class TPiece: public Piece {
private:
  void setInitialPieceBoard() {
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00001110;
    pieceBoard[2] = 0b00000100;
    pieceBoard[3] = 0b00000000;
  }
public:
  TPiece(int randomNumber) {
    id = 2;
    name = "T-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~TPiece() {

  }
  
};
class JPiece: public Piece {
private:
  void setInitialPieceBoard() {
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000010;
    pieceBoard[2] = 0b00000010;
    pieceBoard[3] = 0b00000110;
  }
public:
  JPiece(int randomNumber) {
    id = 3;
    name = "J-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~JPiece() {

  }
  
};
class LPiece: public Piece {
private:
  void setInitialPieceBoard() {
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000100;
    pieceBoard[2] = 0b00000100;
    pieceBoard[3] = 0b00000110;
  }
public:
  LPiece(int randomNumber) {
    id = 4;
    name = "L-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~LPiece() {

  }
  
};
class OPiece: public Piece {
private:
  void setInitialPieceBoard() {
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000110;
    pieceBoard[2] = 0b00000110;
    pieceBoard[3] = 0b00000000;
  }
public:
  OPiece(int randomNumber) {
    id = 5;
    name = "O-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~OPiece() {

  }
  
};
class IPiece: public Piece {
private:
  void setInitialPieceBoard() {
    pieceBoard[0] = 0b00000100;
    pieceBoard[1] = 0b00000100;
    pieceBoard[2] = 0b00000100;
    pieceBoard[3] = 0b00000100;
  }
public:
  IPiece(int randomNumber) {
    id = 6;
    name = "I-Shape";
    setInitialPieceBoard();
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~IPiece() {

  }
  
};

class PieceFactory {
public:
  static Piece* getPiece(int id,int rotOri) {
    if (id==0) return new ZPiece(rotOri);
    else if (id==1) return new SPiece(rotOri);
    else if (id==2) return new TPiece(rotOri);
    else if (id==3) return new JPiece(rotOri);
    else if (id==4) return new LPiece(rotOri);
    else if (id==5) return new OPiece(rotOri);
    else if (id==6) return new IPiece(rotOri);
    return new ZPiece(rotOri);
  }
};
class ThumbController {
public:
  bool hasComeToMiddle;
  int xCount;
  int yCount;
  int xTotalValue;
  int yTotalValue;
  int xVal;
  int yVal;
  bool pressed;
  bool thumbUnpressed;
  ThumbController() {
    xCount = yCount = xTotalValue = yTotalValue = xVal = yVal = 0;
    hasComeToMiddle = true;
    pressed = true;
    thumbUnpressed = true;
  }
  void reset() {
    xCount = yCount = xTotalValue = yTotalValue = xVal = yVal = 0;
    pressed = true;
  }
};
class LEDMatrix {
public:
  String highScorer;
  int32_t highScore;
  int32_t score;
  int fallSpeed;
  int divSecCount;
  ThumbController* thumbController;
  unsigned char* displayBoard;
  int rowSize;
  int colSize;
  int buffer;
  int rotateOffset;
  Piece* currentPiece;
  Piece* nextPiece;
  bool isGameOver;
  bool isContinueAvailable;
  bool inMenu;
  bool isMusicOff;
  bool enteringName;
  int currentMenuPosition;
  Piece* generateRandomPiece() {
    // there are 7 pieces in total
    int randomNumber = random(7);
    int rotOri = random(4);
    return PieceFactory::getPiece(randomNumber,rotOri);     
  }
  void printCurrentPiece() {
    if (!isMusicOff) {
      ////tmrpcm.stopPlayback();
      tmrpcm.play("music/brickFall2.wav");
      //delay(5000);
    }
    for (int i=0;i<currentPiece->getRowSize();i++) {
      if ((currentPiece->getOriginC()-currentPiece->getColSize()+1)<0) {
        displayBoard[currentPiece->getOriginR()+i] |= (currentPiece->getPieceBoard()[i]>>(-(currentPiece->getOriginC()-currentPiece->getColSize()+1)));
      }
      else {
        displayBoard[currentPiece->getOriginR()+i] |= (currentPiece->getPieceBoard()[i]<<(currentPiece->getOriginC()-currentPiece->getColSize()+1));
      }
      
    }
    delete currentPiece;
    currentPiece = nextPiece;
    nextPiece = generateRandomPiece();
  }
  bool permitMoveDown() {
    bool ok = true;
    for (int i=currentPiece->getRowSize()-1;i>=0;i--) {
      unsigned char tempPieceRow = 0b00000000;
      if ((currentPiece->getOriginC()-currentPiece->getColSize()+1)<0) {
        tempPieceRow |= (currentPiece->getPieceBoard()[i]>>(-(currentPiece->getOriginC()-currentPiece->getColSize()+1)));
      }
      else {
        tempPieceRow |= (currentPiece->getPieceBoard()[i]<<(currentPiece->getOriginC()-currentPiece->getColSize()+1));
      }
      if((currentPiece->getOriginR()-buffer+i+1>=rowSize && tempPieceRow) || (displayBoard[currentPiece->getOriginR()+i+1]&tempPieceRow)) {
        ok = false;
        break;
      }
    }
    return ok;
  }
  bool permitMoveLeft() {
    bool ok = true;
    for (int i=currentPiece->getColSize()-1;i>=0;i--) {
      unsigned char tempPieceCol = 0b00000000;
      unsigned char tempDisplayCol = 0b00000000;
      for (int j=0;j<currentPiece->getRowSize();j++) {
        if (currentPiece->getPieceBoard()[j]&(1<<i)) {
          tempPieceCol |= (1<<(currentPiece->getRowSize()-j-1));
        }
        if (displayBoard[currentPiece->getOriginR()+j]&(1<<(currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)+1))) {
          tempDisplayCol |= (1<<(currentPiece->getRowSize()-j-1));
        }        
      }
      if ((currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)+1>=colSize && tempPieceCol) || tempPieceCol&tempDisplayCol) {
        ok = false;
        break;
      }
    }
    return ok;
  }
  bool permitMoveRight() {
    bool ok = true;
    for (int i=0;i<currentPiece->getColSize();i++) {
      unsigned char tempPieceCol = 0b00000000;
      unsigned char tempDisplayCol = 0b00000000;
      for (int j=0;j<currentPiece->getRowSize();j++) {
        if (currentPiece->getPieceBoard()[j]&(1<<i)) {
          tempPieceCol |= (1<<(currentPiece->getRowSize()-j-1));
        }
        if ((currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1)<0) {
          if (displayBoard[currentPiece->getOriginR()+j]&(1>>(-(currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1)))) {
            tempDisplayCol |= (1<<(currentPiece->getRowSize()-j-1));
          }
        }
        else {
          if (displayBoard[currentPiece->getOriginR()+j]&(1<<(currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1))) {
            tempDisplayCol |= (1<<(currentPiece->getRowSize()-j-1));
          }
        }
        if (displayBoard[currentPiece->getOriginR()+j]&(((currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1)<0?(1>>1):1<<(currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1)))) {
          tempDisplayCol |= (1<<(currentPiece->getRowSize()-j-1));
        }        
      }
      if ((currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1<0 && tempPieceCol) || tempPieceCol&tempDisplayCol) {
        ok = false;
        break;
      }
    }
    return ok;
  }
  bool permitActionRotateAdvanced() {
    rotateOffset = 0;
    bool ok = true;
    unsigned char* spacemap = new unsigned char[rowSize];
    int irow,icol,crow,ccol;
    for (int i=0;i<rowSize;i++) {
      spacemap[i] = 0b00000000;
    }
    int colHigh,colLow,rowHigh,rowLow;
    colLow = colSize;
    colHigh = -1;
    rowLow = rowSize+buffer;
    rowHigh = -1;
    for (int i=0;i<currentPiece->getRowSize();i++) {
      for (int j=0;j<currentPiece->getColSize();j++) {
        if (currentPiece->getPieceBoard()[i]&(1<<j)) {
          irow = i;
          icol = j;
          crow = currentPiece->getRowSize()-icol-1;
          ccol = irow;
          rowLow = min(rowLow,currentPiece->getOriginR()+irow);
          rowHigh = max(rowHigh,currentPiece->getOriginR()+irow);
          rowLow = min(rowLow,currentPiece->getOriginR()+crow);
          rowHigh = max(rowHigh,currentPiece->getOriginR()+crow);
          colLow = min(colLow,currentPiece->getOriginC()-currentPiece->getColSize()+1+icol);
          colHigh = max(colHigh,currentPiece->getOriginC()-currentPiece->getColSize()+1+icol);
          colLow = min(colLow,currentPiece->getOriginC()-currentPiece->getColSize()+1+ccol);
          colHigh = max(colHigh,currentPiece->getOriginC()-currentPiece->getColSize()+1+ccol);      
          if (crow<=irow) {
            if (ccol>icol) {
                for (;icol<=ccol;icol++) spacemap[irow] |= (1<<icol);
                icol = ccol;
            }
            if (ccol<=icol) {
                for (;irow>=crow;irow--) spacemap[irow] |= (1<<icol);
                irow = crow;
            }
            if (ccol<icol) {
                for (;icol>=ccol;icol--) spacemap[irow] |= (1<<icol);
                icol = ccol;
            }
          }
          else {
            if (ccol<icol) {
                for (;icol>=ccol;icol--) spacemap[irow] |= (1<<icol);
                icol = ccol;
            }
            if (ccol>=icol) {
                for (;irow<=crow;irow++) spacemap[irow] |= (1<<icol);
                irow = crow;
            }
            if (ccol>icol) {
                for (;icol<=ccol;icol++) spacemap[irow] |= (1<<icol);
                icol = ccol;
            }
          }
        }
      }
    }
    if ((rowLow>=0 && rowHigh<=rowSize+buffer-1 && rowLow<=rowHigh) && (colLow<=colHigh)) {
      if (colLow<0) {
        rotateOffset = colLow;
      }
      else if (colHigh>colSize-1) {
        rotateOffset = colHigh-colSize+1;
      }
      unsigned char temp;
      int shift;
      for (int i=0;i<currentPiece->getRowSize();i++) {
        shift = currentPiece->getOriginC()-currentPiece->getColSize()+1-rotateOffset;
        temp = (shift<0?spacemap[i]>>(-shift):spacemap[i]<<shift);
        if (displayBoard[currentPiece->getOriginR()+i]&temp) {
          ok = false;
          break;
        }
      }
    }
    else {
      ok = false;
    }
    return ok;    
  }
  bool permitActionRotate() {
    rotateOffset = 0;
    bool ok = true;
    int colHigh,colLow,rowHigh,rowLow;
    colLow = colSize;
    colHigh = -1;
    rowLow = rowSize+buffer;
    rowHigh = -1;
    for (int i=0;i<currentPiece->getRowSize();i++) {
      if (currentPiece->getPieceBoard()[i]) {
        rowLow = min(rowLow,currentPiece->getOriginR()+i);
        rowHigh = max(rowHigh,currentPiece->getOriginR()+i);
      }
    }
    for (int i=0;i<currentPiece->getColSize();i++) {
      unsigned char tempPieceCol = 0b00000000;
      for (int j=0;j<currentPiece->getRowSize();j++) {
        if (currentPiece->getPieceBoard()[j]&(1<<i)) {
          tempPieceCol |= (1<<(currentPiece->getRowSize()-j-1));
        }
      }
      if (tempPieceCol) {
        colLow = min(colLow,currentPiece->getOriginC()-currentPiece->getColSize()+1+i);
        colHigh = max(colHigh,currentPiece->getOriginC()-currentPiece->getColSize()+1+i);
      }
    }
    unsigned char* rotatedPieceBoard = currentPiece->getOneStepRotatedPieceBoard();
    for (int i=0;i<currentPiece->getRowSize();i++) {
      if (rotatedPieceBoard[i]) {
        rowLow = min(rowLow,currentPiece->getOriginR()+i);
        rowHigh = max(rowHigh,currentPiece->getOriginR()+i);
      }
    }
    for (int i=0;i<currentPiece->getColSize();i++) {
      unsigned char tempPieceCol = 0b00000000;
      for (int j=0;j<currentPiece->getRowSize();j++) {
        if (rotatedPieceBoard[j]&(1<<i)) {
          tempPieceCol |= (1<<(currentPiece->getRowSize()-j-1));
        }
      }
      if (tempPieceCol) {
        colLow = min(colLow,currentPiece->getOriginC()-currentPiece->getColSize()+1+i);
        colHigh = max(colHigh,currentPiece->getOriginC()-currentPiece->getColSize()+1+i);
      }
    }
    if ((rowLow>=0 && rowHigh<=rowSize+buffer-1 && rowLow<=rowHigh) && (colLow>=0 && colHigh<=colSize-1 && colLow<=colHigh)) {
      for (int i=rowLow;i<=rowHigh;i++) {
        for (int j=colLow;j<=colHigh;j++) {
          if (displayBoard[i]&(1<<j)) {
            ok = false;
            return ok;
          }
        }
      }
    }
    else {
      ok = false;
    }    
    return ok;
  }
  void removeFilledRowsAndAddScore() {
    // check error in multiple row?
    int removeRowCount = 0;
    bool recheck;
    for (int i=rowSize+buffer-1;i>=0;i--) {
      recheck = false;
      if (i-removeRowCount>=0 && displayBoard[i-removeRowCount]==0b11111111) {
        removeRowCount++;
        recheck = true;
      }
      if (i-removeRowCount>=0) displayBoard[i]=displayBoard[i-removeRowCount];
      else displayBoard[i] = 0b00000000;
      if (recheck) i++;
    }
    if (!isMusicOff) {
      if (removeRowCount>=1) {
        //tmrpcm.stopPlayback();
        tmrpcm.play("gameRotate.wav");
      }
    }
    score+=removeRowCount*(removeRowCount+1);
    if (score>LEVEL_5_SCORE) fallSpeed = 2;
    else if (score>LEVEL_4_SCORE) fallSpeed = 3;
    else if (score>LEVEL_3_SCORE) fallSpeed = 4;
    else if (score>LEVEL_2_SCORE) fallSpeed = 5;
    else if (score>LEVEL_1_SCORE) fallSpeed = 6;
    else fallSpeed = 7;
    printScoreAndNextPieceInLCD();
  }
  bool gameOver() {
    bool over = false;
    for (int i=buffer-1;i>=0;i--) {
      if (displayBoard[i]) {
        over = true;
        break;
      }
    }
    return over;
  }
  //
  LEDMatrix() {
    highScorer = "Player";
    highScore = 0;
    fallSpeed = SPEED_LOW;
    rotateOffset = 0;
    divSecCount = 0;
    isGameOver = false;
    score = 0;
    rowSize = 24;
    colSize = 8;
    buffer = 4;
    thumbController = new ThumbController();
    displayBoard = new unsigned char[rowSize+buffer];
    isContinueAvailable = false;   
    inMenu = true;
    currentMenuPosition = 4;
    isMusicOff = false;
    enteringName = false;
  }
  bool getIsGameOver() {
    return isGameOver;
  }
  void setIsGameOver(bool val) {
    isGameOver = val;
  }
  void clearDisplayBoard() {
    for (int i=0;i<rowSize+4;i++) {
      displayBoard[i] = 0b00000000;
    }
  }
  void outputDisplayBoard() {
    unsigned char rowVal = 0;
    
    for (int i=0;i<rowSize;i++) {
      digitalWriteByte(ROW_HIGH,ROW_LOW,rowVal,4,0);    //row 5 digit for using decoder
      unsigned char temp = 0b00000000;
      if (i+buffer>=currentPiece->getOriginR() && i+buffer<=currentPiece->getOriginR()+currentPiece->getRowSize()-1) {
        if ((currentPiece->getOriginC()-currentPiece->getColSize()+1)<0) {
          temp |= (currentPiece->getPieceBoard()[i+buffer-currentPiece->getOriginR()]>>(-(currentPiece->getOriginC()-currentPiece->getColSize()+1)));
        }
        else {
          temp |= (currentPiece->getPieceBoard()[i+buffer-currentPiece->getOriginR()]<<(currentPiece->getOriginC()-currentPiece->getColSize()+1));
        }
      }
      digitalWriteByte(COL_HIGH,COL_LOW,displayBoard[i+buffer]|temp);
      digitalWrite(ROW_ENABLE,HIGH);
      // for (int j=0;j<8;j++) {
      //   digitalWriteByte(COL_HIGH,COL_LOW,displayBoard[i+buffer]&(1<<j));
      //   delay(100);
      // }
      delay(.9);
      digitalWrite(ROW_ENABLE,LOW);
      rowVal++;
    }
  }
  //lcd codes
  void printScoreAndNextPieceInLCD() {
    lcd.setCursor(0,0);
    String s = "Score: ";
    s += score;
    for (int p=s.length();p<16;p++) s+=" ";
    lcd.print(s);
    lcd.setCursor(0,1);
    s = "Next: ";
    s += matrix->nextPiece->getName();
    for (int p=s.length();p<16;p++) s+=" ";
    lcd.print(s);
    
    lcd.println();
  }
  void menuSelect() {
    int show = 1;
    int cnt = 0;
    while (inMenu) {
      lcd.setCursor(0,0);
      if (isContinueAvailable && currentMenuPosition==0) {
        lcd.print("Resume          ");
      }
      else if (currentMenuPosition==4) {
        lcd.print("New Game        ");
      }
      else if (currentMenuPosition==8) {
        String s = "Music: ";
        if (isMusicOff) {
          s += "Off";
        }
        else {
          s += "On ";
        }
        s += "      ";
        lcd.print(s);
      }
      else if (currentMenuPosition==12) {
        if (cnt<5) {
          String s = "High Score: ";
          s += highScore;
          for (int p=s.length();p<16;p++) s+=" ";
          lcd.print(s);
        }
        else {
          String s = "By ";
          s += highScorer;
          for (int p=s.length();p<16;p++) s+=" ";
          lcd.print(s);
        }
      }
      if (show==1) {
        lcd.setCursor(0, 1);
        if (isContinueAvailable) {
          lcd.write((uint8_t)0);
          lcd.write(1);
        }
        else {
          lcd.write("  ");
        }
        lcd.print("  ");
        lcd.setCursor(4, 1);
        lcd.write(2);
        lcd.write(3);
        lcd.print("  ");
        lcd.setCursor(8, 1);
        lcd.write(4);
        lcd.write(5);
        lcd.print("  ");
        lcd.setCursor(12, 1);
        lcd.write(6);
        lcd.write(7);
        lcd.print("  ");
      }
      else {
        lcd.setCursor(currentMenuPosition, 1);
        lcd.print("  ");
      }
      delay(300);
      show *= -1;
      cnt = (cnt+1)%10;
    }
  }
  void initializeNewGame() {
    clearDisplayBoard();
    currentPiece = generateRandomPiece();
    nextPiece = generateRandomPiece();
    score = 0;
    fallSpeed = SPEED_LOW;
    rotateOffset = 0;
    divSecCount = 0;
    isGameOver = false;
    isContinueAvailable = true;    
  }
  void playGame() {
    while (true) {
      TCCR3A = 0;
      TCCR3B = 0;
      OCR3A = 1562;
      TCCR3B |= (1 << WGM32);
      TCCR3B |= (1 << CS32) | (1 << CS30);  
      TIMSK3 |= (1 << OCIE3A);

      sei();
      TCNT3  = 0;

      menuSelect();
      cli();
      //clear save state file
      file = SD.open("state.txt", O_TRUNC | FILE_WRITE);
      if (file) file.close();

      printScoreAndNextPieceInLCD();
      TCCR3A = 0;
      TCCR3B = 0;
      OCR3A = 1562;
      TCCR3B |= (1 << WGM32);
      TCCR3B |= (1 << CS32) | (1 << CS30);  
      
      TIMSK3 |= (1 << OCIE3A);

      sei();
      TCNT3  = 0;
  
      while(!inMenu && !isGameOver) {
        outputDisplayBoard();
      }
      cli(); 
      if (isGameOver) {
        if (score>highScore) {
          TCCR3A = 0;
          TCCR3B = 0;
          OCR3A = 1562;
          TCCR3B |= (1 << WGM32);
          TCCR3B |= (1 << CS32) | (1 << CS30);  
          TIMSK3 |= (1 << OCIE3A);
          sei();
          TCNT3  = 0;
          highScorer = "";
          currentMenuPosition = 0;
          int show = 1;
          char ch;
          enteringName = true;
          lcd.createChar(0,entersign);
          lcd.createChar(1, backspacesign);         
          while (enteringName) {
            lcd.setCursor(0, 0);
            lcd.print("Enter name:     ");
            if (show==1) {
              lcd.setCursor(0, 1);
              lcd.print(highScorer);
              if (currentMenuPosition==52) {
                lcd.write((uint8_t)0);
              }
              else if (currentMenuPosition==53) {
                lcd.write(1);
              }
              else {
                ch = (currentMenuPosition&1?'a':'A');
                ch += currentMenuPosition/2;
                lcd.print(ch);
              }
              String s = "";
              for (int p=highScorer.length()+1;p<16;p++) s+= " ";
              lcd.print(s);
            }
            else {
              lcd.setCursor(highScorer.length(), 1);
              lcd.print(" ");
            }
            delay(400);
            show *= -1;
          }
          cli();
          lcd.createChar(0, continue_Left);
          lcd.createChar(1, continue_Right);
          highScore = score;
          file = SD.open("score.txt", O_TRUNC | FILE_WRITE);
          if (file) {
            file.println(highScorer);
            file.println(highScore);
            file.close();
          }
        }
        isContinueAvailable = false;
        currentMenuPosition = 4;
        inMenu = true;
      }
      else {
        // save state
        file = SD.open("state.txt", O_TRUNC | FILE_WRITE);
        if (file) {
          file.println(score);
          file.println(fallSpeed);
          file.println(divSecCount);
          file.println(currentPiece->getId());
          file.println(currentPiece->rotateOrientation);
          file.println(currentPiece->getOriginR());
          file.println(currentPiece->getOriginC());
          file.println(nextPiece->getId());
          file.println(nextPiece->rotateOrientation);
          file.println(isGameOver);
          for (int i=0;i<rowSize+buffer;i++) {
            file.println(displayBoard[i]);
          }
          file.close();
        }
      }
    }
  }
};

ISR(TIMER3_COMPA_vect) {
  cli();
  matrix->divSecCount++;
  if (!matrix->inMenu) {
    if (!matrix->enteringName) {
      if (matrix->divSecCount%matrix->fallSpeed==0) {
        matrix->divSecCount = 0;
        if (matrix->permitMoveDown()) {
          matrix->currentPiece->setOriginR(matrix->currentPiece->getOriginR()+1);
        }
        else {
          matrix->printCurrentPiece();
          matrix->removeFilledRowsAndAddScore();
          if (matrix->gameOver()) {
            matrix->setIsGameOver(true);
            if (!matrix->isMusicOff) {
              tmrpcm.play("caution.wav");
            }
          }
        }
      }
      matrix->thumbController->xVal = analogRead(X_THUMB);
      matrix->thumbController->yVal = analogRead(Y_THUMB);
      matrix->thumbController->pressed = digitalRead(THUMB_PRESS);
      if (matrix->thumbController->pressed) {
        matrix->thumbController->thumbUnpressed = true;
      }
      if (matrix->thumbController->xVal<600 && matrix->thumbController->xVal>400) {
        matrix->thumbController->hasComeToMiddle = true;
      }
      matrix->thumbController->yCount++;
      matrix->thumbController->yTotalValue += matrix->thumbController->yVal;
      matrix->thumbController->xCount++;
      matrix->thumbController->xTotalValue += matrix->thumbController->xVal;
      int yAvg = matrix->thumbController->yTotalValue/matrix->thumbController->yCount;
      int xAvg = matrix->thumbController->xTotalValue/matrix->thumbController->xCount;
      if (!matrix->thumbController->pressed) {
        if (matrix->thumbController->thumbUnpressed) {
          matrix->inMenu = true;
          matrix->currentMenuPosition = 0;
          matrix->thumbController->thumbUnpressed = false;
        }
      }
      else if (abs(yAvg-504)>abs(xAvg-504)) {
        if (yAvg>650) {
          if (matrix->permitMoveLeft()) {
            matrix->currentPiece->setOriginC(matrix->currentPiece->getOriginC()+1);
            if (!matrix->isMusicOff) {
              //tmrpcm.stopPlayback();
              tmrpcm.play("brickRotate2.wav");
            }
          }
        }
        else if (yAvg<350) {
          if (matrix->permitMoveRight()) {
            matrix->currentPiece->setOriginC(matrix->currentPiece->getOriginC()-1);
            if (!matrix->isMusicOff) {
              //tmrpcm.stopPlayback();
              tmrpcm.play("brickRotate2.wav");
            }
          }
        }
      }
      else {
        if (xAvg<350) {
          if (matrix->thumbController->hasComeToMiddle && matrix->permitActionRotateAdvanced()) {
            matrix->currentPiece->rotatePieceBoard();
            matrix->currentPiece->setOriginC(matrix->currentPiece->getOriginC()-matrix->rotateOffset);
            matrix->thumbController->hasComeToMiddle = false;
            if (!matrix->isMusicOff) {
              //tmrpcm.stopPlayback();
              tmrpcm.play("brickRotate2.wav");
            }
          }
        }
        else if (xAvg>650) {
          if (matrix->permitMoveDown()) matrix->currentPiece->setOriginR(matrix->currentPiece->getOriginR()+1);
        }
      }
      matrix->thumbController->reset();
    }
    else {
      matrix->divSecCount = 0;
      matrix->thumbController->xVal = analogRead(X_THUMB);
      matrix->thumbController->yVal = analogRead(Y_THUMB);
      matrix->thumbController->pressed = digitalRead(THUMB_PRESS);
      if (matrix->thumbController->yVal<600 && matrix->thumbController->yVal>400) {
        matrix->thumbController->hasComeToMiddle = true;
      }
      if (matrix->thumbController->pressed) {
        matrix->thumbController->thumbUnpressed = true;
      }
      matrix->thumbController->yCount++;
      matrix->thumbController->yTotalValue += matrix->thumbController->yVal;
      matrix->thumbController->xCount++;
      matrix->thumbController->xTotalValue += matrix->thumbController->xVal;
      int yAvg = matrix->thumbController->yTotalValue/matrix->thumbController->yCount;
      int xAvg = matrix->thumbController->xTotalValue/matrix->thumbController->xCount;
      if (!matrix->thumbController->pressed) {
        if (matrix->thumbController->thumbUnpressed) {
          if (matrix->currentMenuPosition==52) {
            matrix->enteringName = false;
            if (matrix->highScorer=="") matrix->highScorer = "Player";
          }
          else if (matrix->currentMenuPosition==53) {
            if (matrix->highScorer.length()>0) matrix->highScorer = matrix->highScorer.substring(0,matrix->highScorer.length()-1);  //check for 1 length name
          }
          else {
            if (matrix->highScorer.length()<12) {
              char ch = (matrix->currentMenuPosition&1?'a':'A');
              ch += matrix->currentMenuPosition/2;
              matrix->highScorer += ch;           
            }
          }
          matrix->currentMenuPosition = 0;
          matrix->thumbController->thumbUnpressed = false;
        }
      }
      else if (abs(yAvg-504)>abs(xAvg-504)) {
        if (matrix->thumbController->hasComeToMiddle) {
          if (yAvg>650) {
            //left
            matrix->currentMenuPosition = (matrix->currentMenuPosition-1+54)%54;
            matrix->thumbController->hasComeToMiddle = false;
          }
          else if (yAvg<350) {
            // right
            matrix->currentMenuPosition = (matrix->currentMenuPosition+1)%54;
            matrix->thumbController->hasComeToMiddle = false;
          }
        }
      }
      else {
        if (xAvg>650) {
          // down
          matrix->currentMenuPosition = (matrix->currentMenuPosition+1)%54;
        }
        else if (xAvg<350) {
          // up
          matrix->currentMenuPosition = (matrix->currentMenuPosition-1+54)%54;
        }
      }
      matrix->thumbController->reset();     
    }
  }
  else {

    matrix->divSecCount = 0;
    matrix->thumbController->xVal = analogRead(X_THUMB);
    matrix->thumbController->yVal = analogRead(Y_THUMB);
    matrix->thumbController->pressed = digitalRead(THUMB_PRESS);
    if (matrix->thumbController->yVal<600 && matrix->thumbController->yVal>400) {
      matrix->thumbController->hasComeToMiddle = true;
    }
    if (matrix->thumbController->pressed) {
      matrix->thumbController->thumbUnpressed = true;
    }
    matrix->thumbController->yCount++;
    matrix->thumbController->yTotalValue += matrix->thumbController->yVal;
    matrix->thumbController->xCount++;
    matrix->thumbController->xTotalValue += matrix->thumbController->xVal;
    int yAvg = matrix->thumbController->yTotalValue/matrix->thumbController->yCount;
    int xAvg = matrix->thumbController->xTotalValue/matrix->thumbController->xCount;
    if (!matrix->thumbController->pressed) {
      if (matrix->thumbController->thumbUnpressed) {
        if (matrix->currentMenuPosition==0) {
          matrix->inMenu = false;
        }
        else if (matrix->currentMenuPosition==4) {
          matrix->initializeNewGame();
          matrix->inMenu = false;
        }
        else if (matrix->currentMenuPosition==8) {
          matrix->isMusicOff = !matrix->isMusicOff;
          if (matrix->isMusicOff) {
            lcd.createChar(5,soundoffsign);
          }
          else {
            lcd.createChar(5,soundonsign);
          }
        }
        matrix->thumbController->thumbUnpressed = false;
      }
    }
    else if (abs(yAvg-504)>abs(xAvg-504)) {
      if (matrix->thumbController->hasComeToMiddle) {
        if (yAvg>650) {
          //left
          matrix->currentMenuPosition = (matrix->currentMenuPosition-4+16)%16;
          if (!matrix->isContinueAvailable && matrix->currentMenuPosition==0) matrix->currentMenuPosition = 12;
          matrix->thumbController->hasComeToMiddle = false;
        }
        else if (yAvg<350) {
          // right
          matrix->currentMenuPosition = (matrix->currentMenuPosition+4)%16;
          if (!matrix->isContinueAvailable && matrix->currentMenuPosition==0) matrix->currentMenuPosition = 4;
          matrix->thumbController->hasComeToMiddle = false;
        }
      }
    }
    matrix->thumbController->reset();
  }
  sei();
  TCNT3  = 0;
}

void setup() {
  randomSeed(analogRead(A10));
  pinMode(ROW_ENABLE,OUTPUT);
  pinMode(THUMB_PRESS,INPUT);
  setPinMode(ROW_HIGH,ROW_LOW,OUTPUT);
  setPinMode(COL_HIGH,COL_LOW,OUTPUT);
  digitalWrite(ROW_ENABLE,LOW);
  digitalWrite(THUMB_PRESS,HIGH);
  tmrpcm.speakerPin = 11;
  pinMode(12,OUTPUT);
  Serial.begin(9600);
  matrix = new LEDMatrix();
  // sd card
  if (!SD.begin(SD_CARD_CS)) {
    Serial.println("Can't open sdcard!");
  }
  else {
    file = SD.open("score.txt", FILE_WRITE);
    if (file) {
      file.seek(0);
      if (file.available()) {
        matrix->highScorer = file.readStringUntil('\n');
        matrix->highScorer = matrix->highScorer.substring(0,matrix->highScorer.length()-1);
      }
      if (file.available()) {
        matrix->highScore = file.parseInt();
      }
      file.close();
    }
    int cid,crotOri,nid,nrotOri;
    file = SD.open("state.txt", FILE_WRITE);
    if (file) {
      file.seek(0);
      int c = 0;
      if (file.available()) {
        matrix->score = file.parseInt();
        c++;
      }
      if (file.available()) {
        matrix->fallSpeed = file.parseInt();
        c++;
      }
      if (file.available()) {
        matrix->divSecCount = file.parseInt();
        c++;
      }
      if (file.available()) {
        cid = file.parseInt();
        c++;
      }
      if (file.available()) {
        crotOri = file.parseInt();
        c++;
      }
      matrix->currentPiece = PieceFactory::getPiece(cid,crotOri);
      if (file.available()) {
        matrix->currentPiece->setOriginR(file.parseInt());
        c++;
      }
      if (file.available()) {
        matrix->currentPiece->setOriginC(file.parseInt());
        c++;
      }
      if (file.available()) {
        nid = file.parseInt();
        c++;
      }
      if (file.available()) {
        nrotOri = file.parseInt();
        c++;
      }
      matrix->nextPiece = PieceFactory::getPiece(nid,nrotOri);
      if (file.available()) {
        matrix->isGameOver = file.parseInt(); // bool type
        c++;
      }
      for (int i=0;i<matrix->rowSize+matrix->buffer;i++) {
        if (file.available()) {
          matrix->displayBoard[i] = file.parseInt(); //unsigned char check
          c++;
        }
      }
      if (c==38) {
        matrix->isContinueAvailable = true;
        matrix->currentMenuPosition = 0;
      }
      file.close();
    }
  }
  //lcd
  lcd.createChar(0, continue_Left);
  lcd.createChar(1, continue_Right);
  lcd.createChar(2, newgame_Left);
  lcd.createChar(3, newgame_Right);
  lcd.createChar(4,soundsign);
  lcd.createChar(5,soundonsign);
  lcd.createChar(6, highscore_Left);
  lcd.createChar(7, highscore_Right);

  lcd.begin(16,2);
  
  tmrpcm.setVolume(7);
}

void loop() {
    matrix->playGame();
}