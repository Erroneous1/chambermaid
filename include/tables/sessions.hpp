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

#ifndef BREWPIPP_TABLES_SESSIONS_HPP
#define BREWPIPP_TABLES_SESSIONS_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	namespace tables {
		namespace sessions_ {
			struct SessionsKey {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(session_key)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct SessionsLogin {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(session_login)
					
					using _traits = sqlpp::make_traits<sqlpp::time_point,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			struct SessionsLogout {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(session_logout)
					
					using _traits = sqlpp::make_traits<sqlpp::time_point,sqlpp::can_be_null>;
				};
			};
			
			struct UserId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(user_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
			
			
			struct NetAddressId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(net_address_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				}
			};
			
			struct NetAddressIP {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(net_address_ip)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert>;
				};
			};
		}
		
		struct Sessions : sqlpp::table_t<
			sessions_::SessionsKey,
			sessions_::SessionsLogin,
			sessions_::SessionsLogout,
			sessions_::UserId,
			sessions_::NetAddressId,
			sessions_::NetAddressIP
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS sessions ("
				"session_key "		"VARBINARY(255) "	"NOT NULL,"
				"session_login "	"DATETIME "			"NOT NULL,"
				"session_logout "	"DATETIME "			"DEFAULT NULL,"
				"user_id "			"SMALLINT "			"NOT NULL,"
				"net_address_id "	"SMALLINT "			"NOT NULL,"
				"net_address_ip "	"BINARY(16) "		"NOT NULL,"
				"FOREIGN KEY (user_id) REFERENCES users(user_id),"
				"FOREIGN KEY (net_address_id) REFERENCES net_addresses(net_address_id),"
				"CONSTRAINT unique_session UNIQUE KEY (user_id, session_key)"
				")";
			struct _alias_t {
				SQLPP_COLUMN_NAMED(sessions)
			};
		};
	}
}

#endif //BREWPIPP_TABLES_SESSIONS_HPP