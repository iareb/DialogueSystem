#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <span>

namespace Dialogue {
	enum class EntityNames {
		Hero,
		Villain
	};

	enum class EmotionTypes {
		Normal,
		Happy,
		Sad,
		Angry,
	};
	
	using EntityId = std::uint8_t;

	constexpr std::string_view ToString(EntityNames Name) {
		switch (Name) {
			case EntityNames::Hero:
				return "Hero";
			case EntityNames::Villain:
				return "Villain";	
		}
		return "Unknown";
	}

	struct DialogueNode {
		int Id = -1;
	
		/// Short int for efficiency - the game is small and we'll never have more than 
		/// 255 different entities who participate in dialogues.
		EntityId Speaker = 0;
		EntityId Listener = 1;
	
		std::string Text;
		// std::string Condition;
	
		EmotionTypes EmotionType = EmotionTypes::Normal;
	
		bool TriggersQuest = false;
		bool StartsCombat = false;
		bool EndsDialogue = false;
	
		std::uint8_t MoralWeight = 0;
	};

	class DialogueGraph {
	private:
		std::unordered_map<int, DialogueNode> Nodes;
		std::unordered_map<int, std::vector<int>> Edges;

		int RootNodeId = -1;
		bool Started = false;
		bool Ended = false;

	public:
		DialogueGraph() = default;

		DialogueGraph(std::unordered_map<int, DialogueNode> Nodes, std::unordered_map<int, std::vector<int>> Edges) 
		: Nodes(std::move(Nodes)), Edges(std::move(Edges))
		{}

		const DialogueNode* GetRoot() const {
			return GetNode(RootNodeId);
		}

		const DialogueNode* GetNode(int Id) const {
			auto it = Nodes.find(Id);

			if (it == Nodes.end()) {
				return nullptr;
			}

			return &it->second;
		}

		std::span<const int> GetEdges(int NodeId) const
		{
			auto it = Edges.find(NodeId);

			if (it == Edges.end()) {
				return {};
			}

			return it->second;
		}

		bool AddNode(DialogueNode DNode) {
			return Nodes.emplace(DNode.Id, std::move(DNode)).second;
		}

		bool AddEdge(int From, int To) {
			if (!Nodes.contains(From) || !Nodes.contains(To)) {
				return false;
			}

			auto& connections = Edges[From];

			// Connection already in the graph
			if (std::find(connections.begin(), connections.end(), To) != connections.end()) {
				return false;
			}

			connections.push_back(To);

			return true;
		}

		std::string_view GetSpeakerName(int NodeId) const {
			const DialogueNode* Node = GetNode(NodeId);
			return ToString(static_cast<EntityNames>(Node->Speaker));
		}

		std::string_view GetListenerName(int NodeId) {
			const DialogueNode* Node = GetNode(NodeId);
			return ToString(static_cast<EntityNames>(Node->Listener));
		}

		std::unordered_set<std::uint8_t> GetSpeakers() {
			std::unordered_set<std::uint8_t> Speakers;
			for (const auto& [Key, DNode] : Nodes) {
				Speakers.insert(DNode.Speaker);
			}

			return Speakers;
		}

		bool HasStarted() const {
			return Started;
		}

		bool HasEnded() const {
			return Ended;
		}

		void PrintGraph() const
		{
			for (const auto& [nodeId, node] : Nodes)
			{
				std::cout << GetSpeakerName(nodeId) << ": "
					<< node.Text << "\n";

				auto edgeIt = Edges.find(nodeId);
				if (edgeIt != Edges.end())
				{
					std::cout << "  -> ";

					for (int next : edgeIt->second)
					{
						std::cout << GetSpeakerName(next) << " ";
					}

					std::cout << "\n";
				}
				std::cout << "\n";
			}
		}
	};
	
}
