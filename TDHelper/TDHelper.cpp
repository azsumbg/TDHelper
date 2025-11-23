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






