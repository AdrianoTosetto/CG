namespace algebra{

#ifndef CALCULATIONS_HPP
#define CALCULATIONS_HPP

double d2r(double d) {
  	return (d / 180.0) * ((double) M_PI);
}

Matrix getRotationMatrix(double x, double y, double rad) {
	Matrix trans1 = Matrix::identidade(3);
	Matrix rotating = Matrix::identidade(3);
	Matrix trans2 = Matrix::identidade(3);
	Matrix result = Matrix::identidade(3);

	trans1.setValue(2,0,-x);
	trans1.setValue(2,1,-y);
	rotating.setValue(0,0, cos(rad));
	rotating.setValue(0,1, (-1) * sin(rad));
	rotating.setValue(1,0, sin(rad));
	rotating.setValue(1,1, cos(rad));
	trans2.setValue(2,0,x);
	trans2.setValue(2,1,y);

	result = trans1 * rotating;
	result = result * trans2;

	return result;
}

Matrix getScalingMatrix(double x, double y, double scale) {
	Matrix trans1 = Matrix::identidade(3);
	Matrix scaling = Matrix::identidade(3);
	Matrix trans2 = Matrix::identidade(3);
	Matrix result = Matrix::identidade(3);

	trans1.setValue(2,0,-x);
	trans1.setValue(2,1,-y);
	scaling.setValue(0,0, scale);
	scaling.setValue(1,1, scale);
	trans2.setValue(2,0,x);
	trans2.setValue(2,1,y);

	result = trans1 * scaling;
	result = result * trans2;

	return result;
}

#endif

}