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




// FUNCTION DEFINITIONS *********************************

TDHELPER_API dll::ASSETS* dll::AssetFactory(assets what_type, float start_x, float start_y)
{
	dll::ASSETS* ret{ nullptr };

	ret = new dll::ASSETS(what_type, start_x, start_y);

	return ret;
}