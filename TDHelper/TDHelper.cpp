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

// BUILDINGS CLASS ********************

dll::BUILDINGS::BUILDINGS(buildings _what, float _sx, float _sy) :PROTON(_sx, _sy)
{
	_type = _what;

	switch (_type)
	{
	case buildings::castle:
		new_dims(180.0f, 182.0f);
		strenght = 5;
		lifes = 50;
		fire_rate = 200;
		range = 75.0f;
		max_frames = 4;
		frame_delay = 15;
		break;

	case buildings::arhcer:
		new_dims(37.0f, 85.0f);
		strenght = 5;
		lifes = 50;
		fire_rate = 50;
		range = 75.0f;
		max_frames = 7;
		frame_delay = 9;
		break;

	case buildings::small_cannon:
		new_dims(75.0f, 34.0f);
		strenght = 10;
		lifes = 75;
		fire_rate = 75;
		range = 100.0f;
		max_frames = 57;
		frame_delay = 1;
		break;

	case buildings::mid_cannon:
		new_dims(85.0f, 64.0f);
		strenght = 15;
		lifes = 100;
		fire_rate = 100;
		range = 130.0f;
		max_frames = 9;
		frame_delay = 7;
		break;

	case buildings::big_cannon:
		new_dims(90.0f, 86.0f);
		strenght = 30;
		fire_rate = 130;
		lifes = 150;
		range = 150.0f;
		max_frames = 90;
		frame_delay = 1;
		break;

	case buildings::small_mage:
		new_dims(65.0f, 85.0f);
		strenght = 12;
		lifes = 100;
		fire_rate = 50;
		range = 130.0f;
		max_frames = 36;
		frame_delay = 2;
		break;

	case buildings::mid_mage:
		new_dims(44.0f, 85.0f);
		strenght = 18;
		lifes = 120;
		fire_rate = 75;
		range = 150.0f;
		max_frames = 34;
		frame_delay = 2;
		break;

	case buildings::big_mage:
		new_dims(32.0f, 90.0f);
		strenght = 30;
		lifes = 180;
		fire_rate = 100;
		range = 200.0f;
		max_frames = 97;
		frame_delay = 1;
		break;
	}
}
buildings dll::BUILDINGS::get_type()const
{
	return _type;
}
void dll::BUILDINGS::set_type(buildings what)
{
	frame = 0;
	_type = what;

	switch (_type)
	{
	case buildings::castle:
		new_dims(180.0f, 182.0f);
		strenght = 5;
		lifes = 50;
		fire_rate = 200;
		range = 75.0f;
		max_frames = 4;
		frame_delay = 15;
		break;

	case buildings::arhcer:
		new_dims(37.0f, 85.0f);
		strenght = 5;
		lifes = 50;
		fire_rate = 50;
		range = 75.0f;
		max_frames = 7;
		frame_delay = 9;
		break;

	case buildings::small_cannon:
		new_dims(75.0f, 34.0f);
		strenght = 10;
		lifes = 75;
		fire_rate = 75;
		range = 100.0f;
		max_frames = 57;
		frame_delay = 1;
		break;

	case buildings::mid_cannon:
		new_dims(85.0f, 64.0f);
		strenght = 15;
		lifes = 100;
		fire_rate = 100;
		range = 130.0f;
		max_frames = 9;
		frame_delay = 7;
		break;

	case buildings::big_cannon:
		new_dims(90.0f, 86.0f);
		strenght = 30;
		fire_rate = 130;
		lifes = 150;
		range = 150.0f;
		max_frames = 90;
		frame_delay = 1;
		break;

	case buildings::small_mage:
		new_dims(65.0f, 85.0f);
		strenght = 12;
		lifes = 100;
		fire_rate = 50;
		range = 130.0f;
		max_frames = 36;
		frame_delay = 2;
		break;

	case buildings::mid_mage:
		new_dims(44.0f, 85.0f);
		strenght = 18;
		lifes = 120;
		fire_rate = 75;
		range = 150.0f;
		max_frames = 34;
		frame_delay = 2;
		break;

	case buildings::big_mage:
		new_dims(32.0f, 90.0f);
		strenght = 30;
		lifes = 180;
		fire_rate = 100;
		range = 200.0f;
		max_frames = 97;
		frame_delay = 1;
		break;
	}
}
int dll::BUILDINGS::attack()
{
	++fire_status;
	if (fire_status >= fire_rate)
	{
		fire_status = 0;
		return strenght;
	}

	return 0;
}
int dll::BUILDINGS::get_frame()
{
	--frame_delay;

	if (frame_delay <= 0)
	{
		switch (_type)
		{
		case buildings::castle:
			frame_delay = 15;
			break;

		case buildings::arhcer:
			frame_delay = 9;
			break;

		case buildings::small_cannon:
			frame_delay = 1;
			break;

		case buildings::mid_cannon:
			frame_delay = 7;
			break;

		case buildings::big_cannon:
			frame_delay = 1;
			break;

		case buildings::small_mage:
			frame_delay = 2;
			break;

		case buildings::mid_mage:
			frame_delay = 2;
			break;

		case buildings::big_mage:
			frame_delay = 1;
			break;
		}

		++frame;
		if(frame > max_frames)frame = 0;
	}

	return frame;
}
int dll::BUILDINGS::get_range()const
{
	return range;
}
void dll::BUILDINGS::Release()
{
	delete this;
}

///////////////////////////////////////









// FUNCTION DEFINITIONS *********************************

float dll::Distance(FPOINT first_center, FPOINT second_center)
{
	float a = (float)(pow(abs(second_center.x - first_center.x), 2));
	float b = (float)(pow(abs(second_center.y - first_center.y), 2));

	return (float)(sqrt(a + b));
}

bool dll::Intersect(FPOINT first_center, FPOINT second_center, float first_radius_x, float second_radius_x,
	float first_radius_y, float second_radius_y)
{
	if (abs(second_center.x - first_center.x) <= first_radius_x + second_radius_x
		&& abs(second_center.y - first_center.y) <= first_radius_y + second_radius_y)return true;
	return false;
}

bool dll::Sort(BAG<FPOINT>& Mesh, FPOINT ref_point)
{
	if (Mesh.empty() || Mesh.size() < 2)return false;

	bool sorted = false;

	while (!sorted)
	{
		sorted = true;

		for (size_t count = 0; count < Mesh.size() - 1; ++count)
		{
			if (Distance(Mesh[count], ref_point) > Distance(Mesh[count + 1], ref_point))
			{
				FPOINT temp = Mesh[count];

				Mesh[count] = Mesh[count + 1];
				Mesh[count + 1] = temp;
				sorted = false;
			}

		}
	}

	return true;
}


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

TDHELPER_API dll::BUILDINGS* dll::BuildingFactory(buildings what, float sx, float sy)
{
	BUILDINGS* ret{ nullptr };
	ret = new BUILDINGS(what, sx, sy);

	return ret;
}