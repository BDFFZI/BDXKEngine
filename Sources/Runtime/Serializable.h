#pragma once
#include "Importer.h"
#include "Exporter.h"

namespace BDXKEngine {
	class Serializable {
		virtual void Import(Importer& transfer) = 0;
		virtual void Export(Exporter& transfer) = 0;
	};
}