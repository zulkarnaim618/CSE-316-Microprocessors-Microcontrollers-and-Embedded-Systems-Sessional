#include<string.h>
#include<stdio.h>
#define FRAME_RATE 80
#define ROW_HIGH 49
#define ROW_LOW 45
#define COL_HIGH 37
#define COL_LOW 30
#define ROW_ENABLE 44
#define X_THUMB 0
#define Y_THUMB 1
#define THUMB_PRESS 22
void setPinMode(int highPin, int lowPin, int MODE);
void digitalWriteByte(int highPin, int lowPin, unsigned char byte, int highBit = 7, int lowBit = 0);
class LEDMatrix;

LEDMatrix* matrix;

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

class Piece {
protected:
  unsigned char* pieceBoard;
  int rowSize;
  int colSize;
  int originR;
  int originC;
public:
  Piece() {
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
  int id;
  String name;
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
  ZPiece() {
    id = 0;
    name = "Z-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~ZPiece() {

  }
  
};
class SPiece: public Piece {
private:
  int id;
  String name;
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000110;
    pieceBoard[2] = 0b00001100;
    pieceBoard[3] = 0b00000000;
  }
public:
  SPiece() {
    id = 1;
    name = "S-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~SPiece() {

  }
  
};
class TPiece: public Piece {
private:
  int id;
  String name;
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00001110;
    pieceBoard[2] = 0b00000100;
    pieceBoard[3] = 0b00000000;
  }
public:
  TPiece() {
    id = 2;
    name = "T-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~TPiece() {

  }
  
};
class JPiece: public Piece {
private:
  int id;
  String name;
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000010;
    pieceBoard[2] = 0b00000010;
    pieceBoard[3] = 0b00000110;
  }
public:
  JPiece() {
    id = 3;
    name = "J-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~JPiece() {

  }
  
};
class LPiece: public Piece {
private:
  int id;
  String name;
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000100;
    pieceBoard[2] = 0b00000100;
    pieceBoard[3] = 0b00000110;
  }
public:
  LPiece() {
    id = 4;
    name = "L-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~LPiece() {

  }
  
};
class OPiece: public Piece {
private:
  int id;
  String name;
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000000;
    pieceBoard[1] = 0b00000110;
    pieceBoard[2] = 0b00000110;
    pieceBoard[3] = 0b00000000;
  }
public:
  OPiece() {
    id = 5;
    name = "O-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~OPiece() {

  }
  
};
class IPiece: public Piece {
private:
  int id;
  String name;
  void setInitialPieceBoard() {
    // this is the design of the shape
    // first 4 bit are ignored, keep them 0
    // last 4 bit is the design
    // try to center the shape in the last 4*4 as much as possible
    pieceBoard[0] = 0b00000100;
    pieceBoard[1] = 0b00000100;
    pieceBoard[2] = 0b00000100;
    pieceBoard[3] = 0b00000100;
  }
public:
  IPiece() {
    id = 6;
    name = "I-Shape";
    setInitialPieceBoard();
    int randomNumber = random(4);
    for (int i=0;i<randomNumber;i++) {
      rotatePieceBoard();
    }
  }
  ~IPiece() {

  }
  
};

