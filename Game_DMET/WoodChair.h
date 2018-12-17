#pragma once
#include "pch.h"
#include <glut.h>

class WoodPrism : public GameObject {
public:
	GLTexture tex;
	WoodPrism() {
		transform.localScale = Vector3f(0.2, 0.2, 0.2);
	}

	WoodPrism(float _width, float _height, float _thickness) {
		transform.localScale = Vector3f(_width, _height, _thickness);

		char buffer[80];						
		strcpy(buffer, "Textures/blu-sky-3.bmp");
		tex.Load(buffer);				// Loads a bitmap
		//loadBMP(&tex, buffer , true);

	}

	void Display() override
	{
		
		glPushMatrix();

		GLUquadricObj * qobj;
		qobj = gluNewQuadric();
		tex.Use();
		gluQuadricTexture(qobj, true);
		//gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 1, 5, 5);
		gluDeleteQuadric(qobj);


		glPopMatrix();
	}
};

class WoodChair : public GameObject {

public:
	WoodPrism chairLeg[4];
	WoodPrism chairPlate;
	WoodPrism chairBackStick[4];

	WoodChair()
	{
		transform.position = Vector3f(0, -0.5, 0);
	}

	void Start() override
	{

		float width, height, thickness, height_offset;
		width = 0.4; height = 1;
		thickness = 0.07;
		height_offset = 0.18;
		chairPlate = WoodPrism(width, thickness, width);
		chairPlate.transform.position = Vector3f(0, height_offset, 0);
		addChild(&chairPlate, true);

		
		width = width / 2 - 0.05;
		height = 0.42;
		thickness = 0.03;
		
		chairLeg[0] = WoodPrism(thickness, height, thickness);
		chairLeg[0].transform.position = Vector3f(width, 0, width);
		addChild(&chairLeg[0], true);
		chairLeg[1] = WoodPrism(thickness, height, thickness);
		chairLeg[1].transform.position = Vector3f(-width, 0, width);
		addChild(&chairLeg[1], true);
		chairLeg[2] = WoodPrism(thickness, height, thickness);
		chairLeg[2].transform.position = Vector3f(-width, 0, -width);
		addChild(&chairLeg[2], true);
		chairLeg[3] = WoodPrism(thickness, height, thickness);
		chairLeg[3].transform.position = Vector3f(width, 0, -width);
		addChild(&chairLeg[3], true);

		thickness = 0.04;
		height = 0.4;
		height_offset = 0.35;
		chairBackStick[0] = WoodPrism(thickness, height, thickness);
		chairBackStick[0].transform.position = Vector3f(-width, height_offset, width);
		addChild(&chairBackStick[0], true);
		chairBackStick[1] = WoodPrism(thickness, height, thickness);
		chairBackStick[1].transform.position = Vector3f(-width, height_offset, -width);
		addChild(&chairBackStick[1], true);
		
		thickness = 0.015;
		height = 0.05;
		height_offset = 0.35;
		chairBackStick[2] = WoodPrism(thickness, height, 0.4);
		chairBackStick[2].transform.position = Vector3f(-width, height_offset, 0);
		addChild(&chairBackStick[2], true);
		chairBackStick[3] = WoodPrism(thickness, height, 0.4);
		chairBackStick[3].transform.position = Vector3f(-width, height_offset + height + 0.01, 0);
		addChild(&chairBackStick[3], true);
	}
};
