rdrawing 1.1£º

  - enhancement: add enum ImageJoinStyle and ImageSetJoinStyle() ImageGetJoinStyle() functions
  - fix: ImageDrawSectorEx error
  - fix: ImageFillTriangleEx dead lock, when three vertices are on the same vertical line
  - fix: ImageDrawLineEx error, when length of line is shorter than line width
  - enhancement: add arc function()

rturtle 1.1:

  - change: clear() don't reset turtle to it's origin
  - enhancement: add waitClose()
  - fix: truncation error not correctly handled in fd()
  - add: beginFill() / endFill() / isFilling()
  - enhancement: add setAutoUpdateWorld()/updateWorld()/waitMilliseconds() functions
  - change: setXY() will leave tracks when pen down
  - add: setX() / setY()
  - remove: drawCircle() / fillCircle()

rturtle 1.0
rdrawing 1.0

 - initial release