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

#ifndef BREWPIPP_TABLES_USERS_HPP
#define BREWPIPP_TABLES_USERS_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	namespace tables {
		namespace users_ {
			struct UserId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(user_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::must_not_insert>;
				};
			};
			
			struct UserEmail {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(user_name)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert,sqlpp::must_not_update>;
				};
			};
			
			struct UserPassword {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(user_password)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert>;
				};
			};
		}
		
		struct Users : sqlpp::table_t<
			users_::UserId,
			users_::UserEmail,
			users_::UserPassword
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS users ("
				"user_id "			"SMALLINT "		"NOT NULL AUTO INCREMENT,"
				"user_email "		"VARCHAR(255) "	"NOT NULL,"
				"user_password "	"VARCHAR(255) "	"NOT NULL,"
				"PRIMARY KEY (user_id),"
				"CONSTRAINT unique_email UNIQUE KEY (user_email)"
				")";
			struct _alias_t {
				SQLPP_COLUMN_NAMED(devices)
			};
		};
	}
}

#endif //BREWPIPP_TABLES_USERS_HPP