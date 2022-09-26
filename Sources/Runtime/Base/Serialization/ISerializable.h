#pragma once
#include "Importer.h"
#include "Exporter.h"

namespace BDXKEngine {
	struct ISerializable {
		virtual ~ISerializable() = default;
		virtual void Import(Importer& importer) = 0;
		virtual void Export(Exporter& exporter) = 0;
	};
}