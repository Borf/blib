#include "json.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <set>
#include <cstdlib>

namespace blib
{
	namespace json
	{
	
		Value Value::null;
		//constructors
		Value::Value()
		{
			type = Type::nullValue;
			value.objectValue = NULL;
		}

		Value::Value(Type type)
		{
			this->type = type;
			if (type == Type::stringValue)
				value.stringValue = new std::string();
			if (type == Type::arrayValue)
				value.arrayValue = new std::vector<Value>();
			if (type == Type::objectValue)
				value.objectValue = new std::map<std::string, Value>();
		}

		Value::Value(int value)
		{
			type = Type::intValue;
			this->value.intValue = value;
		}

		Value::Value(float value)
		{
			type = Type::floatValue;
			this->value.floatValue = value;
		}

		Value::Value(bool value)
		{
			type = Type::boolValue;
			this->value.boolValue = value;
		}

		Value::Value(const std::string &value)
		{
			type = Type::stringValue;
			this->value.stringValue = new std::string();
			this->value.stringValue->assign(value);
		}
		Value::Value(const char* value)
		{
			type = Type::stringValue;
			this->value.stringValue = new std::string();
			this->value.stringValue->assign(value);
		}

		Value::Value(const Value& other) : Value(other.type)
		{
			if (type == Type::objectValue)
				*this->value.objectValue = *other.value.objectValue;
			else if (type == Type::arrayValue)
				*this->value.arrayValue = *other.value.arrayValue;
			else if (type == Type::stringValue)
				this->value.stringValue->assign(*other.value.stringValue);
			else
				this->value = other.value;
		}

		void Value::operator=(const Value& other)
		{
			if (type != other.type)
			{
				if (type == Type::stringValue)
					delete value.stringValue;
				if (type == Type::arrayValue)
					delete value.arrayValue;
				if (type == Type::objectValue)
					delete value.objectValue;
				this->type = other.type;
				if (type == Type::stringValue)
					value.stringValue = new std::string();
				if (type == Type::arrayValue)
					value.arrayValue = new std::vector<Value>();
				if (type == Type::objectValue)
					value.objectValue = new std::map<std::string, Value>();
			}

			if (type == Type::objectValue)
				*this->value.objectValue = *other.value.objectValue;
			else if (type == Type::arrayValue)
				*this->value.arrayValue = *other.value.arrayValue;
			else if (type == Type::stringValue)
				this->value.stringValue->assign(*other.value.stringValue);
			else
				this->value = other.value;
		}

		Value::~Value()
		{
			if (type == Type::stringValue)
				delete value.stringValue;
			else if (type == Type::arrayValue)
				delete value.arrayValue;
			else if (type == Type::objectValue)
				delete value.objectValue;
		}

		size_t Value::size() const
		{
			assert(type == Type::arrayValue || type == Type::objectValue);
			if (type == Type::arrayValue)
				return value.arrayValue->size();
			else if (type == Type::objectValue)
				return value.objectValue->size();
			throw "Unsupported";
		}

		void Value::push_back(const Value& value)
		{
			assert(type == Type::arrayValue || type == Type::nullValue);
			if (type == Type::nullValue)
			{
				type = Type::arrayValue;
				this->value.arrayValue = new std::vector<Value>();
			}
			this->value.arrayValue->push_back(value);
		}


		Value& Value::operator[](const std::string &key)
		{
			assert(type == Type::objectValue);
			return (*value.objectValue)[key];
		}
		Value& Value::operator[](const std::string &key) const
		{
			assert(type == Type::objectValue);
			return (*value.objectValue)[key];
		}

		Value& Value::operator[](const char* key)
		{
			assert(type == Type::objectValue || type == Type::nullValue);
			if (type == Type::nullValue)
			{
				type = Type::objectValue;
				value.objectValue = new std::map<std::string, Value>();
			}
			return (*value.objectValue)[std::string(key)];
		}

		Value& Value::operator[](const char* key) const
		{
			assert(type == Type::objectValue);
			return (*value.objectValue)[std::string(key)];
		}

