#pragma once

#include <GL/glut.h>


class Bar {
	
	public:
		const int count = 30; // number of bars
		float scaleBar; // value to scale the bars
		float barGap = 0.06;
		float maxScale = 4.5;
		// float barScale[];

		GLfloat vertices[8] = {
			-0.90,  0.05,
			-0.95,  0.05,
			-0.95, -0.05,
			-0.90, -0.05,
		};

		// color format: CMY
		GLfloat colorsLow[12] = {
		
			
			1.0, 1.0, 1.0, //default
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			
			
		};


		GLfloat colorsHigh[12] = {
			
			0.2314, 0.2824, 0.2039, // light purple
			0.2314, 0.2824, 0.2039,
			0.2314, 0.2824, 0.2039,
			0.2314, 0.2824, 0.2039,
		
		};

		// bar indices
		GLubyte indices[4] = {
			0, 1, 
			2, 3,
		};

	
	
};
