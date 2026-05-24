#pragma once
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <queue>
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
		INVALID,
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
		static constexpr EntityId InvalidEntity = 255;
		EntityId Speaker = InvalidEntity;
		EntityId Listener = InvalidEntity;
	
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

	public:
		DialogueGraph() = default;

		DialogueGraph(
			std::unordered_map<int, DialogueNode> Nodes,
			std::unordered_map<int, std::vector<int>> Edges,
			int RootNodeId
		)
		: Nodes(std::move(Nodes)), Edges(std::move(Edges)), RootNodeId(RootNodeId)
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

		std::string_view GetListenerName(int NodeId) const {
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

		// Breadth First Search
		void PrintGraph() const {
			const DialogueNode* Current = GetRoot();
			if (!Current) return;

			std::unordered_set<int> Visited;
			std::queue<const DialogueNode*> Queue;
			Queue.push(Current);

			while (!Queue.empty()) {
				Current = Queue.front();
				Queue.pop();

				if (Visited.contains(Current->Id)) {
					continue;
				}

				Visited.insert(Current->Id);

				std::cout << GetSpeakerName(Current->Id) << ": "
						  << Current->Text << "\n";

				for (int NextId : GetEdges(Current->Id)) {
					std::cout << " -> " << GetSpeakerName(NextId) << "\n";

					const DialogueNode* Next = GetNode(NextId);
					if (Next && !Visited.contains(NextId)) {
						Queue.push(Next);
					}
				}
				std::cout << "\n";
			}
		}
		/*
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
		*/
	};
	
}