		Value& Value::operator[](size_t index)
		{
			assert(type == Type::arrayValue);
			return (*value.arrayValue)[index];
		}
		Value& Value::operator[](size_t index) const
		{
			assert(type == Type::arrayValue);
			return (*value.arrayValue)[index];
		}
		Value& Value::operator[](int index)
		{
			assert(type == Type::arrayValue);
			return (*value.arrayValue)[index];
		}
		Value& Value::operator[](int index) const
		{
			assert(type == Type::arrayValue);
			return (*value.arrayValue)[index];
		}

		void Value::erase(size_t index)
		{
			throw "Cannot cast";
		}














		Value::Iterator Value::end() const
		{
			if (type == Type::objectValue)
				return Iterator(value.objectValue->end());
			else if (type == Type::arrayValue)
				return Iterator(value.arrayValue->end());
			throw "oops";
		}

		Value::Iterator Value::begin() const
		{
			if (type == Type::objectValue)
				return Iterator(value.objectValue->begin());
			else if (type == Type::arrayValue)
				return Iterator(value.arrayValue->begin());
			throw "oops";
		}

		///iterator stuff

		Value Value::Iterator::operator*()
		{
			if (type == Type::objectValue)
				return this->objectIterator->second;
			else  if (type == Type::arrayValue)
				return *arrayIterator;
			throw "Oops";
		}

		bool Value::Iterator::operator!=(const Iterator &other)
		{
			if (type == Type::objectValue)
				return this->objectIterator != other.objectIterator;
			else  if (type == Type::arrayValue)
				return this->arrayIterator != other.arrayIterator;
			throw "Oops";
		}

		void Value::Iterator::operator++()
		{
			if (type == Type::objectValue)
				this->objectIterator++;
			else  if (type == Type::arrayValue)
				this->arrayIterator++;
		}
		void Value::Iterator::operator++(int)
		{
			if (type == Type::objectValue)
				this->objectIterator++;
			else  if (type == Type::arrayValue)
				this->arrayIterator++;
		}

		Value::Iterator::Iterator(const std::vector<Value>::iterator& arrayIterator)
		{
			type = Type::arrayValue;
			this->arrayIterator = arrayIterator;
		}

		Value::Iterator::Iterator(const std::map<std::string, Value>::iterator& objectIterator)
		{
			type = Type::objectValue;
			this->objectIterator = objectIterator;
		}

		std::string Value::Iterator::key()
		{
			assert(type == Type::objectValue);
			return this->objectIterator->first;
		}
		Value& Value::Iterator::value()
		{
			assert(type == Type::objectValue);
			return this->objectIterator->second;
		}







		static void ltrim(std::istream& stream);
		static void eatComment(std::istream& stream);
		static Value eatString(std::istream& stream);
		static Value eatObject(std::istream& stream);
		static Value eatArray(std::istream& stream);
		static Value eatNumeric(std::istream& stream, char firstChar);
		static Value eatBool(std::istream& stream);
		static Value eatNull(std::istream& stream);
		static Value eatValue(std::istream& stream);

