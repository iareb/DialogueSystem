#include "DialogueGraph.h"
#include "DialogueParser.h"

int main()
{
	/*
	Dialogue::DialogueNode FirstNode{
		.Id = 0,
		.Speaker = 0,
		.Listener = 1,
		.Text = "Hello",
		.EmotionType = Dialogue::EmotionTypes::Normal,
	};
	Dialogue::DialogueNode SecondNode{
		.Id = 1,
		.Speaker = 1,
		.Listener = 0,
		.Text = "Fuck you!",
		.EmotionType = Dialogue::EmotionTypes::Angry,
	};
	*/

	Dialogue::DialogueGraph Graph = Dialogue::DialogueParser("files/first.json").BuildGraph();
	/*
	Graph.AddNode(FirstNode);
	Graph.AddNode(SecondNode);
	Graph.AddEdge(FirstNode.Id, SecondNode.Id);
	
	auto NextNodes = Graph.GetEdges(FirstNode.Id);

	Graph.AddEdge(SecondNode.Id, FirstNode.Id);
	*/

	Graph.PrintGraph();

	return 0;
}
