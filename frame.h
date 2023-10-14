#pragma once

struct frame {
	
	bool	capture,
			instructions;

	int8_t 
			up_lid_left,
			down_lid_left,
			horizontal_left,
			vertical_left,
			up_lid_right,
			down_lid_right,
			horizontal_right,
			vertical_right,
			jaw_hinge,
			jaw_forward,
			jaw_sideways,
			up_lip_left,
			up_lip_right,
			down_lip_left,
			down_lip_right,
			side_lip_left,
			side_lip_right,
			neck_1,
			neck_2,
			neck_3,
			neck_4,
			neck_5,
			eyebrow_1_left,
			eyebrow_2_left,
			eyebrow_1_right,
			eyebrow_2_right;
};