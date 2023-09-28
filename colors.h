struct COLORS {
  byte r;
  byte g;
  byte b;

   COLORS() : r(0), g(0), b(0) {}

  COLORS(byte red, byte green, byte blue) : r(red), g(green), b(blue) {}
};

const COLORS red[3] = {
  COLORS(82, 0, 0),  
  COLORS(255, 0, 0),   
  COLORS(150, 50, 50) 
};

const COLORS blue[3] = {
  COLORS(0, 0, 102),  
  COLORS(0, 0, 255),
  COLORS(0, 153, 255) 
};

const COLORS green[3] = {
  COLORS(0, 82, 0), 
  COLORS(0, 255, 0),  
  COLORS(102, 255, 102)
};

const COLORS purple[3] = {
  COLORS(60, 0, 90),
  COLORS(150, 0, 200),
  COLORS(255, 105, 180)
};
