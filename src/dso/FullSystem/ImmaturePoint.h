/**
* This file is part of DSO.
*
* Copyright 2016 Technical University of Munich and Intel.
* Developed by Jakob Engel <engelj at in dot tum dot de>,
* for more information see <http://vision.in.tum.de/dso>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* DSO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DSO. If not, see <http://www.gnu.org/licenses/>.
*/



#pragma once

#include "util/NumType.h"
#include "FullSystem/HessianBlocks.h"



namespace dso
{

struct ImmaturePointTemporaryResidual
{
public:
	ResState state_state;
	double state_energy;
	ResState state_NewState;
	double state_NewEnergy;
	FrameHessian* target;
};


enum ImmaturePointStatus {
	IPS_GOOD=0,					// Traced is good
	IPS_OOB,					// Out of Bounds: end tracking & marginalize
	IPS_OUTLIER,				// Energy too high, treat as outlier
	IPS_OUTLIER_OUT,			// Energy too high, stop optimizing
	IPS_SKIPPED,				// Good enough to not need tracing, use current values
	IPS_BADCONDITION,			// Not traced because bad error bound
	IPS_UNINITIALIZED};			// Not even traced once


class ImmaturePoint
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	int wG0;
	int hG0;

	// static values
	float color[MAX_RES_PER_POINT];
	float weights[MAX_RES_PER_POINT];

	Eigen::Vector3f colour3[MAX_RES_PER_POINT];
	bool colourValid;

	Mat22f gradH;
	Vec2f gradH_ev;
	Mat22f gradH_eig;
	float energyTH;
	float u,v;
	FrameHessian* host;
	int idxInImmaturePoints;

	float quality;

	float my_type;

	float idepth_min;
	float idepth_max;
	ImmaturePoint(int u_, int v_, int ww, int hh, FrameHessian* host_, float type, CalibHessian* HCalib, GlobalSettings& globalSettings_);
	~ImmaturePoint();

	ImmaturePointStatus traceOn(FrameHessian* frame, const Mat33f &hostToFrame_KRKi, const Vec3f &hostToFrame_Kt, const Vec2f &hostToFrame_affine, CalibHessian* HCalib, bool debugPrint=false);

	ImmaturePointStatus lastTraceStatus;
	Vec2f lastTraceUV;
	float lastTracePixelInterval;

	float idepth_GT;

	double linearizeResidual(
			CalibHessian *  HCalib, const float outlierTHSlack,
			ImmaturePointTemporaryResidual* tmpRes,
			float &Hdd, float &bd,
			float idepth);

private:
	GlobalSettings& globalSettings;

};

}

