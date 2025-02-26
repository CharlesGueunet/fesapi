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

#include "../resqml2/BlockedWellboreRepresentation.h"

namespace RESQML2_2_NS
{
	/** A blocked wellbore representation. */
	class BlockedWellboreRepresentation final : public RESQML2_NS::BlockedWellboreRepresentation
	{
	public:

		/**
		 * Only to be used in partial transfer context
		 *
		 * @param [in,out]	partialObject	If non-null, the partial object.
		 *
		 * 
		 */
		DLL_IMPORT_OR_EXPORT BlockedWellboreRepresentation(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : RESQML2_NS::BlockedWellboreRepresentation(partialObject) {}

		/**
		 * @brief	Creates an instance of this class in a gsoap context.
		 *
		 * @exception	std::invalid_argument	If <tt>traj == nullptr</tt>.
		 *
		 * @param [in]	interp	The interpretation the instance represents.
		 * @param 	  	guid  	A guid for the instance to create.
		 * @param 	  	title 	A title for the instance to create.
		 * @param [in]	traj  	The wellbore trajectory this intance is based on.
		 */
		BlockedWellboreRepresentation(RESQML2_NS::WellboreInterpretation * interp,
			const std::string & guid, const std::string & title, RESQML2_NS::WellboreTrajectoryRepresentation * traj);

		/**
		 * Creates an instance of this class by wrapping a gsoap instance.
		 *
		 * @param [in,out]	fromGsoap	If non-null, from gsoap.
		 */
		BlockedWellboreRepresentation(gsoap_eml2_3::_resqml22__BlockedWellboreRepresentation* fromGsoap) : RESQML2_NS::BlockedWellboreRepresentation(fromGsoap) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		~BlockedWellboreRepresentation() = default;

		DLL_IMPORT_OR_EXPORT void setIntervalGridCells(int8_t const* gridIndices, int8_t gridIndicesNullValue, int64_t const* cellIndices,
			int8_t const* localFacePairPerCellIndices, int8_t localFacePairPerCellIndicesNullValue, EML2_NS::AbstractHdfProxy * hdfProxy) final;

		DLL_IMPORT_OR_EXPORT uint64_t getCellCount() const final;

		DLL_IMPORT_OR_EXPORT int8_t getGridIndices(int8_t* gridIndices) const final;

		DLL_IMPORT_OR_EXPORT int64_t getCellIndices(int64_t* cellIndices) const final;

		DLL_IMPORT_OR_EXPORT int8_t getLocalFacePairPerCellIndices(int8_t* localFacePairPerCellIndices) const final;

		DLL_IMPORT_OR_EXPORT void pushBackSupportingGridRepresentation(RESQML2_NS::AbstractGridRepresentation * supportingGridRep) final;

		DLL_IMPORT_OR_EXPORT uint64_t getSupportingGridRepresentationCount() const final;

		COMMON_NS::DataObjectReference getSupportingGridRepresentationDor(uint64_t index) const final;

		/**
		* The standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT static const char* XML_NS;

		/**
		* Get the standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT std::string getXmlNamespace() const final { return XML_NS; }

	private:

		/**
		 * Initializes this object
		 *
		 * @param 		  	guid 	Unique identifier.
		 * @param 		  	title	The title.
		 * @param [in,out]	traj 	If non-null, the traj.
		 */
		void init(const std::string & guid, const std::string & title, RESQML2_NS::WellboreTrajectoryRepresentation * traj);
	};
}
