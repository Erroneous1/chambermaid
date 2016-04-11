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

#include "util/serial.hpp"
#include <boost/asio.hpp>
#include <algorithm>
#include <json/json.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <iostream>

namespace brewpipp {
	namespace util {
		
		FwVersionInfo::FwVersionInfo(const Json::Value& reply) :
			version(reply["v"]),
			is_simulator(reply.get("y", Json::Value(false))),
			log(reply.get("l", Json::Value(std::string()))),
			build(reply.get("n", Json::Value(std::string()))),
			commit(reply.get("c", Json::Value(std::string()))),
			shield(reply.get("s", Json::Value(std::string()))),
			board(reply.get("b", Json::Value(std::string()))) {}
		
		std::string FwVersionInfo::to_string() const {
			Json::Value root;
			root["v"] = version;
			root["y"] = is_simulator;
			root["b"] = board;
			root["s"] = shield;
			root["l"] = log;
			root["n"] = build;
			root["c"] = commit;
			return Json::FastWriter().write(root);
		}
		
		
		std::string strip_carriage_return(const std::string& s) {
			return std::string(s.begin(), s.begin()+std::min(s.rfind('\r'),s.length()));
		}
		FwVersionInfo get_version_info(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException) {
			std::string line;
			buffered_io.write("n");
			for(uint8_t tries = 10; tries != 0 && (line.empty() || line[0] != 'N'); tries--) {
				try {
					const auto data = buffered_io.readStringUntil(boost::posix_time::seconds(1), "\n");
					if(data.empty()) {
						buffered_io.write("n");
					} else {
						line = strip_carriage_return(data);
					}
				}catch(const BufferedIO::TimeoutException& e) {
					buffered_io.write("n");
				}
			}
			
			if(line.length() < 2 || line[0] != 'N') {
				return FwVersionInfo();
			}
			Json::Value reply;
			if(line.length() < 2 || line[0] != 'N' || !Json::Reader().parse(line.substr(2), reply)) {
				return FwVersionInfo();
			}
			return FwVersionInfo(reply);
		}
		
		void sound_alarm(BufferedIO& buffered_io, const bool& isOn) throw(boost::system::system_error) {
			buffered_io.write(isOn ? "A" : "a");
		}
		
		Json::Value compact_or_verbose(const Json::Value& v, const std::string& compact, const std::string& verbose) {
			const auto compact_find = v.find(compact.c_str(), compact.c_str()+compact.size());
			if(compact_find) {
				return Json::Value(*compact_find);
			} else {
				return Json::Value(v[verbose]);
			}
		}
		
		Temperatures::Temperatures(const Json::Value& reply) :
		beerReading(compact_or_verbose(reply, "bt", "BeerTemp").asDouble()),
		beerSetting(compact_or_verbose(reply, "bs", "BeerSet").asDouble()),
		beerAnnotation(compact_or_verbose(reply, "ba", "BeerAnn").asString()),
		fridgeReading(compact_or_verbose(reply, "ft", "FridgeTemp").asDouble()),
		fridgeSetting(compact_or_verbose(reply, "fs", "FridgeSet").asDouble()),
		fridgeAnnotation(compact_or_verbose(reply, "fa", "FridgeAnn").asString()),
		roomTemp(compact_or_verbose(reply, "rt", "RoomTemp").asDouble()),
		state(Temperatures::State::Null) {
			const Json::Value state_value = compact_or_verbose(reply, "s", "State");
			if(!state_value.isNull() && state_value.isConvertibleTo(Json::intValue)) {
				int state_int = state_value.asInt();
				if(state_int < 0 || state_int >= static_cast<int>(Temperatures::State::MAX_VALUE)) {
					state = Temperatures::State::Null;
				} else {
					state = static_cast<State>(state_int);
				}
			}
		}
		
		Temperatures::Temperatures() : beerReading(0.0), beerSetting(0.0), beerAnnotation(), fridgeReading(0.0), fridgeSetting(0.0), fridgeAnnotation(), roomTemp(0.0), state(State::Null) {}
		
