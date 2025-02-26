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
#include "AbstractGridRepresentation.h"

#include <algorithm>

#include "../eml2/AbstractHdfProxy.h"

#include "AbstractIjkGridRepresentation.h"
#include "AbstractStratigraphicOrganizationInterpretation.h"
#include "GridConnectionSetRepresentation.h"
#include "RockFluidOrganizationInterpretation.h"
#include "UnstructuredGridRepresentation.h"

using namespace RESQML2_NS;
using namespace std;

std::vector<RESQML2_NS::GridConnectionSetRepresentation *> AbstractGridRepresentation::getGridConnectionSetRepresentationSet() const
{
	return repository->getSourceObjects<RESQML2_NS::GridConnectionSetRepresentation>(this);
}

uint64_t AbstractGridRepresentation::getGridConnectionSetRepresentationCount() const
{
	return getGridConnectionSetRepresentationSet().size();
}

RESQML2_NS::GridConnectionSetRepresentation * AbstractGridRepresentation::getGridConnectionSetRepresentation(uint64_t index) const
{
	const std::vector<RESQML2_NS::GridConnectionSetRepresentation *>& gridConnectionSetRepresentationSet = getGridConnectionSetRepresentationSet();
	return gridConnectionSetRepresentationSet.at(index);
}

gsoap_resqml2_0_1::resqml20__AbstractParentWindow* AbstractGridRepresentation::getParentWindow2_0_1() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		return static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->ParentWindow;
	}

	return nullptr;
}

gsoap_eml2_3::resqml22__AbstractParentWindow* AbstractGridRepresentation::getParentWindow2_2() const
{
	if (gsoapProxy2_3 != nullptr) {
		return static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->ParentWindow;
	}

	return nullptr;
}

COMMON_NS::DataObjectReference AbstractGridRepresentation::getParentGridDor() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr) {
			switch (parentWindow->soap_type()) {
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow: return COMMON_NS::DataObjectReference(static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->ParentGrid);
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow: return COMMON_NS::DataObjectReference(static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow)->ParentGrid);
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__CellParentWindow: return COMMON_NS::DataObjectReference(static_cast<gsoap_resqml2_0_1::resqml20__CellParentWindow*>(parentWindow)->ParentGrid);
			default: throw logic_error("Unexpected parent window type.");
			}
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr) {
			switch (parentWindow->soap_type()) {
			case SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow: return COMMON_NS::DataObjectReference(static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->ParentIjkGridRepresentation);
			case SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow: return COMMON_NS::DataObjectReference(static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow)->ParentColumnLayerGridRepresentation);
			case SOAP_TYPE_gsoap_eml2_3_resqml22__CellParentWindow: return COMMON_NS::DataObjectReference(static_cast<gsoap_eml2_3::resqml22__CellParentWindow*>(parentWindow)->ParentGridRepresentation);
			default: throw logic_error("Unexpected parent window type.");
			}
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}

	return COMMON_NS::DataObjectReference();
}

AbstractGridRepresentation* AbstractGridRepresentation::getParentGrid() const
{
	const string parentGridUuid = getParentGridDor().getUuid();

	if (!parentGridUuid.empty()) {
		return static_cast<AbstractGridRepresentation*>(getRepository()->getDataObjectByUuid(parentGridUuid));
	}

	return nullptr;
}

std::vector<RESQML2_NS::AbstractGridRepresentation *> AbstractGridRepresentation::getChildGridSet() const
{
	return repository->getSourceObjects<RESQML2_NS::AbstractGridRepresentation>(this);
}

uint64_t AbstractGridRepresentation::getChildGridCount() const {
	return getChildGridSet().size();
}

AbstractGridRepresentation * AbstractGridRepresentation::getChildGrid(uint64_t index) const
{
	const std::vector<RESQML2_NS::AbstractGridRepresentation *>& childGridSet = getChildGridSet();
	return childGridSet.at(index);
}

gsoap_resqml2_0_1::resqml20__Regrid* AbstractGridRepresentation::createRegrid2_0_1(unsigned int indexRegridStart, unsigned int * childCellCountPerInterval, unsigned int * parentCellCountPerInterval, uint64_t intervalCount, double * childCellWeights,
	const std::string & dimension, EML2_NS::AbstractHdfProxy * proxy, bool forceConstantCellCountPerInterval)
{
	gsoap_resqml2_0_1::resqml20__Regrid* regrid = gsoap_resqml2_0_1::soap_new_resqml20__Regrid(gsoapProxy2_0_1->soap);
	regrid->InitialIndexOnParentGrid = indexRegridStart;
	regrid->Intervals = gsoap_resqml2_0_1::soap_new_resqml20__Intervals(gsoapProxy2_0_1->soap);
	regrid->Intervals->IntervalCount = intervalCount;

	if (intervalCount == 0) {
		throw invalid_argument("Cannot regrid an empty list of intervals.");
	}
	else if (intervalCount == 1 || forceConstantCellCountPerInterval) {
		gsoap_resqml2_0_1::resqml20__IntegerConstantArray* xmlChildCountPerInterval = gsoap_resqml2_0_1::soap_new_resqml20__IntegerConstantArray(gsoapProxy2_0_1->soap);
		xmlChildCountPerInterval->Value = *childCellCountPerInterval;
		xmlChildCountPerInterval->Count = intervalCount;
		regrid->Intervals->ChildCountPerInterval = xmlChildCountPerInterval;

		gsoap_resqml2_0_1::resqml20__IntegerConstantArray* xmlParentCountPerInterval = gsoap_resqml2_0_1::soap_new_resqml20__IntegerConstantArray(gsoapProxy2_0_1->soap);
		xmlParentCountPerInterval->Value = *parentCellCountPerInterval;
		xmlParentCountPerInterval->Count = intervalCount;
		regrid->Intervals->ParentCountPerInterval = xmlParentCountPerInterval;
	}
	else {
		if (proxy == nullptr) {
			proxy = getRepository()->getDefaultHdfProxy();
			if (proxy == nullptr) {
				throw std::invalid_argument("A (default) HDF Proxy must be provided.");
			}
		}
		getRepository()->addRelationship(this, proxy);

		gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* hdf5ChildCountPerInterval = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(gsoapProxy2_0_1->soap);
		regrid->Intervals->ChildCountPerInterval = hdf5ChildCountPerInterval;
		hdf5ChildCountPerInterval->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write count null value in this method
		hdf5ChildCountPerInterval->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(gsoapProxy2_0_1->soap);
		hdf5ChildCountPerInterval->Values->HdfProxy = proxy->newResqmlReference();
		hdf5ChildCountPerInterval->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_" + dimension + "Regrid_ChildCountPerInterval";

		gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* hdf5ParentCountPerInterval = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(gsoapProxy2_0_1->soap);
		regrid->Intervals->ParentCountPerInterval = hdf5ParentCountPerInterval;
		hdf5ParentCountPerInterval->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write count null value in this method
		hdf5ParentCountPerInterval->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(gsoapProxy2_0_1->soap);
		hdf5ParentCountPerInterval->Values->HdfProxy = proxy->newResqmlReference();
		hdf5ParentCountPerInterval->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_" + dimension + "Regrid_ParentCountPerInterval";

		// HDF
		proxy->writeArrayNd(getHdfGroup(), "ParentWindow_" + dimension + "Regrid_ChildCountPerInterval", COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32, childCellCountPerInterval, &intervalCount, 1);
		proxy->writeArrayNd(getHdfGroup(), "ParentWindow_" + dimension + "Regrid_ParentCountPerInterval", COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32, parentCellCountPerInterval, &intervalCount, 1);
	}

	if (childCellWeights != nullptr) {
		if (proxy == nullptr) {
			proxy = getRepository()->getDefaultHdfProxy();
			if (proxy == nullptr) {
				throw std::invalid_argument("A (default) HDF Proxy must be provided.");
			}
		}
		getRepository()->addRelationship(this, proxy);

		gsoap_resqml2_0_1::resqml20__DoubleHdf5Array* hdf5ChildCellWeights = gsoap_resqml2_0_1::soap_new_resqml20__DoubleHdf5Array(gsoapProxy2_0_1->soap);
		regrid->Intervals->ChildCellWeights = hdf5ChildCellWeights;
		hdf5ChildCellWeights->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(gsoapProxy2_0_1->soap);
		hdf5ChildCellWeights->Values->HdfProxy = proxy->newResqmlReference();
		hdf5ChildCellWeights->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_" + dimension + "Regrid_ChildCellWeights";

		// HDF
		uint64_t numValues = *childCellCountPerInterval;
		if (forceConstantCellCountPerInterval) {
			numValues *= intervalCount - 1;
		}
		else {
			for (unsigned int i = 1; i < intervalCount; ++i) {
				numValues += childCellCountPerInterval[i];
			}
		}
		proxy->writeArrayNdOfDoubleValues(getHdfGroup(), "ParentWindow_" + dimension + "Regrid_ChildCellWeights", childCellWeights, &numValues, 1);
	}

	return regrid;
}

