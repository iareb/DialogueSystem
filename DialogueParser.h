#pragma once
#include "nlohmann/json.hpp"
#include "DialogueGraph.h"

using json = nlohmann::json;

namespace Dialogue {
	class DialogueParser {
	private:
		json JsonFile;
	};
}