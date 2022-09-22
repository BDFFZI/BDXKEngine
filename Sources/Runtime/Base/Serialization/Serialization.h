#pragma once
#include "Importer.h"
#include "Exporter.h"

namespace BDXKEngine {
	class Serialization {
	public:
		virtual ~Serialization() = default;
	private:
		virtual void Import(Importer& importer) = 0;
		virtual void Export(Exporter& exporter) = 0;
	};
}