class PieceFactory {
public:
  static Piece* getPiece(int id) {
    if (id==0) return new ZPiece();
    else if (id==1) return new SPiece();
    else if (id==2) return new TPiece();
    else if (id==3) return new JPiece();
    else if (id==4) return new LPiece();
    else if (id==5) return new OPiece();
    else if (id==6) return new IPiece();
    return new ZPiece();
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
  ThumbController() {
    xCount = yCount = xTotalValue = yTotalValue = xVal = yVal = 0;
    hasComeToMiddle = true;
  }
  void reset() {
    xCount = yCount = xTotalValue = yTotalValue = xVal = yVal = 0;
    //Serial.print("Resetting --------------------------\n");
  }
};
class LEDMatrix {
public:
  int divSecCount;
  ThumbController* thumbController;
  unsigned char* displayBoard;
  int64_t score;
  int rowSize;
  int colSize;
  int buffer;
  int rotateOffset;
  Piece* currentPiece;
  bool isGameOver;
  Piece* generateRandomPiece() {
    // there are 7 pieces in total
    int randomNumber = random(7);
    return PieceFactory::getPiece(randomNumber);
  }
  void printCurrentPiece() {
    for (int i=0;i<currentPiece->getRowSize();i++) {
      if ((currentPiece->getOriginC()-currentPiece->getColSize()+1)<0) {
        displayBoard[currentPiece->getOriginR()+i] |= (currentPiece->getPieceBoard()[i]>>(-(currentPiece->getOriginC()-currentPiece->getColSize()+1)));
      }
      else {
        displayBoard[currentPiece->getOriginR()+i] |= (currentPiece->getPieceBoard()[i]<<(currentPiece->getOriginC()-currentPiece->getColSize()+1));
      }
      
    }
    delete currentPiece;
    currentPiece = generateRandomPiece();
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
        // if (currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)+1>=colSize && tempPieceCol) {
        //   Serial.print("Left Wall reached\n");
        //   Serial.print("Col: ");
        //   Serial.print(currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)+1);
        //   Serial.print("\n");
        // }
        // else {
        //   Serial.print("Piece: ");
        //   Serial.print(tempPieceCol);
        //   Serial.print("\nBoard: ");
        //   Serial.print(tempDisplayCol);
        //   Serial.print("\nLeft Block found\n");
        // }
        ok = false;
        break;
      }
    }
    // Serial.print("left: ");
    // Serial.print(ok);
    // Serial.print("\n");
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
          // Serial.print("Col: ");
          // Serial.print(i);
          // Serial.print("Row: ");
          // Serial.print(j);
          // Serial.print(" setting\n");
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
        // if (currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1<0 && tempPieceCol) {
        //   Serial.print("Right Wall reached\n");
        //   Serial.print(currentPiece->getOriginC());
        //   Serial.print(" ");
        //   Serial.print((currentPiece->getColSize()-i-1));
        //   Serial.print("\n");
        //   Serial.print("Col: ");
        //   Serial.print(currentPiece->getOriginC()-(currentPiece->getColSize()-i-1)-1);
        //   Serial.print("\n");
        //   Serial.print("Piece: ");
        //   Serial.print(tempPieceCol);
        //   Serial.print("\n");
          
