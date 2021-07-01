#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <memory>
#include <vector>
#include <string>
#include "chatbot.h"


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    
    // Task 4 - change raw pointer _childEdges to a vector of unique_ptr to graph edges 
    // pointing to subsequent nodes/outgoing edges. 
    //std::vector<GraphEdge *> _childEdges;  // edges to subsequent nodes
    std::vector<std::unique_ptr<GraphEdge>> _childEdges; 

    std::vector<GraphEdge *> _parentEdges; 
    ChatBot _chatBot;

    int _id;
    std::vector<std::string> _answers;

public:
    GraphNode(int id);
    ~GraphNode();

    int GetID() const { return _id; }
    int GetNumberOfChildEdges() const { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() const { return _answers; }
    int GetNumberOfParents() const { return _parentEdges.size(); }

    void AddToken(std::string token);
    void AddEdgeToParentNode(GraphEdge *edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);

    void MoveChatbotHere(ChatBot chatbot);

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */