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

#ifndef BREWPIPP_TABLES_LCD_HPP
#define BREWPIPP_TABLES_LCD_HPP

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include "table_helper.hpp"

namespace brewpipp {
	namespace tables {
		namespace lcd_ {
			struct Line1 {
				SQLPP_COLUMN_NAMED(lcd_line_1)
				using _traits = sqlpp::make_traits<sqlpp::varchar>;
			};
			struct Line2 {
				SQLPP_COLUMN_NAMED(lcd_line_2)
				using _traits = sqlpp::make_traits<sqlpp::varchar>;
			};
			struct Line3 {
				SQLPP_COLUMN_NAMED(lcd_line_3)
				using _traits = sqlpp::make_traits<sqlpp::varchar>;
			};
			struct Line4 {
				SQLPP_COLUMN_NAMED(lcd_line_4)
				using _traits = sqlpp::make_traits<sqlpp::varchar>;
			};
			struct GroupId {
				SQLPP_COLUMN_NAMED(group_id)
				using _traits = sqlpp::make_traits<sqlpp::tinyint, sqlpp::tag::must_not_update, sqlpp::tag::require_insert>;
			};
		}
		struct LCD : sqlpp::table_t<LCD,
			lcd_::GroupId,
			lcd_::Line1,
			lcd_::Line2,
			lcd_::Line3,
			lcd_::Line4
		> {
			static constexpr const char create[] =
			"CREATE TABLE IF NOT EXISTS lcd ("
			"group_id "			"TINYINT "		"NOT NULL,"
			"lcd_line_1 "		"VARCHAR(16) "	"NOT NULL DEFAULT 'Needs Setup',"
			"lcd_line_2 "		"VARCHAR(16) "	"NOT NULL DEFAULT '',"
			"lcd_line_3 "		"VARCHAR(16) "	"NOT NULL DEFAULT '',"
			"lcd_line_4 "		"VARCHAR(16) "	"NOT NULL DEFAULT '',"
			"FOREIGN KEY group_id REFERENCES groups(group_id),"
			"CONSTRAINT (unique_group) UNIQUE KEY (group_id)"
			")";
			SQLPP_TABLE_NAMED(lcd)
		};
	}
}

#endif //BREWPIPP_TABLES_LCD_HPP