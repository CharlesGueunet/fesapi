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

#include "AbstractBigIjkGridRepresentationTest.h"

namespace resqml2_test {
	class BigIjkGridExplicitRepresentationTest : public AbstractBigIjkGridRepresentationTest {
	public:

		static constexpr char const* defaultUuid = "f889e5d2-249e-4827-8532-ce60a1d05b99";
		static constexpr char const* defaultTitle = "Ijk Grid Explicit Representation";
		static constexpr char const* discretePropertyUuid = "265343eb-6d1b-4015-b08b-8b4abbe7f84b";
		static constexpr char const* discretePropertyTitle = "Explicit IJK Grid K Index";
		static constexpr char const* continuousPropertyUuid = "a4e8e307-0844-47e9-b9fd-6ad0a1f99b88";
		static constexpr char const* continuousPropertyTitle = "Explicit IJK Grid Continuous Property";
	
		/**
		* Creation of an explicit IJK grid representation from an EPC document path. Resulting grid is 
		* aligned along axis (I direction along X, J direction along Y and K direction along Z). 
		* Resulting grid carries both discrete and continuous properties on cells.
		* At serialize() call, exising .epc file will be erased. 
		* @param repoPath	the path of the .epc file (including .epc extension)
		* @param iCount		number of cells in the I direction.
		* @param jCount		number of cells in the J direction.
		* @param kCount		number of cells in the K direction.
		* @param faultCount	number of faults. Faults are parallel to YZ plane (they fit with i-interfaces). faultCount in [0; iCount[.
		* @param xMin		minimum x value of the grid.
		* @param xMax		maximum x value of the grid.
		* @param yMin		minimum y value of the grid.
		* @param yMax		maximum y value of the grid.
		* @param zMin		minimum z value of the grid (without considering any fault throw).
		* @param zMax		maximum z value of the grid (without considering any fault throw).
		* @param faultThrow	length of the fault throw along z axis. 
		*/
		BigIjkGridExplicitRepresentationTest(
			const std::string & repoPath,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, 
			unsigned int faultCount, 
			double xMin, double xMax, double yMin, double yMax, double zMin, double zMax,
			double faultThrow);

	protected:
		void initRepo();
		void readRepo();
	};
}

