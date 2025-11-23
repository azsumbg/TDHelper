#pragma once

#ifdef TDHELPER_EXPORTS
#define TDHELPER_API __declspec(dllexport)
#else
#define TDHELPER_API __declspec(dllimport)
#endif

#include <random>
#include <type_traits>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };
constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr int err_pointer{ 1111 };
constexpr int err_index{ 1112 };
constexpr int err_data{ 1113 };
constexpr int err_wrong_copy{ 1114 };

enum class dirs { up = 0, right = 1, left = 2, down = 3, stop = 4 };

enum class orcs { warrior1 = 0, warrior2 = 1, mage = 2, crusher = 3, flyer = 4, champion = 5 };
enum class buildings {
	arhcer = 0, small_cannon = 1, mid_cannon = 2, big_cannon = 3, small_mage = 4,
	mid_mage = 5, big_mage = 6, castle = 7, wall = 7
};
enum class assets { rock = 0, small_tree = 1, mid_tree = 2, big_tree = 3 };

enum class shots { arrow = 0, cannonball = 1, spell = 2 };

struct TDHELPER_API FPOINT
{
	float x{ 0 };
	float y{ 0 };
};

struct TDHELPER_API FRECT
{
	float left{ 0 };
	float right{ 0 };
	float up{ 0 };
	float down{ 0 };
};

namespace dll
{
	class TDHELPER_API EXCEPTION
	{
	private:
		const wchar_t* pointer_error{ L"Error in base pointer !" };
		const wchar_t* index_error{ L"Error in index passed !" };
		const wchar_t* data_error{ L"Invalid data read !" };
		const wchar_t* copy_error{ L"Error in copy parameter!" };
		const wchar_t* unknown_error{ L"Unknown error !" };

		int error_occurred = -1;

	public:
		EXCEPTION(int what_happend)
		{
			error_occurred = what_happend;
		}

		const wchar_t* GetMsg()
		{
			switch (error_occurred)
			{
			case err_pointer:
				return pointer_error;

			case err_index:
				return index_error;

			case err_data:
				return data_error;

			case err_wrong_copy:
				return copy_error;
			}
		
			return unknown_error;
		}
	};
	
	template<typename T>concept primes = (std::is_same<T, int>::value || std::is_same<T, float>::value
		|| std::is_same<T, double>::value || std::is_same<T, char>::value);

	template<typename T> class BAG
	{
	private:
		T* m_ptr{ nullptr };
		size_t max_size{ 0 };
		size_t next_pos{ 0 };
		bool is_valid{ false };

	public:

		BAG() :m_ptr{ reinterpret_cast<T*>(calloc(1,sizeof(T))) } 
		{
			max_size = 1;
		};
		BAG(size_t lenght) :m_ptr{ reinterpret_cast<T*>(calloc(lenght,sizeof(T))) }
		{
			max_size = lenght;
		}

		BAG(BAG& other)
		{
			if (m_ptr != other->m_ptr)
			{
				if (!other->m_ptr || other->empty())throw EXCEPTION(err_wrong_copy);

				free(m_ptr);

				max_size = other->max_size;
				next_pos = other->next_pos;

				for (size_t i = 0; i < next_pos; ++i)m_ptr[i] = other[i];
			}
		}
		BAG(BAG&& other)
		{
			if (m_ptr != other->m_ptr)
			{
				if (!other->m_ptr || other->empty())throw EXCEPTION(err_wrong_copy);

				free(m_ptr);

				max_size = other->max_size;
				next_pos = other->next_pos;

				for (size_t i = 0; i < next_pos; ++i)m_ptr[i] = other[i];

				other->m_ptr = nullptr;
			}
		}

		~BAG()
		{
			free(m_ptr);
		}

		bool empty()const
		{
			if (is_valid)return false;

			return true;
		}
		size_t size()const
		{
			return next_pos;
		}
		size_t capacity()const
		{
			return max_size;
		}

		void push_back(T what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = what;
					++next_pos;
					is_valid = true;
					return;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * max_size));
					
					if (!m_ptr)throw EXCEPTION(err_pointer);
					else
					{
						m_ptr[next_pos] = what;
						++next_pos;
						is_valid = true;
						return;
					}
				}
			}
		}
		void push_back(T* what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = (*what);
					++next_pos;
					is_valid = true;
					return;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * max_size));

					if (!m_ptr)throw EXCEPTION(err_pointer);
					else
					{
						m_ptr[next_pos] = (*what);
						++next_pos;
						is_valid = true;
						return;
					}
				}
			}
		}

		void push_front(T what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				(*m_ptr) = what;
				is_valid = true;
			}
		}
		void push_front(T* what)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			else
			{
				(*m_ptr) = (*what);
				is_valid = true;
			}
		}

		void erase(size_t index)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			if (index < 0 || index >= next_pos)throw EXCEPTION(err_index);
			if (empty())throw EXCEPTION(err_data);

			T* temp_ptr{ reinterpret_cast<T*>(calloc(max_size - 1,sizeof(T))) };
			for (int i = 0; i < next_pos; ++i)
			{
				if (i < index)temp_ptr[i] = m_ptr[i];
				else temp_ptr[i] = m_ptr[i + 1];
			}

			free(m_ptr);
			m_ptr = temp_ptr;
			--max_size;
			--next_pos;
		}

		T& operator[](size_t index)
		{
			if (!m_ptr)throw EXCEPTION(err_pointer);
			if (index < 0 || index >= next_pos)throw EXCEPTION(err_index);
			if (empty())throw EXCEPTION(err_data);

			return m_ptr[index];
		}

	};




}

