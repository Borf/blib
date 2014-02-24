#if defined (_MSC_VER) && (_MSC_VER >= 1020)
	#pragma once
#endif

#include<Windows.h>
#include <string>

namespace blib
{
	namespace platform
	{
		namespace win32
		{
			class RegistryKey
			{
			private:
				HKEY start;
				std::string name;
				HKEY key;

			public:
				RegistryKey( void );
				RegistryKey( HKEY start, std::string name );
				~RegistryKey( void );

				bool exists( void );
				bool create( void );
				bool erase( void );
				bool open( void );
				bool isOpen( void );
				void close( void );
				HKEY getKey( void );
			};

			class RegistryValue
			{
			private:
				RegistryKey key;
				std::string name;

				long size( void );

			public:
				RegistryValue( RegistryKey key, std::string name );
				~RegistryValue( void );

				bool exists( void );

				// REG_SZ
				std::string readString( std::string defaultValue );
				bool writeString( std::string value );

				// REG_DWORD
				int readInt( int defaultValue );
				bool writeInt( int value );

				// REG_QWORD
				long readLong( long defaultValue );
				bool writeLong( long value );
			};

		}
	}
}
