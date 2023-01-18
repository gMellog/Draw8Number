#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h> 

void drawDisc(float R, float X, float Y, float Z)
{
	const int N = 40;
	float t;
	int i;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(X, Y, Z);
	for (i = 0; i <= N; ++i)
	{
		t = 2 * M_PI * i / N;
		glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
	}
	glEnd();
}

void draw8WithDiscsAndZBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	drawDisc(15, 50.0, 35.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawDisc(7.5, 50.0, 35.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	drawDisc(10, 50.0, 55.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawDisc(4.5, 50.0, 55.0, 1.0);

	glDisable(GL_DEPTH_TEST);

}

void drawCircleAnnulus(float InR, float OutR, float X, float Y, float Z)
{
	int numVert = 40;

	glBegin(GL_TRIANGLE_STRIP);

	float t = 0;
	for (int i = 0; i <= numVert; ++i)
	{
		t = (2 * M_PI * i / numVert);

		glVertex3f(X + OutR * cos(t), Y + OutR * sin(t), Z);
		glVertex3f(X + InR * cos(t), Y + InR * sin(t), Z);
	}

	glEnd();

	glFlush();
}

void draw8WithCircleAnnuluses()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	drawCircleAnnulus(4.5, 10, 50.0, 55.0, 0.0);
	drawCircleAnnulus(7.5, 15, 50.0, 35.0, 0.0);
}

struct Draw8Number
{
	struct RangeValue
	{
		RangeValue(float pUp, float pDown, float pVal)
			:
			up{ pUp },
			down{ pDown },
			val{ pVal }
		{

		}

		float up;
		float down;
		float val;
	};

	using Ranges = std::vector<RangeValue>;

	struct UpPart
	{
		float InR;
		float OutR;
		int numVert;
		Ranges angleVertsRanges;
	};

	struct Part
	{
		Part(float pX = 0.f, float pY = 0.f, float pInR = 0.f,
			float pOutR = 0.f, float pAngleLine = 0.f,
			int pNumVert = 0,
			const Ranges& pAngleVertsRanges = {}
		)
			:
			X{ pX }, Y{ pY }, InR{ pInR }, OutR{ pOutR },
			angleLine{ pAngleLine }, numVert{ pNumVert },
			angleVertsRanges{ pAngleVertsRanges }
		{

		}

		Part(const UpPart& upPart)
			:
			Part()
		{
			InR = upPart.InR;
			OutR = upPart.OutR;
			numVert = upPart.numVert;
			angleVertsRanges = upPart.angleVertsRanges;
		}

		int getVertLine(int angle) const
		{
			int r = 0;

			for (const auto& rangeVal : angleVertsRanges)
			{
				if (rangeVal.up >= angle && angle > rangeVal.down)
				{
					r = rangeVal.val;
					break;
				}
			}

			return r;
		}

		float X;
		float Y;
		float InR;
		float OutR;
		float angleLine;
		int numVert;

	private:

		Ranges angleVertsRanges;
	};


	static Draw8Number& get()
	{
		Ranges upPartRanges =
		{
			{75.f, 25.f, 5.f},
			{25.f, 10.f, 3.f},
			{10.f, 6.f, 2.f},
			{6.f, 1.f, 1.f}
		};

		UpPart upPart{ 4.5, 10.0, 30, upPartRanges };

		Ranges downPartRanges =
		{
			{75.f, 25.f, 7.f} ,
			{25.f, 21.f, 5.f},
			{21.f, 10.f, 3.f},
			{ 10.f, 6.f, 2.f},
			{6.f, 1.f, 1.f}
		};

		Part downPart{ 50.0, 35.0, 7.5, 15.0, 3.0, 40, downPartRanges };

		static Draw8Number machine{ upPart, downPart };

		return machine;
	}

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(0.0, 0.0, 0.0);

		drawDownPart();
		drawUpPart();

		glFlush();
	}

	void setDownAngle(float angle) noexcept
	{
		if (downPart.angleLine == 0.f && angle > downPart.angleLine)
		{
			downPart.angleLine = angle = 3.f;
		}

		if (angle > 75.f)
		{
			return;
		}

		separated = (angle < 3.f);
		if (separated)
			downPart.angleLine = 0.f;
		else
			downPart.angleLine = angle;

		calcPropsForUp();
	}

	float getDownAngle() const noexcept
	{
		return downPart.angleLine / M_PI * 180;
	}

	static bool isWire;

private:

	Draw8Number(const UpPart& pUpPart, const Part& pDownPart)
		:
		upPart{ pUpPart },
		downPart{ pDownPart },
		separated{}
	{
		if (downPart.angleLine < 3.f)
		{
			separated = true;
			downPart.angleLine = 0.f;
		}

		calcPropsForUp();
	}

	Part downPart;
	Part upPart;
	bool separated;

	void calcPropsForUp()
	{
		const auto angleOfRightTrigInRads = ((downPart.angleLine) / 180) * M_PI;
		downPart.angleLine = angleOfRightTrigInRads;
		const auto commonLeg = downPart.OutR * sin(angleOfRightTrigInRads / 2);

		const auto YAddDown = sqrt(static_cast<double>(downPart.OutR * downPart.OutR) - commonLeg * commonLeg);
		const auto YAddUp = sqrt(static_cast<double>(upPart.OutR * upPart.OutR) - commonLeg * commonLeg);

		upPart.Y = downPart.Y + YAddDown + YAddUp + (separated ? 0.5 : 0);
		upPart.X = downPart.X;

		const auto halfAngleInRads = asin(commonLeg / upPart.OutR);
		upPart.angleLine = halfAngleInRads * 2;
	}

	void drawAlg(const Part& part, float startPos, bool up)
	{
		const auto numVertCircle = part.numVert;
		const auto numVertCircleLine = part.getVertLine(downPart.angleLine / M_PI * 180);
		const auto OutR = part.OutR;
		const auto InR = part.InR;
		const auto angle = part.angleLine;
		const auto X = part.X;
		const auto Y = part.Y;
		const auto Z = 0.f;

		float t = 0;

		glBegin(GL_TRIANGLE_STRIP);

		float start = startPos;
		float path = (2 * M_PI - angle);

		for (int i = 0; i <= numVertCircle; ++i)
		{
			t = start + (path * i * (up ? -1 : 1) / numVertCircle);
			glVertex3f(X + OutR * cos(t), Y + OutR * sin(t), Z);
			glVertex3f(X + InR * cos(t), Y + InR * sin(t), Z);
		}

		if (!separated)
		{
			const auto holdYArg = t;
			start = t;
			path = angle;

			for (int i = 0; i <= numVertCircleLine; ++i)
			{
				t = start + (path * i * (up ? -1 : 1) / numVertCircleLine);
				glVertex3f(X + OutR * cos(t), Y + OutR * sin(holdYArg), Z);
				glVertex3f(X + InR * cos(t), Y + InR * sin(t), Z);
			}
		}

		glEnd();
	}

	void drawUpPart()
	{
		drawAlg(upPart, 3 * M_PI_2 - upPart.angleLine / 2, true);
	}

	void drawDownPart()
	{
		drawAlg(downPart, (M_PI_2 + downPart.angleLine / 2), false);
	}
};

bool Draw8Number::isWire = false;