gsoap_eml2_3::resqml22__Regrid* AbstractGridRepresentation::createRegrid2_2(unsigned int indexRegridStart, unsigned int * childCellCountPerInterval, unsigned int * parentCellCountPerInterval, uint64_t intervalCount, double * childCellWeights,
	const std::string & dimension, EML2_NS::AbstractHdfProxy * proxy, bool forceConstantCellCountPerInterval)
{
	gsoap_eml2_3::resqml22__Regrid* regrid = gsoap_eml2_3::soap_new_resqml22__Regrid(gsoapProxy2_3->soap);
	regrid->InitialIndexOnParentGrid = indexRegridStart;
	regrid->Intervals = gsoap_eml2_3::soap_new_resqml22__Intervals(gsoapProxy2_3->soap);
	regrid->Intervals->IntervalCount = intervalCount;

	if (intervalCount == 0) {
		throw invalid_argument("Cannot regrid an empty list of intervals.");
	}
	else if (intervalCount == 1 || forceConstantCellCountPerInterval) {
		gsoap_eml2_3::eml23__IntegerConstantArray* xmlChildCountPerInterval = gsoap_eml2_3::soap_new_eml23__IntegerConstantArray(gsoapProxy2_3->soap);
		xmlChildCountPerInterval->Value = *childCellCountPerInterval;
		xmlChildCountPerInterval->Count = intervalCount;
		regrid->Intervals->ChildCountPerInterval = xmlChildCountPerInterval;

		gsoap_eml2_3::eml23__IntegerConstantArray* xmlParentCountPerInterval = gsoap_eml2_3::soap_new_eml23__IntegerConstantArray(gsoapProxy2_3->soap);
		xmlParentCountPerInterval->Value = *parentCellCountPerInterval;
		xmlParentCountPerInterval->Count = intervalCount;
		regrid->Intervals->ParentCountPerInterval = xmlParentCountPerInterval;
	}
	else {
		if (proxy == nullptr) {
			proxy = getRepository()->getDefaultHdfProxy();
			if (proxy == nullptr) {
				throw std::invalid_argument("A (default) HDF Proxy must be provided.");
			}
		}
		getRepository()->addRelationship(this, proxy);

		gsoap_eml2_3::eml23__IntegerExternalArray* hdf5ChildCountPerInterval = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(gsoapProxy2_3->soap);
		regrid->Intervals->ChildCountPerInterval = hdf5ChildCountPerInterval;
		hdf5ChildCountPerInterval->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write count null value in this method
		hdf5ChildCountPerInterval->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(gsoapProxy2_3->soap);
		hdf5ChildCountPerInterval->Values->ExternalDataArrayPart.push_back(createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_" + dimension + "Regrid_ChildCountPerInterval", intervalCount, proxy));

		gsoap_eml2_3::eml23__IntegerExternalArray* hdf5ParentCountPerInterval = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(gsoapProxy2_3->soap);
		regrid->Intervals->ParentCountPerInterval = hdf5ParentCountPerInterval;
		hdf5ParentCountPerInterval->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write count null value in this method
		hdf5ParentCountPerInterval->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(gsoapProxy2_3->soap);
		hdf5ParentCountPerInterval->Values->ExternalDataArrayPart.push_back(createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_" + dimension + "Regrid_ParentCountPerInterval", intervalCount, proxy));

		// HDF
		proxy->writeArrayNd(getHdfGroup(), "ParentWindow_" + dimension + "Regrid_ChildCountPerInterval", COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32, childCellCountPerInterval, &intervalCount, 1);
		proxy->writeArrayNd(getHdfGroup(), "ParentWindow_" + dimension + "Regrid_ParentCountPerInterval", COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32, parentCellCountPerInterval, &intervalCount, 1);
	}

	if (childCellWeights != nullptr) {
		if (proxy == nullptr) {
			proxy = getRepository()->getDefaultHdfProxy();
			if (proxy == nullptr) {
				throw std::invalid_argument("A (default) HDF Proxy must be provided.");
			}
		}
		getRepository()->addRelationship(this, proxy);

		gsoap_eml2_3::eml23__FloatingPointExternalArray* hdf5ChildCellWeights = gsoap_eml2_3::soap_new_eml23__FloatingPointExternalArray(gsoapProxy2_3->soap);
		regrid->Intervals->ChildCellWeights = hdf5ChildCellWeights;
		hdf5ChildCellWeights->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(gsoapProxy2_3->soap);
		uint64_t numValues = 0;
		for (uint64_t i = 0; i < intervalCount; ++i) {
			numValues += childCellCountPerInterval[i];
		}
		hdf5ChildCellWeights->Values->ExternalDataArrayPart.push_back(createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_" + dimension + "Regrid_ChildCellWeights", numValues, proxy));

		// HDF
		proxy->writeArrayNdOfDoubleValues(getHdfGroup(), "ParentWindow_" + dimension + "Regrid_ChildCellWeights", childCellWeights, &numValues, 1);
	}

	return regrid;
}

void AbstractGridRepresentation::setParentWindow(uint64_t * cellIndices, uint64_t cellIndexCount, UnstructuredGridRepresentation* parentGrid, EML2_NS::AbstractHdfProxy * proxy)
{
	if (cellIndexCount == 0 || cellIndices == nullptr || parentGrid == nullptr) {
		throw invalid_argument("One of the mandatory method parameter is zero or null");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1);

		gsoap_resqml2_0_1::resqml20__CellParentWindow* cpw = gsoap_resqml2_0_1::soap_new_resqml20__CellParentWindow(rep->soap);
		rep->ParentWindow = cpw;

		cpw->ParentGrid = parentGrid->newResqmlReference();
		if (cellIndexCount > 1) {
			if (proxy == nullptr) {
				proxy = getRepository()->getDefaultHdfProxy();
				if (proxy == nullptr) {
					throw std::invalid_argument("A (default) HDF Proxy must be provided.");
				}
			}
			getRepository()->addRelationship(this, proxy);

			gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* hdf5CellIndices = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(rep->soap);
			cpw->CellIndices = hdf5CellIndices;

			hdf5CellIndices->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write cell index null value in this method
			hdf5CellIndices->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(rep->soap);
			hdf5CellIndices->Values->HdfProxy = proxy->newResqmlReference();
			hdf5CellIndices->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_CellIndices";

			// HDF
			proxy->writeArrayNdOfUInt64Values(getHdfGroup(), "ParentWindow_CellIndices", cellIndices, &cellIndexCount, 1);
		}
		else { // cellIndexCount == 1
			gsoap_resqml2_0_1::resqml20__IntegerConstantArray* xmlCellIndices = gsoap_resqml2_0_1::soap_new_resqml20__IntegerConstantArray(rep->soap);
			xmlCellIndices->Value = *cellIndices;
			xmlCellIndices->Count = 1;
			cpw->CellIndices = xmlCellIndices;
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto rep = static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3);

		gsoap_eml2_3::resqml22__CellParentWindow* cpw = gsoap_eml2_3::soap_new_resqml22__CellParentWindow(rep->soap);
		rep->ParentWindow = cpw;

		cpw->ParentGridRepresentation = parentGrid->newEml23Reference();
		if (cellIndexCount > 1) {
			if (proxy == nullptr) {
				proxy = getRepository()->getDefaultHdfProxy();
				if (proxy == nullptr) {
					throw std::invalid_argument("A (default) HDF Proxy must be provided.");
				}
			}
			getRepository()->addRelationship(this, proxy);

			auto hdf5CellIndices = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(rep->soap);
			cpw->CellIndices = hdf5CellIndices;

			hdf5CellIndices->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write cell index null value in this method
			hdf5CellIndices->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(rep->soap);
			hdf5CellIndices->Values->ExternalDataArrayPart.push_back(createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_CellIndices", cellIndexCount, proxy));

			// HDF
			proxy->writeArrayNdOfUInt64Values(getHdfGroup(), "ParentWindow_CellIndices", cellIndices, &cellIndexCount, 1);
		}
		else { // cellIndexCount == 1
			auto xmlCellIndices = gsoap_eml2_3::soap_new_eml23__IntegerConstantArray(rep->soap);
			xmlCellIndices->Value = *cellIndices;
			xmlCellIndices->Count = 1;
			cpw->CellIndices = xmlCellIndices;
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}

	repository->addRelationship(this, parentGrid);
}

void AbstractGridRepresentation::setParentWindow(unsigned int * columnIndices, uint64_t columnIndexCount,
	unsigned int kLayerIndexRegridStart,
	unsigned int * childCellCountPerInterval, unsigned int * parentCellCountPerInterval, unsigned int intervalCount,
	AbstractColumnLayerGridRepresentation* parentGrid,
	EML2_NS::AbstractHdfProxy * proxy, double * childCellWeights)
{
	if (columnIndexCount == 0 || columnIndices == nullptr ||
		childCellCountPerInterval == nullptr || parentCellCountPerInterval == nullptr || intervalCount == 0 || parentGrid == nullptr) {
		throw invalid_argument("One of the mandatory method parameter is zero or null");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1);

		gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow* clpw = gsoap_resqml2_0_1::soap_new_resqml20__ColumnLayerParentWindow(rep->soap);
		rep->ParentWindow = clpw;

		clpw->ParentGrid = parentGrid->newResqmlReference();

		// COLUMN INDICES
		if (columnIndexCount > 1)
		{
			if (proxy == nullptr) {
				proxy = getRepository()->getDefaultHdfProxy();
				if (proxy == nullptr) {
					throw std::invalid_argument("A (default) HDF Proxy must be provided.");
				}
			}
			getRepository()->addRelationship(this, proxy);

			gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* hdf5ColumnIndices = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(rep->soap);
			clpw->ColumnIndices = hdf5ColumnIndices;

			hdf5ColumnIndices->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write column index null value in this method
			hdf5ColumnIndices->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(rep->soap);
			hdf5ColumnIndices->Values->HdfProxy = proxy->newResqmlReference();
			hdf5ColumnIndices->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_ColumnIndices";

			// HDF
			proxy->writeArrayNd(getHdfGroup(), "ParentWindow_ColumnIndices", COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32, columnIndices, &columnIndexCount, 1);
		}
		else if (columnIndexCount == 1)
		{
			gsoap_resqml2_0_1::resqml20__IntegerConstantArray* xmlColumnIndices = gsoap_resqml2_0_1::soap_new_resqml20__IntegerConstantArray(rep->soap);
			xmlColumnIndices->Value = *columnIndices;
			xmlColumnIndices->Count = 1;
			clpw->ColumnIndices = xmlColumnIndices;
		}

		// K Regrid
		clpw->KRegrid = createRegrid2_0_1(kLayerIndexRegridStart, childCellCountPerInterval, parentCellCountPerInterval, intervalCount, childCellWeights, "K", proxy);
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto rep = static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3);

		gsoap_eml2_3::resqml22__ColumnLayerParentWindow* clpw = gsoap_eml2_3::soap_new_resqml22__ColumnLayerParentWindow(rep->soap);
		rep->ParentWindow = clpw;

		clpw->ParentColumnLayerGridRepresentation = parentGrid->newEml23Reference();

		// COLUMN INDICES
		if (columnIndexCount > 1)
		{
			if (proxy == nullptr) {
				proxy = getRepository()->getDefaultHdfProxy();
				if (proxy == nullptr) {
					throw std::invalid_argument("A (default) HDF Proxy must be provided.");
				}
			}
			getRepository()->addRelationship(this, proxy);

			gsoap_eml2_3::eml23__IntegerExternalArray* hdf5ColumnIndices = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(rep->soap);
			clpw->ColumnIndices = hdf5ColumnIndices;

			hdf5ColumnIndices->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write column index null value in this method
			hdf5ColumnIndices->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(rep->soap);
			hdf5ColumnIndices->Values->ExternalDataArrayPart.push_back(createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_ColumnIndices", columnIndexCount, proxy));

			// HDF
			proxy->writeArrayNd(getHdfGroup(), "ParentWindow_ColumnIndices", COMMON_NS::AbstractObject::numericalDatatypeEnum::UINT32, columnIndices, &columnIndexCount, 1);
		}
		else if (columnIndexCount == 1)
		{
			gsoap_eml2_3::eml23__IntegerConstantArray* xmlColumnIndices = gsoap_eml2_3::soap_new_eml23__IntegerConstantArray(rep->soap);
			xmlColumnIndices->Value = *columnIndices;
			xmlColumnIndices->Count = 1;
			clpw->ColumnIndices = xmlColumnIndices;
		}

		// K Regrid
		clpw->KRegrid = createRegrid2_2(kLayerIndexRegridStart, childCellCountPerInterval, parentCellCountPerInterval, intervalCount, childCellWeights, "K", proxy);
	}
	else {
		throw logic_error("Not implemented yet");
	}

	repository->addRelationship(this, parentGrid);
}

void AbstractGridRepresentation::setParentWindow(
	unsigned int iCellIndexRegridStart, unsigned int * childCellCountPerIInterval, unsigned int * parentCellCountPerIInterval, unsigned int iIntervalCount,
	unsigned int jCellIndexRegridStart, unsigned int * childCellCountPerJInterval, unsigned int * parentCellCountPerJInterval, unsigned int jIntervalCount,
	unsigned int kCellIndexRegridStart, unsigned int * childCellCountPerKInterval, unsigned int * parentCellCountPerKInterval, unsigned int kIntervalCount,
	AbstractIjkGridRepresentation* parentGrid, EML2_NS::AbstractHdfProxy * proxy, double * iChildCellWeights, double * jChildCellWeights, double * kChildCellWeights)
{
	if (childCellCountPerIInterval == nullptr || parentCellCountPerIInterval == nullptr || iIntervalCount == 0 ||
		childCellCountPerJInterval == nullptr || parentCellCountPerJInterval == nullptr || jIntervalCount == 0 ||
		childCellCountPerKInterval == nullptr || parentCellCountPerKInterval == nullptr || kIntervalCount == 0 ||
		parentGrid == nullptr) {
		throw invalid_argument("One of the mandatory method parameter is zero or null");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__IjkParentWindow* ijkpw = gsoap_resqml2_0_1::soap_new_resqml20__IjkParentWindow(gsoapProxy2_0_1->soap);
		static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->ParentWindow = ijkpw;

		ijkpw->ParentGrid = parentGrid->newResqmlReference();

		// Regrids
		ijkpw->IRegrid = createRegrid2_0_1(iCellIndexRegridStart, childCellCountPerIInterval, parentCellCountPerIInterval, iIntervalCount, iChildCellWeights, "I", proxy);
		ijkpw->JRegrid = createRegrid2_0_1(jCellIndexRegridStart, childCellCountPerJInterval, parentCellCountPerJInterval, jIntervalCount, jChildCellWeights, "J", proxy);
		ijkpw->KRegrid = createRegrid2_0_1(kCellIndexRegridStart, childCellCountPerKInterval, parentCellCountPerKInterval, kIntervalCount, kChildCellWeights, "K", proxy);
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto ijkpw = gsoap_eml2_3::soap_new_resqml22__IjkParentWindow(gsoapProxy2_3->soap);
		static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->ParentWindow = ijkpw;

		ijkpw->ParentIjkGridRepresentation = parentGrid->newEml23Reference();

		// Regrids
		ijkpw->IRegrid = createRegrid2_2(iCellIndexRegridStart, childCellCountPerIInterval, parentCellCountPerIInterval, iIntervalCount, iChildCellWeights, "I", proxy);
		ijkpw->JRegrid = createRegrid2_2(jCellIndexRegridStart, childCellCountPerJInterval, parentCellCountPerJInterval, jIntervalCount, jChildCellWeights, "J", proxy);
		ijkpw->KRegrid = createRegrid2_2(kCellIndexRegridStart, childCellCountPerKInterval, parentCellCountPerKInterval, kIntervalCount, kChildCellWeights, "K", proxy);
	}
	else {
		throw logic_error("Not implemented yet");
	}

	repository->addRelationship(this, parentGrid);
}

void AbstractGridRepresentation::setParentWindow(
	unsigned int iCellIndexRegridStart, unsigned int constantChildCellCountPerIInterval, unsigned int constantParentCellCountPerIInterval, unsigned int iIntervalCount,
	unsigned int jCellIndexRegridStart, unsigned int constantChildCellCountPerJInterval, unsigned int constantParentCellCountPerJInterval, unsigned int jIntervalCount,
	unsigned int kCellIndexRegridStart, unsigned int constantChildCellCountPerKInterval, unsigned int constantParentCellCountPerKInterval, unsigned int kIntervalCount,
	AbstractIjkGridRepresentation* parentGrid, EML2_NS::AbstractHdfProxy * proxy, double * iChildCellWeights, double * jChildCellWeights, double * kChildCellWeights)
{
	if (constantChildCellCountPerIInterval == 0 || constantParentCellCountPerIInterval == 0 || iIntervalCount == 0 ||
		constantChildCellCountPerJInterval == 0 || constantParentCellCountPerJInterval == 0 || jIntervalCount == 0 ||
		constantChildCellCountPerKInterval == 0 || constantParentCellCountPerKInterval == 0 || kIntervalCount == 0 ||
		parentGrid == nullptr) {
		throw invalid_argument("One of the mandatory method parameter is zero or null");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__IjkParentWindow* ijkpw = gsoap_resqml2_0_1::soap_new_resqml20__IjkParentWindow(gsoapProxy2_0_1->soap);
		static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->ParentWindow = ijkpw;

		ijkpw->ParentGrid = parentGrid->newResqmlReference();

		// Regrids
		ijkpw->IRegrid = createRegrid2_0_1(iCellIndexRegridStart, &constantChildCellCountPerIInterval, &constantParentCellCountPerIInterval, iIntervalCount, iChildCellWeights, "I", proxy, true);
		ijkpw->JRegrid = createRegrid2_0_1(jCellIndexRegridStart, &constantChildCellCountPerJInterval, &constantParentCellCountPerJInterval, jIntervalCount, jChildCellWeights, "J", proxy, true);
		ijkpw->KRegrid = createRegrid2_0_1(kCellIndexRegridStart, &constantChildCellCountPerKInterval, &constantParentCellCountPerKInterval, kIntervalCount, kChildCellWeights, "K", proxy, true);
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto ijkpw = gsoap_eml2_3::soap_new_resqml22__IjkParentWindow(gsoapProxy2_3->soap);
		static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->ParentWindow = ijkpw;

		ijkpw->ParentIjkGridRepresentation = parentGrid->newEml23Reference();

		// Regrids
		ijkpw->IRegrid = createRegrid2_2(iCellIndexRegridStart, &constantChildCellCountPerIInterval, &constantParentCellCountPerIInterval, iIntervalCount, iChildCellWeights, "I", proxy, true);
		ijkpw->JRegrid = createRegrid2_2(jCellIndexRegridStart, &constantChildCellCountPerJInterval, &constantParentCellCountPerJInterval, jIntervalCount, jChildCellWeights, "J", proxy, true);
		ijkpw->KRegrid = createRegrid2_2(kCellIndexRegridStart, &constantChildCellCountPerKInterval, &constantParentCellCountPerKInterval, kIntervalCount, kChildCellWeights, "K", proxy, true);
	}
	else {
		throw logic_error("Not implemented yet");
	}

	repository->addRelationship(this, parentGrid);
}

void AbstractGridRepresentation::setParentWindow(
	unsigned int iCellIndexRegridStart, unsigned int iChildCellCount, unsigned int iParentCellCount,
	unsigned int jCellIndexRegridStart, unsigned int jChildCellCount, unsigned int jParentCellCount,
	unsigned int kCellIndexRegridStart, unsigned int kChildCellCount, unsigned int kParentCellCount,
	AbstractIjkGridRepresentation* parentGrid, EML2_NS::AbstractHdfProxy * proxy, double * iChildCellWeights, double * jChildCellWeights, double * kChildCellWeights)
{
	setParentWindow(
		iCellIndexRegridStart, &iChildCellCount, &iParentCellCount, 1,
		jCellIndexRegridStart, &jChildCellCount, &jParentCellCount, 1,
		kCellIndexRegridStart, &kChildCellCount, &kParentCellCount, 1,
		parentGrid, proxy, iChildCellWeights, jChildCellWeights, kChildCellWeights);
}

void AbstractGridRepresentation::setForcedNonRegridedParentCell(uint64_t * cellIndices, uint64_t cellIndexCount)
{
	if (cellIndices == nullptr) {
		throw invalid_argument("cellIndices cannot be nullptr.");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			gsoap_resqml2_0_1::resqml20__AbstractIntegerArray* xmlCellIndices = nullptr;

			EML2_NS::AbstractHdfProxy * proxy = getRepository()->getDefaultHdfProxy();
			if (cellIndexCount > 1 && proxy != nullptr)
			{
				getRepository()->addRelationship(this, proxy);
				xmlCellIndices = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(parentWindow->soap);
				static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(xmlCellIndices)->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write cell index null value in this method
				static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(xmlCellIndices)->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(parentWindow->soap);
				static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(xmlCellIndices)->Values->HdfProxy = proxy->newResqmlReference();
				static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(xmlCellIndices)->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_CellIndices";

				// HDF
				proxy->writeArrayNdOfUInt64Values(getHdfGroup(), "ParentWindow_CellIndices", cellIndices, &cellIndexCount, 1);
			}
			else if (cellIndexCount == 1)
			{
				xmlCellIndices = gsoap_resqml2_0_1::soap_new_resqml20__IntegerConstantArray(parentWindow->soap);
				static_cast<gsoap_resqml2_0_1::resqml20__IntegerConstantArray*>(xmlCellIndices)->Value = *cellIndices;
				static_cast<gsoap_resqml2_0_1::resqml20__IntegerConstantArray*>(xmlCellIndices)->Count = 1;
			}
			else if (cellIndexCount == 0)
				throw invalid_argument("Cannot force an empty list of cells.");
			else
				throw invalid_argument("The HDF proxy is missing.");

			switch (parentWindow->soap_type()) {
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow: static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->OmitParentCells = xmlCellIndices; break;
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow: static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow)->OmitParentCells = xmlCellIndices; break;
			default: throw invalid_argument("You can only force parent cell on a ijk or column layer parent window.");
			}
		}
		else {
			throw invalid_argument("You cannot force parent cell without a parent window. Set the parent window first please.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			gsoap_eml2_3::eml23__AbstractIntegerArray* xmlCellIndices = nullptr;

			EML2_NS::AbstractHdfProxy * proxy = getRepository()->getDefaultHdfProxy();
			if (cellIndexCount > 1 && proxy != nullptr)
			{
				getRepository()->addRelationship(this, proxy);
				xmlCellIndices = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(parentWindow->soap);
				static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(xmlCellIndices)->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write cell index null value in this method
				static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(xmlCellIndices)->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(parentWindow->soap);
				static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(xmlCellIndices)->Values->ExternalDataArrayPart.push_back(
					createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_CellIndices", cellIndexCount, proxy));

				// HDF
				proxy->writeArrayNdOfUInt64Values(getHdfGroup(), "ParentWindow_CellIndices", cellIndices, &cellIndexCount, 1);
			}
			else if (cellIndexCount == 1)
			{
				xmlCellIndices = gsoap_eml2_3::soap_new_eml23__IntegerConstantArray(parentWindow->soap);
				static_cast<gsoap_eml2_3::eml23__IntegerConstantArray*>(xmlCellIndices)->Value = *cellIndices;
				static_cast<gsoap_eml2_3::eml23__IntegerConstantArray*>(xmlCellIndices)->Count = 1;
			}
			else if (cellIndexCount == 0)
				throw invalid_argument("Cannot force an empty list of cells.");
			else
				throw invalid_argument("The HDF proxy is missing.");

			switch (parentWindow->soap_type()) {
			case SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow: static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->OmitParentCells = xmlCellIndices; break;
			case SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow: static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow)->OmitParentCells = xmlCellIndices; break;
			default: throw invalid_argument("You can only force parent cell on a ijk or column layer parent window.");
			}
		}
		else {
			throw invalid_argument("You cannot force parent cell without a parent window. Set the parent window first please.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

bool AbstractGridRepresentation::hasForcedNonRegridedParentCell() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr) {
			switch (parentWindow->soap_type()) {
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow: return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->OmitParentCells != nullptr;
			case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow: return static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow)->OmitParentCells != nullptr;
			default: return false;
			}
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr) {
			switch (parentWindow->soap_type()) {
			case SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow: return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->OmitParentCells != nullptr;
			case SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow: return static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow)->OmitParentCells != nullptr;
			default: return false;
			}
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::setCellOverlap(uint64_t parentChildCellPairCount, uint64_t * parentChildCellPair,
	const std::string& volumeUom, double * overlapVolumes)
{
	if (parentChildCellPair == nullptr) {
		throw invalid_argument("parentChildCellPair cannot be nullptr.");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			EML2_NS::AbstractHdfProxy * proxy = getRepository()->getDefaultHdfProxy();
			if (parentChildCellPairCount > 0 && proxy != nullptr)
			{
				getRepository()->addRelationship(this, proxy);
				parentWindow->CellOverlap = gsoap_resqml2_0_1::soap_new_resqml20__CellOverlap(gsoapProxy2_0_1->soap);

				gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* hdf5CellPairs = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(parentWindow->soap);
				parentWindow->CellOverlap->__CellOverlap_sequence = gsoap_resqml2_0_1::soap_new___resqml20__CellOverlap_sequence(parentWindow->soap);
				parentWindow->CellOverlap->__CellOverlap_sequence->Count = parentChildCellPairCount;
				parentWindow->CellOverlap->__CellOverlap_sequence->ParentChildCellPairs = hdf5CellPairs;
				hdf5CellPairs->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write cell index null value in this method
				hdf5CellPairs->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(parentWindow->soap);
				hdf5CellPairs->Values->HdfProxy = proxy->newResqmlReference();
				hdf5CellPairs->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_CellOverlap";

				// HDF
				uint64_t numValues[] = { parentChildCellPairCount, 2 };
				proxy->writeArrayNdOfUInt64Values(getHdfGroup(), "ParentWindow_CellOverlap", parentChildCellPair, numValues, 2);
			}
			else if (parentChildCellPairCount == 0)
				throw invalid_argument("Cannot give cell overlap information with an empty list of cell pairs.");
			else
				throw invalid_argument("The HDF proxy is missing.");

			if (overlapVolumes != nullptr)
			{
				parentWindow->CellOverlap->__CellOverlap_sequence->OverlapVolume = gsoap_resqml2_0_1::soap_new_resqml20__OverlapVolume(parentWindow->soap);
				parentWindow->CellOverlap->__CellOverlap_sequence->OverlapVolume->__OverlapVolume_sequence = gsoap_resqml2_0_1::soap_new___resqml20__OverlapVolume_sequence(parentWindow->soap);
				gsoap_resqml2_0_1::eml20__VolumeUom uomEnum;
				gsoap_resqml2_0_1::soap_s2eml20__VolumeUom(parentWindow->soap, volumeUom.c_str(), &uomEnum);
				parentWindow->CellOverlap->__CellOverlap_sequence->OverlapVolume->__OverlapVolume_sequence->VolumeUom = uomEnum;

				gsoap_resqml2_0_1::resqml20__DoubleHdf5Array* hdf5OverlapVolume = gsoap_resqml2_0_1::soap_new_resqml20__DoubleHdf5Array(parentWindow->soap);
				parentWindow->CellOverlap->__CellOverlap_sequence->OverlapVolume->__OverlapVolume_sequence->OverlapVolumes = hdf5OverlapVolume;
				hdf5OverlapVolume->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(parentWindow->soap);
				hdf5OverlapVolume->Values->HdfProxy = proxy->newResqmlReference();
				hdf5OverlapVolume->Values->PathInHdfFile = getHdfGroup() + "/ParentWindow_OverlapVolume";

				// HDF
				proxy->writeArrayNdOfDoubleValues(getHdfGroup(), "ParentWindow_OverlapVolume", overlapVolumes, &parentChildCellPairCount, 1);
			}
		}
		else {
			throw invalid_argument("You cannot give cell overlap information without a parent window. Set the parent window first please.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			EML2_NS::AbstractHdfProxy * proxy = getRepository()->getDefaultHdfProxy();
			if (parentChildCellPairCount > 0 && proxy != nullptr)
			{
				getRepository()->addRelationship(this, proxy);
				parentWindow->CellOverlap = gsoap_eml2_3::soap_new_resqml22__CellOverlap(gsoapProxy2_3->soap);

				gsoap_eml2_3::eml23__IntegerExternalArray* hdf5CellPairs = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(parentWindow->soap);
				parentWindow->CellOverlap->Count = parentChildCellPairCount;
				parentWindow->CellOverlap->ParentChildCellPairs = hdf5CellPairs;
				hdf5CellPairs->NullValue = -1; // Arbitrarily decided to something almost impossible since it has no interest to write cell index null value in this method
				hdf5CellPairs->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(parentWindow->soap);
				hdf5CellPairs->Values->ExternalDataArrayPart.push_back(
					createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_CellOverlap", parentChildCellPairCount * 2, proxy));

				// HDF
				uint64_t numValues[] = { parentChildCellPairCount, 2 };
				proxy->writeArrayNdOfUInt64Values(getHdfGroup(), "ParentWindow_CellOverlap", parentChildCellPair, numValues, 2);
			}
			else if (parentChildCellPairCount == 0)
				throw invalid_argument("Cannot give cell overlap information with an empty list of cell pairs.");
			else
				throw invalid_argument("The HDF proxy is missing.");

			if (overlapVolumes != nullptr)
			{
				parentWindow->CellOverlap->OverlapVolume = gsoap_eml2_3::soap_new_resqml22__OverlapVolume(parentWindow->soap);
				gsoap_eml2_3::eml23__VolumeUom uomEnum;
				gsoap_eml2_3::soap_s2eml23__VolumeUom(parentWindow->soap, volumeUom.c_str(), &uomEnum);
				parentWindow->CellOverlap->OverlapVolume->VolumeUom = uomEnum;

				gsoap_eml2_3::eml23__FloatingPointExternalArray* hdf5OverlapVolume = gsoap_eml2_3::soap_new_eml23__FloatingPointExternalArray(parentWindow->soap);
				parentWindow->CellOverlap->OverlapVolume->OverlapVolumes = hdf5OverlapVolume;
				hdf5OverlapVolume->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(parentWindow->soap);
				hdf5OverlapVolume->Values->ExternalDataArrayPart.push_back(
					createExternalDataArrayPart(getHdfGroup() + "/ParentWindow_OverlapVolume", parentChildCellPairCount, proxy));

				// HDF
				proxy->writeArrayNdOfDoubleValues(getHdfGroup(), "ParentWindow_OverlapVolume", overlapVolumes, &parentChildCellPairCount, 1);
			}
		}
		else {
			throw invalid_argument("You cannot give cell overlap information without a parent window. Set the parent window first please.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

int64_t AbstractGridRepresentation::getParentCellIndexCount() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__CellParentWindow)
			{
				gsoap_resqml2_0_1::resqml20__CellParentWindow* cpw = static_cast<gsoap_resqml2_0_1::resqml20__CellParentWindow*>(parentWindow);
				if (cpw->CellIndices->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array)
				{
					gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(cpw->CellIndices)->Values;
					EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
					return hdfProxy->getElementCount(dataset->PathInHdfFile);
				}
				else
					throw invalid_argument("This list of cells can only be stored in HDF5 file.");
			}
			else
				throw invalid_argument("This information is only avaialble for cell parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__CellParentWindow)
			{
				auto cpw = static_cast<gsoap_eml2_3::resqml22__CellParentWindow*>(parentWindow);
				if (cpw->CellIndices->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray)
				{
					gsoap_eml2_3::eml23__ExternalDataArray const * dataset = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(cpw->CellIndices)->Values;
					EML2_NS::AbstractHdfProxy * hdfProxy = getOrCreateHdfProxyFromDataArrayPart(dataset->ExternalDataArrayPart[0]);
					return hdfProxy->getElementCount(dataset->ExternalDataArrayPart[0]->PathInExternalFile);
				}
				else
					throw invalid_argument("This list of cells can only be stored in HDF5 file.");
			}
			else
				throw invalid_argument("This information is only avaialble for cell parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getParentCellIndices(uint64_t * parentCellIndices) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__CellParentWindow)
			{
				gsoap_resqml2_0_1::resqml20__CellParentWindow const* cpw = static_cast<gsoap_resqml2_0_1::resqml20__CellParentWindow*>(parentWindow);
				readArrayNdOfUInt64Values(cpw->CellIndices, parentCellIndices);
			}
			else
				throw invalid_argument("This information is only available for cell parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__CellParentWindow)
			{
				gsoap_eml2_3::resqml22__CellParentWindow const* cpw = static_cast<gsoap_eml2_3::resqml22__CellParentWindow*>(parentWindow);
				readArrayNdOfUInt64Values(cpw->CellIndices, parentCellIndices);
			}
			else
				throw invalid_argument("This information is only available for cell parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

int64_t AbstractGridRepresentation::getParentColumnIndexCount() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow)
			{
				gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow* clpw = static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow);
				if (clpw->ColumnIndices->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array) {
					gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(clpw->ColumnIndices)->Values;
					EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
					return hdfProxy->getElementCount(dataset->PathInHdfFile);
				}
				else {
					throw invalid_argument("This list of columns can only be stored in HDF5 file.");
				}
			}
			else {
				throw invalid_argument("This information is only avaialble for column layer parent window.");
			}
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow)
			{
				auto cpw = static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow);
				if (cpw->ColumnIndices->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray) {
					gsoap_eml2_3::eml23__ExternalDataArray const * dataset = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(cpw->ColumnIndices)->Values;
					EML2_NS::AbstractHdfProxy * hdfProxy = getOrCreateHdfProxyFromDataArrayPart(dataset->ExternalDataArrayPart[0]);
					return hdfProxy->getElementCount(dataset->ExternalDataArrayPart[0]->PathInExternalFile);
				}
				else
					throw invalid_argument("This list of cells can only be stored in HDF5 file.");
			}
			else
				throw invalid_argument("This information is only available for cell parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getParentColumnIndices(uint64_t * parentColumnIndices) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow)
			{
				gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow const* clpw = static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow);
				readArrayNdOfUInt64Values(clpw->ColumnIndices, parentColumnIndices);
			}
			else {
				throw invalid_argument("This information is only avaialble for column layer parent window.");
			}
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow)
			{
				gsoap_eml2_3::resqml22__ColumnLayerParentWindow const* cpw = static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow);
				readArrayNdOfUInt64Values(cpw->ColumnIndices, parentColumnIndices);
			}
			else {
				throw invalid_argument("This information is only available for cell parent window.");
			}
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

uint64_t AbstractGridRepresentation::getRegridStartIndexOnParentGrid(char dimension) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow)
			{
				if (dimension == 'i' || dimension == 'I')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->IRegrid->InitialIndexOnParentGrid;
				}
				else if (dimension == 'j' || dimension == 'J')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->JRegrid->InitialIndexOnParentGrid;
				}
				else if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->KRegrid->InitialIndexOnParentGrid;
				}
				else
					throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow)
			{
				if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow)->KRegrid->InitialIndexOnParentGrid;
				}
				else
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
			else
				throw invalid_argument("Regrid information is only avaialble for ijk or column layer parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow)
			{
				if (dimension == 'i' || dimension == 'I')
				{
					return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->IRegrid->InitialIndexOnParentGrid;
				}
				else if (dimension == 'j' || dimension == 'J')
				{
					return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->JRegrid->InitialIndexOnParentGrid;
				}
				else if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->KRegrid->InitialIndexOnParentGrid;
				}
				else
					throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow)
			{
				if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow)->KRegrid->InitialIndexOnParentGrid;
				}
				else
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
			else
				throw invalid_argument("Regrid information is only avaialble for ijk or column layer parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

uint64_t AbstractGridRepresentation::getRegridIntervalCount(char dimension) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow)
			{
				if (dimension == 'i' || dimension == 'I')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->IRegrid->Intervals != nullptr
						? static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->IRegrid->Intervals->IntervalCount
						: 0;
				}
				else if (dimension == 'j' || dimension == 'J')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->JRegrid->Intervals != nullptr
						? static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->JRegrid->Intervals->IntervalCount
						: 0;
				}
				else if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->KRegrid->Intervals != nullptr
						? static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow)->KRegrid->Intervals->IntervalCount
						: 0;
				}
				else
					throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow)
			{
				if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow)->KRegrid->Intervals != nullptr
						? static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow)->KRegrid->Intervals->IntervalCount
						: 0;
				}
				else
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
			else
				throw invalid_argument("The parent window has not got any regrid information.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr)
		{
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow)
			{
				if (dimension == 'i' || dimension == 'I')
				{
					return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->IRegrid->Intervals != nullptr
						? static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->IRegrid->Intervals->IntervalCount
						: 0;
				}
				else if (dimension == 'j' || dimension == 'J')
				{
					return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->JRegrid->Intervals != nullptr
						? static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->JRegrid->Intervals->IntervalCount
						: 0;
				}
				else if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->KRegrid->Intervals != nullptr
						? static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow)->KRegrid->Intervals->IntervalCount
						: 0;
				}
				else
					throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow)
			{
				if (dimension == 'k' || dimension == 'K')
				{
					return static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow)->KRegrid->Intervals != nullptr
						? static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow)->KRegrid->Intervals->IntervalCount
						: 0;
				}
				else
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
			else
				throw invalid_argument("Regrid information is only avaialble for ijk or column layer parent window.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

gsoap_resqml2_0_1::resqml20__AbstractIntegerArray* AbstractGridRepresentation::getCellCountPerInterval2_0_1(char dimension, bool childVsParentCellCount) const
{
	gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

	if (parentWindow != nullptr) {
		if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow) {
			gsoap_resqml2_0_1::resqml20__IjkParentWindow const * const ijkpw = static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow const*>(parentWindow);
			gsoap_resqml2_0_1::resqml20__Regrid const * regrid = nullptr;
			if (dimension == 'i' || dimension == 'I') {
				regrid = ijkpw->IRegrid;
			}
			else if (dimension == 'j' || dimension == 'J') {
				regrid = ijkpw->JRegrid;
			}
			else if (dimension == 'k' || dimension == 'K') {
				regrid = ijkpw->KRegrid;
			}
			else {
				throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}

			if (regrid->Intervals != nullptr) {
				return childVsParentCellCount ? regrid->Intervals->ChildCountPerInterval : regrid->Intervals->ParentCountPerInterval;
			}
			else {
				throw invalid_argument("No interval for this regrid.");
			}
		}
		else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow) {
			if (dimension == 'k' || dimension == 'K') {
				gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow const* const clpw = static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow const*>(parentWindow);
				if (clpw->KRegrid->Intervals != nullptr) {
					return childVsParentCellCount ? clpw->KRegrid->Intervals->ChildCountPerInterval : clpw->KRegrid->Intervals->ParentCountPerInterval;
				}
				else {
					throw invalid_argument("No interval for K regrid.");
				}
			}
			else {
				throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
		}
		else {
			throw invalid_argument("The parent window has not got any regrid information.");
		}
	}
	else {
		throw invalid_argument("There is no parent window on this grid.");
	}
}

gsoap_eml2_3::eml23__AbstractIntegerArray* AbstractGridRepresentation::getCellCountPerInterval2_2(char dimension, bool childVsParentCellCount) const
{
	auto parentWindow = getParentWindow2_2();

	if (parentWindow != nullptr) {
		if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow) {
			gsoap_eml2_3::resqml22__IjkParentWindow const * const ijkpw = static_cast<gsoap_eml2_3::resqml22__IjkParentWindow const*>(parentWindow);
			gsoap_eml2_3::resqml22__Regrid const * regrid = nullptr;
			if (dimension == 'i' || dimension == 'I') {
				regrid = ijkpw->IRegrid;
			}
			else if (dimension == 'j' || dimension == 'J') {
				regrid = ijkpw->JRegrid;
			}
			else if (dimension == 'k' || dimension == 'K') {
				regrid = ijkpw->KRegrid;
			}
			else {
				throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}

			if (regrid->Intervals != nullptr) {
				return childVsParentCellCount ? regrid->Intervals->ChildCountPerInterval : regrid->Intervals->ParentCountPerInterval;
			}
			else {
				throw invalid_argument("No interval for this regrid.");
			}
		}
		else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow) {
			if (dimension == 'k' || dimension == 'K') {
				gsoap_eml2_3::resqml22__ColumnLayerParentWindow const* const clpw = static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow const*>(parentWindow);
				if (clpw->KRegrid->Intervals != nullptr) {
					return childVsParentCellCount ? clpw->KRegrid->Intervals->ChildCountPerInterval : clpw->KRegrid->Intervals->ParentCountPerInterval;
				}
				else {
					throw invalid_argument("No interval for K regrid.");
				}
			}
			else {
				throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
		}
		else {
			throw invalid_argument("The parent window has not got any regrid information.");
		}
	}
	else {
		throw invalid_argument("There is no parent window on this grid.");
	}
}

