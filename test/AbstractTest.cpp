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
#include "AbstractTest.h"

#include "catch.hpp"

#include "common/EpcDocument.h"

#include "resqml2/LocalDepth3dCrs.h"

using namespace std;
using namespace commontest;

AbstractTest::AbstractTest(const string & epcDocPath) :
	defaultEmlVersion(COMMON_NS::DataObjectRepository::EnergisticsStandard::EML2_0),
	defaultResqmlVersion(COMMON_NS::DataObjectRepository::EnergisticsStandard::RESQML2_0_1),
	repo(nullptr),
	epcDocPath(epcDocPath.find("::") == string::npos ? epcDocPath : epcDocPath.substr(0, epcDocPath.find("::")) + "__" + epcDocPath.substr(epcDocPath.find("::") + 2, epcDocPath.size() - epcDocPath.find("::"))) {
}

void AbstractTest::serialize() {
	COMMON_NS::EpcDocument epcDocument(epcDocPath);
	repo = new COMMON_NS::DataObjectRepository();
	repo->setDefaultStandard(defaultEmlVersion);
	repo->setDefaultStandard(defaultResqmlVersion);
	repo->createHdfProxy("75f5b460-3ccb-4102-a06e-e9c1019769b2", "Hdf Proxy Test", epcDocument.getStorageDirectory(), epcDocument.getName() + ".h5", COMMON_NS::DataObjectRepository::openingMode::OVERWRITE);
	repo->createLocalDepth3dCrs("7b40b49e-b783-4b3f-8380-b3bdc42e8ae7", "Default CRS", 1000, 2000, 3000, .0, gsoap_resqml2_0_1::eml20__LengthUom::m, 23031, gsoap_resqml2_0_1::eml20__LengthUom::ft, "Unknown", false);

	initRepo();
	
	epcDocument.serializeFrom(*repo);
	epcDocument.close();

	delete repo;
}

void AbstractTest::deserialize() {
	COMMON_NS::EpcDocument epcDocument(epcDocPath);
	repo = new COMMON_NS::DataObjectRepository();

	std::string validationResult = epcDocument.deserializeInto(*repo);
	if (!validationResult.empty()) {
		cout << "EPC document " << epcDocument.getName() << ".epc deserialized with warnings" << endl;
		cout << validationResult << endl;
	}

	readRepo();
	epcDocument.close();

	delete repo;
}
