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

#include "tables/authority.hpp"
#include "tables/devices.hpp"
#include "tables/groups.hpp"
#include "tables/lcd.hpp"
#include "tables/logs.hpp"
#include "tables/measurements.hpp"
#include "tables/net_addresses.hpp"
#include "tables/runs.hpp"
#include "tables/sessions.hpp"
#include "tables/users.hpp"

namespace brewpipp {
	namespace tables {
		constexpr std::array<std::string,10> list_creates() {
			return std::array<std::string,10>({
				Authority::create,
				Devices::create,
				Groups::create,
				LCD::create,
				Logs::create,
				Measurements::create,
				NetAddresses::create,
				Runs::create,
				Sessions::create,
				Users::create
			});
		}
	}
}