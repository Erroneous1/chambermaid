/*
 * Distrubuted under The MIT License (MIT)
 *
 * Copyright (c) 2016 Aaron Bishop
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BREWPIPP_TABLES_DEVICES_HPP
#define BREWPIPP_TABLES_DEVICES_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"
#include "groups.hpp"

namespace brewpipp {
	
	namespace tables {
		
		namespace devices_ {
			struct DeviceId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::must_not_insert>;
				};
			};
			
			struct DeviceType {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_type)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct DeviceIsGet {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_is_get)
					
					using _traits = sqlpp::make_traits<sqlpp::boolean,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct DeviceIsSet {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_is_set)
					
					using _traits = sqlpp::make_traits<sqlpp::boolean,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct DeviceLocation {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_location)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert>;
				};
			};
			
			struct GroupId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(group_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct DeviceConfig {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_config)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert>;
				};
			};
			
		}
		
		struct Devices : sqlpp::table_t<
			devices_::DeviceId,
			devices_::DeviceType,
			devices_::DeviceIsGet,
			devices_::DeviceIsSet,
			devices_::DeviceLocation,
			devices_::GroupId,
			devices_::DeviceConfig
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS devices ("
				"device_id "		"SMALLINT "		"NOT NULL AUTO INCREMENT,"
				"device_type "		"SMALLINT "		"NOT NULL,"
				"device_is_get "	"BIT "			"NOT NULL,"
				"device_is_set "	"BIT "			"NOT NULL,"
				"device_location "	"VARCHAR(255) "	"NOT NULL,"
				"group_id "			"TINYINT "		"NOT NULL,"
				"device_config "	"VARCHAR(255) "	"NOT NULL,"
				"PRIMARY KEY (device_id),"
				"FOREIGN KEY group_id REFERENCES groups(group_id)"
				")";
			struct _alias_t {
				SQLPP_COLUMN_NAMED(devices)
			};
			
			typedef int16_t device_id_t;
			typedef int16_t device_type_t;
			enum class DeviceType : device_type_t {
				SerialPort = 1,
				DS18B20 = 2,
				Compressor = 3,
				HeatingElement = 4
			};
		};
	}
}

#endif //BREWPIPP_TABLES_DEVICES_HPP