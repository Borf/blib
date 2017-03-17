#pragma once

#pragma error("Oops")

#if 0
#include <string>
#include <map>
#include <vector>
#include <assert.h>

namespace blib
{
	namespace json
	{
		enum class Type
		{
			intValue,
			floatValue,
			boolValue,
			stringValue,
			arrayValue,
			objectValue,
			nullValue
		};

        class Value
		{
		public:
			Type type;
			union ValueHolder
			{
				int								intValue;
				float							floatValue;
				bool							boolValue;
				std::string*					stringValue;
				std::vector<Value>*				arrayValue;
				std::map<std::string, Value>*	objectValue;
			} value;

			static Value null;

			Value();
			Value(Type type);
			Value(int value);
			Value(float value);
			Value(bool value);
			Value(const std::string &value);
			Value(const char* value);
			Value(const Value& other);
			virtual ~Value();

			void operator = (const Value& other);




			inline operator int() const						{ return asInt(); }
			inline operator float()	const					{ return asFloat(); }
			inline operator bool() const					{ return asBool(); }
			inline operator const std::string&() const		{ return asString(); }


			inline int asInt() const					{ assert(type == Type::intValue); return value.intValue; }
			inline float asFloat() const				{ assert(type == Type::floatValue || type == Type::intValue); return type == Type::floatValue ? value.floatValue : value.intValue; }
			inline bool asBool() const					{ assert(type == Type::boolValue); return value.boolValue; }
			inline const std::string& asString() const	{ assert(type == Type::stringValue); return *value.stringValue; }
			inline bool isNull() const					{ return type == Type::nullValue; }
			inline bool isString() const				{ return type == Type::stringValue; }
			inline bool isInt() const					{ return type == Type::intValue; }
			inline bool isBool() const					{ return type == Type::boolValue; }
			inline bool isFloat() const					{ return type == Type::floatValue; }
			inline bool isObject() const				{ return type == Type::objectValue; }
			inline bool isArray() const					{ return type == Type::arrayValue;  }
			inline bool isMember(const std::string &name) const				{ assert(type == Type::objectValue); return value.objectValue->find(name) != value.objectValue->end(); }
			//array/object
			virtual size_t size() const;
			//array
			virtual void push_back(const Value& value);
			virtual void erase(size_t index);
			virtual Value& operator [] (size_t index);
			virtual Value& operator [] (int index);
			virtual Value& operator [] (size_t index) const;
			virtual Value& operator [] (int index) const;

			virtual Value& operator [] (const std::string &key);
			virtual Value& operator [] (const char* key);
			virtual Value& operator [] (const std::string &key) const;
			virtual Value& operator [] (const char* key) const;

			virtual bool operator == (const std::string &other) { return asString() == other; }
			virtual bool operator == (const int other) { return asInt() == other; }
			virtual bool operator == (const float other) { return asFloat() == other; }


			std::ostream& prettyPrint(std::ostream& stream, json& printConfig = null, int level = 0) const;

			class Iterator;
			Iterator begin() const;
			Iterator end() const;
		};

        class Value::Iterator
        {
        private:
            Type type;
            std::map<std::string, Value>::iterator objectIterator;
            std::vector<Value>::iterator arrayIterator;
        public:
            Iterator(const std::map<std::string, Value>::iterator& objectIterator);
            Iterator(const std::vector<Value>::iterator& arrayIterator);
            
            void operator ++();
            void operator ++(int);
            bool operator != (const Iterator &other);
            Value operator*();
            
            std::string key();
            Value& value();
            
        };



		Value readJson(const std::string &data);
		Value readJson(std::istream &stream);
		std::ostream &operator << (std::ostream &stream, const Value& value);	//serializes json data


	}
}
#endif