bool AbstractGridRepresentation::isRegridCellCountPerIntervalConstant(char dimension, bool childVsParentCellCount) const {
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractIntegerArray* cellCountPerInterval = getCellCountPerInterval2_0_1(dimension, childVsParentCellCount);
		if (cellCountPerInterval->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerConstantArray) {
			return true;
		}
		else if (cellCountPerInterval->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array) {
			const uint64_t intervalCount = getRegridIntervalCount(dimension);
			std::unique_ptr<uint64_t[]> values(new uint64_t[intervalCount]);
			getRegridCellCountPerInterval(dimension, values.get(), childVsParentCellCount);

			for (uint64_t index = 1; index < intervalCount; ++index) {
				if (values[index] != values[0]) {
					return false;
				}
			}

			return true;
		}
		return false;
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::eml23__AbstractIntegerArray* cellCountPerInterval = getCellCountPerInterval2_2(dimension, childVsParentCellCount);
		if (cellCountPerInterval->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerConstantArray) {
			return true;
		}
		else if (cellCountPerInterval->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray) {
			const uint64_t intervalCount = getRegridIntervalCount(dimension);
			std::unique_ptr<uint64_t[]> values(new uint64_t[intervalCount]);
			getRegridCellCountPerInterval(dimension, values.get(), childVsParentCellCount);

			for (uint64_t index = 1; index < intervalCount; ++index) {
				if (values[index] != values[0]) {
					return false;
				}
			}

			return true;
		}
		return false;
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

uint64_t AbstractGridRepresentation::getRegridConstantCellCountPerInterval(char dimension, bool childVsParentCellCount) const {
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractIntegerArray const* const cellCountPerInterval = getCellCountPerInterval2_0_1(dimension, childVsParentCellCount);
		switch (cellCountPerInterval->soap_type()) {
		case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerConstantArray: return static_cast<gsoap_resqml2_0_1::resqml20__IntegerConstantArray const*>(cellCountPerInterval)->Value;
		case SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array: {
			const uint64_t intervalCount = getRegridIntervalCount(dimension);
			std::unique_ptr<uint64_t[]> values(new uint64_t[intervalCount]);
			getRegridCellCountPerInterval(dimension, values.get(), childVsParentCellCount);
			uint64_t result = values[0];
			return result;
		}
		default: throw logic_error("Not implemented yet");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::eml23__AbstractIntegerArray const* cellCountPerInterval = getCellCountPerInterval2_2(dimension, childVsParentCellCount);
		switch (cellCountPerInterval->soap_type()) {
		case SOAP_TYPE_gsoap_eml2_3_eml23__IntegerConstantArray: return static_cast<gsoap_eml2_3::eml23__IntegerConstantArray const*>(cellCountPerInterval)->Value;
		case SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray: {
			const uint64_t intervalCount = getRegridIntervalCount(dimension);
			std::unique_ptr<uint64_t[]> values(new uint64_t[intervalCount]);
			getRegridCellCountPerInterval(dimension, values.get(), childVsParentCellCount);
			uint64_t result = values[0];
			return result;
		}
		default: throw logic_error("Not implemented yet");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getRegridCellCountPerInterval(char dimension, uint64_t * childCellCountPerInterval, bool childVsParentCellCount) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		readArrayNdOfUInt64Values(getCellCountPerInterval2_0_1(dimension, childVsParentCellCount), childCellCountPerInterval);
	}
	else if (gsoapProxy2_3 != nullptr) {
		readArrayNdOfUInt64Values(getCellCountPerInterval2_2(dimension, childVsParentCellCount), childCellCountPerInterval);
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

namespace {
	template <typename T>
	bool hasRegridChildCellWeightsInIjkParentWindow(T ijkpw, char dimension) {
		if (dimension == 'i' || dimension == 'I') {
			if (ijkpw->IRegrid->Intervals != nullptr) {
				return ijkpw->IRegrid->Intervals->ChildCellWeights != nullptr;
			}
			else
				throw invalid_argument("No interval for I regrid.");
		}
		else if (dimension == 'j' || dimension == 'J') {
			if (ijkpw->JRegrid->Intervals != nullptr) {
				return ijkpw->JRegrid->Intervals->ChildCellWeights != nullptr;
			}
			else
				throw invalid_argument("No interval for J regrid.");
		}
		else if (dimension == 'k' || dimension == 'K') {
			if (ijkpw->KRegrid->Intervals != nullptr) {
				return ijkpw->KRegrid->Intervals->ChildCellWeights != nullptr;
			}
			else {
				throw invalid_argument("No interval for K regrid.");
			}
		}
		else {
			throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
		}
	}
}

bool AbstractGridRepresentation::hasRegridChildCellWeights(char dimension) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr) {
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow)
			{
				return hasRegridChildCellWeightsInIjkParentWindow(static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow), dimension);
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow)
			{
				if (dimension == 'k' || dimension == 'K')
				{
					gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow* clpw = static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow);
					if (clpw->KRegrid->Intervals != nullptr) {
						return clpw->KRegrid->Intervals->ChildCellWeights != nullptr;
					}
					else {
						throw invalid_argument("No interval for K regrid.");
					}
				}
				else {
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
				}
			}
			else
				throw invalid_argument("The parent window has not got any regrid information.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr) {
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow)
			{
				return hasRegridChildCellWeightsInIjkParentWindow(static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow), dimension);
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow)
			{
				if (dimension == 'k' || dimension == 'K')
				{
					gsoap_eml2_3::resqml22__ColumnLayerParentWindow* clpw = static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow);
					if (clpw->KRegrid->Intervals != nullptr) {
						return clpw->KRegrid->Intervals->ChildCellWeights != nullptr;
					}
					else {
						throw invalid_argument("No interval for K regrid.");
					}
				}
				else {
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
				}
			}
			else
				throw invalid_argument("The parent window has not got any regrid information.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getRegridChildCellWeights(char dimension, double * childCellWeights) const
{
	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractParentWindow* parentWindow = getParentWindow2_0_1();

		if (parentWindow != nullptr) {
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IjkParentWindow) {
				gsoap_resqml2_0_1::resqml20__IjkParentWindow* ijkpw = static_cast<gsoap_resqml2_0_1::resqml20__IjkParentWindow*>(parentWindow);
				if (dimension == 'i' || dimension == 'I') {
					if (ijkpw->IRegrid->Intervals != nullptr && ijkpw->IRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(ijkpw->IRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for I regrid.");
				}
				else if (dimension == 'j' || dimension == 'J') {
					if (ijkpw->JRegrid->Intervals != nullptr && ijkpw->JRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(ijkpw->JRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for J regrid.");
				}
				else if (dimension == 'k' || dimension == 'K') {
					if (ijkpw->KRegrid->Intervals != nullptr && ijkpw->KRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(ijkpw->KRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for K regrid.");
				}
				else
					throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__ColumnLayerParentWindow) {
				if (dimension == 'k' || dimension == 'K') {
					gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow* clpw = static_cast<gsoap_resqml2_0_1::resqml20__ColumnLayerParentWindow*>(parentWindow);
					if (clpw->KRegrid->Intervals != nullptr  && clpw->KRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(clpw->KRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for K regrid.");
				}
				else
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
			else
				throw invalid_argument("The parent window has not got any regrid information.");
		}
		else {
			throw invalid_argument("There is no parent window on this grid.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto parentWindow = getParentWindow2_2();

		if (parentWindow != nullptr) {
			if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__IjkParentWindow) {
				gsoap_eml2_3::resqml22__IjkParentWindow* ijkpw = static_cast<gsoap_eml2_3::resqml22__IjkParentWindow*>(parentWindow);
				if (dimension == 'i' || dimension == 'I') {
					if (ijkpw->IRegrid->Intervals != nullptr && ijkpw->IRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(ijkpw->IRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for I regrid.");
				}
				else if (dimension == 'j' || dimension == 'J') {
					if (ijkpw->JRegrid->Intervals != nullptr && ijkpw->JRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(ijkpw->JRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for J regrid.");
				}
				else if (dimension == 'k' || dimension == 'K') {
					if (ijkpw->KRegrid->Intervals != nullptr && ijkpw->KRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(ijkpw->KRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for K regrid.");
				}
				else
					throw invalid_argument("Only i, j or k dimension can be accessed for an IJK Parent Window.");
			}
			else if (parentWindow->soap_type() == SOAP_TYPE_gsoap_eml2_3_resqml22__ColumnLayerParentWindow) {
				if (dimension == 'k' || dimension == 'K') {
					gsoap_eml2_3::resqml22__ColumnLayerParentWindow* clpw = static_cast<gsoap_eml2_3::resqml22__ColumnLayerParentWindow*>(parentWindow);
					if (clpw->KRegrid->Intervals != nullptr  && clpw->KRegrid->Intervals->ChildCellWeights != nullptr) {
						readArrayNdOfDoubleValues(clpw->KRegrid->Intervals->ChildCellWeights, childCellWeights);
					}
					else
						throw invalid_argument("No interval for K regrid.");
				}
				else
					throw invalid_argument("Only k dimension can be accessed for a ColumnLayer Parent Window.");
			}
			else
				throw invalid_argument("The parent window has not got any regrid information.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::loadTargetRelationships()
{
	AbstractRepresentation::loadTargetRelationships();

	// LGR backward relationships
	auto dor = getParentGridDor();
	if (!dor.isEmpty()) {
		convertDorIntoRel(dor);
	}

	// Strati org backward relationships
	dor = getStratigraphicOrganizationInterpretationDor();
	if (!dor.isEmpty()) {
		convertDorIntoRel(dor);
	}

	dor = getRockFluidOrganizationInterpretationDor();
	if (!dor.isEmpty()) {
		convertDorIntoRel(dor);
	}
}

void AbstractGridRepresentation::setCellAssociationWithStratigraphicOrganizationInterpretation(int64_t * stratiUnitIndices, int64_t nullValue, RESQML2_NS::AbstractStratigraphicOrganizationInterpretation * stratiOrgInterp)
{
	if (stratiOrgInterp == nullptr) {
		throw invalid_argument("The stratigraphic organization interpretation cannot be null.");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1);
		rep->CellStratigraphicUnits = gsoap_resqml2_0_1::soap_new_resqml20__CellStratigraphicUnits(rep->soap);
		rep->CellStratigraphicUnits->StratigraphicOrganization = stratiOrgInterp->newResqmlReference();

		EML2_NS::AbstractHdfProxy * proxy = getRepository()->getDefaultHdfProxy();
		if (proxy == nullptr) {
			throw invalid_argument("The HDF proxy is missing.");
		}
		gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* xmlDataset = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(rep->soap);
		xmlDataset->NullValue = nullValue;
		xmlDataset->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(gsoapProxy2_0_1->soap);
		xmlDataset->Values->HdfProxy = proxy->newResqmlReference();
		xmlDataset->Values->PathInHdfFile = getHdfGroup() + "/CellStratigraphicUnits";
		rep->CellStratigraphicUnits->UnitIndices = xmlDataset;

		// ************ HDF *************
		const uint64_t dim = getCellCount();
		proxy->writeArrayNd(getHdfGroup(), "CellStratigraphicUnits", COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64, stratiUnitIndices, &dim, 1);
	}
	else {
		throw logic_error("Only RESQML 2.0.1 allows to assocaite cells with Stratigraphic Organization Interpretation. Use IntervalStratigraphicUnits instead.");
	}
}

void AbstractGridRepresentation::setCellAssociationWithRockFluidOrganizationInterpretation(int64_t * rockFluidUnitIndices, int64_t nullValue, RESQML2_NS::RockFluidOrganizationInterpretation* rockfluidOrgInterp)
{
	EML2_NS::AbstractHdfProxy * proxy = getRepository()->getDefaultHdfProxy();
	if (proxy == nullptr) {
		throw invalid_argument("The HDF proxy is missing.");
	}

	// ************ HDF *************
	const uint64_t dim = getCellCount();
	proxy->writeArrayNd(getHdfGroup(), "CellFluidPhaseUnits", COMMON_NS::AbstractObject::numericalDatatypeEnum::INT64, rockFluidUnitIndices, &dim, 1);

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1);
		rep->CellFluidPhaseUnits = gsoap_resqml2_0_1::soap_new_resqml20__CellFluidPhaseUnits(rep->soap);
		rep->CellFluidPhaseUnits->FluidOrganization = rockfluidOrgInterp->newResqmlReference();

		gsoap_resqml2_0_1::resqml20__IntegerHdf5Array* xmlDataset = gsoap_resqml2_0_1::soap_new_resqml20__IntegerHdf5Array(rep->soap);
		xmlDataset->NullValue = nullValue;
		xmlDataset->Values = gsoap_resqml2_0_1::soap_new_eml20__Hdf5Dataset(gsoapProxy2_0_1->soap);
		xmlDataset->Values->HdfProxy = proxy->newResqmlReference();
		xmlDataset->Values->PathInHdfFile = getHdfGroup() + "/CellFluidPhaseUnits";
		rep->CellFluidPhaseUnits->PhaseUnitIndices = xmlDataset;
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3);
		rep->CellFluidPhaseUnits = gsoap_eml2_3::soap_new_resqml22__CellFluidPhaseUnits(rep->soap);
		rep->CellFluidPhaseUnits->RockFluidOrganizationInterpretation = rockfluidOrgInterp->newEml23Reference();

		rep->CellFluidPhaseUnits->PhaseUnitIndices = gsoap_eml2_3::soap_new_eml23__JaggedArray(rep->soap);
		// element XML
		gsoap_eml2_3::eml23__IntegerExternalArray* elementDataset = gsoap_eml2_3::soap_new_eml23__IntegerExternalArray(rep->soap);
		elementDataset->NullValue = nullValue;
		elementDataset->Values = gsoap_eml2_3::soap_new_eml23__ExternalDataArray(gsoapProxy2_3->soap);
		auto* daPart = createExternalDataArrayPart(getHdfGroup() + "/CellFluidPhaseUnits", dim, proxy);
		elementDataset->Values->ExternalDataArrayPart.push_back(daPart);
		rep->CellFluidPhaseUnits->PhaseUnitIndices->Elements = elementDataset;

		// cumulative XML
		gsoap_eml2_3::eml23__IntegerLatticeArray* cumulativeDataset = gsoap_eml2_3::soap_new_eml23__IntegerLatticeArray(rep->soap);
		cumulativeDataset->StartValue = 1;
		gsoap_eml2_3::eml23__IntegerConstantArray* constantArray = gsoap_eml2_3::soap_new_eml23__IntegerConstantArray(rep->soap);
		constantArray->Count = dim - 1;
		constantArray->Value = 1;
		cumulativeDataset->Offset.push_back(constantArray);
		rep->CellFluidPhaseUnits->PhaseUnitIndices->CumulativeLength = cumulativeDataset;
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

COMMON_NS::DataObjectReference AbstractGridRepresentation::getStratigraphicOrganizationInterpretationDor() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		return static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->CellStratigraphicUnits == nullptr
			? COMMON_NS::DataObjectReference()
			: COMMON_NS::DataObjectReference(static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->CellStratigraphicUnits->StratigraphicOrganization);
	}
	else if (gsoapProxy2_3 != nullptr) {
		return static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->IntervalStratigraphicUnits == nullptr
			? COMMON_NS::DataObjectReference()
			: COMMON_NS::DataObjectReference(static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->IntervalStratigraphicUnits->StratigraphicOrganizationInterpretation);
	}

	throw logic_error("Not implemented yet");
}

COMMON_NS::DataObjectReference AbstractGridRepresentation::getRockFluidOrganizationInterpretationDor() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		return static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->CellFluidPhaseUnits == nullptr
			? COMMON_NS::DataObjectReference()
			: COMMON_NS::DataObjectReference(static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->CellFluidPhaseUnits->FluidOrganization);
	}
	else if (gsoapProxy2_3 != nullptr) {
		return static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->CellFluidPhaseUnits == nullptr
			? COMMON_NS::DataObjectReference()
			: COMMON_NS::DataObjectReference(static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->CellFluidPhaseUnits->RockFluidOrganizationInterpretation);
	}

	throw logic_error("Not implemented yet");
}

RESQML2_NS::AbstractStratigraphicOrganizationInterpretation* AbstractGridRepresentation::getStratigraphicOrganizationInterpretation() const
{
	const string stratigraphicOrganizationInterpretationUuid = getStratigraphicOrganizationInterpretationDor().getUuid();
	if (stratigraphicOrganizationInterpretationUuid.empty()) {
		return nullptr;
	}

	return getRepository()->getDataObjectByUuid<RESQML2_NS::AbstractStratigraphicOrganizationInterpretation>(stratigraphicOrganizationInterpretationUuid);
}

bool AbstractGridRepresentation::hasCellStratigraphicUnitIndices() const
{
	return gsoapProxy2_0_1 != nullptr
		? static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->CellStratigraphicUnits != nullptr
		: false;
}

bool AbstractGridRepresentation::hasCellFluidPhaseUnitIndices() const
{
	if (gsoapProxy2_0_1 != nullptr) {
		return static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1)->CellFluidPhaseUnits != nullptr;
	}
	else if (gsoapProxy2_3 != nullptr) {
		return static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3)->CellFluidPhaseUnits != nullptr;
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

int64_t AbstractGridRepresentation::getCellStratigraphicUnitIndices(int64_t * stratiUnitIndices)
{
	if (!hasCellStratigraphicUnitIndices()) {
		throw invalid_argument("This grid has no CellStratigraphicUnits information");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1);
		readArrayNdOfInt64Values(rep->CellStratigraphicUnits->UnitIndices, stratiUnitIndices);
		return static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->CellStratigraphicUnits->UnitIndices)->NullValue;
	}
	else {
		throw logic_error("Only RESQML 2.0.1 allows to assocaite cells with Stratigraphic Organization Interpretation. Use IntervalStratigraphicUnits instead.");
	}
}

int64_t AbstractGridRepresentation::getCellFluidPhaseUnitIndices(int64_t * rockFluidUnitIndices)
{
	if (!hasCellFluidPhaseUnitIndices()) {
		throw invalid_argument("This grid has no CellFluidPhaseUnits information");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractGridRepresentation*>(gsoapProxy2_0_1);
		readArrayNdOfInt64Values(rep->CellFluidPhaseUnits->PhaseUnitIndices, rockFluidUnitIndices);
		return static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->CellFluidPhaseUnits->PhaseUnitIndices)->NullValue;
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto rep = static_cast<gsoap_eml2_3::resqml22__AbstractGridRepresentation*>(gsoapProxy2_3);
		if (rep->CellFluidPhaseUnits->PhaseUnitIndices->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerLatticeArray) {
			gsoap_eml2_3::eml23__IntegerLatticeArray* latticeArray = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->CellFluidPhaseUnits->PhaseUnitIndices->CumulativeLength);
			if (latticeArray->StartValue == 1 && latticeArray->Offset.size() == 1 &&
				latticeArray->Offset[0]->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerConstantArray &&
				static_cast<gsoap_eml2_3::eml23__IntegerConstantArray*>(latticeArray->Offset[0])->Value == 1 &&
				static_cast<uint64_t>(static_cast<gsoap_eml2_3::eml23__IntegerConstantArray*>(latticeArray->Offset[0])->Count) == getCellCount()) {
				auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->CellFluidPhaseUnits->PhaseUnitIndices->Elements)->Values->ExternalDataArrayPart[0];
				EML2_NS::AbstractHdfProxy * hdfProxy = getOrCreateHdfProxyFromDataArrayPart(daPart);
				hdfProxy->readArrayNdOfInt64Values(daPart->PathInExternalFile, rockFluidUnitIndices);
				return static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->CellFluidPhaseUnits->PhaseUnitIndices->Elements)->NullValue;
			}
		}

		throw logic_error("Not implemented yet.");
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

RESQML2_NS::RockFluidOrganizationInterpretation* AbstractGridRepresentation::getRockFluidOrganizationInterpretation() const
{
	const string rockfluidOrganizationInterpretationUuid = getRockFluidOrganizationInterpretationDor().getUuid();
	if (rockfluidOrganizationInterpretationUuid.empty()) {
		return nullptr;
	}

	return static_cast<RESQML2_NS::RockFluidOrganizationInterpretation*>(getRepository()->getDataObjectByUuid(rockfluidOrganizationInterpretationUuid));

}

bool AbstractGridRepresentation::isTruncated() const
{
	return withTruncatedPillars;
}

uint64_t AbstractGridRepresentation::getTruncatedFaceCount() const
{
	if (!isTruncated()) {
		return 0;
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);
		return rep->TruncationCells->TruncationFaceCount;
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);
		return rep->TruncationCellPatch->TruncationFaceCount;
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getNodeIndicesOfTruncatedFaces(uint64_t * nodeIndices) const
{
	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->NodesPerTruncationFace->Elements->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array)
		{
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->TruncationCells->NodesPerTruncationFace->Elements)->Values;
			EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
			hdfProxy->readArrayNdOfUInt64Values(dataset->PathInHdfFile, nodeIndices);
		}
		else
			throw logic_error("Not yet implemented");
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->NodesPerTruncationFace->Elements->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray)
		{
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->TruncationCellPatch->NodesPerTruncationFace->Elements)->Values->ExternalDataArrayPart[0];
			EML2_NS::AbstractHdfProxy * hdfProxy = getOrCreateHdfProxyFromDataArrayPart(daPart);
			hdfProxy->readArrayNdOfUInt64Values(daPart->PathInExternalFile, nodeIndices);
		}
		else
			throw logic_error("Not yet implemented");
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getCumulativeNodeCountPerTruncatedFace(uint64_t * nodeCountPerFace) const
{
	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->NodesPerTruncationFace->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array)
		{
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->TruncationCells->NodesPerTruncationFace->CumulativeLength)->Values;
			EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
			hdfProxy->readArrayNdOfUInt64Values(dataset->PathInHdfFile, nodeCountPerFace);
		}
		else if (rep->TruncationCells->NodesPerTruncationFace->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerLatticeArray)
		{
			nodeCountPerFace[0] = static_cast<gsoap_resqml2_0_1::resqml20__IntegerLatticeArray*>(rep->TruncationCells->NodesPerTruncationFace->CumulativeLength)->StartValue;
			const int64_t offsetValue = static_cast<gsoap_resqml2_0_1::resqml20__IntegerLatticeArray*>(rep->TruncationCells->NodesPerTruncationFace->CumulativeLength)->Offset[0]->Value;
			const uint64_t faceCount = getTruncatedFaceCount();
			for (uint64_t nodeCountPerFaceIndex = 1; nodeCountPerFaceIndex < faceCount; ++nodeCountPerFaceIndex) {
				nodeCountPerFace[nodeCountPerFaceIndex] = nodeCountPerFace[nodeCountPerFaceIndex - 1] + offsetValue;
			}
		}
		else if (rep->TruncationCells->NodesPerTruncationFace->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerConstantArray)
		{
			throw range_error("The *cumulative* length of nodes count per cells cannot be constant.");
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->NodesPerTruncationFace->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray)
		{
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->TruncationCellPatch->NodesPerTruncationFace->CumulativeLength)->Values->ExternalDataArrayPart[0];
			EML2_NS::AbstractHdfProxy * hdfProxy = getOrCreateHdfProxyFromDataArrayPart(daPart);
			hdfProxy->readArrayNdOfUInt64Values(daPart->PathInExternalFile, nodeCountPerFace);
		}
		else if (rep->TruncationCellPatch->NodesPerTruncationFace->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerLatticeArray)
		{
			nodeCountPerFace[0] = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->TruncationCellPatch->NodesPerTruncationFace->CumulativeLength)->StartValue;
			const int64_t offsetValue = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->TruncationCellPatch->NodesPerTruncationFace->CumulativeLength)->Offset[0]->Value;
			const uint64_t faceCount = getTruncatedFaceCount();
			for (uint64_t nodeCountPerFaceIndex = 1; nodeCountPerFaceIndex < faceCount; ++nodeCountPerFaceIndex) {
				nodeCountPerFace[nodeCountPerFaceIndex] = nodeCountPerFace[nodeCountPerFaceIndex - 1] + offsetValue;
			}
		}
		else if (rep->TruncationCellPatch->NodesPerTruncationFace->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerConstantArray)
		{
			throw range_error("The *cumulative* length of nodes count per cells cannot be constant.");
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getNodeCountPerTruncatedFace(uint64_t * nodeCountPerFace) const
{
	getCumulativeNodeCountPerTruncatedFace(nodeCountPerFace);
	const uint64_t faceCount = getTruncatedFaceCount();
	uint64_t buffer = nodeCountPerFace[0];
	uint64_t bufferCumCount = 0;
	for (uint64_t cumulativeNodeCountPerFaceIndex = 1; cumulativeNodeCountPerFaceIndex < faceCount; ++cumulativeNodeCountPerFaceIndex)
	{
		bufferCumCount = nodeCountPerFace[cumulativeNodeCountPerFaceIndex];
		nodeCountPerFace[cumulativeNodeCountPerFaceIndex] -= buffer;
		buffer = bufferCumCount;
	}
}

uint64_t AbstractGridRepresentation::getTruncatedCellCount() const
{
	if (!isTruncated()) {
		return 0;
	}

	if (gsoapProxy2_0_1 != nullptr) {
		return static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1)->TruncationCells->TruncationCellCount;
	}
	else if (gsoapProxy2_3 != nullptr) {
		return static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3)->TruncationCellPatch->TruncationCellCount;
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getTruncatedCellIndices(uint64_t* cellIndices) const
{
	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation const* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);
		readArrayNdOfUInt64Values(rep->TruncationCells->ParentCellIndices, cellIndices);
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation const* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);
		readArrayNdOfUInt64Values(rep->TruncationCellPatch->ParentCellIndices, cellIndices);
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getTruncatedFaceIndicesOfTruncatedCells(uint64_t * faceIndices) const
{
	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->TruncationFacesPerCell->Elements->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array) {
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->TruncationCells->TruncationFacesPerCell->Elements)->Values;
			getHdfProxyFromDataset(dataset)->readArrayNdOfUInt64Values(dataset->PathInHdfFile, faceIndices);
		}
		else
			throw logic_error("Not implemented yet");
	}
	else if (gsoapProxy2_3 != nullptr) {
		auto rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->TruncationFacesPerCell->Elements->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray) {
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->TruncationCellPatch->TruncationFacesPerCell->Elements)->Values->ExternalDataArrayPart[0];
			getOrCreateHdfProxyFromDataArrayPart(daPart)->readArrayNdOfUInt64Values(daPart->PathInExternalFile, faceIndices);
		}
		else
			throw logic_error("Not implemented yet");
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getCumulativeTruncatedFaceCountPerTruncatedCell(uint64_t * cumulativeFaceCountPerCell) const
{

	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->TruncationFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array) {
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->TruncationCells->TruncationFacesPerCell->CumulativeLength)->Values;
			EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
			hdfProxy->readArrayNdOfUInt64Values(dataset->PathInHdfFile, cumulativeFaceCountPerCell);
		}
		else if (rep->TruncationCells->TruncationFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerLatticeArray) {
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_resqml2_0_1::resqml20__IntegerLatticeArray*>(rep->TruncationCells->TruncationFacesPerCell->CumulativeLength)->StartValue;
			const int64_t offsetValue = static_cast<gsoap_resqml2_0_1::resqml20__IntegerLatticeArray*>(rep->TruncationCells->TruncationFacesPerCell->CumulativeLength)->Offset[0]->Value;
			const uint64_t cellCount = getTruncatedCellCount();
			for (uint64_t cumulativeFaceCountPerCellIndex = 1; cumulativeFaceCountPerCellIndex < cellCount; ++cumulativeFaceCountPerCellIndex) {
				cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex] = cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex - 1] + offsetValue;
			}
		}
		else if (rep->TruncationCells->TruncationFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerConstantArray) {
			if (getTruncatedCellCount() > 1)
				throw range_error("The cumulative length of faces count per cells cannot be constant if there is more than one cell in the grid");
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_resqml2_0_1::resqml20__IntegerConstantArray*>(rep->TruncationCells->TruncationFacesPerCell->CumulativeLength)->Value;
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray) {
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength)->Values->ExternalDataArrayPart[0];
			getOrCreateHdfProxyFromDataArrayPart(daPart)->readArrayNdOfUInt64Values(daPart->PathInExternalFile, cumulativeFaceCountPerCell);
		}
		else if (rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerLatticeArray) {
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength)->StartValue;
			const int64_t offsetValue = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength)->Offset[0]->Value;
			const uint64_t cellCount = getTruncatedCellCount();
			for (uint64_t cumulativeFaceCountPerCellIndex = 1; cumulativeFaceCountPerCellIndex < cellCount; ++cumulativeFaceCountPerCellIndex) {
				cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex] = cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex - 1] + offsetValue;
			}
		}
		else if (rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerConstantArray) {
			if (getTruncatedCellCount() > 1)
				throw range_error("The cumulative length of faces count per cells cannot be constant if there is more than one cell in the grid");
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_eml2_3::eml23__IntegerConstantArray*>(rep->TruncationCellPatch->TruncationFacesPerCell->CumulativeLength)->Value;
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getTruncatedFaceCountPerTruncatedCell(uint64_t * faceCountPerCell) const
{
	getCumulativeTruncatedFaceCountPerTruncatedCell(faceCountPerCell);
	const uint64_t cellCount = getTruncatedCellCount();
	uint64_t buffer = faceCountPerCell[0];
	uint64_t bufferCumCount = 0;
	for (uint64_t cumulativeFaceCountPerCellIndex = 1; cumulativeFaceCountPerCellIndex < cellCount; ++cumulativeFaceCountPerCellIndex)
	{
		bufferCumCount = faceCountPerCell[cumulativeFaceCountPerCellIndex];
		faceCountPerCell[cumulativeFaceCountPerCellIndex] -= buffer;
		buffer = bufferCumCount;
	}
}

void AbstractGridRepresentation::getNonTruncatedFaceIndicesOfTruncatedCells(uint64_t * faceIndices) const
{
	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->LocalFacesPerCell->Elements->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array) {
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->TruncationCells->LocalFacesPerCell->Elements)->Values;
			EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
			hdfProxy->readArrayNdOfUInt64Values(dataset->PathInHdfFile, faceIndices);
		}
		else
			throw logic_error("Not yet implemented");
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->LocalFacesPerCell->Elements->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray) {
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->TruncationCellPatch->LocalFacesPerCell->Elements)->Values->ExternalDataArrayPart[0];
			getOrCreateHdfProxyFromDataArrayPart(daPart)->readArrayNdOfUInt64Values(daPart->PathInExternalFile, faceIndices);
		}
		else
			throw logic_error("Not yet implemented");
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getCumulativeNonTruncatedFaceCountPerTruncatedCell(uint64_t * cumulativeFaceCountPerCell) const
{

	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->LocalFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerHdf5Array) {
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__IntegerHdf5Array*>(rep->TruncationCells->LocalFacesPerCell->CumulativeLength)->Values;
			EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
			hdfProxy->readArrayNdOfUInt64Values(dataset->PathInHdfFile, cumulativeFaceCountPerCell);
		}
		else if (rep->TruncationCells->LocalFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerLatticeArray) {
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_resqml2_0_1::resqml20__IntegerLatticeArray*>(rep->TruncationCells->LocalFacesPerCell->CumulativeLength)->StartValue;
			const int64_t offsetValue = static_cast<gsoap_resqml2_0_1::resqml20__IntegerLatticeArray*>(rep->TruncationCells->LocalFacesPerCell->CumulativeLength)->Offset[0]->Value;
			const uint64_t cellCount = getTruncatedCellCount();
			for (uint64_t cumulativeFaceCountPerCellIndex = 1; cumulativeFaceCountPerCellIndex < cellCount; ++cumulativeFaceCountPerCellIndex) {
				cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex] = cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex - 1] + offsetValue;
			}
		}
		else if (rep->TruncationCells->LocalFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__IntegerConstantArray) {
			if (getTruncatedCellCount() > 1)
				throw range_error("The cumulative length of faces count per cells cannot be constant if there is more than one cell in the grid");
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_resqml2_0_1::resqml20__IntegerConstantArray*>(rep->TruncationCells->LocalFacesPerCell->CumulativeLength)->Value;
		}
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerExternalArray) {
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__IntegerExternalArray*>(rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength)->Values->ExternalDataArrayPart[0];
			getOrCreateHdfProxyFromDataArrayPart(daPart)->readArrayNdOfUInt64Values(daPart->PathInExternalFile, cumulativeFaceCountPerCell);
		}
		else if (rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerLatticeArray) {
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength)->StartValue;
			const int64_t offsetValue = static_cast<gsoap_eml2_3::eml23__IntegerLatticeArray*>(rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength)->Offset[0]->Value;
			const uint64_t cellCount = getTruncatedCellCount();
			for (uint64_t cumulativeFaceCountPerCellIndex = 1; cumulativeFaceCountPerCellIndex < cellCount; ++cumulativeFaceCountPerCellIndex) {
				cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex] = cumulativeFaceCountPerCell[cumulativeFaceCountPerCellIndex - 1] + offsetValue;
			}
		}
		else if (rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__IntegerConstantArray) {
			if (getTruncatedCellCount() > 1)
				throw range_error("The cumulative length of faces count per cells cannot be constant if there is more than one cell in the grid");
			cumulativeFaceCountPerCell[0] = static_cast<gsoap_eml2_3::eml23__IntegerConstantArray*>(rep->TruncationCellPatch->LocalFacesPerCell->CumulativeLength)->Value;
		}
	}
	else {
		throw logic_error("Not implemented yet");
	}
}

