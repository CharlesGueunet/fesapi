/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#include "WellTest.h"

#include <stdexcept>

#include "../catch.hpp"

#include "resqml2/WellboreFeature.h"

#include "witsml2/Well.h"
#include "witsml2/Wellbore.h"

using namespace std;
using namespace witsml2_test;
using namespace COMMON_NS;
using namespace WITSML2_NS;

const char* WellTest::defaultUuid = "4b4ef814-a891-46f5-bf10-3cc30be396a5";
const char* WellTest::defaultTitle = "Witsml Well Test";
const time_t WellTest::defaultTimestamp = 1553268493; // 2019-03-22T15:28:13+00:00

WellTest::WellTest(const string & epcDocPath)
	: AbstractTest(epcDocPath) {
}

void WellTest::initRepo() {
	Well* well = repo->createWell(defaultUuid, defaultTitle, false);
	Wellbore* wellbore = repo->createWellbore(well, witsmlWellboreUuid, "", false);

	RESQML2_NS::WellboreFeature* resqmlWellboreFeature = repo->createWellboreFeature(resqmlWellboreFeatureUuid, "");
	resqmlWellboreFeature->setWitsmlWellbore(wellbore);
	
	well->setBlock("my Block");
	// No county
	well->setDTimLicense(defaultTimestamp);
	well->setGroundElevation(10, gsoap_eml2_3::eml23__LengthUom::m);
	REQUIRE_THROWS(well->setTimeZone(true, 24, 0));
	REQUIRE_THROWS(well->setTimeZone(true, 22, 65));
	well->setTimeZone(true, 0, 0); // time zone == 'Z'
	well->setStatusWell(gsoap_eml2_3::eml23__WellStatus::active);
}

void WellTest::readRepo() {
	auto* well = repo->getDataObjectByUuid<Well>(defaultUuid);
	auto* wellbore = repo->getDataObjectByUuid<Wellbore>(witsmlWellboreUuid);
	auto* resqmlWellboreFeature = repo->getDataObjectByUuid<RESQML2_NS::WellboreFeature>(resqmlWellboreFeatureUuid);
	REQUIRE(resqmlWellboreFeature->getWitsmlWellbore() == wellbore);
	REQUIRE(wellbore->getWell() == well);

	REQUIRE(well != nullptr);
	REQUIRE(well->hasBlock());
	REQUIRE(well->getBlock() == "my Block");
	REQUIRE_FALSE(well->hasCounty());
	REQUIRE(well->getDTimLicense() == defaultTimestamp);
	REQUIRE(well->getGroundElevationValue() == 10);
	REQUIRE(well->getGroundElevationUom() == gsoap_eml2_3::eml23__LengthUom::m);
	REQUIRE(well->getTimeZoneDirection() == true);
	REQUIRE(well->getTimeZoneHours() == 0);
	REQUIRE(well->getTimeZoneMinutes() == 0);
	REQUIRE(well->getStatusWell() == gsoap_eml2_3::eml23__WellStatus::active);
}