		//reading
		static void ltrim(std::istream& stream)
		{
			char c = stream.peek();
			while (c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{
				stream.get();
				c = stream.peek();
			}
		};


		static Value eatString(std::istream& stream)
		{
			std::string value = "";
			bool escaped = false;
			while (!stream.eof())
			{
				char c = stream.get();
				if (c == '\\' && !escaped)
					escaped = !escaped;
				else if (c == '\"' && !escaped)
					return Value(value);
				else
				{
					value += c;
					escaped = false;
				}
			}
			return Value(value);
		};




		static Value eatObject(std::istream& stream)
		{
			Value obj(Type::objectValue);
			while (!stream.eof())
			{
				ltrim(stream);
				char token = stream.get();
				if (token == '}')
					break; //empty object
				if (token == '/')
				{
					eatComment(stream);
					token = stream.get();
				}

				assert(token == '"');
				Value key = eatString(stream);
				ltrim(stream);
				token = stream.get();
				assert(token == ':');
				ltrim(stream);
				Value val = eatValue(stream);
				obj[key.asString()] = val;
				ltrim(stream);

				token = stream.get();
				if (token == '}')
					break;
				assert(token == ',');
			}
			return obj;
		};
		static Value eatArray(std::istream& stream)
		{
			Value obj(Type::arrayValue);
			while (!stream.eof())
			{
				ltrim(stream);
				if (stream.peek() == ']')
				{
					stream.get();
					break;
				}
				obj.push_back(eatValue(stream));
				ltrim(stream);
				char token = stream.get();
				if (token == '/')
				{
					eatComment(stream);
					token = stream.get();
				}
				if (token == ']')
					break;
				assert(token == ',');
			}
			return obj;
		};
		static Value eatNumeric(std::istream& stream, char firstChar)
		{
			std::string numeric(1, firstChar);
			while (!stream.eof())
			{
				char token = stream.peek();
				if ((token >= '0' && token <= '9') || token == '.' || token == '-')
					numeric += stream.get();
				else
					break;
			}
			if (numeric.find('.') == std::string::npos)
				return Value(atoi(numeric.c_str()));
			else
				return Value((float)atof(numeric.c_str()));
		};

		static Value eatBool(std::istream& stream)
		{
			char token = stream.get();
			if (token == 'a') //fAlse
			{
				stream.get(); //l
				stream.get(); //s
				stream.get(); //e
				return false;
			}
			else if (token == 'r') //tRue
			{
				stream.get(); // u
				stream.get(); // e
				return true;
			}
			return Value(Type::nullValue);
		};
		static Value eatNull(std::istream& stream)
		{
			return Value(Type::nullValue);
		};

		//precondition: / is already eaten
		static void eatComment(std::istream& stream)
		{
			char token = stream.get();
			assert(token == '/' || token == '*');
			if (token == '*')
			{
				char last = token;
				while ((last != '*' || token != '/') && !stream.eof())
				{
					last = token;
					token = stream.get();
				}
			}
			else if (token == '/')
				while (token != '\n' && !stream.eof())
					token = stream.get();
			ltrim(stream);
		}


		static Value eatValue(std::istream& stream)
		{
			ltrim(stream);
			char token = stream.get();
			if (token == '{')
				return eatObject(stream);
			if (token == '[')
				return eatArray(stream);
			if ((token >= '0' && token <= '9') || token == '.' || token == '-')
				return eatNumeric(stream, token);
			if (token == '"')
				return eatString(stream);
			if (token == 't' || token == 'f')
				return eatBool(stream);
			if (token == 'n')
				return eatNull(stream);
			if (token == '/')
			{
				eatComment(stream);
				return eatValue(stream);
			}
			throw "Unable to parse json";
		};


		Value readJson(const std::string &data)
		{
			std::stringstream stream;
			stream << data;
			return eatValue(stream);
		}

		Value readJson(std::istream &stream)
		{
			return eatValue(stream);
		}



		std::ostream& indent(std::ostream& stream, int level)
		{
			for (int i = 0; i < level; i++)
				stream << '\t';
			return stream;
		}

		std::ostream& Value::prettyPrint(std::ostream& stream, blib::json::Value& printConfig, int level) const
		{
			stream << std::fixed << std::setprecision(6);
			switch (type)
			{
			case Type::intValue:
				stream << value.intValue;
				break;
			case Type::floatValue:
				assert(!isnan(value.floatValue));
				//assert(isnormal(value.floatValue));
				if (value.floatValue >= 0)
					stream << " ";
				stream << value.floatValue;
				break;
			case Type::boolValue:
				stream << (value.boolValue ? "true" : "false");
				break;
			case Type::stringValue:
				stream << "\"" << *value.stringValue << "\""; //TODO: escape \'s
				break;
			case Type::arrayValue:
			{
				stream << "[";
				int wrap = 99999;
				if (value.arrayValue->size() > 10)
					wrap = 1;
				if (value.arrayValue->at(0).isArray() || value.arrayValue->at(0).isObject())
					wrap = 1;
				else
					wrap = 3;

				std::string seperator = " ";

				if (!printConfig.isNull() && printConfig.isMember("wrap"))
					wrap = printConfig["wrap"];
				if (!printConfig.isNull() && printConfig.isMember("seperator"))
					seperator = printConfig["seperator"].asString();


				int index = 0;

                if ((long)size() > wrap)
				{
					stream << "\n";
					indent(stream, level + 1);
				}
				for (auto v : *this)
				{
					if (index > 0)
					{
						stream << "," << seperator;
						if (index % wrap == 0)
						{
							stream << "\n";
							indent(stream, level + 1);
						}
					}

					blib::json::Value childPrintConfig = blib::json::Value::null;
					if (!printConfig.isNull())
					{
						if (printConfig.isMember("elements"))
							childPrintConfig = printConfig["elements"];
						else if (printConfig.isMember("recursive") && printConfig["recursive"].asBool() == true)
							childPrintConfig = printConfig;
					}

					v.prettyPrint(stream, childPrintConfig, level + 1);
					index++;
				}
                if ((long)size() > wrap)
				{
					stream << "\n";
					indent(stream, level);
				}
				stream << "]";
				break;
			}
			case Type::objectValue:
			{
				stream << "{\n";
				int wrap = 99999;
				if (value.arrayValue->size() > 10)
					wrap = 1;
				if (value.arrayValue->at(0).isArray() || value.arrayValue->at(0).isObject())
					wrap = 1;
				else
					wrap = 3;

				std::string seperator = " ";

				if (!printConfig.isNull() && printConfig.isMember("wrap"))
					wrap = printConfig["wrap"];
				if (!printConfig.isNull() && printConfig.isMember("seperator"))
					seperator = printConfig["seperator"].asString();


				int index = 0;
				indent(stream, level + 1);



				//for (auto v : *value.objectValue)
				auto printEl = [&](const std::pair<const std::string&, const Value&> v)
				{
					if (index > 0)
					{
						stream << "," << seperator;
						if (index % wrap == 0)
						{
							stream << "\n";
							indent(stream, level + 1);
						}
					}
					stream << "\"" << v.first << "\" : ";
                    if (
                            (v.second.isArray() || v.second.isObject()) &&
                            (printConfig.isNull() ||
                             (
                                 printConfig.isMember(v.first) &&
								 printConfig[v.first].isObject() &&
                                 printConfig[v.first].isMember("wrap") &&
                                 printConfig[v.first]["wrap"].asInt() < (int)v.second.size())
                             )
                            )
					{
						stream << "\n";
						indent(stream, level + 1);
					}

					blib::json::Value childPrintConfig = blib::json::Value::null;
					if (!printConfig.isNull())
					{
						if (printConfig.isMember(v.first))
							childPrintConfig = printConfig[v.first];
						else if (printConfig.isMember("recursive") && printConfig["recursive"].asBool() == true)
							childPrintConfig = printConfig;
					}

					v.second.prettyPrint(stream, childPrintConfig, level + 1);;
					index++;
				};



				std::set<std::string> printed;
				if (!printConfig.isNull())
				{
					if (printConfig.isMember("sort"))
					{
						for (std::string el : printConfig["sort"])
						{
							if (isMember(el))
							{
								printEl(std::pair<const std::string&, const Value&>(el, (*value.objectValue)[el]));
								printed.insert(el);
							}
						}
					}
				}

				for (auto v : *value.objectValue)
					if (printed.find(v.first) == printed.end())
						printEl(v);

				stream << "\n";
				indent(stream, level);
				stream << "}";
				break;
			}
			case Type::nullValue:
				stream << "null";
				break;
			}
			return stream;
		}


		std::ostream & operator<<(std::ostream &stream, const Value& value)
		{
			stream << std::fixed<< std::setprecision(6);
			switch (value.type)
			{
			case Type::intValue:
				stream << value.value.intValue;
				break;
			case Type::floatValue:
				assert(!isnan(value.value.floatValue));
				//assert(isnormal(value.value.floatValue));
				stream << value.value.floatValue;
				break;
			case Type::boolValue:
				stream << (value.value.boolValue ? "true" : "false");
				break;
			case Type::stringValue:
				stream << "\"" << *value.value.stringValue << "\""; //TODO: escape \'s
				break;
			case Type::arrayValue:
			{
				stream << "[";
				bool first = true;
				for (auto v : value)
				{
					if (!first)
						stream << ", ";
					stream << v;
					first = false;
				}
				stream << "]";
				break;
			}
			case Type::objectValue:
			{
				stream << "{";
				bool first = true;
				for (auto v : *value.value.objectValue)
				{
					if (!first)
						stream << ", ";
					stream << "\"" << v.first << "\" : " << v.second << std::endl;
					first = false;
				}
				stream << "}";
				break;
			}
			case Type::nullValue:
				stream << "null";
				break;
			}
			return stream;
		}
	}
}
