//posted by metamal at
//http://stackoverflow.com/questions/99353/how-to-test-if-a-line-segment-intersects-an-axis-aligned-rectange-in-2d

bool SegmentIntersectRectangle(double a_rectangleMinX,
							   double a_rectangleMinY,
							   double a_rectangleMaxX,
							   double a_rectangleMaxY,
							   double a_p1x,
							   double a_p1y,
							   double a_p2x,
							   double a_p2y){

	// Find min and max X for the segment
	double minX = a_p1x;
	double maxX = a_p2x;	
	if(a_p1x > a_p2x){
		minX = a_p2x;
		maxX = a_p1x;
	}
	
	// Find the intersection of the segment's and rectangle's x-projections
	if(maxX > a_rectangleMaxX){
		maxX = a_rectangleMaxX;
	}	
	if(minX < a_rectangleMinX){
		minX = a_rectangleMinX;
	}	
	if(minX > maxX){// If their projections do not intersect return false
		return false;
	}
	
	// Find corresponding min and max Y for min and max X we found before
	double minY = a_p1y;
	double maxY = a_p2y;
	double dx = a_p2x - a_p1x;
	if(fabs(dx) > 0.0000001){
		double a = (a_p2y - a_p1y) / dx;
		double b = a_p1y - a * a_p1x;
		minY = a * minX + b;
		maxY = a * maxX + b;
	}
	if(minY > maxY){
		double tmp = maxY;
		maxY = minY;
		minY = tmp;
	}
	
	// Find the intersection of the segment's and rectangle's y-projections
	if(maxY > a_rectangleMaxY){
		maxY = a_rectangleMaxY;
	}
	if(minY < a_rectangleMinY){
		minY = a_rectangleMinY;
	}
	if(minY > maxY){// If Y-projections do not intersect return false
		return false;
	}
	
	return true;
}
