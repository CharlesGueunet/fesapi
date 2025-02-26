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
#pragma once

#include "AbstractTest.h"

namespace resqml2_test {
	class FaultTest;

	class FaultInterpretationTest : public commontest::AbstractTest {
	public:

		static constexpr char const* faultUuid = "a2e84855-738e-462a-82c5-863abd058be1";
		static constexpr char const* faultTitle = "Fault";
		static constexpr char const* defaultUuid = "91f90343-2581-48c5-893f-667a2995088c";
		static constexpr char const* defaultTitle = "FaultInterpretationTest";

		/**
		* Creation of a testing object from an EPC document path. At serialize() call,
		* exising .epc file will be erased. 
		* @param repoPath the path of the .epc file (including .epc extension)
		*/
		FaultInterpretationTest(const std::string & repoPath);

	protected:
		void initRepo();
		void readRepo();
	};
}
