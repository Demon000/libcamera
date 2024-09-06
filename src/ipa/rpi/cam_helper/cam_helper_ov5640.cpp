/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2019, Raspberry Pi Ltd
 *
 * camera information for ov5640 sensor
 */

#include <assert.h>

#include "cam_helper.h"

using namespace RPiController;

class CamHelperOv5640 : public CamHelper
{
public:
	CamHelperOv5640();
	uint32_t gainCode(double gain) const override;
	double gain(uint32_t gainCode) const override;
	void getDelays(int &exposureDelay, int &gainDelay,
		       int &vblankDelay, int &hblankDelay) const override;

private:
	/*
	 * Smallest difference between the frame length and integration time,
	 * in units of lines.
	 */
	static constexpr int frameIntegrationDiff = 4;
};

/*
 * OV5640 doesn't output metadata, so we have to use the "unicam parser" which
 * works by counting frames.
 */

CamHelperOv5640::CamHelperOv5640()
	: CamHelper({}, frameIntegrationDiff)
{
}

uint32_t CamHelperOv5640::gainCode(double gain) const
{
	return static_cast<uint32_t>(gain * 16.0);
}

double CamHelperOv5640::gain(uint32_t gainCode) const
{
	return static_cast<double>(gainCode) / 16.0;
}

void CamHelperOv5640::getDelays(int &exposureDelay, int &gainDelay,
				int &vblankDelay, int &hblankDelay) const
{
	/*
	 * We run this sensor in a mode where the gain delay is bumped up to
	 * 2. It seems to be the only way to make the delays "predictable".
	 */
	exposureDelay = 2;
	gainDelay = 2;
	vblankDelay = 2;
	hblankDelay = 2;
}

static CamHelper *create()
{
	return new CamHelperOv5640();
}

static RegisterCamHelper reg("ov5640", &create);