        // }
        // else {
        //   Serial.print("Piece: ");
        //   Serial.print(tempPieceCol);
        //   Serial.print("\nBoard: ");
        //   Serial.print(tempDisplayCol);
        //   Serial.print("\nRight Block found\n");
        // }
        ok = false;
        break;
      }
    }
    // Serial.print("Right: ");
    // Serial.print(ok);
    // Serial.print("\n");
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
          // Serial.print("Row: ");
          // Serial.print(i);
          // Serial.print("\tCol: ");
          // Serial.print(j);
          // Serial.print("\tCol low: ");
          // Serial.print(colLow);
          // Serial.print("\tCol High: ");
          // Serial.println(colHigh);       
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
    // Serial.print("Col Low: ");
    // Serial.println(colLow);
    // Serial.print("Col High: ");
    // Serial.println(colHigh);
    // for (int i=0;i<currentPiece->getRowSize();i++) {
    //   if (spacemap[i]) {
    //     rowLow = min(rowLow,currentPiece->getOriginR()+i);
    //     rowHigh = max(rowHigh,currentPiece->getOriginR()+i);
    //   }
    // }
    // for (int i=0;i<currentPiece->getColSize();i++) {
    //   unsigned char tempPieceCol = 0b00000000;
    //   for (int j=0;j<currentPiece->getRowSize();j++) {
    //     if (spacemap[j]&(1<<i)) {
    //       tempPieceCol |= (1<<(currentPiece->getRowSize()-j-1));
    //     }
    //   }
    //   if (tempPieceCol) {
    //     colLow = min(colLow,currentPiece->getOriginC()-currentPiece->getColSize()+1+i);
    //     colHigh = max(colHigh,currentPiece->getOriginC()-currentPiece->getColSize()+1+i);
    //   }
    // }
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
    for (int i=rowSize+buffer-1;i>=0;i--) {
      if (displayBoard[i]==0b11111111) {
        removeRowCount++;
      }
      if (i-removeRowCount>=0) displayBoard[i]=displayBoard[i-removeRowCount];
      else displayBoard[i] = 0b00000000;
    }
    if (removeRowCount<4) score+=removeRowCount;  // if not 4 rows then add 1 one each row
    else score = score<<2; // multiply by 4 for tetris
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
    rotateOffset = 0;
    divSecCount = 0;
    isGameOver = false;
    score = 0;
    rowSize = 24;
    colSize = 8;
    buffer = 4;
    thumbController = new ThumbController();
    displayBoard = new unsigned char[rowSize+buffer];
    clearDisplayBoard();
    currentPiece = generateRandomPiece();
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
        
        //temp = 0b00111100;
        // for (int j=0;j<colSize;j++) {
        //   if (j>=currentPiece->getOriginC()-currentPiece->getColSize() && j<=currentPiece->getOriginC()) {
        //     if (currentPiece->getPieceBoard()[i+buffer-currentPiece->getOriginR()]&(1<<(j-(currentPiece->getOriginC()-currentPiece->getColSize())))) {
        //       temp |= (1<<j);
        //     }
        //   }
        // }
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
  void playGame() {
    clearDisplayBoard();
    score = 0;
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B

    // set compare match register for 1hz increments
    OCR1A = 1562; // 1/5th of a second// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    
    TIMSK1 |= (1 << OCIE1A);

    sei();//allow interrupts
    TCNT1  = 0;
    while(!isGameOver) {
      // for (int i=0;i<FRAME_RATE;i++) {
        char s[100];
        outputDisplayBoard();
        // thumbController->xVal = analogRead(X_THUMB);
        // thumbController->yVal = analogRead(Y_THUMB);
        // thumbController->yCount++;
        // thumbController->yTotalValue += thumbController->yVal;
        // thumbController->xCount++;
        // thumbController->xTotalValue += thumbController->xVal;
        //sprintf(s,"xCount: %d xVal: %d xTotalValue: %d\n",thumbController->xCount,thumbController->xVal,thumbController->xTotalValue);
        //Serial.print(s);
        // if (i%27==25) {
        //   if (abs((thumbController->yTotalValue/(float)thumbController->yCount)-504)>abs((thumbController->xTotalValue/(float)thumbController->xCount)-504)) {
        //     if (thumbController->yTotalValue/(float)thumbController->yCount>650) {
        //       if (permitMoveLeft()) currentPiece->setOriginC(currentPiece->getOriginC()+1);
        //     }
        //     else if (thumbController->yTotalValue/(float)thumbController->yCount<350) {
        //       if (permitMoveRight()) currentPiece->setOriginC(currentPiece->getOriginC()-1);
        //     }
        //   }
        //   else {
        //     if (thumbController->xTotalValue/(float)thumbController->xCount<350) {
        //       if (permitActionRotate()) currentPiece->rotatePieceBoard();
        //     }
        //     else if (thumbController->xTotalValue/(float)thumbController->xCount>650) {
        //       if (permitMoveDown()) currentPiece->setOriginR(currentPiece->getOriginR()+1);
        //     }
        //   }
        //   thumbController->reset();
        // }
      }
      // Serial.print("Origin: ");
      // Serial.print(currentPiece->getOriginC());
      // Serial.print("\n");
      // if (permitMoveDown()) {
      //   currentPiece->setOriginR(currentPiece->getOriginR()+1);
      // }
      // else {
      //   printCurrentPiece();
      //   removeFilledRowsAndAddScore();
      //   if (gameOver()) {
      //     cli();
      //     break;
      //   }
      // }
    //}
    // write score to sd card
  }
};

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  cli();
  matrix->divSecCount++;
  if (matrix->divSecCount%10==0) {
    matrix->divSecCount = 0;
    if (matrix->permitMoveDown()) {
      matrix->currentPiece->setOriginR(matrix->currentPiece->getOriginR()+1);
    }
    else {
      matrix->printCurrentPiece();
      matrix->removeFilledRowsAndAddScore();
      if (matrix->gameOver()) {
        cli();
        matrix->setIsGameOver(true);
      }
    }
  }
  else {
    matrix->thumbController->xVal = analogRead(X_THUMB);
    matrix->thumbController->yVal = analogRead(Y_THUMB);
    if (matrix->thumbController->xVal<600 && matrix->thumbController->xVal>400) {
      matrix->thumbController->hasComeToMiddle = true;
    }
    matrix->thumbController->yCount++;
    matrix->thumbController->yTotalValue += matrix->thumbController->yVal;
    matrix->thumbController->xCount++;
    matrix->thumbController->xTotalValue += matrix->thumbController->xVal;
    int yAvg = matrix->thumbController->yTotalValue/matrix->thumbController->yCount;
    int xAvg = matrix->thumbController->xTotalValue/matrix->thumbController->xCount;
    //Serial.print("yAvg: ");
    //Serial.print(yAvg);
    // Serial.print("\nxAvg: ");
    //  Serial.print(xAvg);
    // Serial.print("\n"); 
     char s[100];
    // Serial.print("Avg printint\n");
    //sprintf(s,"xCount: %d xTotalVal: %d xAvg: %d\n",matrix->thumbController->xCount,matrix->thumbController->xTotalValue,xAvg);
    //Serial.print(s);
    if (abs(yAvg-504)>abs(xAvg-504)) {
      if (yAvg>650) {
        if (matrix->permitMoveLeft()) matrix->currentPiece->setOriginC(matrix->currentPiece->getOriginC()+1);
      }
      else if (yAvg<350) {
        if (matrix->permitMoveRight()) matrix->currentPiece->setOriginC(matrix->currentPiece->getOriginC()-1);
      }
    }
    else {
      if (xAvg<350) {
        if (matrix->thumbController->hasComeToMiddle && matrix->permitActionRotateAdvanced()) {
          matrix->currentPiece->rotatePieceBoard();
          //Serial.print("Rotate offset: ");
          //Serial.println(matrix->rotateOffset);
          matrix->currentPiece->setOriginC(matrix->currentPiece->getOriginC()-matrix->rotateOffset);
          matrix->thumbController->hasComeToMiddle = false;
        }
      }
      else if (xAvg>650) {
        if (matrix->permitMoveDown()) matrix->currentPiece->setOriginR(matrix->currentPiece->getOriginR()+1);
      }
    }
    matrix->thumbController->reset();

  }
  
  sei();//allow interrupts
  TCNT1  = 0;
}


// setup and loop
void setup() {
  // high pin number always represents high bit and low pin number represents low bit
  randomSeed(analogRead(10));
  pinMode(ROW_ENABLE,OUTPUT);
  pinMode(THUMB_PRESS,INPUT);
  setPinMode(ROW_HIGH,ROW_LOW,OUTPUT);
  setPinMode(COL_HIGH,COL_LOW,OUTPUT);
  digitalWrite(ROW_ENABLE,LOW);
  digitalWrite(THUMB_PRESS,HIGH);
  Serial.begin(9600);
  matrix = new LEDMatrix();
}

void loop() {
    matrix->playGame();
}