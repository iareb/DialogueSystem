#pragma once
#include <fstream>
#include "nlohmann/json.hpp"
#include "DialogueGraph.h"

using json = nlohmann::json;

namespace Dialogue {
	class DialogueParser {
	private:
		json JsonDoc;

	public:
		DialogueParser(const std::string& filename) {
			std::ifstream File(filename);
			if (!File.is_open()) {
				throw std::runtime_error("Could not open file " + filename);
			}

			try {
				JsonDoc = json::parse(File);
			}
			catch (const json::parse_error& e) {
				throw std::runtime_error(std::string("JSON parse error: ") + e.what());
			}
		}

		DialogueGraph BuildGraph() {
			return DialogueGraph(ParseNodes(), ParseEdges(), ParseRootNode());
		}

		std::unordered_map<int, DialogueNode> ParseNodes() {
			std::unordered_map<int, DialogueNode> Nodes;

			assert(!JsonDoc.empty());
			for (const auto& JsonNode : JsonDoc["nodes"]) {
				DialogueNode Node;
				Node.Id = JsonNode["id"];
				Node.Speaker = JsonNode["speaker"];
				Node.Listener = JsonNode["listener"];
				Node.Text = JsonNode["text"];
				Node.EmotionType = ParseEmotion(JsonNode["emotion"].get<std::string>());

				Node.TriggersQuest = JsonNode.value("triggers_quest", false);
				Node.StartsCombat = JsonNode.value("starts_combat", false);
				Node.EndsDialogue = JsonNode.value("ends_dialogue", false);
				Node.MoralWeight = JsonNode.value("moral_weight", 0);

				Nodes[JsonNode["id"]] = Node;
			}

			return Nodes;
		}

		std::unordered_map<int, std::vector<int>> ParseEdges() {
			std::unordered_map<int, std::vector<int>> Edges;

			for (const auto& JsonNode : JsonDoc["edges"]) {
				Edges[JsonNode["from"]].emplace_back(JsonNode["to"]);
			}

			return Edges;
		}

		int ParseRootNode() const {
			return JsonDoc.value("root_node_id", 0);
		}

		static EmotionTypes ParseEmotion(std::string_view Emotion) {
			if (Emotion == "normal") return EmotionTypes::Normal;
			if (Emotion == "happy") return EmotionTypes::Happy;
			if (Emotion == "sad") return EmotionTypes::Sad;
			if (Emotion == "angry") return EmotionTypes::Angry;

			return EmotionTypes::INVALID;
		}

		void PrintDoc() {
			std::cout << JsonDoc.dump() << std::endl;
		}
	};
}