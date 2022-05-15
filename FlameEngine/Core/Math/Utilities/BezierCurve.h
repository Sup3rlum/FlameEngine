#pragma once


#include "Core/Common/CoreCommon.h"
#include "../Types/Vector.h"



template<typename TVec>
struct TBezierCurve
{
	virtual TVec GetPoint(float t) const = 0;
	virtual TVec GetVelocity(float t) const = 0;
};



template<typename TVec>
struct QuadBezier: public TBezierCurve<TVec>
{

private:

	TVec Anchor1, Anchor2, Control;


	QuadBezier(TVec anch1, TVec contr, TVec anch2) :
		Anchor1(anch1),
		Anchor2(anch2),
		Control(contr)
	{

	}

	TVec GetPoint(float t) override
	{
		const float t2 = t * t;

		return 
			(t2 - 2 * t + 1) * Anchor1 + 
			(2 * t - 2 * t2) * Control + 
			(t2) * Anchor2;
	}

	TVec GetVelocity(float t) override
	{
		return
			(2 * t - 2) * Anchor1 +
			(2 - 4 * t) * Control +
			(2 * t) * Anchor2;
	}

};


template<typename TVec>
struct CubicBezier : public TBezierCurve<TVec>
{

private:

	TVec Anchor1, Anchor2, Control1, Control2;
	CubicBezier(TVec anch1, TVec contr1, TVec contr2, TVec anch2) :
		Anchor1(anch1),
		Anchor2(anch2),
		Control1(contr1),
		Control2(contr2)
	{

	}

	TVec GetPoint(float t) const override
	{
		const float t3 = t * t * t;
		const float t2 = t * t;

		return
			(-t3 + 3 * t2 - 3 * t + 1) * Anchor1 +
			(3 * t3 - 6 * t2 + 3 * t) * Control1 +
			(-3 * t3 + 3 * t2) * Control2 +
			(t3) * Anchor2;
	}

	TVec GetVelocity(float t) const override
	{
		const float t2 = t * t;

		return
			(-3 * t2 + 6 * t - 3) * Anchor1 +
			(9 * t2 - 12 * t + 3) * Control1 +
			(-9 * t2 + 6 * t) * Control2 +
			(3 * t2) * Anchor2;
	}

};

template<typename TVec>
struct NPointBezier : public TBezierCurve<TVec>
{

private:

	FArray<TVec> ControlPoints;
	NPointBezier(const FArray<TVec> points) :
		ControlPoints(points)
	{

	}

	TVec GetPoint(float t) const override
	{
		int index = ControlPoints.Length();
		auto copy = ControlPoints;

		while (index > 1)
		{
			for (int i = 0; i < index - 1; i++)
			{
				copy[i] = TVec::Lerp(copy[i], copy[i + 1], t);
			}

			index--;
		}
		return copy[0];
	}

	TVec GetVelocity(float t) const override
	{
		return TVec();
	}
};