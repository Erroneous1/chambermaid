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

#ifndef BREWPIPP_TABLES_GROUPS_HPP
#define BREWPIPP_TABLES_GROUPS_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	
	namespace tables {
		
		namespace groups_ {
			struct GroupId {
				SQLPP_COLUMN_NAMED(group_id)
					
				using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::tag::must_not_update,sqlpp::tag::must_not_insert>;
			};
			
			struct GroupName {
				SQLPP_COLUMN_NAMED(group_name)
					
				using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::tag::require_insert>;
			};
		}
		
		struct Groups : sqlpp::table_t<Groups,
			groups_::GroupId,
			groups_::GroupName
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS groups ("
				"group_id "		"TINYINT "		"NOT NULL AUTO INCREMENT,"
				"group_name "	"VARCHAR(32)"	"NOT NULL,"
				"PRIMARY KEY (group_id),"
				"CONSTRAINT unique_name UNIQUE (group_name)"
				") AUTO_INCREMENT=1";
			
			SQLPP_COLUMN_NAMED(groups)
			
			typedef uint8_t group_id_t;
		};
	}
}

#endif //BREWPIPP_TABLES_GROUPS_HPP