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

#ifndef BREWPIPP_TABLES_NET_ADDRESSES_HPP
#define BREWPIPP_TABLES_NET_ADDRESSES_HPP

#include <sqlpp11/table.h>
#include "table_helper.hpp"

namespace brewpipp {
	namespace tables {
		namespace net_addresses_ {
			struct NetAddressId {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(net_address_id)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::must_not_update,sqlpp::must_not_insert>;
				}
			};
			
			struct NetAddressIP {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(net_address_ip)
					
					using _traits = sqlpp::make_traits<sqlpp::text,sqlpp::require_insert>;
				};
			};
			
			struct NetAddressNetMask {
				struct _alias_t {
					SQLPP_COLUMN_NAMED(net_address_mask)
					
					using _traits = sqlpp::make_traits<sqlpp::integral,sqlpp::require_insert>;
				};
			};
		}
		
		struct NetAddresses : sqlpp::table_t<
			net_addresses_::NetAddressId,
			net_addresses_::NetAddressIP,
			net_addresses_::NetAddressNetMask
		> {
			static constexpr const char create[] =
				"CREATE TABLE IF NOT EXISTS net_addresses ("
				"net_address_id "	"SMALLINT "		"AUTO INCREMENT NOT NULL,"
				"net_address_ip "	"BINARY(16) "	"NOT NULL,"
				"net_address_mask "	"TINYINT "		"UNSIGNED NOT NULL,"
				"FOREIGN KEY (user_id) REFERENCES users(user_id),"
				"FOREIGN KEY (net_address_id) REFERENCES net_addresses(net_address_id),"
				"CONSTRAINT unique_net_addresses UNIQUE KEY (user_id, net_address, net_addresses)"
				")";
			struct _alias_t {
				SQLPP_COLUMN_NAMED(net_addresses)
			};
		};
	}
}

#endif //BREWPIPP_TABLES_NET_ADDRESSES_HPP