		template<typename T>
		void set_if(Json::Value& root, const char* key, const T& value, const T& null_value) {
			if(value == null_value) {
				root[key] = Json::Value();
			} else {
				root[key] = value;
			}
		}
		template<>
		void set_if(Json::Value& root, const char* key, const char& value, const char& null_value) {
			if(value == null_value) {
				root[key] = Json::Value();
			} else {
				root[key] = std::string(1, value);
			}
		}
		void set_if_not_empty(Json::Value& root, const char* key, const std::string& value) {
			if(value.empty()) {
				root[key] = Json::Value();
			} else {
				root[key] = value;
			}
		}
		
		std::string Temperatures::to_string() {
			Json::Value root;
			set_if(root, "BeerTemp", beerReading, 0.0);
			set_if(root, "BeerSet", beerSetting, 0.0);
			set_if_not_empty(root, "BeerAnn", beerAnnotation);
			set_if(root, "FridgeTemp", fridgeReading, 0.0);
			set_if(root, "FridgeSet", fridgeSetting, 0.0);
			set_if_not_empty(root, "FridgeAnn", fridgeAnnotation);
			set_if(root, "State", static_cast<int>(state), static_cast<int>(Temperatures::State::Null));
			return Json::FastWriter().write(root);
		}
		
		ControlSettings::ControlSettings(const Json::Value& reply) :
		mode(ControlSettings::Mode::Null),
		beerSetting(reply["beerSet"].asDouble()),
		fridgeSetting(reply["fridgeSet"].asDouble()),
		heatEstimator(reply["heatEst"].asDouble()),
		coolEstimator(reply["coolEst"].asDouble()) {
			const std::string mode_value = reply["mode"].asString();
			if(!mode_value.empty()) {
				switch(mode_value[0]) {
					case static_cast<char>(ControlSettings::Mode::BeerConstant):
						mode = ControlSettings::Mode::BeerConstant;
						break;
					case static_cast<char>(ControlSettings::Mode::BeerProfile):
						mode = ControlSettings::Mode::BeerProfile;
						break;
					case static_cast<char>(ControlSettings::Mode::FridgeConstant):
						mode = ControlSettings::Mode::FridgeConstant;
						break;
					case static_cast<char>(ControlSettings::Mode::Off):
						mode = ControlSettings::Mode::Off;
						break;
					case static_cast<char>(ControlSettings::Mode::Test):
						mode = ControlSettings::Mode::Test;
						break;
				}
			}
		}
		
		std::string ControlSettings::to_string() {
			Json::Value root;
			set_if(root, "beerSet", beerSetting, 0.0);
			set_if(root, "fridgeSet", fridgeSetting, 0.0);
			set_if(root, "heatEst", heatEstimator, 0.0);
			set_if(root, "coolEst", coolEstimator, 0.0);
			set_if(root, "mode", static_cast<char>(mode), static_cast<char>(Mode::Null));
			return Json::FastWriter().write(root);
		}
		
		ControlConstants::ControlConstants(const Json::Value& reply) :
			tempSettingMin(reply["tempSetMin"].asDouble()),
			tempSettingMax(reply["tempSetMax"].asDouble()),
			iMaxError(reply["iMaxError"].asDouble()),
			idleRangeHigh(reply["idleRangeH"].asDouble()),
			idleRangeLow(reply["idleRangeL"].asDouble()),
			heatingTargetUpper(reply["heatTargetH"].asDouble()),
			heatingTargetLower(reply["heatTargetL"].asDouble()),
			coolingTargetUpper(reply["coolTargetH"].asDouble()),
			coolingTargetLower(reply["coolTargetL"].asDouble()),
			maxHeatTimeForEstimate(reply["maxHeatTimeForEst"].asInt64()),
			maxCoolTimeForEstimate(reply["maxCoolTimeForEst"].asInt64()),
			fridgeFastFilter(static_cast<filter_t>(reply["fridgeFastFilt"].asUInt())),
			fridgeSlowFilter(static_cast<filter_t>(reply["fridgeSlowFilt"].asUInt())),
			fridgeSlopeFilter(static_cast<filter_t>(reply["fridgeSlopeFilt"].asUInt())),
			beerFastFilter(static_cast<filter_t>(reply["beerFastFilt"].asUInt())),
			beerSlowFilter(static_cast<filter_t>(reply["beerSlowFilt"].asUInt())),
			beerSlopeFilter(static_cast<filter_t>(reply["beerSlopeFilt"].asUInt())),
			lightAsHeater(reply["lah"].asBool()),
			rotaryHalfSteps(reply["hs"].asBool()) {}
		
