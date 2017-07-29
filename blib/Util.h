#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <string.h>
#include <functional>
#include <algorithm>
#include <cctype>
#include <locale>

#ifndef WIN32
#include <sys/time.h>
#endif

namespace blib
{
	namespace util
	{
		glm::vec2 fromAngle(float angle);
		float wrapAngleRad(float angle);
		float wrapAngleDeg(float angle);

		std::string toString(int number);
		std::string toString(float number);
		std::vector<std::string> split(std::string toSplit, const std::string &seperator, bool removeEmpty = true);
		static inline std::string ltrim(std::string s) {
			s.erase(0, s.find_first_not_of(" \n\r\t"));
			return s;
		}

		// trim from end
		static inline std::string rtrim(std::string s) {
			s.erase(s.find_last_not_of(" \n\r\t") + 1);
			return s;
		}

		// trim from both ends
		static inline std::string trim(std::string &s) {
			return ltrim(rtrim(s));
		}

		std::string replace(std::string orig, const std::string &find, const std::string &replace);


		std::string getDataDir();

		long tickcount();

		template<class T>
		T swapByteOrder( T val)
		{
			char* buf = (char*)&val;
			std::swap(buf[0], buf[3]);
			std::swap(buf[1], buf[2]);
			return val;
		}

		struct compareVec4
		{
			bool operator () (const glm::vec4 &a, const glm::vec4 &b) const;
		};

#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<strlen(s)?strlen(s)-1-(i):strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))

#define HASH(s)    ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))

		void fixConsole();
		glm::ivec2 getResolution();
		template <typename T>
		class make_vector {
		public:
			typedef make_vector<T> my_type;
			my_type& operator<< (const T& val) {
				data_.push_back(val);
				return *this;
			}
			operator std::vector<T>() const {
				return data_;
			}
		private:
			std::vector<T> data_;
		};


		std::string callstack();

		glm::vec3 hsv2rgb(glm::vec3 hsv);
		glm::vec3 rgb2hsv(glm::vec3 rgb);
		std::string rgb2hex(const glm::vec3 &rgb);
		glm::vec3 hex2rgb(std::string hex);


		bool isMainModule();

	}
}