void AbstractGridRepresentation::getNonTruncatedFaceCountPerTruncatedCell(uint64_t * faceCountPerCell) const
{
	getCumulativeNonTruncatedFaceCountPerTruncatedCell(faceCountPerCell);
	const uint64_t cellCount = getTruncatedCellCount();
	uint64_t buffer = faceCountPerCell[0];
	uint64_t bufferCumCount = 0;
	for (uint64_t cumulativeFaceCountPerCellIndex = 1; cumulativeFaceCountPerCellIndex < cellCount; ++cumulativeFaceCountPerCellIndex) {
		bufferCumCount = faceCountPerCell[cumulativeFaceCountPerCellIndex];
		faceCountPerCell[cumulativeFaceCountPerCellIndex] -= buffer;
		buffer = bufferCumCount;
	}
}

void AbstractGridRepresentation::getTruncatedFaceIsRightHanded(uint8_t* cellFaceIsRightHanded) const
{
	if (!isTruncated()) {
		throw invalid_argument("The grid is not truncated");
	}

	if (gsoapProxy2_0_1 != nullptr) {
		gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_resqml2_0_1::resqml20__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_0_1);

		if (rep->TruncationCells->TruncationCellFaceIsRightHanded->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__BooleanHdf5Array) {
			gsoap_resqml2_0_1::eml20__Hdf5Dataset const * dataset = static_cast<gsoap_resqml2_0_1::resqml20__BooleanHdf5Array*>(rep->TruncationCells->TruncationCellFaceIsRightHanded)->Values;
			EML2_NS::AbstractHdfProxy * hdfProxy = getHdfProxyFromDataset(dataset);
			hdfProxy->readArrayNdOfUInt8Values(dataset->PathInHdfFile, cellFaceIsRightHanded);
		}
		else if (rep->TruncationCells->TruncationCellFaceIsRightHanded->soap_type() == SOAP_TYPE_gsoap_resqml2_0_1_resqml20__BooleanConstantArray) {
			for (uint64_t i = 0; i < static_cast<gsoap_resqml2_0_1::resqml20__BooleanConstantArray*>(rep->TruncationCells->TruncationCellFaceIsRightHanded)->Count; ++i) {
				cellFaceIsRightHanded[i] = static_cast<gsoap_resqml2_0_1::resqml20__BooleanConstantArray*>(rep->TruncationCells->TruncationCellFaceIsRightHanded)->Value;
			}
		}
		else
			throw logic_error("Not implemented yet.");
	}
	else if (gsoapProxy2_3 != nullptr) {
		gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation* rep = static_cast<gsoap_eml2_3::resqml22__AbstractTruncatedColumnLayerGridRepresentation*>(gsoapProxy2_3);

		if (rep->TruncationCellPatch->TruncationCellFaceIsRightHanded->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__BooleanExternalArray) {
			auto const* daPart = static_cast<gsoap_eml2_3::eml23__BooleanExternalArray*>(rep->TruncationCellPatch->TruncationCellFaceIsRightHanded)->Values->ExternalDataArrayPart[0];
			getOrCreateHdfProxyFromDataArrayPart(daPart)->readArrayNdOfUInt8Values(daPart->PathInExternalFile, cellFaceIsRightHanded);
		}
		else if (rep->TruncationCellPatch->TruncationCellFaceIsRightHanded->soap_type() == SOAP_TYPE_gsoap_eml2_3_eml23__BooleanConstantArray) {
			for (int64_t i = 0; i < static_cast<gsoap_eml2_3::eml23__BooleanConstantArray*>(rep->TruncationCellPatch->TruncationCellFaceIsRightHanded)->Count; ++i) {
				cellFaceIsRightHanded[i] = static_cast<gsoap_eml2_3::eml23__BooleanConstantArray*>(rep->TruncationCellPatch->TruncationCellFaceIsRightHanded)->Value;
			}
		}
		else
			throw logic_error("Not implemented yet.");
	}
	else {
		throw logic_error("Not implemented yet");
	}
}