		std::string ControlConstants::to_string() {
			Json::Value root;
			root["tempSetMin"] = tempSettingMin;
			root["tempSetMax"] = tempSettingMax;
			root["iMaxError"] = iMaxError;
			root["idleRangeH"] = idleRangeHigh;
			root["idleRangeL"] = idleRangeLow;
			root["heatTargetH"] = heatingTargetUpper;
			root["heatTargetL"] = heatingTargetLower;
			root["coolTargetH"] = coolingTargetUpper;
			root["coolTargetL"] = coolingTargetLower;
			root["maxHeatTimeForEst"] = static_cast<Json::Int64>(maxHeatTimeForEstimate);
			root["maxCoolTimeForEst"] = static_cast<Json::Int64>(maxCoolTimeForEstimate);
			root["fridgeFastFilt"] = fridgeFastFilter;
			root["fridgeSlowFilt"] = fridgeSlowFilter;
			root["fridgeSlopeFilt"] = fridgeSlopeFilter;
			root["beerFastFilt"] = beerFastFilter;
			root["beerSlowFilt"] = beerSlowFilter;
			root["beerSlopeFilt"] = beerSlopeFilter;
			root["lah"] = lightAsHeater ? 1 : 0;
			root["hs"] = rotaryHalfSteps ? 1 : 0;
			return Json::FastWriter().write(root);
		}

		ControlVariables::ControlVariables(const Json::Value& reply) :
			beerDiff(reply["beerDiff"].asDouble()),
			diffIntegral(reply["diffIntegral"].asDouble()),
			beerSlope(reply["beerSlope"].asDouble()),
			p(reply["p"].asDouble()),
			i(reply["i"].asDouble()),
			d(reply["d"].asDouble()),
			estPeak(reply["estPeak"].asDouble()),
			negPeakEst(reply["negPeakEst"].asDouble()),
			posPeakEst(reply["posPeakEst"].asDouble()),
			negPeak(reply["negPeak"].asDouble()),
			posPeak(reply["posPeak"].asDouble()) {}
		
		std::string ControlVariables::to_string() {
			Json::Value root;
			root["beerDiff"] = beerDiff;
			root["diffIntegral"] = diffIntegral;
			root["beerSlope"] = beerSlope;
			root["p"] = p;
			root["i"] = i;
			root["d"] = d;
			root["estPeak"] = estPeak;
			root["negPeakEst"] = negPeakEst;
			root["posPeakEst"] = posPeakEst;
			root["negPeak"] = negPeak;
			root["posPeak"] = posPeak;
			return Json::FastWriter().write(root);
		}

		
		Temperatures get_temperatures(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException) {
			buffered_io.write("t");
			const std::string line = strip_carriage_return(buffered_io.readStringUntil(boost::posix_time::seconds(1),"\n"));
			Json::Value reply;
			if(line.length() <= 2 || line[0] != 'T' || !Json::Reader().parse(line.substr(2), reply)) {
				return Temperatures();
			}
			return Temperatures(reply);
		}
		
		ControlSettings get_control_settings(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException) {
			buffered_io.write("s");
			const std::string line = strip_carriage_return(buffered_io.readStringUntil(boost::posix_time::seconds(1),"\n"));
			Json::Value reply;
			if(line.length() <= 2 || line[0] != 'S' || !Json::Reader().parse(line.substr(2), reply)) {
				return ControlSettings();
			}
			return ControlSettings(reply);
		}
		
		ControlConstants get_control_constants(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException) {
			buffered_io.write("c");
			const std::string line = strip_carriage_return(buffered_io.readStringUntil(boost::posix_time::seconds(1),"\n"));
			Json::Value reply;
			if(line.length() <= 2 || line[0] != 'C' || !Json::Reader().parse(line.substr(2), reply)) {
				return ControlConstants();
			}
			return ControlConstants(reply);
		}
		
		ControlVariables get_control_variables(BufferedIO& buffered_io) throw(boost::system::system_error,BufferedIO::TimeoutException) {
			buffered_io.write("v");
			const std::string line = strip_carriage_return(buffered_io.readStringUntil(boost::posix_time::seconds(1),"\n"));
			Json::Value reply;
			if(line.length() <= 2 || line[0] != 'V' || !Json::Reader().parse(line.substr(2), reply)) {
				return ControlVariables();
			}
			return ControlVariables(reply);
		}
	}
}