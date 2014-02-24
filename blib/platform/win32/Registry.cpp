#include "Registry.h"

namespace blib
{
	namespace platform
	{
		namespace win32
		{


			RegistryKey::RegistryKey( void ){
				this->key = NULL;
			}

			RegistryKey::RegistryKey( HKEY start, std::string name ){
				this->start = start;
				this->name = name;
				this->key = NULL;
			}

			RegistryKey::~RegistryKey( void ){
				this->close();
			}

			bool RegistryKey::exists( void ){
				if( this->isOpen() ){
					return true;
				}

				bool ret = this->open();

				if( ret ){
					this->close();
				}

				return ret;
			}

			bool RegistryKey::create( void ){
				if( this->exists() ){
					return true;
				}else{
					if( RegCreateKeyExA( this->start, this->name.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &this->key, NULL ) == ERROR_SUCCESS ){
						this->close();

						return true;
					}else{
						return false;
					}
				}
			}

			bool RegistryKey::erase( void ){
				if( this->exists() ){
					if( this->isOpen() ){
						this->close();
					}
		
					return RegDeleteKeyA( this->start, this->name.c_str() ) == ERROR_SUCCESS;
				}else{
					return false;
				}
			}

			bool RegistryKey::open( void ){
				if( this->isOpen() || RegOpenKeyA( this->start, this->name.c_str(), &this->key ) == ERROR_SUCCESS ){
					return true;
				}else{
					return false;
				}
			}

			bool RegistryKey::isOpen( void ){
				return this->key != NULL;
			}

			void RegistryKey::close( void ){
				if( this->key ){
					RegCloseKey( this->key );
					this->key = NULL;
				}
			}

			HKEY RegistryKey::getKey( void ){
				if( this->open() ){
					return this->key;
				}else{
					return NULL;
				}
			}

			RegistryValue::RegistryValue( RegistryKey key, std::string name ){
				this->key = key;
				this->name = name;
			}

			RegistryValue::~RegistryValue( void ){
				this->key.close();
			}

			bool RegistryValue::exists( void ){
				return RegQueryValueExA( this->key.getKey(), this->name.c_str(), NULL, NULL, NULL, NULL ) == ERROR_FILE_NOT_FOUND;
			}

			std::string RegistryValue::readString( std::string defaultValue ){
				TCHAR ret[512];
				DWORD size(sizeof(ret));

				if( RegQueryValueExA( this->key.getKey(), this->name.c_str(),  0, NULL, reinterpret_cast<LPBYTE>( &ret ), &size ) == ERROR_SUCCESS ){
					return std::string( (char *)ret );
				}else{
					return defaultValue;
				}
			}

			bool RegistryValue::writeString( std::string value ){
				return RegSetValueExA( this->key.getKey(), this->name.c_str(), 0, REG_SZ, (BYTE *)value.c_str(), value.length() ) == ERROR_SUCCESS;
			}

			int RegistryValue::readInt( int defaultValue ){
				DWORD ret(0);
				DWORD size(4);

				if( RegQueryValueExA( this->key.getKey(), this->name.c_str(),  0, NULL, reinterpret_cast<LPBYTE>( &ret ), &size ) == ERROR_SUCCESS ){
					return ret;
				}else{
					return defaultValue;
				}
			}

			bool RegistryValue::writeInt( int value ){
				return RegSetValueExA( this->key.getKey(), this->name.c_str(), 0, REG_DWORD, (BYTE *)&value, sizeof( int ) ) == ERROR_SUCCESS;
			}

			long RegistryValue::readLong( long defaultValue ){
				// TODO: doesnt work, whyever...
				/*DWORD ret(0);
				DWORD size(sizeof(long));

				if( RegQueryValueExA( this->key.getKey(), this->name.c_str(),  0, NULL, reinterpret_cast<LPBYTE>( &ret ), &size ) == ERROR_SUCCESS ){
					return ret;
				}else{
					return defaultValue;
				}*/
				return defaultValue;
			}

			bool RegistryValue::writeLong( long value ){
				// TODO: also doesnt work...
				// return RegSetValueExA( this->key.getKey(), this->name.c_str(), 0, REG_QWORD, (BYTE *)&value, sizeof( long ) ) == ERROR_SUCCESS;
				return false;
			}

		}
	}
}
