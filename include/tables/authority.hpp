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

#ifndef BREWPIPP_TABLES_AUTHORITY_HPP
#define BREWPIPP_TABLES_AUTHORITY_HPP

#include <bitset>
#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	namespace tables {
		namespace authority_ {
			struct UserId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(user_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert,sqlpp::can_be_null>;
				};
			};
			
			struct AuthorityKey {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(authority_key)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				}
			};
			
			struct NetAddressId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(net_address_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::require_insert>;
				};
			};
		}
		
		struct Authority : sqlpp::table_t<
			authority_::UserId,
			authority_::AuthorityKey,
			authority_::NetAddressId
		> {
			struct _alias_t {
				SQLPP_COLUMN_NAMED(authority)
			};
			
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS authority ("
				"user_id "			"SMALLINT "		"AUTO INCREMENT,"
				"authority_value "	"INT "			"NOT NULL,"
				"net_address_id "	"SMALLINT "		"NOT NULL,"
				"FOREIGN KEY (user_id) REFERENCES users(user_id),"
				"FOREIGN KEY (net_address_id) REFERENCES net_addresses(net_address_id),"
				"CONSTRAINT unique_authority UNIQUE KEY (user_id, net_address, authority)"
				")";
		};
	}
}

#endif //BREWPIPP_TABLES_AUTHORITY_HPP