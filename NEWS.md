rturtle 1.3:

  - fix: when background color is set to black, all drawing is not visible

rturtle 1.2:

  - fix: doesn't show background image 
  - enhancement: F4 toggle hide/unhide background image

rdrawing 1.1��

  - enhancement: add enum ImageJoinStyle and ImageSetJoinStyle() ImageGetJoinStyle() functions
  - fix: ImageDrawSectorEx error
  - fix: ImageFillTriangleEx dead lock, when three vertices are on the same vertical line
  - fix: ImageDrawLineEx error, when length of line is shorter than line width
  - enhancement: add ImageDrawCubicBezierEx()

rturtle 1.1:

  - change: clear() don't reset turtle to it's origin
  - enhancement: add waitClose()
  - fix: truncation error not correctly handled in fd()
  - add: beginFill() / endFill() / isFilling()
  - enhancement: add setAutoUpdateWorld()/updateWorld()/waitMilliseconds() functions
  - change: setXY() will leave tracks when pen down
  - add: setX() / setY()
  - remove: drawCircle() / fillCircle()
  - enhancement: add arc function()

rturtle 1.0
rdrawing 1.0

 - initial release