#include "pch.h"
#include "TDHelper.h"


// RANDIT CLASS *************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}

/////////////////////////////////////////

// PROTON CLASS *************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_radius = (end.x - start.x) / 2.0f;
	y_radius = (end.y - start.y) / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}
dll::PROTON::PROTON(float sx, float sy)
{
	start.x = sx;
	start.y = sy;

	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_radius = (end.x - start.x) / 2.0f;
	y_radius = (end.y - start.y) / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}
dll::PROTON::PROTON(float sx, float sy, float swidth, float sheight)
{
	start.x = sx;
	start.y = sy;

	_width = swidth;
	_height = sheight;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_radius = (end.x - start.x) / 2.0f;
	y_radius = (end.y - start.y) / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}

void dll::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_radius = (end.x - start.x) / 2.0f;
	y_radius = (end.y - start.y) / 2.0f;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}
void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}

float dll::PROTON::get_width()const
{
	return _width;
}
float dll::PROTON::get_height()const
{
	return _height;
}

void dll::PROTON::set_width(float new_width)
{
	_width = new_width;
	end.x = start.x + _width;
	
	x_radius = (end.x - start.x) / 2.0f;
	
	center.x = start.x + x_radius;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;
	end.y = start.y + _height;

	y_radius = (end.y - start.y) / 2.0f;

	center.y = start.y + y_radius;
}

//////////////////////////////////////////

// ASSETS CLASS *************************

dll::ASSETS::ASSETS(assets _what_type, float _start_x, float _start_y) :PROTON(_start_x, _start_y)
{
	_type = _what_type;

	switch (_type)
	{
	case assets::rock:
		new_dims(100.0f, 100.0f);
		break;

	case assets::small_tree:
		new_dims(30.0f, 50.0f);
		break;

	case assets::mid_tree:
		new_dims(45.0f, 50.0f);
		break;

	case assets::big_tree:
		new_dims(65.0f, 60.0f);
		break;
	}
}

assets dll::ASSETS::get_type()const
{
	return _type;
}

void dll::ASSETS::Release()
{
	delete this;
}

////////////////////////////////////////

// SHOTS CLASS ************************

void dll::SHOTS::SetPathInfo(float _ex, float _ey)
{
	move_sx = start.x;
	move_sy = start.y;

	move_ex = end.x;
	move_ey = end.y;

	ver_dir = false;
	hor_dir = false;

	if (move_sx == move_ex || (move_ex > start.x && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_sy == move_ey || (move_ey > start.y && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - start.x * slope;
}

dll::SHOTS::SHOTS(shots _what, float _where_x, float _where_y, 
	float _to_where_x, float _to_where_y, int _shot_modifier) :PROTON(_where_x, _where_y)
{
	_type = _what;

	SetPathInfo(_to_where_x, _to_where_y);

	switch (_type)
	{
	case shots::arrow:
		new_dims(20.0f, 20.0f);
		if (move_sy > move_ey)
		{
			if (move_sx > move_ex)dir = dirs::up_left;
			else dir = dirs::up_right;
		}
		else
		{
			if (move_sx > move_ex)dir = dirs::down_left;
			else dir = dirs::down_right;
		}
		strenght = 1.0f + _shot_modifier;
		speed = 5.0f + _shot_modifier;
		break;

	case shots::cannonball:
		new_dims(40.0f, 40.0f);
		strenght = 5.0f + _shot_modifier;
		speed = 3.0f + _shot_modifier;
		break;

	case shots::spell:
		new_dims(50.0f, 50.0f);
		strenght = 8.0f + _shot_modifier;
		speed = 4.0f + _shot_modifier;
		break;
	}
}

bool dll::SHOTS::move()
{
	if (ver_dir)
	{
		if (move_ey < move_sy)
		{
			start.y -= speed;
			set_edges();
			if (start.y <= move_ey)return false;
		}
		else if (move_ey > move_sy)
		{
			start.y += speed;
			set_edges();
			if (end.y >= move_ey)return false;
		}
	}
	if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			start.x -= speed;
			set_edges();
			if (start.x <= move_ex)return false;
		}
		else if (move_ex > move_sy)
		{
			start.x += speed;
			set_edges();
			if (end.x >= move_ex)return false;
		}
	}

	if (move_sx > move_ex)
	{
		start.x -= speed;
		start.y = start.x * slope + intercept;
		set_edges();
		if (start.y >= ground || end.y <= sky || start.x >= scr_width || end.x <= 0 || start.x <= move_ex)return false;
	}
	if (move_sx < move_ex)
	{
		start.x += speed;
		start.y = start.x * slope + intercept;
		set_edges();
		if (start.y >= ground || end.y <= sky || start.x >= scr_width || end.x <= 0 || start.x >= move_ex)return false;
	}

	return true;
}
int dll::SHOTS::get_frame()
{
	if (_type != shots::spell)return 0;

	current_frame++;
	if (current_frame > max_frames)current_frame = 0;

	return current_frame;
}
int dll::SHOTS::get_strenght()const
{
	return strenght;
}
shots dll::SHOTS::get_type()const
{
	return _type;
}
void dll::SHOTS::Release()
{
	delete this;
}

///////////////////////////////////////











// FUNCTION DEFINITIONS *********************************

TDHELPER_API dll::ASSETS* dll::AssetFactory(assets what_type, float start_x, float start_y)
{
	dll::ASSETS* ret{ nullptr };

	ret = new dll::ASSETS(what_type, start_x, start_y);

	return ret;
}

TDHELPER_API dll::SHOTS* dll::ShotFactory(shots what, float where_x, float where_y,
	float to_where_x, float to_where_y, int shot_modifier)
{
	SHOTS* ret{ nullptr };

	ret = new SHOTS(what, where_x, where_y, to_where_x, to_where_y, shot_modifier);
	
	return ret;
}