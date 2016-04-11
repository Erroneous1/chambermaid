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

#ifndef BREWPIPP_TABLES_RUNS_HPP
#define BREWPIPP_TABLES_RUNS_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	
	namespace tables {
		
		namespace runs_ {
			struct RunId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(run_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::must_not_insert>;
				};
			};
			
			struct RunName {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(run_name)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert>;
				};
			};
			
			struct RunsStart {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(run_start)
					
					using _traits = sqlpp::make_traits<sqlpp::time_point,sqlpp::require_insert>;
				};
			};
			
			struct RunStop {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(run_stop)
					
					using _traits = sqlpp::make_traits<sqlpp::time_point,sqlpp11::must_not_insert,sqlpp::can_be_null>;
				};
			};
			
			struct GroupId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(group_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::require_insert,sqlpp::must_not_update>;
				};
			};
		}
		
		struct Runs : sqlpp::table_t<
			runs_::RunId,
			runs_::RunName,
			runs_::RunStart,
			runs_::RunStop,
			runs_::GroupId
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS runs ("
				"run_id "		"BIGINT "		"NOT NULL AUTO INCREMENT,"
				"run_name "		"VARCHAR(32)"	"NOT NULL,"
				"run_start "	"DATETIME "		"NOT NULL,"
				"run_stop "		"DATETIME "		"DEFAULT NULL,"
				"group_id "		"TINYINT "		"NOT NULL,"
				"PRIMARY KEY (run_id),"
				"FOREIGN KEY group_id REFERENCES groups(group_id)"
				") AUTO_INCREMENT=1";
			struct _alias_t {
				SQLPP_COLUMN_NAMED(runs)
			};
		};
	}
}

#endif //BREWPIPP_TABLES_RUNS_HPP