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

#ifndef BREWPIPP_TABLES_MEASUREMENTS_HPP
#define BREWPIPP_TABLES_MEASUREMENTS_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	namespace tables {
		namespace measurements_ {
			struct Timestamp {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(timestamp)
					
					using _traits = sqlpp::make_traits<sqlpp::time_point,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct Value {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(value)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct DeviceId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(device_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
		}
		
		struct Measurements : sqlpp::table_t<
			measurements_::Timestamp,
			measurements_::Value,
			measurements_::DeviceId
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS measurements ("
				"timestamp "		"DATETIME "	"NOT NULL,"
				"value "			"SMALLINT "	"NOT NULL,"
				"device_id "		"SMALLINT "	"NOT NULL,"
				"FOREIGN KEY (device_id) REFERENCES devices(device_id),"
				"CONSTRAINT unique_measurements UNIQUE (timestamp, device_id)"
				")";
			struct _alias_t {
				SQLPP_COLUMN_NAMED(measurements)
			};
		};
	}
}

#endif //BREWPIPP_TABLES_MEASUREMENTS_HPP